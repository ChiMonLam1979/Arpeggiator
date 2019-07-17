#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class LatchMode : public AudioProcessorValueTreeState::Listener
{
public:

	LatchMode();
	~LatchMode();

	bool IsEnabled() const;
	bool StateChanged();

private:

	bool stateHasChanged { false };
	Enums::latchMode state { Enums::latchMode::off };
	Enums::latchMode previousState { Enums::latchMode::off };

	void parameterChanged(const String& parameterID, float newValue) override;
};