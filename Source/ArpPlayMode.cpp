#include "ArpPlayMode.h"
#include "LatchMode.h"

ArpPlayMode::ArpPlayMode() : selectedState(Enums::playMode::up), currentState(Enums::playMode::up), stateHasChanged(false)
{
}

ArpPlayMode::~ArpPlayMode()
{
}

AudioParameterChoice* ArpPlayMode::GetParameter() const
{
	return playModes;
}

void ArpPlayMode::Set()
{
	selectedState = static_cast<Enums::playMode>(playModes->getIndex());

	stateHasChanged = currentState != selectedState;
	if (stateHasChanged)
	{
		currentState = selectedState;
	}
}