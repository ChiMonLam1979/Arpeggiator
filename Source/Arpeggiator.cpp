#include "../JuceLibraryCode/JuceHeader.h"
#include "Arpeggiator.h"
#include "ArpeggiatorEditor.h"

Arpeggiator::Arpeggiator() : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo(), true)),
treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
{
	treeState.state = ValueTree(IDs::TreeStateId);
	treeState.addParameterListener(IDs::NoteDivisionId, &noteDivision);
	treeState.addParameterListener(IDs::ArpPlayModeId, &playMode);
	treeState.addParameterListener(IDs::LatchModeId, &latchMode);
	treeState.addParameterListener(IDs::LatchLockId, &latchLock);

	addParameter(lengthFactor);
	addParameter(swingFactor);
	addParameter(noteShift);
}

AudioProcessorValueTreeState::ParameterLayout Arpeggiator::createParameterLayout()
{
	std::vector<std::unique_ptr<RangedAudioParameter>> parameters;

	auto noteDivionParameter = std::make_unique<AudioParameterChoice>(IDs::NoteDivisionId, ParameterNames::NoteDivisionName, ParamterChoices::noteDivisionChoices, 0);
	auto playModeParameter = std::make_unique<AudioParameterChoice>(IDs::ArpPlayModeId, ParameterNames::ArpPlayModeName, ParamterChoices::playModeChoices, 0);
	auto latchModeParameter = std::make_unique<AudioParameterChoice>(IDs::LatchModeId, ParameterNames::LatchModeName, ParamterChoices::latchModeChoices, 0);
	auto latchLockParameter = std::make_unique<AudioParameterChoice>(IDs::LatchLockId, ParameterNames::LatchLockName, ParamterChoices::latchLockChoices, 0);

	parameters.push_back(std::move(noteDivionParameter));
	parameters.push_back(std::move(playModeParameter));
	parameters.push_back(std::move(latchModeParameter));
	parameters.push_back(std::move(latchLockParameter));

	return { parameters.begin(), parameters.end() };
}

AudioProcessorEditor* Arpeggiator::createEditor()
{
	return new ArpeggiatorEditor(*this);
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

	const auto noteDivisionFactor = noteDivision.factor;

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

	notes.noteLengthInSamples = noteLengthInSamples;
	notes.numberOfSamplesInBuffer = numberOfSamplesInBuffer;

	notes.ProcessBuffer(midiMessages);

	midiMessages.clear();

	if (positionInfo.isPlaying)
	{
		if (notes.ShouldAddNoteOff() || (noteDivision.stateHasChanged && notes.lastNoteWasNoteOn))
		{
			notes.AddNoteOff(midiMessages);
			noteDivision.stateHasChanged = false;
		}

		if(NoteDivisionStartPositionAsInt <= NoteDivisionEndPositionAsInt)
		{
			noteOnOffset = CalculateNoteOnOffset(NoteDivisionStartPositionAsInt, EvenNoteDivisionStartPosition);
			notes.AddNotes(midiMessages, noteOnOffset);
		}

		if(OddNoteDivisionStartPositionAsInt <= OddNoteDivisionEndPositionAsInt)
		{
			noteOnOffset = CalculateNoteOnOffset(OddNoteDivisionStartPositionAsInt, OddNoteDivisionStartPosition);
			notes.AddNotes(midiMessages, noteOnOffset);
		}

		notes.samplesFromLastNoteOnUntilBufferEnds = (notes.samplesFromLastNoteOnUntilBufferEnds + numberOfSamplesInBuffer);
	}

	if (!positionInfo.isPlaying)
	{
		midiMessages.clear();

		notes.Reset();
	}
}

int Arpeggiator::CalculateNoteOnOffset(int beatPos, double notePos) const
{
	return static_cast<int>(samplesPerNoteDivisionHalved * (beatPos - notePos));
}

void Arpeggiator::getStateInformation(MemoryBlock& destData)
{
	MemoryOutputStream(destData, true).writeInt(*noteShift);
	MemoryOutputStream(destData, true).writeFloat(*lengthFactor);
	MemoryOutputStream(destData, true).writeFloat(*swingFactor);
}

void Arpeggiator::setStateInformation(const void* data, int sizeInBytes)
{
	lengthFactor->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	swingFactor->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	noteShift->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
}