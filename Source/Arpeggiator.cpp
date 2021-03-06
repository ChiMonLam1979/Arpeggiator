#include "../JuceLibraryCode/JuceHeader.h"
#include "Arpeggiator.h"
#include "ArpeggiatorEditor.h"

Arpeggiator::Arpeggiator() : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo(), true)),
treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
{
	treeState.state = ValueTree(IDs::TreeStateId);
	treeState.addParameterListener(IDs::NoteDivisionId, &noteDivision);
	treeState.addParameterListener(IDs::ArpModeId, &arpMode);
	treeState.addParameterListener(IDs::LatchModeId, &latchMode);
	treeState.addParameterListener(IDs::LatchLockId, &latchLock);

	treeState.addParameterListener(IDs::Slot1Id, &slotController);
	treeState.addParameterListener(IDs::Slot2Id, &slotController);
	treeState.addParameterListener(IDs::Slot3Id, &slotController);
	treeState.addParameterListener(IDs::Slot4Id, &slotController);

	treeState.addParameterListener(IDs::Slot1PlayCountId, &slotPlayCountService);
	treeState.addParameterListener(IDs::Slot2PlayCountId, &slotPlayCountService);
	treeState.addParameterListener(IDs::Slot3PlayCountId, &slotPlayCountService);
	treeState.addParameterListener(IDs::Slot4PlayCountId, &slotPlayCountService);

	treeState.addParameterListener(IDs::Slot1OrderId, &slotOrderService);
	treeState.addParameterListener(IDs::Slot2OrderId, &slotOrderService);
	treeState.addParameterListener(IDs::Slot3OrderId, &slotOrderService);
	treeState.addParameterListener(IDs::Slot4OrderId, &slotOrderService);

	treeState.addParameterListener(IDs::PatternModeId, &patternMode);
}

