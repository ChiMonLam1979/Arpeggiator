#include "SlotController.h"
#include "Parameters.h"

SlotController::SlotController(PatternMode& patternMode) : patternMode(patternMode)
{

}

SlotController::~SlotController()
{

}

void SlotController::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	if(!patternMode.IsEnabled() && parameterID != IDs::PatternModeId)
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

	if(parameterID == IDs::PatternModeId)
	{
		auto mode = static_cast<Enums::patternMode>(choice);

		switch (mode)
		{
		case Enums::patternsOn: UpdatePattern();
			break;
		default: break;
		}
	}	
}

void SlotController::UpdatePattern()
{
	
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