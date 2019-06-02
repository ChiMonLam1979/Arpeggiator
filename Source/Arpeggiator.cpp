#include "../JuceLibraryCode/JuceHeader.h"
#include "Arpeggiator.h"
#include "Extensions.h"

Arpeggiator::Arpeggiator() : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo(), true))
{
	addParameter(noteDivision.GetParameter());
	addParameter(playMode.GetParameter());
	addParameter(lengthFactor);
	addParameter(swingFactor);
	addParameter(latchMode.GetParameter());
	addParameter(latchLock.GetParameter());
	addParameter(noteShift);
}

void Arpeggiator::prepareToPlay(double sampleRate, int)
{
	rate = sampleRate;
	currentNoteIndex = -1;
	lastNoteWasNoteOn = false;
	samplesFromLastNoteOnUntilBufferEnds = 0;
	noteLengthInSamples = 0;
	numberOfSamplesInBuffer = 0;
	noteOffRequiredThisBuffer = false;
	transposeIsEnabled = false;
	shiftFactor = 0;
	samplesPerQuarterNote = 0.0;
	samplesPerNoteDivision = 0.0;
	noteLength = 0;
	maxSwingPPQ = 0;
	noteOnOffset = 0;
	noteDivisionFactorHalved = 0;
	samplesPerNoteDivisionHalved = 0;
}

void Arpeggiator::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	auto playHead = getPlayHead();
	if (playHead == nullptr) return;

	AudioPlayHead::CurrentPositionInfo positionInfo{};
	playHead->getCurrentPosition(positionInfo);

	noteDivision.Set();
	const auto noteDivisionFactor = noteDivision.currentFactor;
	const auto noteDivisionFactorChanged = noteDivision.stateChanged;

	const auto quarterNotesPerMinute = positionInfo.bpm;
	const auto quarterNotesPerSecond = quarterNotesPerMinute / 60;
	samplesPerQuarterNote = rate / quarterNotesPerSecond;

	noteDivisionFactorHalved = noteDivisionFactor / 2;
	samplesPerNoteDivision = samplesPerQuarterNote / noteDivisionFactor;
	samplesPerNoteDivisionHalved = samplesPerQuarterNote / noteDivisionFactorHalved;

	const auto samplesPer128thNote = (samplesPerQuarterNote / 32);
	const auto NoteLengthInSamplesAsInt = std::ceil(samplesPerNoteDivision * *lengthFactor);
	noteLengthInSamples = jmax(NoteLengthInSamplesAsInt, samplesPer128thNote);

	numberOfSamplesInBuffer = buffer.getNumSamples();
	shiftFactor = noteShift->get();

	noteLength = (0.5 * *lengthFactor);
	maxSwingPPQ = (0.5 - noteLength);

	// start position of the current buffer in quarter note ticks with respect to host timeline
	const double partsPerQuarterNoteStartPosition = positionInfo.ppqPosition + (PPQ128th * shiftFactor);

	const auto oddNoteOffset = 0.5;
	const auto SwingOffset = maxSwingPPQ * *swingFactor;
	const auto TotalOddNoteOffset = oddNoteOffset + SwingOffset;

	// start position of the current buffer in custom note-divisions ticks with respect to host timeline
	const double OddNoteDivisionStartPosition = (partsPerQuarterNoteStartPosition * noteDivisionFactorHalved) - TotalOddNoteOffset;
	const double EvenNoteDivisionStartPosition = (partsPerQuarterNoteStartPosition * noteDivisionFactorHalved);

	// end position of the current buffer in ticks with respect to host timeline
	const double OddNoteDivisionEndPosition = OddNoteDivisionStartPosition + (numberOfSamplesInBuffer / samplesPerNoteDivisionHalved);
	const double EvenNoteDivisionEndPosition = EvenNoteDivisionStartPosition + (numberOfSamplesInBuffer / samplesPerNoteDivisionHalved);

	// trick to calculate when a new note should occur..everytime start position rounded up = end position rounded down
	const int OddNoteDivisionStartPositionAsInt = std::ceil(OddNoteDivisionStartPosition);
	const int OddNoteDivisionEndPositionAsInt = std::floor(OddNoteDivisionEndPosition);

	const int NoteDivisionStartPositionAsInt = std::ceil(EvenNoteDivisionStartPosition);
	const int NoteDivisionEndPositionAsInt = std::floor(EvenNoteDivisionEndPosition);

	SetNoteRecieveMode();

	notes.latchIsDisabled = latchMode.IsLatchDisabled();
	notes.currentPlayMode = playMode.currentState;

	notes.ProcessBuffer(midiMessages, transposeIsEnabled);

	notes.GetNumberOfNotesToPlay();

	midiMessages.clear();

	SetPlayMode();

	if (positionInfo.isPlaying)
	{
		if (notes.ShouldAddNoteOff(samplesFromLastNoteOnUntilBufferEnds, noteLengthInSamples) || (noteDivisionFactorChanged && notes.lastNoteWasNoteOn))
		{
			noteOffOccursInSameBufferAsLastNoteOn = false;
			const auto offsetForNoteOff = CalculateOffsetForNoteOff();
			AddNoteOffToBuffer(midiMessages, offsetForNoteOff);
		}

		if(NoteDivisionStartPositionAsInt <= NoteDivisionEndPositionAsInt)
		{
			noteOnOffset = CalculateNoteOnOffset(NoteDivisionStartPositionAsInt, EvenNoteDivisionStartPosition);
			AddNotes(midiMessages);
		}

		if(OddNoteDivisionStartPositionAsInt <= OddNoteDivisionEndPositionAsInt)
		{
			noteOnOffset = CalculateNoteOnOffset(OddNoteDivisionStartPositionAsInt, OddNoteDivisionStartPosition);
			AddNotes(midiMessages);
		}

		samplesFromLastNoteOnUntilBufferEnds = (samplesFromLastNoteOnUntilBufferEnds + numberOfSamplesInBuffer);

		noteDivision.Set();
	}

	if (!positionInfo.isPlaying)
	{
		midiMessages.clear();

		notes.toPlay.clear();

		notes.toPlayLatchMode.clear();

		notes.GetNumberOfNotesToPlay();

		notes.InitializeNoteIndex();
	}
}

