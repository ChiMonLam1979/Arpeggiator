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
	Enums::playMode currentPlayMode;

	bool lastNoteWasNoteOn;
	int noteLengthInSamples;
	int numberOfSamplesInBuffer;
	int samplesFromLastNoteOnUntilBufferEnds;

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

	LatchMode latchMode;
	LatchLock latchLock;
	ArpPlayMode playMode;
	bool latchIsEnabled;
	int currentNoteIndex;
	int numberOfNotesToPlay;
	int lastNoteValue;
	bool noteOffOccursInSameBufferAsLastNoteOn;
};