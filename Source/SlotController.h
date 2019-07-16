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

	int slot1Order{ 1 };
	int slot2Order{ 2 };
	int slot3Order{ 3 };
	int slot4Order{ 4 };

	bool patternModeIsOn{ false };

	std::map<int, std::vector<int>> patternData{ { 1, slot1Data }, { 2, slot2Data }, { 3 , slot3Data }, { 4 , slot4Data} };

	std::vector<std::vector<int>> pattern{ slot1Data, slot2Data, slot3Data, slot4Data};

private:

	std::vector<int>& GetSelectedSlot(const String& parameterID);
	void parameterChanged(const String& parameterID, float newValue) override;
};