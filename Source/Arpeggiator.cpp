#include "../JuceLibraryCode/JuceHeader.h"
#include "Arpeggiator.h"
#include "Extensions.h"

Arpeggiator::Arpeggiator(): AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo(), true))
{
	addParameter(noteDivision);
	addParameter(arpPlayMode);
	addParameter(lengthFactor);
	addParameter(arpLatchMode);
}

void Arpeggiator::prepareToPlay(double sampleRate, int)
{
	notesToPlay.clear();
	notesToPlayLatchMode.clear();
	rate = sampleRate;
	currentNoteIndex = -1;
	lastNoteValue = 0;
	lastNoteWasNoteOn = false;
	samplesFromLastNoteOnUntilBufferEnds = 0;
	noteLengthInSamples = 0;
	numberOfSamplesInBuffer = 0;
	noteOffRequiredThisBuffer = false;
	notesToPlay.reserve(100);
	notesToPlayLatchMode.reserve(200);
	numberOfNotesToPlay = 0;
	selectedPlayMode = playMode::up;
	currentPlayMode = playMode::up;
	playModeHasChanged = false;
	noteDivisionFactor = 1;
	noteDivisionFactorChanged = false;
	currentLatchMode = latchMode::off;
	selectedLatchMode = latchMode::off;
	latchModeHasChanged = false;
}

void Arpeggiator::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	auto playHead = getPlayHead();
	if (playHead == nullptr) return;

	AudioPlayHead::CurrentPositionInfo positionInfo{};
	playHead->getCurrentPosition(positionInfo);

	const auto quarterNotesPerMinute = positionInfo.bpm;
	const auto quarterNotesPerSecond = quarterNotesPerMinute / 60;
	const auto samplesPerQuarterNote = rate / quarterNotesPerSecond;
	const auto samplesPerNoteDivision = samplesPerQuarterNote / noteDivisionFactor;
	noteLengthInSamples = static_cast<int> (std::ceil(samplesPerNoteDivision * *lengthFactor));
	numberOfSamplesInBuffer = buffer.getNumSamples();

	// start position of the current buffer in quarter note ticks with respect to host timeline
	const double partsPerQuarterNoteStartPosition = positionInfo.ppqPosition;
	// start position of the current buffer in custom note-divisions ticks with respect to host timeline
	const double NoteDivisionStartPosition = partsPerQuarterNoteStartPosition * noteDivisionFactor;
	// end position of the current buffer in ticks with respect to host timeline
	const double NoteDivisionEndPosition = NoteDivisionStartPosition + (numberOfSamplesInBuffer / samplesPerNoteDivision);
	// trick to calculate when a new note should occur..everytime start position rounded up = end position rounded down
	const int NoteDivisionStartPositionAsInt = std::ceil(NoteDivisionStartPosition);
	const int NoteDivisionEndPositionAsInt = std::floor(NoteDivisionEndPosition);

	SetLatchMode();

	UpdateNotesToPlay();

	MidiMessage message;
	int ignore;

	for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(message, ignore);)
	{
		if (message.isNoteOn())
		{
			notesToPlay.push_back(message.getNoteNumber());

			notesToPlayLatchMode.push_back(message.getNoteNumber());
		}
		else if (message.isNoteOff())
		{
			erase(notesToPlay, message.getNoteNumber());
		}
	}

	numberOfNotesToPlay = GetNumberOfNotesToPlay();

	midiMessages.clear();

	SetPlayMode();

	//const int beatsPerBar = 4 * noteDivisionFactor; // to play with for delaying changes until next whole bar

	if (positionInfo.isPlaying)
	{
		for (int i = NoteDivisionStartPositionAsInt; i <= NoteDivisionEndPositionAsInt; ++i)
		{
			const int noteOnOffset = (int)(samplesPerNoteDivision * (i - NoteDivisionStartPosition));

			if (AnyNotesToPlay())
			{
				UpdateNoteValue();
				AddNoteOnToBuffer(midiMessages, noteOnOffset);
				samplesFromLastNoteOnUntilBufferEnds = numberOfSamplesInBuffer - noteOnOffset;
			}

			if (NoteOffIsRequiredThisBuffer())
			{
				noteOffOccursInSameBufferAsLastNoteOn = true;
				const auto offsetForNoteOff = CalculateOffsetForNoteOff(noteOnOffset);
				AddNoteOffToBuffer(midiMessages, offsetForNoteOff);
			}
		}

		samplesFromLastNoteOnUntilBufferEnds = (samplesFromLastNoteOnUntilBufferEnds + numberOfSamplesInBuffer);

		UpdateNoteDivision();

		if (NoteOffIsRequiredThisBuffer() || (noteDivisionFactorChanged && lastNoteWasNoteOn))
		{
			noteOffOccursInSameBufferAsLastNoteOn = false;
			const auto offsetForNoteOff = CalculateOffsetForNoteOff();
			AddNoteOffToBuffer(midiMessages, offsetForNoteOff);
		}
	}

	if (!positionInfo.isPlaying)
	{
		midiMessages.clear();
		notesToPlay.clear();
		notesToPlayLatchMode.clear();
		numberOfNotesToPlay = GetNumberOfNotesToPlay();
		UpdateCurrentNoteIndex();
	}
}

void Arpeggiator::UpdateNoteDivision()
{
	const auto currentNoteDivisionFactor = noteDivisionDictionary[noteDivision->getCurrentChoiceName()];

	noteDivisionFactorChanged = noteDivisionFactor != currentNoteDivisionFactor;
	if (noteDivisionFactorChanged)
	{
		noteDivisionFactor = currentNoteDivisionFactor;
	}
}

