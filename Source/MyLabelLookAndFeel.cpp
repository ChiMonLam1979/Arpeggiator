#include "MyLabelLookAndFeel.h"
#include "Fonts.h"

void MyLabelLookAndFeel::drawLabel(Graphics& g, Label& label)
{
	auto localWidth = label.getLocalBounds().getWidth() * 1.0f;
	auto localHeight = label.getLocalBounds().getHeight() * 1.0f;

	auto font = getSevenSegmentFont().withHeight(localHeight * 0.6f);

	g.setColour(Colour(0xff78c1de));
	g.setFont(font);
	g.drawFittedText(label.getName(), 0, 0, localWidth, localHeight, Justification::centred, 1, 0.0f);
	g.setColour(Colour(0xff78c1de));
}
