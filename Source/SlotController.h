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

	int slot1PlayCount{ 0 };
	int slot2PlayCount{ 0 };
	int slot3PlayCount{ 0 };
	int slot4PlayCount{ 0 };

	bool patternModeIsOn{ false };

	bool patternModeHasChanged{ false };

	std::vector<double> slotsOrder{ 1.0, 2.0, 3.0, 4.0 };

	std::vector<std::vector<int>> pattern{ slot1Data, slot2Data, slot3Data, slot4Data};

	std::vector<int> patternToPlay;

private:

	std::vector<int>& GetSelectedSlot(const String& parameterID);
	void parameterChanged(const String& parameterID, float newValue) override;
};