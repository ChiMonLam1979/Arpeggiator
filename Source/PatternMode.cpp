#include "PatternMode.h"
#include "Extensions.h"

PatternMode::PatternMode(SlotController& slotController) : slotController(slotController)
{
}

PatternMode::~PatternMode()
{
}

void PatternMode::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	mode = static_cast<Enums::patternMode>(choice);

	slotController.patternModeIsOn = mode == Enums::patternsOn;

	if(slotController.patternModeIsOn)
	{
		UpdatePattern();
	}
}

void PatternMode::UpdatePattern()
{
	slotController.pattern[0] = assignVectorPlayCount(slotController.slot1Data, slotController.slot1PlayCount);
	slotController.pattern[1] = assignVectorPlayCount(slotController.slot2Data, slotController.slot2PlayCount);
	slotController.pattern[2] = assignVectorPlayCount(slotController.slot3Data, slotController.slot3PlayCount);
	slotController.pattern[3] = assignVectorPlayCount(slotController.slot4Data, slotController.slot4PlayCount);
}