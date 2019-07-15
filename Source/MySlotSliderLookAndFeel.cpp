#include "Fonts.h"
#include "MySlotSliderLookAndFeel.h"

Button* MySlotSliderLookAndFeel::createSliderButton(Slider&, bool isIncrement)
{
	auto button = new TextButton(isIncrement ? "PLUS" : "MINUS", String());
	button->setLookAndFeel(&buttonLookAndFeel);
	return button;
}