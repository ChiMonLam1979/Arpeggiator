#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class ArpPlayMode
{
public:

	ArpPlayMode();
	~ArpPlayMode();

	AudioParameterChoice* GetParameter() const;

	void Set();

	Enums::playMode selectedState;
	Enums::playMode currentState;
	bool stateHasChanged;

private:

	AudioParameterChoice* playModes = new AudioParameterChoice
	{
		IDs::arpPlayModeId,
		ParameterNames::arpPlayModeName,
		{
			ParamterChoices::playModeUp,
			ParamterChoices::playModeDown,
			ParamterChoices::playModeRandom,
			ParamterChoices::playModePlayed
		},
		0
	};
};