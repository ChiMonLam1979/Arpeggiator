#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class ArpPlayMode : public AudioProcessorValueTreeState::Listener
{
public:

	ArpPlayMode();
	~ArpPlayMode();

	Enums::playMode state { Enums::playMode::up };
	bool stateHasChanged { false };

private:

	void parameterChanged(const String& parameterID, float newValue) override;
};