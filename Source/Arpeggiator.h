#pragma once
#include "Parameters.h"
#include "ArpPlayMode.h"
#include "LatchMode.h"
#include "NoteDivision.h"
#include "LatchLock.h"
#include "Notes.h"

class Arpeggiator : public AudioProcessor
{
public:

	Arpeggiator();
	~Arpeggiator() {}

	void prepareToPlay(double sampleRate, int) override;
	void releaseResources() override {}
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
	bool isMidiEffect() const override { return true; }

	AudioProcessorEditor*  createEditor() override;
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

	AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	AudioProcessorValueTreeState treeState;

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

	int noteLengthInSamples;
	int samplesFromLastNoteOnUntilBufferEnds;
	int numberOfSamplesInBuffer;
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
	float noteDivisionFactor;
	int CalculateNoteOnOffset(int beatPos, double notePos) const;

	NoteDivision noteDivision;
	LatchMode latchMode;
	ArpPlayMode playMode;
	LatchLock latchLock;
	Notes notes{ latchMode, latchLock, playMode};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};