#include "LatchModeHandler.h"

LatchModeHandler::LatchModeHandler() : latchModeHasChanged(false), currentLatchMode(Enums::latchMode::off), selectedLatchMode(Enums::latchMode::off)
{
}

LatchModeHandler::~LatchModeHandler()
{
}

AudioParameterChoice* LatchModeHandler::GetArpLatchModeParameter() const
{
	return arpLatchMode;
}

void LatchModeHandler::SetLatchMode()
{
	selectedLatchMode = static_cast<Enums::latchMode>(arpLatchMode->getIndex());

	latchModeHasChanged = currentLatchMode != selectedLatchMode;

	currentLatchMode = latchModeHasChanged ? selectedLatchMode : currentLatchMode;
}

bool LatchModeHandler::IsLatchModeOff() const
{
	return currentLatchMode == Enums::latchMode::off;
}
