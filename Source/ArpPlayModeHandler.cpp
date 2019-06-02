#include "ArpPlayModeHandler.h"
#include "LatchModeHandler.h"

ArpPlayModeHandler::ArpPlayModeHandler() : selectedPlayMode(Enums::playMode::up), currentPlayMode(Enums::playMode::up), playModeHasChanged(false)
{
}

ArpPlayModeHandler::~ArpPlayModeHandler()
{
}

AudioParameterChoice* ArpPlayModeHandler::GetArpPlayModeParameter() const
{
	return arpPlayMode;
}

void ArpPlayModeHandler::SetPlayMode()
{
	selectedPlayMode = static_cast<Enums::playMode>(arpPlayMode->getIndex());

	playModeHasChanged = currentPlayMode != selectedPlayMode;
	if (playModeHasChanged)
	{
		currentPlayMode = selectedPlayMode;
	}
}