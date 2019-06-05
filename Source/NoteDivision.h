#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class NoteDivision : public AudioProcessorValueTreeState::Listener
{
public:

	NoteDivision();
	~NoteDivision();

	void Set();

	float currentFactor  { 1.0f };
	float selectedFactor { 1.0f };
	bool stateChanged    { false };

private:

	void parameterChanged(const String& parameterID, float newValue) override;
};
