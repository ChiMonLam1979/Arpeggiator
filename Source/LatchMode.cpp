#include "LatchMode.h"

LatchMode::LatchMode()
{
}

LatchMode::~LatchMode()
{
}

void LatchMode::Set()
{
	stateHasChanged = selectedState != currentState;

	currentState = stateHasChanged ? selectedState : currentState;
}

void LatchMode::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	selectedState = static_cast<Enums::latchMode>(choice);
}

bool LatchMode::IsEnabled() const
{
	return currentState == Enums::latchMode::on;
}