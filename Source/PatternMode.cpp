#include "PatternMode.h"

PatternMode::PatternMode()
{
}

PatternMode::~PatternMode()
{
}

void PatternMode::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	state = static_cast<Enums::patternMode>(choice);
}

bool PatternMode::IsEnabled() const
{
	return state == Enums::patternMode::patternsOn;
}