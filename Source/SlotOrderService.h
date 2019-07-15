#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SlotOrderService : public AudioProcessorValueTreeState::Listener
{
public:

	SlotOrderService();
	~SlotOrderService();

private:

	void parameterChanged(const String& parameterID, float newValue) override;
};