void Arpeggiator::SetPlayMode()
{
	selectedPlayMode = static_cast<playMode>(arpPlayMode->getIndex());

	UpdateOrderOfNotesToPlay();

	playModeHasChanged = currentPlayMode != selectedPlayMode;
	if(playModeHasChanged || !AnyNotesToPlay())
	{
		currentPlayMode = selectedPlayMode;

		UpdateCurrentNoteIndex();
	}
}

void Arpeggiator::SetLatchMode()
{
	selectedLatchMode = static_cast<latchMode>(arpLatchMode->getIndex());

	latchModeHasChanged = currentLatchMode != selectedLatchMode;

	currentLatchMode = latchModeHasChanged ? selectedLatchMode : currentLatchMode;

	//if(latchModeHasChanged) ??
	//{
	//	currentLatchMode = selectedLatchMode;
	//}
}

void Arpeggiator::UpdateNotesToPlay()
{
	if(latchModeHasChanged || LatchModeIsOff())
	{
		notesToPlayLatchMode = notesToPlay;
	}

	//if (currentLatchMode == latchMode::on && latchModeHasChanged) ??
	//{
	//	notesToPlayLatchMode = notesToPlay;
	//}

	//if (currentLatchMode == latchMode::off)
	//{
	//	notesToPlayLatchMode.clear();
	//}
}

void Arpeggiator::UpdateOrderOfNotesToPlay()
{
	//if(currentPlayMode != playMode::played && LatchModeIsOff()) ??
	if (currentPlayMode != playMode::played)
	{
		std::sort(notesToPlay.begin(), notesToPlay.end());

		std::sort(notesToPlayLatchMode.begin(), notesToPlayLatchMode.end());
	}
}

void Arpeggiator::UpdateCurrentNoteIndex()
{
	const auto lastIndexOfNotesToPlay = numberOfNotesToPlay - 1;

	switch (currentPlayMode)
	{
		case playMode::up: currentNoteIndex = -1;
			break;
		case playMode::down: currentNoteIndex = lastIndexOfNotesToPlay;
			break;
		case playMode::random: currentNoteIndex = Random::getSystemRandom().nextInt(Range<int>(0, numberOfNotesToPlay));
			break;
		case playMode:: played: currentNoteIndex = -1;
			break;
	}
}

void Arpeggiator::UpdateNoteValue()
{
	switch (currentPlayMode)
	{
		case playMode::up: currentNoteIndex = (currentNoteIndex + 1) % numberOfNotesToPlay;
			break;
		case playMode::down: currentNoteIndex = inRange(currentNoteIndex, 0, numberOfNotesToPlay) ? (currentNoteIndex - 1) : numberOfNotesToPlay - 1;
			break;
		case playMode::random: currentNoteIndex = Random::getSystemRandom().nextInt(Range<int>(0, numberOfNotesToPlay));
			break;
		case playMode::played: currentNoteIndex = (currentNoteIndex + 1) % numberOfNotesToPlay;
			break;
	}

	lastNoteValue = SetLastNoteValue();
}

int Arpeggiator::SetLastNoteValue()
{
	return LatchModeIsOff() ? notesToPlay[currentNoteIndex] : notesToPlayLatchMode[currentNoteIndex];
}

void Arpeggiator::AddNoteOffToBuffer(MidiBuffer& midiMessages, const int offset)
{
	midiMessages.addEvent(MidiMessage::noteOff(1, lastNoteValue), offset);
	lastNoteWasNoteOn = false;
}

void Arpeggiator::AddNoteOnToBuffer(MidiBuffer& midiMessages, const int offset)
{
	midiMessages.addEvent(MidiMessage::noteOn(1, lastNoteValue, (uint8)127), offset);
	lastNoteWasNoteOn = true;
}

bool Arpeggiator::NoteOffIsRequiredThisBuffer() const
{
	return samplesFromLastNoteOnUntilBufferEnds >= noteLengthInSamples && lastNoteWasNoteOn;
}

int Arpeggiator::CalculateOffsetForNoteOff(int noteOnOffset) const
{
	const auto lastSampleInCurrentBuffer = numberOfSamplesInBuffer - 1;
	int offset;

	if(noteOffOccursInSameBufferAsLastNoteOn)
	{
		offset = noteOnOffset + noteLengthInSamples;
	}
	else
	{
		offset = numberOfSamplesInBuffer - (samplesFromLastNoteOnUntilBufferEnds - noteLengthInSamples);
	}

	return jmin(offset, lastSampleInCurrentBuffer);
}

bool Arpeggiator::AnyNotesToPlay() const
{
	return LatchModeIsOff() ? !notesToPlay.empty() : !notesToPlayLatchMode.empty();
}

int Arpeggiator::GetNumberOfNotesToPlay() const
{
	return LatchModeIsOff() ? notesToPlay.size() : notesToPlayLatchMode.size();
}

bool Arpeggiator::LatchModeIsOff() const
{
	return currentLatchMode == latchMode::off;
}

void Arpeggiator::getStateInformation(MemoryBlock& destData)
{
	MemoryOutputStream(destData, true).writeInt(*noteDivision);
	MemoryOutputStream(destData, true).writeInt(*arpPlayMode);
	MemoryOutputStream(destData, true).writeInt(*arpLatchMode);
	MemoryOutputStream(destData, true).writeFloat(*lengthFactor);
}

void Arpeggiator::setStateInformation(const void* data, int sizeInBytes)
{
	noteDivision->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	arpPlayMode->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	lengthFactor->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	arpLatchMode->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
}