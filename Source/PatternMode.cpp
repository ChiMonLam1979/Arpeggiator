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

	slotController.patternModeHasChanged = true;

	slotController.patternModeIsOn = mode == Enums::patternsOn;

	if(slotController.patternModeIsOn)
	{
		UpdatePattern();
	}
}

void PatternMode::UpdatePattern()
{
	auto slot1Index = static_cast<int>(slotController.slotsOrder[0]) - 1;
	auto slot2Index = static_cast<int>(slotController.slotsOrder[1]) - 1;
	auto slot3Index = static_cast<int>(slotController.slotsOrder[2]) - 1;
	auto slot4Index = static_cast<int>(slotController.slotsOrder[3]) - 1;

	slotController.pattern[slot1Index] = assignVectorPlayCount(slotController.slot1Data, slotController.slot1PlayCount);
	slotController.pattern[slot2Index] = assignVectorPlayCount(slotController.slot2Data, slotController.slot2PlayCount);
	slotController.pattern[slot3Index] = assignVectorPlayCount(slotController.slot3Data, slotController.slot3PlayCount);
	slotController.pattern[slot4Index] = assignVectorPlayCount(slotController.slot4Data, slotController.slot4PlayCount);

	slotController.patternToPlay.clear();

	for(std::vector<int>& vector: slotController.pattern)
	{
		slotController.patternToPlay.insert(slotController.patternToPlay.end(), vector.begin(), vector.end());
	}
}