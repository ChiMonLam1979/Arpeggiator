#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class ArpPlayModeHandler
{
public:

	ArpPlayModeHandler();
	~ArpPlayModeHandler();

	AudioParameterChoice* GetArpPlayModeParameter() const;

	void SetPlayMode();

	Enums::playMode selectedPlayMode;
	Enums::playMode currentPlayMode;
	bool playModeHasChanged;

private:

	AudioParameterChoice* arpPlayMode = new AudioParameterChoice
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