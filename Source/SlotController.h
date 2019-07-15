#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SlotRepeatService.h"
#include "SlotOrderService.h"

class SlotController : public AudioProcessorValueTreeState::Listener
{
public:

	SlotController(SlotRepeatService& slotRepeatService, SlotOrderService& slotOrderService);
	~SlotController();

	std::vector<int> currentNotes;

	std::vector<int> slot1Data;
	std::vector<int> slot2Data;
	std::vector<int> slot3Data;
	std::vector<int> slot4Data;

private:

	SlotRepeatService& slotRepeatService;
	SlotOrderService& slotOrderService;

	std::vector<int>& GetSelectedSlot(const String& parameterID);

	void parameterChanged(const String& parameterID, float newValue) override;
};
