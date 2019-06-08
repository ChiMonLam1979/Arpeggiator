#include "LatchLock.h"

LatchLock::LatchLock()
{
}

LatchLock::~LatchLock()
{
}

void LatchLock::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	state = static_cast<Enums::latchLock>(choice);
}

bool LatchLock::IsEnabled() const
{
	return state == Enums::latchLock::locked;
}
