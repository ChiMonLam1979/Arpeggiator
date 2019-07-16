#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SlotController.h"

class SlotPlayCountService : public AudioProcessorValueTreeState::Listener
{
public:

	SlotPlayCountService(SlotController& slotController);
	~SlotPlayCountService();

private:

	SlotController& slotController;

	void parameterChanged(const String& parameterID, float newValue) override;
};