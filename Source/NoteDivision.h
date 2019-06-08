#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class NoteDivision : public AudioProcessorValueTreeState::Listener
{
public:

	NoteDivision();
	~NoteDivision();

	float factor  { 1.0f };
	bool stateHasChanged    { false };

private:

	void parameterChanged(const String& parameterID, float newValue) override;
};
