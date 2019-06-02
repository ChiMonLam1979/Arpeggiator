#include "LatchLockHandler.h"

LatchLockHandler::LatchLockHandler() : latchLockState(Enums::latchLock::unlocked)
{
}

LatchLockHandler::~LatchLockHandler()
{
}

AudioParameterChoice* LatchLockHandler::GetLatchLockParameter() const
{
	return arpLatchLock;
}

void LatchLockHandler::SetLatchLockMode()
{
	latchLockState = static_cast<Enums::latchLock>(arpLatchLock->getIndex());
}
