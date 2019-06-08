#include "NoteDivision.h"

NoteDivision::NoteDivision()
{
}

NoteDivision::~NoteDivision()
{
}

void NoteDivision::Set()
{
	stateHasChanged = currentFactor != selectedFactor;

	currentFactor = stateHasChanged ? selectedFactor : currentFactor;
}

void NoteDivision::parameterChanged(const String& parameterID, float newValue)
{
	const auto choice = ParamterChoices::noteDivisionChoices[newValue];

	selectedFactor = ParamterChoices::noteDivisionDictionary[choice];
}