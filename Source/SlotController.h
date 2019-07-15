#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SlotController : public AudioProcessorValueTreeState::Listener
{
public:

	SlotController();
	~SlotController();

	std::vector<int> currentNotes;

	std::vector<int> slot1Data;
	std::vector<int> slot2Data;
	std::vector<int> slot3Data;
	std::vector<int> slot4Data;

	int slot1Repeats{ 0 };
	int slot2Repeats{ 0 };
	int slot3Repeats{ 0 };
	int slot4Repeats{ 0 };

	int slot1Order{ 1 };
	int slot2Order{ 2 };
	int slot3Order{ 3 };
	int slot4Order{ 4 };

	std::vector<std::vector<int>> pattern{ slot1Data, slot2Data, slot3Data, slot4Data};

private:

	std::vector<int>& GetSelectedSlot(const String& parameterID);

	void parameterChanged(const String& parameterID, float newValue) override;
};