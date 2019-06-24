#include "ArpMode.h"
#include "LatchMode.h"

ArpMode::ArpMode()
{
}

ArpMode::~ArpMode()
{
}

void ArpMode::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	state = static_cast<Enums::arpMode>(choice);
}

bool ArpMode::StateChanged()
{
	stateHasChanged = previousState != state;

	previousState = stateHasChanged ? state : previousState;

	return  stateHasChanged;
}