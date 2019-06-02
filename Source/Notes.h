#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "LatchMode.h"
#include "LatchLock.h"

class Notes
{
public:

	Notes(LatchMode& latchMode, LatchLock& latchLock);

	~Notes();

	void ProcessBuffer(MidiBuffer& midiMessages);
	void UpdateNotesToPlay();
	void SortNotesToPlay();
	int SetLastNoteValue();
	void GetNumberOfNotesToPlay();
	void UpdateNoteValue();
	void InitializeNoteIndex();
	bool AnyNotesToPlay();
	bool ShouldAddNoteOn();
	bool ShouldAddNoteOff() const;
	bool TransposeIsEnabled() const;
	void AddNotes(MidiBuffer& midiMessages, int numberOfSamplesInBuffer, int noteOnOffset);
	void AddNoteOnToBuffer(MidiBuffer& midiMessages, const int offset);
	void AddNoteOffToBuffer(MidiBuffer& midiMessages, const int offset);
	int CalculateOffsetForNoteOff(int numberOfSamplesInBuffer, int noteOnOffset = 0) const;

	std::vector<int> toPlay;
	std::vector<int> toPlayLatchMode;
	int currentNoteIndex;
	int numberOfNotesToPlay;
	int lastNoteValue;
	bool lastNoteWasNoteOn;
	Enums::playMode currentPlayMode;
	int samplesFromLastNoteOnUntilBufferEnds;
	int noteLengthInSamples;
	bool noteOffOccursInSameBufferAsLastNoteOn;

private:

	LatchMode latchMode;
	LatchLock latchLock;
	bool latchIsEnabled;
};
