#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class LatchMode : public AudioProcessorValueTreeState::Listener
{
public:

	LatchMode();
	~LatchMode();

	bool IsEnabled() const;

	bool stateHasChanged { false };

private:

	Enums::latchMode state{ Enums::latchMode::off };

	void parameterChanged(const String& parameterID, float newValue) override;
};