#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class ArpSlot : public AudioProcessorValueTreeState::Listener
{
public:

	ArpSlot();
	~ArpSlot();

private:

	Enums::slot state{ Enums::slot::slotOne };

	void parameterChanged(const String& parameterID, float newValue) override;
};