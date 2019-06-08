#include "NoteDivision.h"
#include <shlwapi.h>

NoteDivision::NoteDivision()
{
}

NoteDivision::~NoteDivision()
{
}

void NoteDivision::parameterChanged(const String& parameterID, float newValue)
{
	const auto choice = ParamterChoices::NoteDivisionChoices[newValue];

	factor = ParamterChoices::NoteDivisionDictionary[choice];
}

bool NoteDivision::StateChanged()
{
	stateHasChanged = lastFactor != factor;

	lastFactor = stateHasChanged ? factor : lastFactor;

	return  stateHasChanged;
}