#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class LatchMode
{
public:

	LatchMode();
	~LatchMode();

	AudioParameterChoice* GetParameter() const;

	void Set();
	bool IsEnabled() const;

	bool stateHasChanged { false };
	Enums::latchMode currentState { Enums::latchMode::off };
	Enums::latchMode selectedState { Enums::latchMode::off };

private:

	AudioParameterChoice* latchModes = new AudioParameterChoice
	{
		IDs::LatchModeId,
		ParameterNames::LatchModeName,
		{
			ParamterChoices::Off,
			ParamterChoices::On,
		},
		0
	};
};