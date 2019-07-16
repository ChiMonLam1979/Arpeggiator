#include "SlotRepeatService.h"

SlotRepeatService::SlotRepeatService(SlotController& slotController) : slotController(slotController)
{
	
}

SlotRepeatService::~SlotRepeatService()
{

}

void SlotRepeatService::parameterChanged(const String& parameterID, float newValue)
{
	auto numberOfRepeats = static_cast<int>(newValue);

	if (parameterID == IDs::Slot1RepeatId)
	{
		slotController.slot1Repeats = numberOfRepeats;
	}
	if (parameterID == IDs::Slot2RepeatId)
	{
		slotController.slot2Repeats = numberOfRepeats;
	}
	if (parameterID == IDs::Slot3RepeatId)
	{
		slotController.slot3Repeats = numberOfRepeats;
	}
	if (parameterID == IDs::Slot4RepeatId)
	{
		slotController.slot4Repeats = numberOfRepeats;
	}
}