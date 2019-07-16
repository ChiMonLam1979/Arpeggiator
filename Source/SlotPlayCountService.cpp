#include "Parameters.h"
#include "SlotPlayCountService.h"

SlotPlayCountService::SlotPlayCountService(SlotController& slotController) : slotController(slotController)
{
	
}

SlotPlayCountService::~SlotPlayCountService()
{

}

void SlotPlayCountService::parameterChanged(const String& parameterID, float newValue)
{
	auto playCount = static_cast<int>(newValue);

	if (parameterID == IDs::Slot1PlayCountId)
	{
		slotController.slot1PlayCount = playCount;
	}
	if (parameterID == IDs::Slot2PlayCountId)
	{
		slotController.slot2PlayCount = playCount;
	}
	if (parameterID == IDs::Slot3PlayCountId)
	{
		slotController.slot3PlayCount = playCount;
	}
	if (parameterID == IDs::Slot4PlayCountId)
	{
		slotController.slot4PlayCount = playCount;
	}
}