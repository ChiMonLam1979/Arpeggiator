#include "NoteDivision.h"

NoteDivision::NoteDivision()
{
}

NoteDivision::~NoteDivision()
{
}

void NoteDivision::parameterChanged(const String& parameterID, float newValue)
{
	const auto choice = ParameterChoices::NoteDivisionChoices[newValue];

	factor = ParameterChoices::NoteDivisionDictionary[choice];
}

bool NoteDivision::StateChanged()
{
	stateHasChanged = lastFactor != factor;

	lastFactor = stateHasChanged ? factor : lastFactor;

	return  stateHasChanged;
}