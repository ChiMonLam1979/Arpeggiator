#pragma once
#include "Parameters.h"
#include "NoteDivisionHandler.h"

class Arpeggiator : public AudioProcessor
{
public:

	Arpeggiator();
	~Arpeggiator() {}

	void prepareToPlay(double sampleRate, int) override;
	void releaseResources() override {}
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
	bool isMidiEffect() const override { return true; }

	AudioProcessorEditor*  createEditor() override { return new GenericAudioProcessorEditor(this); }
	bool hasEditor() const override { return true; }

	const String getName() const override { return "Arpeggiator"; }

	bool acceptsMidi() const override { return true; }
	bool producesMidi() const override { return true; }
	double getTailLengthSeconds() const override { return 0; }

	int getNumPrograms() override { return 1; }
	int getCurrentProgram() override { return 0; }
	void setCurrentProgram(int) override {}
	const String getProgramName(int) override { return {}; }
	void changeProgramName(int, const String&) override {}

	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

private:
	
		AudioParameterChoice* arpPlayMode = new AudioParameterChoice
		{
			IDs::arpPlayModeId,
			ParameterNames::arpPlayModeName,
			{
				ParamterChoices::playModeUp,
				ParamterChoices::playModeDown,
				ParamterChoices::playModeRandom,
				ParamterChoices::playModePlayed
			},
			0
		};
	
		AudioParameterChoice* arpLatchMode = new AudioParameterChoice
		{
			IDs::latchModeId,
			ParameterNames::latchModeName,
			{
				ParamterChoices::latchModeOff,
				ParamterChoices::latchModeOn,
			},
			0
		};
	
		AudioParameterChoice* arpLatchLock = new AudioParameterChoice
		{
			IDs::latchLockId,
			ParameterNames::latchLockName,
			{
				ParamterChoices::latchLockOff,
				ParamterChoices::latchLockOn
			},
			0
		};
	
		AudioParameterFloat* lengthFactor = new AudioParameterFloat
		{
			IDs::noteLengthId,
			ParameterNames::noteLengthName,
			0.1f,
			1.0f,
			0.5f
		};
	
		AudioParameterFloat* swingFactor = new AudioParameterFloat
		{
			IDs::swingPercentageId,
			ParameterNames::swingPercentageName,
			0.0f,
			1.0f,
			0.0f
		};
	
		AudioParameterInt* noteShift = new AudioParameterInt
		{
			IDs::noteShiftId,
			ParameterNames::noteShiftName,
			-32,
			32,
			0
		};


	std::vector<int> notesToPlay;
	std::vector<int> notesToPlayLatchMode;
	enum playMode { up, down, random, played };
	enum latchMode { off, on };
	enum latchLock {unlocked, locked };
	playMode selectedPlayMode;
	playMode currentPlayMode;
	latchMode currentLatchMode;
	latchMode selectedLatchMode;
	latchLock latchLockState;
	bool lastNoteWasNoteOn;
	bool noteOffRequiredThisBuffer;
	bool noteOffOccursInSameBufferAsLastNoteOn;
	bool playModeHasChanged;
	bool latchModeHasChanged;
	bool latchIsLocked;
	bool transposeIsEnabled;
	int currentNoteIndex;
	int lastNoteValue;
	int noteLengthInSamples;
	int samplesFromLastNoteOnUntilBufferEnds;
	int numberOfSamplesInBuffer;
	int numberOfNotesToPlay;
	int shiftFactor;
	double rate;
	const double PPQ128th = 0.03125;
	double samplesPerNoteDivision;
	double samplesPerQuarterNote;
	double noteLength;
	double maxSwingPPQ;
	int noteOnOffset;
	float noteDivisionFactorHalved;
	double samplesPerNoteDivisionHalved;
	void SortNotesToPlay();
	void SetPlayMode();
	void SetLatchMode();
	void InitializeNoteIndex();
	void AddNoteOffToBuffer(MidiBuffer& midiMessages, const int offset);
	void AddNoteOnToBuffer(MidiBuffer& midiMessages, const int offset);
	void UpdateNoteValue();
	bool AnyNotesToPlay() const;
	bool ShouldAddNoteOff() const;
	bool ShouldAddNoteOn() const;
	int CalculateOffsetForNoteOff(int noteOnOffset = 0) const;
	bool IsLatchModeOff() const;
	int GetNumberOfNotesToPlay() const;
	int SetLastNoteValue();
	void UpdateNotesToPlay();
	bool IsTransposeEnabled() const;
	void SetNoteRecieveMode();
	int CalculateNoteOnOffset(int beatPos, double notePos) const;
	void AddNotes(MidiBuffer& midiMessages);

	NoteDivionHandler noteDivisionHandler;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};
