#include "Parameters.h"
#include "SlotPlayOrder.h"

SlotPlayOrder::SlotPlayOrder(
	Slider& slot1OrderButtons,
	Slider& slot2OrderButtons,
	Slider& slot3OrderButtons,
	Slider& slot4OrderButtons)
	:
	slot1OrderButtons(slot1OrderButtons),
	slot2OrderButtons(slot2OrderButtons),
	slot3OrderButtons(slot3OrderButtons),
	slot4OrderButtons(slot4OrderButtons)
{
}

SlotPlayOrder::~SlotPlayOrder()
{
}

void SlotPlayOrder::sliderValueChanged(Slider* slider)
{
	auto value = static_cast<int>(slider->getValue());

	if(slider->getName() == ParameterNames::Slot1OrderName)
	{
		UpdateSlotButtons(0, value);
	}
	if (slider->getName() == ParameterNames::Slot2OrderName)
	{
		UpdateSlotButtons(1, value);
	}
	if (slider->getName() == ParameterNames::Slot3OrderName)
	{
		UpdateSlotButtons(2, value);
	}
	if (slider->getName() == ParameterNames::Slot4OrderName)
	{
		UpdateSlotButtons(3, value);
	}
}

void SlotPlayOrder::UpdateSlotButtons(int slot, double value)
{
	auto it = std::find(slotButtonsOrder.begin(), slotButtonsOrder.end(), value);

	auto index = std::distance(slotButtonsOrder.begin(), it);

	std::swap(slotButtonsOrder[slot], slotButtonsOrder[index]);
}


void SlotPlayOrder::UpdateDisplay()
{
	slot1OrderButtons.setValue(slotButtonsOrder[0], dontSendNotification);
	slot2OrderButtons.setValue(slotButtonsOrder[1], dontSendNotification);
	slot3OrderButtons.setValue(slotButtonsOrder[2], dontSendNotification);
	slot4OrderButtons.setValue(slotButtonsOrder[3], dontSendNotification);
}