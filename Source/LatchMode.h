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
	bool IsLatchDisabled() const;

	bool stateHasChanged;
	Enums::latchMode currentState;
	Enums::latchMode selectedState;

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