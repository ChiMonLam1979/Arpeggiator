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
}