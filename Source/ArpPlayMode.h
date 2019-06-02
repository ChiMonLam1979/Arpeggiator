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
		IDs::ArpPlayModeId,
		ParameterNames::ArpPlayModeName,
		{
			ParamterChoices::Up,
			ParamterChoices::Down,
			ParamterChoices::Random,
			ParamterChoices::Played
		},
		0
	};
};