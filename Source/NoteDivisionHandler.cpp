#include "NoteDivisionHandler.h"

NoteDivionHandler::NoteDivionHandler() : noteDivisionChanged(false)
{
}

NoteDivionHandler::~NoteDivionHandler()
{
}

void NoteDivionHandler::UpdateNoteDivision()
{
	const auto currentNoteDivisionFactor = noteDivisionDictionary[noteDivision->getCurrentChoiceName()];

	noteDivisionChanged = noteDivisionFactor != currentNoteDivisionFactor;
	if (noteDivisionChanged)
	{
		noteDivisionFactor = currentNoteDivisionFactor;
	}
}


AudioParameterChoice* NoteDivionHandler::GetNoteDivisionParameter() const
{
	return noteDivision;
}

