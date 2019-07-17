#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SlotController.h"

class SlotOrderService : public AudioProcessorValueTreeState::Listener
{
public:

	SlotOrderService(SlotController& slotController);
	~SlotOrderService();

private:

	SlotController& slotController;

	void parameterChanged(const String& parameterID, float newValue) override;
	void UpdateSlotOrder(int slot, int playOrder);
};