AudioProcessorValueTreeState::ParameterLayout Arpeggiator::createParameterLayout()
{
	std::vector<std::unique_ptr<RangedAudioParameter>> parameters;

	auto noteDivionParameter = std::make_unique<AudioParameterChoice>(IDs::NoteDivisionId, ParameterNames::NoteDivisionName, ParameterChoices::NoteDivisionChoices, 0);
	auto arpModeParameter = std::make_unique<AudioParameterChoice>(IDs::ArpModeId, ParameterNames::ArpModeName, ParameterChoices::ArpModeChoices, 0);
	auto latchModeParameter = std::make_unique<AudioParameterChoice>(IDs::LatchModeId, ParameterNames::LatchModeName, ParameterChoices::LatchModeChoices, 0);
	auto latchLockParameter = std::make_unique<AudioParameterChoice>(IDs::LatchLockId, ParameterNames::LatchLockName, ParameterChoices::LatchLockChoices, 0);
	auto noteLengthParameter = std::make_unique<AudioParameterFloat>(IDs::NoteLengthId, ParameterNames::NoteLengthName, ParameterRanges::NoteLengthRange, 0.5f);
	auto swingFactorParameter = std::make_unique<AudioParameterFloat>(IDs::SwingFactorId, ParameterNames::SwingFactorName, ParameterRanges::SwingFactorRange, 0.0f);
	auto noteShiftParameter = std::make_unique<AudioParameterFloat>(IDs::NoteShiftId, ParameterNames::NoteShiftName, -32.0, 32.0, 0);

	auto slot1Parameter = std::make_unique<AudioParameterBool>(IDs::Slot1Id, ParameterNames::Slot1Name, 0);
	auto slot2Parameter = std::make_unique<AudioParameterBool>(IDs::Slot2Id, ParameterNames::Slot2Name, 0);
	auto slot3Parameter = std::make_unique<AudioParameterBool>(IDs::Slot3Id, ParameterNames::Slot3Name, 0);
	auto slot4Parameter = std::make_unique<AudioParameterBool>(IDs::Slot4Id, ParameterNames::Slot4Name, 0);

	auto slot1RepeatParameter = std::make_unique<AudioParameterFloat>(IDs::Slot1PlayCountId, ParameterNames::Slot1PlayCountName, 0.0, 4.0, 0);
	auto slot2RepeatParameter = std::make_unique<AudioParameterFloat>(IDs::Slot2PlayCountId, ParameterNames::Slot2PlayCountName, 0.0, 4.0, 0);
	auto slot3RepeatParameter = std::make_unique<AudioParameterFloat>(IDs::Slot3PlayCountId, ParameterNames::Slot3PlayCountName, 0.0, 4.0, 0);
	auto slot4RepeatParameter = std::make_unique<AudioParameterFloat>(IDs::Slot4PlayCountId, ParameterNames::Slot4PlayCountName, 0.0, 4.0, 0);

	auto slot1OrderParameter = std::make_unique<AudioParameterFloat>(IDs::Slot1OrderId, ParameterNames::Slot1OrderName, 1.0, 4.0, 1);
	auto slot2OrderParameter = std::make_unique<AudioParameterFloat>(IDs::Slot2OrderId, ParameterNames::Slot2OrderName, 1.0, 4.0, 2);
	auto slot3OrderParameter = std::make_unique<AudioParameterFloat>(IDs::Slot3OrderId, ParameterNames::Slot3OrderName, 1.0, 4.0, 3);
	auto slot4OrderParameter = std::make_unique<AudioParameterFloat>(IDs::Slot4OrderId, ParameterNames::Slot4OrderName, 1.0, 4.0, 4);

	auto patternModeParameter = std::make_unique<AudioParameterChoice>(IDs::PatternModeId, ParameterNames::PatternModeName, ParameterChoices::PatternModeChoices, 0);

	parameters.push_back(std::move(noteDivionParameter));
	parameters.push_back(std::move(arpModeParameter));
	parameters.push_back(std::move(latchModeParameter));
	parameters.push_back(std::move(latchLockParameter));
	parameters.push_back(std::move(noteLengthParameter));
	parameters.push_back(std::move(swingFactorParameter));
	parameters.push_back(std::move(noteShiftParameter));

	parameters.push_back(std::move(slot1Parameter));
	parameters.push_back(std::move(slot2Parameter));
	parameters.push_back(std::move(slot3Parameter));
	parameters.push_back(std::move(slot4Parameter));

	parameters.push_back(std::move(slot1RepeatParameter));
	parameters.push_back(std::move(slot2RepeatParameter));
	parameters.push_back(std::move(slot3RepeatParameter));
	parameters.push_back(std::move(slot4RepeatParameter));

	parameters.push_back(std::move(slot1OrderParameter));
	parameters.push_back(std::move(slot2OrderParameter));
	parameters.push_back(std::move(slot3OrderParameter));
	parameters.push_back(std::move(slot4OrderParameter));

	parameters.push_back(std::move(patternModeParameter));

	return { parameters.begin(), parameters.end() };
}

AudioProcessorEditor* Arpeggiator::createEditor()
{
	return new ArpeggiatorEditor(*this);
}

void Arpeggiator::prepareToPlay(double sampleRate, int)
{
	rate = sampleRate;
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

	lengthFactor = treeState.getRawParameterValue(IDs::NoteLengthId);
	const auto NoteLengthInSamplesAsInt = std::ceil(samplesPerNoteDivision * *lengthFactor);
	const auto samplesPer128thNote = (samplesPerQuarterNote / 32);
	noteLengthInSamples = jmax(NoteLengthInSamplesAsInt, samplesPer128thNote);

	numberOfSamplesInBuffer = buffer.getNumSamples();
	shiftFactor = treeState.getRawParameterValue(IDs::NoteShiftId);

	noteLength = (0.5 * *lengthFactor);
	maxSwingPPQ = (0.5 - noteLength);

	// start position of the current buffer in quarter note ticks with respect to host timeline
	const double partsPerQuarterNoteStartPosition = positionInfo.ppqPosition + (PPQ128th * *shiftFactor);

	swingFactor = treeState.getRawParameterValue(IDs::SwingFactorId);
	const auto SwingOffset = maxSwingPPQ * *swingFactor;
	const auto oddNoteOffset = 0.5;
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
		notes.samplesFromLastNoteOnUntilBufferEnds = (notes.samplesFromLastNoteOnUntilBufferEnds + numberOfSamplesInBuffer);

		if (notes.ShouldAddNoteOff() || (noteDivision.StateChanged() && notes.lastNoteWasNoteOn))
		{
			notes.AddNoteOff(midiMessages);
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