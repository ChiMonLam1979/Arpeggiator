#include "SlotRepeatService.h"

SlotRepeatService::SlotRepeatService()
{
	
}

SlotRepeatService::~SlotRepeatService()
{
	
}

void SlotRepeatService::parameterChanged(const String& parameterID, float newValue)
{
	auto numberOfRepeats = static_cast<int>(newValue);
}