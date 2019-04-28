#include "../JuceLibraryCode/JuceHeader.h"
#include "Arpeggiator.h"

Arpeggiator::Arpeggiator(): AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo(), true))
{
	addParameter(noteDivision = new AudioParameterChoice("noteDivisions", "Note Divisions", { "1/4 note", "1/16 note", "1/32 note" }, 0));
	addParameter(arpDirection = new AudioParameterChoice("arpDirection", "Arp Direction", { "Up", "Down", "Random" }, 0));
	addParameter(lengthFactor = new AudioParameterFloat("length", "Note Length", 0.1f, 0.9f, 0.5f));
}

void Arpeggiator::prepareToPlay(double sampleRate, int)
{
	notesToPlay.clear();
	rate = sampleRate;
	currentNoteIndex = -1;
	lastNoteValue = 0;
	lastNoteWasNoteOn = false;
	samplesFromLastNoteOnUntilBufferEnds = 0;
	noteLengthInSamples = 0;
	numberOfSamplesInBuffer = 0;
	noteOffRequiredThisBuffer = false;
	notesToPlay.ensureStorageAllocated(100);
	numberOfNotesToPlay = 0;
	playDirection = up;
	currentPlayDirection = up;
	playDirectionHasChanged = false;
	noteDivisionFactor = 1;
	noteDivisionFactorChanged = false;
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

	const double partsPerQuarterNoteStartPosition = positionInfo.ppqPosition;
	const double NoteDivisionStartPosition = partsPerQuarterNoteStartPosition * noteDivisionFactor;
	const double NoteDivisionEndPosition = NoteDivisionStartPosition + (numberOfSamplesInBuffer / samplesPerNoteDivision);
	// trick to calculate when a new note should occur..
	const int NoteDivisionStartPositionAsInt = std::ceil(NoteDivisionStartPosition);
	const int NoteDivisionEndPositionAsInt = std::floor(NoteDivisionEndPosition);

	MidiMessage message;
	int ignore;

	for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(message, ignore);)
	{
		if (message.isNoteOn())
		{
			notesToPlay.add(message.getNoteNumber());
		}
		else if (message.isNoteOff())
		{
			notesToPlay.removeValue(message.getNoteNumber());
		}
	}

	numberOfNotesToPlay = notesToPlay.size();

	midiMessages.clear();

	GetArpDirection();

	if (positionInfo.isPlaying)
	{
		for (int i = NoteDivisionStartPositionAsInt; i <= NoteDivisionEndPositionAsInt; ++i)
		{
			const int noteOnOffset = (int)samplesPerNoteDivision * (i - NoteDivisionStartPosition);

			if (AnyNotesToPlay())
			{
				UpdateNoteValue();
				AddNoteOnToBuffer(midiMessages, noteOnOffset);
				samplesFromLastNoteOnUntilBufferEnds = numberOfSamplesInBuffer - noteOnOffset;
			}

			if (NoteOffIsRequiredThisBuffer())
			{
				isSameBufferAsLastNoteOn = true;
				const auto offsetForNoteOff = CalculateOffsetForNoteOff(noteOnOffset);
				AddNoteOffToBuffer(midiMessages, offsetForNoteOff);
			}
		}

		samplesFromLastNoteOnUntilBufferEnds = (samplesFromLastNoteOnUntilBufferEnds + numberOfSamplesInBuffer);

		UpdateNoteDivision();

		if ((NoteOffIsRequiredThisBuffer() || noteDivisionFactorChanged) && lastNoteWasNoteOn)
		{
			isSameBufferAsLastNoteOn = false;
			const auto offsetForNoteOff = CalculateOffsetForNoteOff();
			AddNoteOffToBuffer(midiMessages, offsetForNoteOff);
		}
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

void Arpeggiator::GetArpDirection()
{
	playDirection = static_cast<arpPlayDirection>(arpDirection->getIndex());

	playDirectionHasChanged = currentPlayDirection != playDirection;
	if(playDirectionHasChanged)
	{
		currentPlayDirection = playDirection;

		UpdateCurrentNoteIndex();
	}
}

void Arpeggiator::UpdateCurrentNoteIndex()
{
	const auto lastIndexOfNotesToPlay = numberOfNotesToPlay - 1;

	switch (currentPlayDirection)
	{
	case up: currentNoteIndex = -1;
		break;
	case down: currentNoteIndex = lastIndexOfNotesToPlay;
		break;
	case random: currentNoteIndex = Random::getSystemRandom().nextInt(Range<int>(0, lastIndexOfNotesToPlay + 1));
	}
}

void Arpeggiator::UpdateNoteValue()
{
	switch (currentPlayDirection)
	{
	case up: currentNoteIndex = (currentNoteIndex + 1) % numberOfNotesToPlay;
		break;
	case down:currentNoteIndex = currentNoteIndex > 0 ? (currentNoteIndex - 1) % numberOfNotesToPlay : numberOfNotesToPlay - 1;
		break;
	case random: currentNoteIndex = Random::getSystemRandom().nextInt(Range<int>(0, numberOfNotesToPlay));
	}

	lastNoteValue = notesToPlay[currentNoteIndex];
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
	return samplesFromLastNoteOnUntilBufferEnds >= noteLengthInSamples;
}

int Arpeggiator::CalculateOffsetForNoteOff(int noteOnOffset) const
{
	const auto startOfLastSampleInBuffer = numberOfSamplesInBuffer - 1;
	if(isSameBufferAsLastNoteOn)
	{
		const auto offset = noteOnOffset + noteLengthInSamples;
		return jmin(offset, startOfLastSampleInBuffer);
	}
	else
	{
		const auto offset = numberOfSamplesInBuffer - (samplesFromLastNoteOnUntilBufferEnds - noteLengthInSamples);
		return jmin(offset, startOfLastSampleInBuffer);
	}
}

bool Arpeggiator::AnyNotesToPlay() const
{
	return notesToPlay.size() > 0;
}

void Arpeggiator::getStateInformation(MemoryBlock& destData)
{
	MemoryOutputStream(destData, true).writeInt(*noteDivision);
	MemoryOutputStream(destData, true).writeInt(*arpDirection);
	MemoryOutputStream(destData, true).writeFloat(*lengthFactor);
}

void Arpeggiator::setStateInformation(const void* data, int sizeInBytes)
{
	noteDivision->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	arpDirection->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	lengthFactor->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
}