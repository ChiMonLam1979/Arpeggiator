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

	stateHasChanged = true;
}

bool LatchMode::IsEnabled() const
{
	return state == Enums::latchMode::on;
}