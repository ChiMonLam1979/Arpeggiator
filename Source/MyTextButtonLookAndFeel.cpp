#include "MyTextButtonLookAndFeel.h"

void MyTextButtonLookAndFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	{
		auto cornerSize = 6.0f;
		auto bounds = button.getLocalBounds().toFloat().reduced(2.5f, 2.5f);
		auto innerbounds = bounds.reduced(4.0f, 4.0f);

		auto baseButtonColour = Colour(0x402d91b9);

		auto baseColour = baseButtonColour;

		if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
			baseColour = shouldDrawButtonAsDown ? Colour(0xff2d91b9) : Colour(0xaa237190);

		if (button.getToggleState())
		{
			baseColour = Colour(0xff2d91b9);
			buttonIsToggled = true;
		}
		else
		{
			buttonIsToggled = false;
		}

		g.setColour(baseColour);
		g.fillRoundedRectangle(innerbounds, cornerSize);

		g.setColour(Colour(0xff78c1de));
		g.drawRoundedRectangle(bounds, cornerSize, 2.0f);

		drawButtonPressCircles(g, bounds, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
	}
}

void MyTextButtonLookAndFeel::drawButtonPressCircles(Graphics& g, Rectangle<float> buttonBounds, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{

	auto diameter = buttonBounds.getHeight() * 0.015f;
	auto buttonCenter = buttonBounds.getCentreX();
	auto distanceBetweenCircles = buttonBounds.getWidth() * 0.06f;
	auto yPosOfTopCircles = buttonBounds.getY() + (buttonBounds.getHeight() * 0.80f);
	auto yPosOfBottomCircles = buttonBounds.getY() + (buttonBounds.getHeight() * 0.85f);

	auto leftXPos = buttonCenter - distanceBetweenCircles - (diameter * 0.5f);
	auto midXpos = buttonCenter - (diameter * 0.5f);
	auto rightXPos = buttonCenter + distanceBetweenCircles - (diameter * 0.5f);

	auto baseEllipseColour = Colour(0xff2d91b9);
	auto ellipseColour = baseEllipseColour;

	if (shouldDrawButtonAsHighlighted)
	{
		ellipseColour = shouldDrawButtonAsDown ? Colour(0xff051115) : Colour(0xffc1e5f0);
	}

	if(buttonIsToggled)
	{
		ellipseColour = Colour(0xff051115);
	}

	g.setColour(ellipseColour);

	g.fillEllipse(leftXPos, yPosOfTopCircles, diameter, diameter);
	g.fillEllipse(midXpos, yPosOfTopCircles, diameter, diameter);
	g.fillEllipse(rightXPos, yPosOfTopCircles, diameter, diameter);

	g.fillEllipse(leftXPos, yPosOfBottomCircles, diameter, diameter);
	g.fillEllipse(midXpos, yPosOfBottomCircles, diameter, diameter);
	g.fillEllipse(rightXPos, yPosOfBottomCircles, diameter, diameter);
}


void MyTextButtonLookAndFeel::drawButtonText(Graphics& g, TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	Font font(Font("Seven Segment", "Regular", button.getHeight() * 0.3f));
	g.setFont(font);

	auto baseTextColour = Colour(0xff2d91b9);
	auto textColour = baseTextColour;

	if(shouldDrawButtonAsHighlighted)
	{
		textColour = shouldDrawButtonAsDown ? Colour(0xff051115) : Colour(0xffc1e5f0);
	}

	if(buttonIsToggled)
	{
		textColour = Colour(0xff051115);
	}

	g.setColour(textColour);

	const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
	const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

	const int fontHeight = roundToInt(font.getHeight() * 0.6f);
	const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
	const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
	const int textWidth = button.getWidth() - leftIndent - rightIndent;

	if (textWidth > 0)
		g.drawFittedText(button.getButtonText(),
			leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
			Justification::centred, 2);
}
