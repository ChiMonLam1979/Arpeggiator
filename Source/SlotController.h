#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class SlotController : public AudioProcessorValueTreeState::Listener
{
public:

	SlotController();
	~SlotController();

	void UpdateSlots(std::vector<int>& notes);

private:

	std::vector<int> slot1Data;
	std::vector<int> slot2Data;
	std::vector<int> slot3Data;
	std::vector<int> slot4Data;

	Enums::slotAction slot1Action{ Enums::slotAction::clear };
	Enums::slotAction slot2Action{ Enums::slotAction::clear };
	Enums::slotAction slot3Action{ Enums::slotAction::clear };
	Enums::slotAction slot4Action{ Enums::slotAction::clear };

	Enums::slotAction slot1PreviousState{ Enums::slotAction::clear };
	Enums::slotAction slot2PreviousState{ Enums::slotAction::clear };
	Enums::slotAction slot3PreviousState{ Enums::slotAction::clear };
	Enums::slotAction slot4PreviousState{ Enums::slotAction::clear };

	bool slot1StateHasChanged{ false };
	bool slot2StateHasChanged{ false };
	bool slot3StateHasChanged{ false };
	bool slot4StateHasChanged{ false };

	void parameterChanged(const String& parameterID, float newValue) override;
};