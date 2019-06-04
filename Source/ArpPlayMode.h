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

	Enums::playMode selectedState { Enums::playMode::up };
	Enums::playMode currentState { Enums::playMode::up };
	bool stateHasChanged { false };

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