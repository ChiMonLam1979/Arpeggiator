#include "SlotController.h"
#include "Parameters.h"

SlotController::SlotController() 
{

}

SlotController::~SlotController()
{

}

void SlotController::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	if(!patternModeIsOn)
	{
		auto action = static_cast<Enums::slotAction>(choice);

		auto& selectedSlot = GetSelectedSlot(parameterID);

		switch (action)
		{
		case Enums::save: selectedSlot = currentNotes;
			break;
		case Enums::clear: selectedSlot.clear();
		}
	}
}

std::vector<int>& SlotController::GetSelectedSlot(const String& parameterID)
{
	if (parameterID == IDs::Slot1Id)
	{
		return slot1Data;
	}
	if (parameterID == IDs::Slot2Id)
	{
		return slot2Data;
	}
	if (parameterID == IDs::Slot3Id)
	{
		return slot3Data;
	}
	if (parameterID == IDs::Slot4Id)
	{
		return slot4Data;
	}
}