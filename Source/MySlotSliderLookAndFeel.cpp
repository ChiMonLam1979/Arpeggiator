#include "Fonts.h"
#include "MySlotSliderLookAndFeel.h"

Button* MySlotSliderLookAndFeel::createSliderButton(Slider&, bool isIncrement)
{
	auto button = new TextButton(isIncrement ? "PLUS" : "MINUS", String());
	button->setLookAndFeel(&buttonLookAndFeel);
	return button;
}

void MySlotSliderLookAndFeel::drawLabel(Graphics& g, Label& label)
{
	auto yPos = static_cast<float>(label.getLocalBounds().getY());
	auto height = static_cast<float>(label.getLocalBounds().getHeight());
	auto centre = static_cast<float>(label.getLocalBounds().getCentreX());
	auto xPos = static_cast<float>(label.getLocalBounds().getX());
	auto width = static_cast<float>(label.getLocalBounds().getWidth());

	auto font = getSevenSegmentFont().withHeight(height * 0.85);

	if (isVertical)
	{
		auto xPosAdjusted = static_cast<float>(centre - (width * 0.5));
		auto widthAdjusted = static_cast<float>(width);

		Rectangle<float> labelRectangle{ xPosAdjusted, yPos, widthAdjusted, height };

		g.setColour(Colour(0xff78c1de));
		g.setFont(font);
		g.drawFittedText(label.getText(), xPosAdjusted, yPos, widthAdjusted, height, Justification::centred, 1, 0.0f);
		g.setColour(Colour(0xff78c1de));
		g.drawRoundedRectangle(labelRectangle, 5.0f, 2.0f);
	}
	if (!isVertical)
	{
		Rectangle<float> labelRectangle{ xPos, yPos, width, height };

		g.setColour(Colour(0xff78c1de));
		g.setFont(font);
		g.drawFittedText(label.getText(), xPos, yPos, width, height, Justification::centred, 1, 0.0f);
		g.setColour(Colour(0xff78c1de));
		g.drawRoundedRectangle(labelRectangle, 5.0f, 2.0f);
	}
}