int Arpeggiator::CalculateNoteOnOffset(int beatPos, double notePos) const
{
	return (int)(samplesPerNoteDivisionHalved * (beatPos - notePos));
}

void Arpeggiator::AddNotes(MidiBuffer& midiMessages)
{
	if (notes.ShouldAddNoteOn())
	{
		notes.UpdateNoteValue();

		AddNoteOnToBuffer(midiMessages, noteOnOffset);
		samplesFromLastNoteOnUntilBufferEnds = numberOfSamplesInBuffer - noteOnOffset;
	}

	if (notes.ShouldAddNoteOff(samplesFromLastNoteOnUntilBufferEnds, noteLengthInSamples))
	{
		noteOffOccursInSameBufferAsLastNoteOn = true;
		const auto offsetForNoteOff = CalculateOffsetForNoteOff(noteOnOffset);
		AddNoteOffToBuffer(midiMessages, offsetForNoteOff);
	}
}

void Arpeggiator::SetNoteRecieveMode()
{
	latchMode.Set();

	latchLock.Set();

	transposeIsEnabled = IsTransposeEnabled();

	notes.UpdateNotesToPlay(latchMode.stateHasChanged);
}

void Arpeggiator::SetPlayMode()
{
	notes.GetNumberOfNotesToPlay();

	playMode.Set();

	notes.currentPlayMode = playMode.currentState;

	notes.SortNotesToPlay();

	if (playMode.stateHasChanged || !notes.AnyNotesToPlay())
	{
		notes.InitializeNoteIndex();
	}
}

void Arpeggiator::AddNoteOffToBuffer(MidiBuffer& midiMessages, const int offset)
{
	midiMessages.addEvent(MidiMessage::noteOff(1, notes.lastNoteValue), offset);
	notes.lastNoteWasNoteOn = false;
}

void Arpeggiator::AddNoteOnToBuffer(MidiBuffer& midiMessages, const int offset)
{
	midiMessages.addEvent(MidiMessage::noteOn(1, notes.lastNoteValue, (uint8)127), offset);
	notes.lastNoteWasNoteOn = true;
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

bool Arpeggiator::IsTransposeEnabled() const
{
	return (latchMode.currentState == Enums::latchMode::on
		&& latchLock.state == Enums::latchLock::locked
		&& !notes.toPlayLatchMode.empty());
}

void Arpeggiator::getStateInformation(MemoryBlock& destData)
{
	MemoryOutputStream(destData, true).writeFloat(*noteDivision.GetParameter());
	MemoryOutputStream(destData, true).writeInt(*playMode.GetParameter());
	MemoryOutputStream(destData, true).writeInt(*latchMode.GetParameter());
	MemoryOutputStream(destData, true).writeInt(*latchLock.GetParameter());
	MemoryOutputStream(destData, true).writeInt(*noteShift);
	MemoryOutputStream(destData, true).writeFloat(*lengthFactor);
	MemoryOutputStream(destData, true).writeFloat(*swingFactor);
}

void Arpeggiator::setStateInformation(const void* data, int sizeInBytes)
{
	noteDivision.GetParameter()->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	playMode.GetParameter()->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	lengthFactor->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	latchMode.GetParameter()->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	latchLock.GetParameter()->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	swingFactor->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	noteShift->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
}