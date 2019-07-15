#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MyTextButtonLookAndFeel.h"

class MySlotSliderLookAndFeel : public LookAndFeel_V4
{
public:

	MySlotSliderLookAndFeel()
	{
	}

	MyTextButtonLookAndFeel buttonLookAndFeel;

	Button* createSliderButton(Slider&, bool isIncrement) override;

	bool isVertical{ true };
};