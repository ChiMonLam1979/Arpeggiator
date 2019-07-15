#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SlotController.h"

class SlotRepeatService : public AudioProcessorValueTreeState::Listener
{
public:

	SlotRepeatService(SlotController& slotController);
	~SlotRepeatService();

private:

	SlotController& slotController;

	void parameterChanged(const String& parameterID, float newValue) override;
};