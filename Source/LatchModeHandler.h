#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class LatchModeHandler
{
public:

	LatchModeHandler();
	~LatchModeHandler();

	AudioParameterChoice* GetArpLatchModeParameter() const;

	void SetLatchMode();
	bool IsLatchModeOff() const;

	bool latchModeHasChanged;
	Enums::latchMode currentLatchMode;
	Enums::latchMode selectedLatchMode;

private:

	AudioParameterChoice* arpLatchMode = new AudioParameterChoice
	{
		IDs::latchModeId,
		ParameterNames::latchModeName,
		{
			ParamterChoices::latchModeOff,
			ParamterChoices::latchModeOn,
		},
		0
	};
};