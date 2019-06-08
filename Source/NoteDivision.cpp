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
}

bool NoteDivision::StateChanged()
{
	stateHasChanged = lastFactor != factor;

	lastFactor = stateHasChanged ? factor : lastFactor;

	return  stateHasChanged;
}