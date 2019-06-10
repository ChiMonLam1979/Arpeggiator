#include "MySliderLookAndFeel.h"

void MySliderLookAndFeel::drawLinearSlider(
	Graphics& g,
	int x,
	int y,
	int width,
	int height,
	float sliderPos,
	float minSliderPos,
	float maxSliderPos,
	const Slider::SliderStyle style,
	Slider& slider)
{
	if(slider.isVertical())
	{
		slider.setTextBoxStyle(Slider::TextBoxBelow, false, slider.getWidth() / 4, 15);

		auto sliderWidth		= slider.getWidth() * 1.0f;
		auto sliderHeight		= slider.getHeight() * 1.0f;
		auto outerBorderWidth	= sliderWidth * 0.24f;
		auto innerBorderWidth	= outerBorderWidth * 0.98f;
		auto sliderPathWidth	= innerBorderWidth * 0.40f;
		auto thumbHeight		= sliderHeight * 0.10f;
		auto thumbWidth			= innerBorderWidth * 1.0f;
		auto sliderTopPosition = slider.getPositionOfValue(127);

		Rectangle<float> sliderOuterBorder{ (sliderWidth * 0.5f) - (outerBorderWidth * 0.5f), sliderTopPosition - (thumbHeight * 0.5f), outerBorderWidth, height + (thumbHeight) };
		g.setColour(Colour(0xff78c1de));
		g.drawRoundedRectangle(sliderOuterBorder, 5.0f, 2.0f);

		Rectangle<float> sliderInnerBorder{ (sliderWidth * 0.5f) - (innerBorderWidth * 0.5f), sliderTopPosition - (thumbHeight * 0.5f), innerBorderWidth, height + (thumbHeight) };
		g.setColour(Colour(0x402d91b9));
		g.fillRoundedRectangle(sliderInnerBorder, 1.0f);

		Rectangle<float> sliderPath{ (sliderWidth * 0.5f) - (sliderPathWidth * 0.5f), sliderPos, sliderPathWidth, (minSliderPos - sliderPos) };
		g.setColour(Colour(0x505ab9d8));
		g.fillRoundedRectangle(sliderPath, 3.0f);

		Rectangle<float> sliderThumb{ (sliderWidth * 0.5f) - (thumbWidth * 0.5f), sliderPos - (thumbHeight / 2.0f), thumbWidth, thumbHeight };
		g.setColour(Colour(0x80cce8ff));
		g.fillRoundedRectangle(sliderThumb, 5.0f);

		for(int i = 1; i <= height * 0.25; i++)
		{
			auto number = i * 4;
			g.setColour(Colour(0x60c8f0f9));
			Rectangle<float> line{ (sliderWidth * 0.5f) - (sliderPathWidth * 0.5f * 0.4f), maxSliderPos - number * 1.0f, sliderPathWidth * 0.4f, 0.5f };
			g.fillRect(line);
		}
	}
}

Slider::SliderLayout MySliderLookAndFeel::getSliderLayout(Slider& slider)
{
	Slider::SliderLayout layout;

	auto window = slider.getLocalBounds().reduced(slider.getHeight() * 0.06f);

	layout.sliderBounds  = window.removeFromTop(window.getHeight() * 0.85f).reduced(window.getHeight() * 0.04);
	layout.textBoxBounds = slider.getLocalBounds().removeFromBottom(window.getHeight() * 0.8);
	return layout;
}

 void MySliderLookAndFeel::drawLabel(Graphics& g, Label& label)
 {
	auto yPos			= label.getLocalBounds().getY() * 1.0f;
	auto localWidth		= label.getLocalBounds().getWidth() * 1.0f;
	auto localHeight	= label.getLocalBounds().getHeight() * 1.0f;
	auto centre			= label.getLocalBounds().getCentreX();
	auto xPos			= centre - (localWidth * 0.12f);
	auto width			= localWidth * 0.24f;

	Rectangle<float> labelRectangle{ xPos, yPos, width, localHeight };

	g.setColour(Colour(0xff78c1de));
	g.setFont(Font("Seven Segment","Regular", localHeight * 0.85f));
	g.drawFittedText(label.getText(), xPos, yPos, width, localHeight, Justification::centred, 1, 0.0f);
	g.setColour(Colour(0xff78c1de));
	g.drawRoundedRectangle(labelRectangle, 5.0f, 2.0f);
 }

 Button* MySliderLookAndFeel::createSliderButton(Slider&, bool isIncrement)
 {
	auto button = new TextButton(isIncrement ? "+" : "-", String());
	button->setLookAndFeel(&buttonLookAndFeel);
	return button;
 }