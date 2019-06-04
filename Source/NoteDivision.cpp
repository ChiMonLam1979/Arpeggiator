#include "NoteDivision.h"

NoteDivision::NoteDivision()
{
}

NoteDivision::~NoteDivision()
{
}

void NoteDivision::Set()
{
	stateChanged = currentFactor != selectedFactor;
	if (stateChanged)
	{
		currentFactor = selectedFactor;
	}
}

void NoteDivision::parameterChanged(const String& parameterID, float newValue)
{
	const auto choice = ParamterChoices::noteDivisionChoices[newValue];

	selectedFactor = noteDivisionDictionary[choice];
}