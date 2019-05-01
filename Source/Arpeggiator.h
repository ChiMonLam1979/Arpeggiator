#pragma once

class Arpeggiator : public AudioProcessor
{
public:

	Arpeggiator();
	~Arpeggiator() {}

	void prepareToPlay(double sampleRate, int) override;
	void releaseResources() override {}
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
	bool isMidiEffect() const override { return true; }

	AudioProcessorEditor* createEditor() override { return new GenericAudioProcessorEditor(this); }
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

	AudioParameterChoice* noteDivision;
	AudioParameterChoice* arpDirection;
	AudioParameterFloat* lengthFactor;
	std::vector<int> notesToPlay;
	std::map<juce::String, int> noteDivisionDictionary = { {"1/4 note", 1}, {"1/16 note", 4}, {"1/32 note", 8} };;
	enum arpPlayDirection { up, down, random, played };
	arpPlayDirection playDirection;
	arpPlayDirection currentPlayDirection;
	int noteDivisionFactor;
	bool noteDivisionFactorChanged;
	bool lastNoteWasNoteOn;
	bool noteOffRequiredThisBuffer;
	bool isSameBufferAsLastNoteOn;
	bool playDirectionHasChanged;
	int currentNoteIndex;
	int lastNoteValue;
	int noteLengthInSamples;
	int samplesFromLastNoteOnUntilBufferEnds;
	int numberOfSamplesInBuffer;
	int numberOfNotesToPlay;
	double rate;
	void UpdateNoteDivision();
	void UpdateOrderOfNotesToPlay();
	void SetArpDirection();
	void UpdateCurrentNoteIndex();
	void AddNoteOffToBuffer(MidiBuffer& midiMessages, const int offset);
	void AddNoteOnToBuffer(MidiBuffer& midiMessages, const int offset);
	void UpdateNoteValue();
	bool AnyNotesToPlay() const;
	bool NoteOffIsRequiredThisBuffer() const;
	int CalculateOffsetForNoteOff(int noteOnOffset = 0) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};