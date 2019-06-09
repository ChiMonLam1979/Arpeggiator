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

	void getStateInformation(MemoryBlock& destData) override {};
	void setStateInformation(const void* data, int sizeInBytes) override {};

	AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	AudioProcessorValueTreeState treeState;

private:

	int noteLengthInSamples { 0 };
	int samplesFromLastNoteOnUntilBufferEnds { 0 };
	int numberOfSamplesInBuffer { 0 };
	double rate { 0 };
	const double PPQ128th { 0.03125 };
	double samplesPerNoteDivision { 0 };
	double samplesPerQuarterNote { 0 };
	double noteLength { 0 };
	double maxSwingPPQ { 0 };
	int noteOnOffset { 0 };
	float noteDivisionFactorHalved { 0 };
	double samplesPerNoteDivisionHalved { 0 };
	float *lengthFactor = nullptr;
	float *swingFactor = nullptr;
	float *shiftFactor = nullptr;


	int CalculateNoteOnOffset(int beatPos, double notePos) const;

	NoteDivision noteDivision;
	LatchMode latchMode;
	ArpPlayMode playMode;
	LatchLock latchLock;
	Notes notes{ latchMode, latchLock, playMode};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};