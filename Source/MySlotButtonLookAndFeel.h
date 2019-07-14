#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MyTextButtonLookAndFeel.h"

class MySlotButtonLookAndFeel : public MyTextButtonLookAndFeel
{
public:

	MySlotButtonLookAndFeel()
	{
	}

	void drawButtonBackground(Graphics&, Button&, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
	void drawButtonText(Graphics&, TextButton&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
	void drawButtonPressCircles(Graphics&, Rectangle<float> buttonBounds, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown);

private:

	bool buttonIsToggled{ false };
};
