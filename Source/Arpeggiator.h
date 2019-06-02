#pragma once
#include "Parameters.h"
#include "ArpPlayMode.h"
#include "LatchMode.h"
#include "NoteDivision.h"
#include "LatchLock.h"

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
	
		AudioParameterFloat* lengthFactor = new AudioParameterFloat
		{
			IDs::NoteLengthId,
			ParameterNames::NoteLengthName,
			0.1f,
			1.0f,
			0.5f
		};
	
		AudioParameterFloat* swingFactor = new AudioParameterFloat
		{
			IDs::SwingPercentageId,
			ParameterNames::SwingPercentageName,
			0.0f,
			1.0f,
			0.0f
		};
	
		AudioParameterInt* noteShift = new AudioParameterInt
		{
			IDs::NoteShiftId,
			ParameterNames::NoteShiftName,
			-32,
			32,
			0
		};


	std::vector<int> notesToPlay;
	std::vector<int> notesToPlayLatchMode;
	bool lastNoteWasNoteOn;
	bool noteOffRequiredThisBuffer;
	bool noteOffOccursInSameBufferAsLastNoteOn;
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
	void InitializeNoteIndex();
	void AddNoteOffToBuffer(MidiBuffer& midiMessages, const int offset);
	void AddNoteOnToBuffer(MidiBuffer& midiMessages, const int offset);
	void UpdateNoteValue();
	bool AnyNotesToPlay() const;
	bool ShouldAddNoteOff() const;
	bool ShouldAddNoteOn() const;
	int CalculateOffsetForNoteOff(int noteOnOffset = 0) const;
	int GetNumberOfNotesToPlay() const;
	int SetLastNoteValue();
	void UpdateNotesToPlay();
	bool IsTransposeEnabled() const;
	void SetNoteRecieveMode();
	int CalculateNoteOnOffset(int beatPos, double notePos) const;
	void AddNotes(MidiBuffer& midiMessages);

	NoteDivision noteDivision;
	LatchMode latchMode;
	ArpPlayMode playMode;
	LatchLock latchLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};
