#include "ArpPlayMode.h"
#include "LatchMode.h"

ArpPlayMode::ArpPlayMode()
{
}

ArpPlayMode::~ArpPlayMode()
{
}

void ArpPlayMode::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	state = static_cast<Enums::playMode>(choice);

	stateHasChanged = true;
}
