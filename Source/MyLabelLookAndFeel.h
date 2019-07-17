#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MyLabelLookAndFeel : public LookAndFeel_V4
{
public:

	MyLabelLookAndFeel()
	{
	}

	void drawLabel(Graphics&, Label&) override;
};