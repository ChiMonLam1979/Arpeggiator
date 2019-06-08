#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "LatchMode.h"
#include "LatchLock.h"
#include "ArpPlayMode.h"

class Notes
{
public:

	Notes(LatchMode& latchMode, LatchLock& latchLock, ArpPlayMode& playMode);

	~Notes();

	void ProcessBuffer(MidiBuffer& midiMessages);
	bool ShouldAddNoteOn();
	bool ShouldAddNoteOff() const;
	void AddNotes(MidiBuffer& midiMessages, int noteOnOffset);
	void AddNoteOff(MidiBuffer& midiMessages);
	void Reset();

	std::vector<int> notes;
	std::vector<int> notesLatched;
	Enums::playMode currentPlayMode { Enums::playMode::up };

	bool lastNoteWasNoteOn { false };
	int noteLengthInSamples { 0 };
	int numberOfSamplesInBuffer { 0 };
	int samplesFromLastNoteOnUntilBufferEnds { 0 };

private:

	void AssignLatchedNotes();
	void SortNotes();
	int SetLastNoteValue();
	void GetNumberOfNotes();
	void InitializeNoteIndex();
	void UpdateNoteValue();
	bool AnyNotesToPlay();
	bool TransposeIsEnabled() const;
	void AddNoteOnToBuffer(MidiBuffer& midiMessages, const int offset);
	void AddNoteOffToBuffer(MidiBuffer& midiMessages, const int offset);
	int CalculateOffsetForNoteOff(int noteOnOffset = 0) const;
	void PrepareToProcess();

	LatchMode& latchMode;
	LatchLock& latchLock;
	ArpPlayMode& playMode;
	bool latchIsEnabled { false };
	int currentNoteIndex { -1 };
	int numberOfNotesToPlay { 0 };
	int lastNoteValue { 0 };
	bool noteOffOccursInSameBufferAsLastNoteOn { false };
};