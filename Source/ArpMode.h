#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class ArpMode : public AudioProcessorValueTreeState::Listener
{
public:

	ArpMode();
	~ArpMode();

	bool StateChanged();

	Enums::arpMode state { Enums::arpMode::up };

private:

	bool stateHasChanged { false };
	Enums::arpMode previousState { Enums::arpMode::up };

	void parameterChanged(const String& parameterID, float newValue) override;
};