#include "LatchMode.h"

LatchMode::LatchMode()
{
}

LatchMode::~LatchMode()
{
}

void LatchMode::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	state = static_cast<Enums::latchMode>(choice);
}

bool LatchMode::StateChanged()
{
	stateHasChanged = previousState != state;

	previousState = stateHasChanged ? state : previousState;

	return  stateHasChanged;
}

bool LatchMode::IsEnabled() const
{
	return state == Enums::latchMode::on;
}