#include "SlotController.h"

SlotController::SlotController()
{

}

SlotController::~SlotController()
{

}

void SlotController::UpdateSlots(std::vector<int>& notes)
{
	if(slot1StateHasChanged)
	{
		if (slot1Action == Enums::save && slot1Data.empty())
		{
			slot1Data = notes;
		}
		if (slot1Action == Enums::clear && !slot1Data.empty())
		{
			slot1Data.clear();
		}

		slot1StateHasChanged = false;
	}

	if(slot2StateHasChanged)
	{
		if (slot2Action == Enums::save && slot2Data.empty())
		{
			slot2Data = notes;
		}
		if (slot2Action == Enums::clear && !slot2Data.empty())
		{
			slot2Data.clear();
		}

		slot2StateHasChanged = false;
	}

	if(slot3StateHasChanged)
	{
		if (slot3Action == Enums::save && slot3Data.empty())
		{
			slot3Data = notes;
		}
		if (slot3Action == Enums::clear && !slot3Data.empty())
		{
			slot3Data.clear();
		}

		slot3StateHasChanged = false;
	}

	if(slot4StateHasChanged)
	{
		if (slot4Action == Enums::save && slot4Data.empty())
		{
			slot4Data = notes;
		}
		if (slot4Action == Enums::clear && !slot4Data.empty())
		{
			slot4Data.clear();
		}

		slot4StateHasChanged = false;
	}
}

void SlotController::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);
	auto action =  static_cast<Enums::slotAction>(choice);

	if(parameterID == IDs::Slot1Id)
	{
		slot1Action = action;

		slot1StateHasChanged = slot1PreviousState != slot1Action;

		slot1PreviousState = slot1StateHasChanged ? slot1Action : slot1PreviousState;
	}
	if(parameterID == IDs::Slot2Id)
	{
		slot2Action = action;

		slot2StateHasChanged = slot2PreviousState != slot2Action;

		slot2PreviousState = slot2StateHasChanged ? slot2Action : slot2PreviousState;
	}
	if (parameterID == IDs::Slot3Id)
	{
		slot3Action = action;

		slot3StateHasChanged = slot3PreviousState != slot3Action;

		slot3PreviousState = slot3StateHasChanged ? slot3Action : slot3PreviousState;
	}
	if (parameterID == IDs::Slot4Id)
	{
		slot4Action= action;

		slot4StateHasChanged = slot4PreviousState != slot4Action;

		slot4PreviousState = slot4StateHasChanged ? slot4Action : slot4PreviousState;
	}
}