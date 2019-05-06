#pragma once
#define NOTE_DIVISION_ID "noteDivisions"
#define NOTE_DIVISION_NAME "Note Division"
#define ARP_PLAYMODE_ID "arpPlayMode"
#define ARP_PLAYMODE_NAME "Play Mode"
#define PLAYMODE_UP "Up"
#define PLAYMODE_DOWN "Down"
#define PLAYMODE_RANDOM "Random"
#define PLAYMODE_PLAYED "Played"
#define NOTE_LENGTH_ID "noteLength"
#define NOTE_LENGTH_NAME "Note Length"
#define QUARTER_NOTE_DIVISION "1/4 Note"
#define EIGHTH_NOTE_DIVISION "1/8 Note"
#define EIGHTH_NOTE_TRIPLET_DIVISION "1/8 Note Triplet"
#define SIXTEENTH_NOTE_DIVISION "1/16 Note"
#define SIXTEENTH_NOTE_TRIPLET_DIVISION "1/16 Note Triplet"
#define THIRTY_SECOND_NOTE_DIVISION "1/32 Note"
#define THIRTY_SECOND_NOTE_TRIPLET_DIVISION "1/32 Note Triplet"
#define LATCH_MODE_ID "latchMode"
#define LATCH_MODE_NAME "Latch Mode"
#define LATCH_MODE_OFF "Latch Off"
#define LATCH_MODE_ON "Latch On"

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

	AudioParameterChoice* noteDivision = new AudioParameterChoice
	{
		NOTE_DIVISION_ID, NOTE_DIVISION_NAME,
		{ 
			QUARTER_NOTE_DIVISION,
			EIGHTH_NOTE_DIVISION,
			EIGHTH_NOTE_TRIPLET_DIVISION,
			SIXTEENTH_NOTE_DIVISION,
			SIXTEENTH_NOTE_TRIPLET_DIVISION,
			THIRTY_SECOND_NOTE_DIVISION,
			THIRTY_SECOND_NOTE_TRIPLET_DIVISION
		},
		0
	};

	AudioParameterChoice* arpPlayMode = new AudioParameterChoice
	{ 
		ARP_PLAYMODE_ID,
		ARP_PLAYMODE_NAME,
		{
			PLAYMODE_UP,
			PLAYMODE_DOWN,
			PLAYMODE_RANDOM,
			PLAYMODE_PLAYED 
		},
		0
	};

	AudioParameterChoice* arpLatchMode = new AudioParameterChoice
	{
		LATCH_MODE_ID,
		LATCH_MODE_NAME,
		{
			LATCH_MODE_OFF,
			LATCH_MODE_ON,
		},
		0
	};

	AudioParameterFloat* lengthFactor = new AudioParameterFloat
	{ 
		NOTE_LENGTH_ID,
		NOTE_LENGTH_NAME,
		0.1f,
		0.9f,
		0.5f 
	};

	std::map<juce::String, int> noteDivisionDictionary =
	{
		{ QUARTER_NOTE_DIVISION, 1 },
		{ EIGHTH_NOTE_DIVISION, 2 },
		{ EIGHTH_NOTE_TRIPLET_DIVISION, 3 },
		{ SIXTEENTH_NOTE_DIVISION, 4 },
		{ SIXTEENTH_NOTE_TRIPLET_DIVISION, 6 },
		{ THIRTY_SECOND_NOTE_DIVISION, 8 },
		{ THIRTY_SECOND_NOTE_TRIPLET_DIVISION, 12 }
	};

	std::vector<int> notesToPlay;
	std::vector<int> notesToPlayLatchMode;
	enum playMode { up, down, random, played };
	enum latchMode { off, on };
	playMode selectedPlayMode;
	playMode currentPlayMode;
	latchMode currentLatchMode;
	latchMode selectedLatchMode;
	int noteDivisionFactor;
	bool noteDivisionFactorChanged;
	bool lastNoteWasNoteOn;
	bool noteOffRequiredThisBuffer;
	bool noteOffOccursInSameBufferAsLastNoteOn;
	bool playModeHasChanged;
	int currentNoteIndex;
	int lastNoteValue;
	int noteLengthInSamples;
	int samplesFromLastNoteOnUntilBufferEnds;
	int numberOfSamplesInBuffer;
	int numberOfNotesToPlay;
	double rate;
	void UpdateNoteDivision();
	void UpdateOrderOfNotesToPlay();
	void SetPlayMode();
	void SetLatchMode();
	void UpdateCurrentNoteIndex();
	void AddNoteOffToBuffer(MidiBuffer& midiMessages, const int offset);
	void AddNoteOnToBuffer(MidiBuffer& midiMessages, const int offset);
	void UpdateNoteValue();
	bool AnyNotesToPlay() const;
	bool NoteOffIsRequiredThisBuffer() const;
	int CalculateOffsetForNoteOff(int noteOnOffset = 0) const;
	bool LatchModeIsOff() const;
	int GetNumberOfNotesToPlay() const;
	int SetLastNoteValue();
	void UpdateNotesToPlayIfLatched();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};