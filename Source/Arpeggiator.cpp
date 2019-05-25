#include "../JuceLibraryCode/JuceHeader.h"
#include "Arpeggiator.h"
#include "Extensions.h"

Arpeggiator::Arpeggiator() : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo(), true))
{
	addParameter(noteDivision);
	addParameter(arpPlayMode);
	addParameter(lengthFactor);
	addParameter(swingFactor);
	addParameter(arpLatchMode);
	addParameter(arpLatchLock);
	addParameter(noteShift);
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
	latchLockState = latchLock::unlocked;
	transposeIsEnabled = false;
	shiftFactor = 0;
	samplesPerQuarterNote = 0.0;
	samplesPerNoteDivision = 0.0;
	noteDivisionLengthPPQ = 0;
	noteLength = 0;
	maxSwingPPQ = 0;
	noteOnOffset = 0;
}

void Arpeggiator::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	auto playHead = getPlayHead();
	if (playHead == nullptr) return;

	AudioPlayHead::CurrentPositionInfo positionInfo{};
	playHead->getCurrentPosition(positionInfo);

	const auto quarterNotesPerMinute = positionInfo.bpm;
	const auto quarterNotesPerSecond = quarterNotesPerMinute / 60;
	samplesPerQuarterNote = rate / quarterNotesPerSecond;
	samplesPerNoteDivision = samplesPerQuarterNote / noteDivisionFactor;
	noteLengthInSamples = static_cast<int> (std::ceil(samplesPerNoteDivision * *lengthFactor)) / 2;
	numberOfSamplesInBuffer = buffer.getNumSamples();
	shiftFactor = noteShift->get();

	noteDivisionLengthPPQ = (samplesPerNoteDivision / samplesPerQuarterNote);
	noteLength = (noteDivisionLengthPPQ * *lengthFactor);
	maxSwingPPQ = (noteDivisionLengthPPQ - noteLength) / (2 / noteDivisionFactor);

	// start position of the current buffer in quarter note ticks with respect to host timeline
	const double partsPerQuarterNoteStartPosition = positionInfo.ppqPosition + (PPQ128th * shiftFactor);

	// start position of the current buffer in custom note-divisions ticks with respect to host timeline
	const double OddNoteDivisionStartPosition = (partsPerQuarterNoteStartPosition * noteDivisionFactor) - (noteDivisionLengthPPQ / (2 / noteDivisionFactor)) - (maxSwingPPQ * *swingFactor);
	const double NoteDivisionStartPosition = (partsPerQuarterNoteStartPosition * noteDivisionFactor);

	// end position of the current buffer in ticks with respect to host timeline
	const double OddNoteDivisionEndPosition = OddNoteDivisionStartPosition + (numberOfSamplesInBuffer / samplesPerNoteDivision);
	const double NoteDivisionEndPosition = NoteDivisionStartPosition + (numberOfSamplesInBuffer / samplesPerNoteDivision);

	// trick to calculate when a new note should occur..everytime start position rounded up = end position rounded down
	const int OddNoteDivisionStartPositionAsInt = std::ceil(OddNoteDivisionStartPosition);
	const int OddNoteDivisionEndPositionAsInt = std::floor(OddNoteDivisionEndPosition);

	const int NoteDivisionStartPositionAsInt = std::ceil(NoteDivisionStartPosition);
	const int NoteDivisionEndPositionAsInt = std::floor(NoteDivisionEndPosition);

	SetNoteRecieveMode();

	MidiMessage message;
	int ignore;

	for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(message, ignore);)
	{
		if (message.isNoteOn())
		{
			notesToPlay.push_back(message.getNoteNumber());

			transposeIsEnabled ? transposeNotes(notesToPlayLatchMode, message.getNoteNumber()) : notesToPlayLatchMode.push_back(message.getNoteNumber());
		}
		else if (message.isNoteOff())
		{
			erase(notesToPlay, message.getNoteNumber());
		}
	}

	numberOfNotesToPlay = GetNumberOfNotesToPlay();

	midiMessages.clear();

	SetPlayMode();

	if (positionInfo.isPlaying)
	{
		if(NoteDivisionStartPositionAsInt <= NoteDivisionEndPositionAsInt)
		{
			noteOnOffset = CalculateNoteOnOffset(NoteDivisionStartPositionAsInt, NoteDivisionStartPosition);
			AddNotes(midiMessages);
		}
		if(OddNoteDivisionStartPositionAsInt <= OddNoteDivisionEndPositionAsInt)
		{
			noteOnOffset = CalculateNoteOnOffset(OddNoteDivisionStartPositionAsInt, OddNoteDivisionStartPosition);
			AddNotes(midiMessages);
		}

		samplesFromLastNoteOnUntilBufferEnds = (samplesFromLastNoteOnUntilBufferEnds + numberOfSamplesInBuffer);

		UpdateNoteDivision();

		if (ShouldAddNoteOff() || (noteDivisionFactorChanged && lastNoteWasNoteOn))
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
		InitializeNoteIndex();
	}
}

