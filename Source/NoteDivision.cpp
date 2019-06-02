#include "NoteDivision.h"

NoteDivision::NoteDivision() :currentFactor(1.0f), stateChanged(false)
{
}

NoteDivision::~NoteDivision()
{
}

void NoteDivision::Set()
{
	const auto selectedFactor = noteDivisionDictionary[noteDivisions->getCurrentChoiceName()];

	stateChanged = currentFactor != selectedFactor;
	if (stateChanged)
	{
		currentFactor = selectedFactor;
	}
}


AudioParameterChoice* NoteDivision::GetParameter() const
{
	return noteDivisions;
}

