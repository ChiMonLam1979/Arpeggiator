#include "ArpPlayMode.h"
#include "LatchMode.h"

ArpPlayMode::ArpPlayMode()
{
}

ArpPlayMode::~ArpPlayMode()
{
}

void ArpPlayMode::Set()
{
	stateHasChanged = currentState != selectedState;

	currentState = stateHasChanged ? selectedState : currentState;
}

void ArpPlayMode::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	selectedState = static_cast<Enums::playMode>(choice);
}
