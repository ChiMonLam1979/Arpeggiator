#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class NoteDivision : public AudioProcessorValueTreeState::Listener
{
public:

	NoteDivision();
	~NoteDivision();

	bool StateChanged();

	float factor  { 1.0f };

private:

	float lastFactor{ 1.0f };
	bool stateHasChanged{ false };

	void parameterChanged(const String& parameterID, float newValue) override;
};