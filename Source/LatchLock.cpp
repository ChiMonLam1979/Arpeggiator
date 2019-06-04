#include "LatchLock.h"

LatchLock::LatchLock()
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
