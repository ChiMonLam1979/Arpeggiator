#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "LatchMode.h"

class Notes
{
public:

	Notes();
	~Notes();

	void ProcessBuffer(MidiBuffer& midiMessages, bool transposeIsEnabled);
	void UpdateNotesToPlay(bool latchModeStateHasChanged);
	void SortNotesToPlay();
	int SetLastNoteValue();
	void GetNumberOfNotesToPlay();
	void UpdateNoteValue();
	void InitializeNoteIndex();
	bool AnyNotesToPlay() const;
	bool ShouldAddNoteOn() const;
	bool ShouldAddNoteOff(int samplesFromLastNoteOnUntilBufferEnds, int noteLengthInSamples) const;

	std::vector<int> toPlay;
	std::vector<int> toPlayLatchMode;
	int currentNoteIndex;
	int numberOfNotesToPlay;
	int lastNoteValue;
	bool lastNoteWasNoteOn;
	bool latchIsDisabled;
	Enums::playMode currentPlayMode;

private:

};
