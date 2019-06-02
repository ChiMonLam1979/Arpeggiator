#include "LatchLock.h"

LatchLock::LatchLock() : state(Enums::latchLock::unlocked)
{
}

LatchLock::~LatchLock()
{
}

AudioParameterChoice* LatchLock::GetParameter() const
{
	return lockModes;
}

void LatchLock::Set()
{
	state = static_cast<Enums::latchLock>(lockModes->getIndex());
}
