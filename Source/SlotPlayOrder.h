#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SlotPlayOrder : public Slider::Listener
{
public:

	SlotPlayOrder(
		Slider& slot1OrderButtons,
		Slider& slot2OrderButtons,
		Slider& slot3OrderButtons,
		Slider& slot4OrderButtons);

	~SlotPlayOrder();

	Slider& slot1OrderButtons;
	Slider& slot2OrderButtons;
	Slider& slot3OrderButtons;
	Slider& slot4OrderButtons;

private:

	std::vector<double> slotButtonsOrder{ 1.0, 2.0, 3.0, 4.0 };

	void sliderValueChanged(Slider* slider) override;
	void UpdateSlotButtons(int slot, double value);
	void UpdateDisplay();
};