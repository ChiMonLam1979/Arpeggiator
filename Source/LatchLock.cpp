#include "LatchLock.h"

LatchLock::LatchLock()
{
}

LatchLock::~LatchLock()
{
}

//void LatchLock::Set()
//{
//	state = static_cast<Enums::latchLock>(lockModes->getIndex());
//}

void LatchLock::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	state = static_cast<Enums::latchLock>(choice);
}

