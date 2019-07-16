#include "SlotOrderService.h"
#include "Parameters.h"

SlotOrderService::SlotOrderService(SlotController& slotController): slotController(slotController)
{

}

SlotOrderService::~SlotOrderService()
{

}

void SlotOrderService::parameterChanged(const String& parameterID, float newValue)
{
	auto playOrder = static_cast<int>(newValue);

	if(parameterID == IDs::Slot1OrderId)
	{
		UpdateSlotOrder(0, playOrder);
	}
	if (parameterID == IDs::Slot2OrderId)
	{
		UpdateSlotOrder(1, playOrder);
	}
	if (parameterID == IDs::Slot3OrderId)
	{
		UpdateSlotOrder(2, playOrder);
	}
	if (parameterID == IDs::Slot4OrderId)
	{
		UpdateSlotOrder(3, playOrder);
	}
}

void SlotOrderService::UpdateSlotOrder(int slot, int playOrder)
{
	auto it = std::find(slotController.slotsOrder.begin(), slotController.slotsOrder.end(), playOrder);

	auto index = std::distance(slotController.slotsOrder.begin(), it);

	std::swap(slotController.slotsOrder[slot], slotController.slotsOrder[index]);
}