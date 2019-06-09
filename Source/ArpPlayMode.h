#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class ArpPlayMode : public AudioProcessorValueTreeState::Listener
{
public:

	ArpPlayMode();
	~ArpPlayMode();

	bool StateChanged();

	Enums::playMode state { Enums::playMode::up };

private:

	bool stateHasChanged { false };
	Enums::playMode previousState { Enums::playMode::up };

	void parameterChanged(const String& parameterID, float newValue) override;
};