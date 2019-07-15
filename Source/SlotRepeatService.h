#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SlotRepeatService : public AudioProcessorValueTreeState::Listener
{
public:

	SlotRepeatService();
	~SlotRepeatService();

private:

	void parameterChanged(const String& parameterID, float newValue) override;
};