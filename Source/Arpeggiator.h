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
#define LATCH_LOCK_ID "latchLock"
#define LATCH_LOCK_NAME "Latch Lock"
#define LATCH_LOCK_OFF "Latch Lock Off"
#define LATCH_LOCK_ON "Latch Lock On"
#define NOTE_SHIFT_ID "noteShift"
#define NOTE_SHIFT_NAME "Note Shift"
#define SWING_PERCENTAGE_ID "swingAmount"
#define SWING_PERCENTAGE_NAME "Swing Amount"

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

	AudioParameterChoice* arpLatchLock = new AudioParameterChoice
	{
		LATCH_LOCK_ID,
		LATCH_LOCK_NAME,
		{
			LATCH_LOCK_OFF,
			LATCH_LOCK_ON
		},
		0
	};

	AudioParameterFloat* lengthFactor = new AudioParameterFloat
	{ 
		NOTE_LENGTH_ID,
		NOTE_LENGTH_NAME,
		0.1f,
		1.0f,
		0.5f 
	};

	AudioParameterFloat* swingFactor = new AudioParameterFloat
	{
		SWING_PERCENTAGE_ID,
		SWING_PERCENTAGE_NAME,
		0.0f,
		1.0f,
		0.0f
	};

	AudioParameterInt* noteShift = new AudioParameterInt
	{
		NOTE_SHIFT_ID,
		NOTE_SHIFT_NAME,
		-32,
		32,
		0
	};

	std::map<juce::String, float> noteDivisionDictionary =
	{
		{ QUARTER_NOTE_DIVISION, 1.0f },
		{ EIGHTH_NOTE_DIVISION, 2.0f },
		{ EIGHTH_NOTE_TRIPLET_DIVISION, 3.0f },
		{ SIXTEENTH_NOTE_DIVISION, 4.0f },
		{ SIXTEENTH_NOTE_TRIPLET_DIVISION, 6.0f },
		{ THIRTY_SECOND_NOTE_DIVISION, 8.0f },
		{ THIRTY_SECOND_NOTE_TRIPLET_DIVISION, 12.0f }
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
	float noteDivisionFactor;
	bool noteDivisionFactorChanged;
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
	void UpdateNoteDivision();
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
	const double PPQ128th = 0.03125;
	double samplesPerNoteDivision;
	double samplesPerQuarterNote;
	double noteDivisionLengthPPQ;
	double noteLength;
	double maxSwingPPQ;
	int CalculateNoteOnOffset(int beatPos, double notePos) const;
	int noteOnOffset;
	void AddNotes(MidiBuffer& midiMessages);
	float noteDivisionFactorHalved;
	double samplesPerNoteDivisionHalved;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};