int Arpeggiator::CalculateNoteOnOffset(int beatPos, double notePos) const
{
	return (int)(samplesPerNoteDivision * (beatPos - notePos));
}

void Arpeggiator::AddNotes(MidiBuffer& midiMessages)
{
	if (ShouldAddNoteOn())
	{
		UpdateNoteValue();
		AddNoteOnToBuffer(midiMessages, noteOnOffset);
		samplesFromLastNoteOnUntilBufferEnds = numberOfSamplesInBuffer - noteOnOffset;
	}

	if (ShouldAddNoteOff())
	{
		noteOffOccursInSameBufferAsLastNoteOn = true;
		const auto offsetForNoteOff = CalculateOffsetForNoteOff(noteOnOffset);
		AddNoteOffToBuffer(midiMessages, offsetForNoteOff);
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

void Arpeggiator::SetNoteRecieveMode()
{
	SetLatchMode();

	latchLockState = static_cast<latchLock>(arpLatchLock->getIndex());

	transposeIsEnabled = IsTransposeEnabled();

	UpdateNotesToPlay();
}

void Arpeggiator::SetLatchMode()
{
	selectedLatchMode = static_cast<latchMode>(arpLatchMode->getIndex());

	latchModeHasChanged = currentLatchMode != selectedLatchMode;

	currentLatchMode = latchModeHasChanged ? selectedLatchMode : currentLatchMode;
}

void Arpeggiator::UpdateNotesToPlay()
{
	notesToPlayLatchMode = latchModeHasChanged ? notesToPlay : notesToPlayLatchMode;
}

void Arpeggiator::SetPlayMode()
{
	selectedPlayMode = static_cast<playMode>(arpPlayMode->getIndex());

	numberOfNotesToPlay = GetNumberOfNotesToPlay();

	SortNotesToPlay();

	playModeHasChanged = currentPlayMode != selectedPlayMode;
	if (playModeHasChanged || !AnyNotesToPlay())
	{
		currentPlayMode = selectedPlayMode;

		InitializeNoteIndex();
	}
}

void Arpeggiator::SortNotesToPlay()
{
	if (currentPlayMode != playMode::played)
	{
		std::sort(notesToPlay.begin(), notesToPlay.end());

		std::sort(notesToPlayLatchMode.begin(), notesToPlayLatchMode.end());
	}
}

void Arpeggiator::InitializeNoteIndex()
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
	case playMode::played: currentNoteIndex = -1;
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
	return IsLatchModeOff() ? notesToPlay[currentNoteIndex] : notesToPlayLatchMode[currentNoteIndex];
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

bool Arpeggiator::ShouldAddNoteOn() const
{
	return (AnyNotesToPlay() && !lastNoteWasNoteOn);
}

bool Arpeggiator::ShouldAddNoteOff() const
{
	return (samplesFromLastNoteOnUntilBufferEnds >= noteLengthInSamples && lastNoteWasNoteOn);
}

int Arpeggiator::CalculateOffsetForNoteOff(int noteOnOffset) const
{
	const auto lastSampleInCurrentBuffer = numberOfSamplesInBuffer - 1;
	int offset;

	if (noteOffOccursInSameBufferAsLastNoteOn)
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
	return IsLatchModeOff() ? !notesToPlay.empty() : !notesToPlayLatchMode.empty();
}

int Arpeggiator::GetNumberOfNotesToPlay() const
{
	return IsLatchModeOff() ? notesToPlay.size() : notesToPlayLatchMode.size();
}

bool Arpeggiator::IsLatchModeOff() const
{
	return currentLatchMode == latchMode::off;
}

bool Arpeggiator::IsTransposeEnabled() const
{
	return (currentLatchMode == latchMode::on
		&& latchLockState == latchLock::locked
		&& !notesToPlayLatchMode.empty());
}

void Arpeggiator::getStateInformation(MemoryBlock& destData)
{
	MemoryOutputStream(destData, true).writeFloat(*noteDivision);
	MemoryOutputStream(destData, true).writeInt(*arpPlayMode);
	MemoryOutputStream(destData, true).writeInt(*arpLatchMode);
	MemoryOutputStream(destData, true).writeInt(*arpLatchLock);
	MemoryOutputStream(destData, true).writeInt(*noteShift);
	MemoryOutputStream(destData, true).writeFloat(*lengthFactor);
	MemoryOutputStream(destData, true).writeFloat(*swingFactor);
}

void Arpeggiator::setStateInformation(const void* data, int sizeInBytes)
{
	noteDivision->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	arpPlayMode->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	lengthFactor->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	arpLatchMode->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	arpLatchLock->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	swingFactor->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	noteShift->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
}