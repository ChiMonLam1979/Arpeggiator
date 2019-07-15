#include "SlotOrderService.h"

SlotOrderService::SlotOrderService()
{

}

SlotOrderService::~SlotOrderService()
{

}

void SlotOrderService::parameterChanged(const String& parameterID, float newValue)
{
	auto playOrder = static_cast<int>(newValue);
}