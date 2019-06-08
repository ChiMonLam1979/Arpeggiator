#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class LatchMode : public AudioProcessorValueTreeState::Listener
{
public:

	LatchMode();
	~LatchMode();

	void Set();
	bool IsEnabled() const;

	bool stateHasChanged { false };
	Enums::latchMode currentState { Enums::latchMode::off };
	Enums::latchMode selectedState { Enums::latchMode::off };

private:

	void parameterChanged(const String& parameterID, float newValue) override;
};