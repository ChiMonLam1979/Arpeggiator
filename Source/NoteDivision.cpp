#include "NoteDivision.h"

NoteDivision::NoteDivision()
{
}

NoteDivision::~NoteDivision()
{
}

void NoteDivision::parameterChanged(const String& parameterID, float newValue)
{
	const auto choice = ParamterChoices::noteDivisionChoices[newValue];

	factor = ParamterChoices::noteDivisionDictionary[choice];

	stateHasChanged = true;
}