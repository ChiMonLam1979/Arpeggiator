#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class PatternMode : public AudioProcessorValueTreeState::Listener
{
public:

	PatternMode();
	~PatternMode();

	bool IsEnabled() const;

private:

	Enums::patternMode state { Enums::patternMode::patternsOff };

	void parameterChanged(const String& parameterID, float newValue) override;
};