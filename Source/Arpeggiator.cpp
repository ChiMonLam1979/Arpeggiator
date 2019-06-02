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
	noteLengthInSamples = 0;
	numberOfSamplesInBuffer = 0;
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

	notes.currentPlayMode = playMode.currentState;
	notes.noteLengthInSamples = noteLengthInSamples;

	notes.ProcessBuffer(midiMessages);

	midiMessages.clear();

	SetPlayMode();

	if (positionInfo.isPlaying)
	{
		if (notes.ShouldAddNoteOff() || (noteDivisionFactorChanged && notes.lastNoteWasNoteOn))
		{
			notes.noteOffOccursInSameBufferAsLastNoteOn = false;
			const auto offsetForNoteOff = notes.CalculateOffsetForNoteOff(numberOfSamplesInBuffer);
			notes.AddNoteOffToBuffer(midiMessages, offsetForNoteOff);
		}

		if(NoteDivisionStartPositionAsInt <= NoteDivisionEndPositionAsInt)
		{
			noteOnOffset = CalculateNoteOnOffset(NoteDivisionStartPositionAsInt, EvenNoteDivisionStartPosition);
			notes.AddNotes(midiMessages, numberOfSamplesInBuffer, noteOnOffset);
		}

		if(OddNoteDivisionStartPositionAsInt <= OddNoteDivisionEndPositionAsInt)
		{
			noteOnOffset = CalculateNoteOnOffset(OddNoteDivisionStartPositionAsInt, OddNoteDivisionStartPosition);
			notes.AddNotes(midiMessages, numberOfSamplesInBuffer, noteOnOffset);
		}

		notes.samplesFromLastNoteOnUntilBufferEnds = (notes.samplesFromLastNoteOnUntilBufferEnds + numberOfSamplesInBuffer);

		noteDivision.Set();
	}

	if (!positionInfo.isPlaying)
	{
		midiMessages.clear();

		notes.toPlay.clear();

		notes.toPlayLatchMode.clear();

		notes.InitializeNoteIndex();
	}
}

int Arpeggiator::CalculateNoteOnOffset(int beatPos, double notePos) const
{
	return (int)(samplesPerNoteDivisionHalved * (beatPos - notePos));
}

void Arpeggiator::SetPlayMode()
{
	playMode.Set();

	notes.currentPlayMode = playMode.currentState;

	notes.SortNotesToPlay();

	if (playMode.stateHasChanged || !notes.AnyNotesToPlay())
	{
		notes.InitializeNoteIndex();
	}
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