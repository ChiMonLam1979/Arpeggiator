#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"
#include "SlotController.h"

class PatternMode : public AudioProcessorValueTreeState::Listener
{
public:

	PatternMode(SlotController& slotController);
	~PatternMode();

private:

	SlotController& slotController;

	Enums::patternMode mode{ Enums::patternMode::patternsOff };

	void parameterChanged(const String& parameterID, float newValue) override;
	void UpdatePattern();
};