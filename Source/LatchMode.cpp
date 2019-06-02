#include "LatchMode.h"

LatchMode::LatchMode() : stateHasChanged(false), currentState(Enums::latchMode::off), selectedState(Enums::latchMode::off)
{
}

LatchMode::~LatchMode()
{
}

AudioParameterChoice* LatchMode::GetParameter() const
{
	return latchModes;
}

void LatchMode::Set()
{
	selectedState = static_cast<Enums::latchMode>(latchModes->getIndex());

	stateHasChanged = selectedState != currentState;

	currentState = stateHasChanged ? selectedState : currentState;
}

bool LatchMode::IsEnabled() const
{
	return currentState == Enums::latchMode::on;
}