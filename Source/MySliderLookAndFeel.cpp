#include "MySliderLookAndFeel.h"
#include "Fonts.h"

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
		isVertical = true;

		slider.setTextBoxStyle(Slider::TextBoxBelow, false, slider.getWidth() / 4, 15);

		auto sliderWidth		= slider.getWidth() * 1.0f;
		auto sliderHeight		= slider.getHeight() * 1.0f;
		auto sliderTopPosition	= slider.getPositionOfValue(127);

		auto thumbWidth			= sliderWidth * 0.2352f;
		auto thumbHeight		= sliderHeight * 0.10f;
		auto thumbXPos			= (sliderWidth * 0.5f) - (thumbWidth * 0.5f);
		auto thumbYPos			= sliderPos - (thumbHeight / 2.0f);

		auto outerBorderWidth	= sliderWidth * 0.24f;
		auto outerBorderHeight	= height + (thumbHeight);
		auto outerBorderXPos	= (sliderWidth * 0.5f) - (outerBorderWidth * 0.5f);
		auto outerBorderYPos	= sliderTopPosition - (thumbHeight * 0.5f);

		auto innerBorderWidth	= outerBorderWidth * 0.98f;
		auto innerBorderHeight	= height + (thumbHeight);
		auto innerBorderXPos	= (sliderWidth * 0.5f) - (innerBorderWidth * 0.5f);
		auto innerBorderYPos	= sliderTopPosition - (thumbHeight * 0.5f);

		auto pathWidth			= innerBorderWidth * 0.40f;
		auto pathHeight			= (minSliderPos - sliderPos);
		auto pathXPos			= (sliderWidth * 0.5f) - (pathWidth * 0.5f);
		auto pathYPos			= sliderPos;

		Rectangle<float> sliderOuterBorder{ outerBorderXPos, outerBorderYPos, outerBorderWidth, outerBorderHeight };
		g.setColour(Colour(0xff78c1de));
		g.drawRoundedRectangle(sliderOuterBorder, 5.0f, 2.0f);

		Rectangle<float> sliderInnerBorder{ innerBorderXPos, innerBorderYPos, innerBorderWidth, innerBorderHeight };
		g.setColour(Colour(0x402d91b9));
		g.fillRoundedRectangle(sliderInnerBorder, 1.0f);

		Rectangle<float> sliderPath{ pathXPos, pathYPos, pathWidth, pathHeight };
		g.setColour(Colour(0x505ab9d8));
		g.fillRoundedRectangle(sliderPath, 3.0f);

		Rectangle<float> sliderThumb{ thumbXPos, thumbYPos, thumbWidth, thumbHeight };
		g.setColour(Colour(0x80cce8ff));
		g.fillRoundedRectangle(sliderThumb, 5.0f);

		for(int i = 1; i <= height * 0.25; i++)
		{
			auto number = i * 4;
			g.setColour(Colour(0x60c8f0f9));
			Rectangle<float> line{ (sliderWidth * 0.5f) - (pathWidth * 0.5f * 0.4f), maxSliderPos - number * 1.0f, pathWidth * 0.4f, 0.5f };
			g.fillRect(line);
		}
	}
	else if(slider.isHorizontal())
	{
		isVertical = false;

		slider.setTextBoxStyle(Slider::TextBoxBelow, false, slider.getWidth() / 4, 15);

		auto sliderHeight			= slider.getHeight() * 1.0f;
		auto sliderBottomPosition	= slider.getPositionOfValue(slider.getMinimum());

		auto thumbWidth				= sliderHeight * 0.5292f;
		auto thumbHeight			= thumbWidth;
		auto thumbXPos				= sliderPos - (thumbHeight / 2.0f);
		auto thumbYPos				= (sliderHeight * 0.5f) - (thumbHeight * 0.5f);

		auto outerBorderWidth		= width + thumbHeight;
		auto outerBorderHeight		= sliderHeight * 0.54f;
		auto outerBorderXPos		= sliderBottomPosition - (thumbHeight * 0.5f);
		auto outerBorderYPos		= (sliderHeight * 0.5f) - (outerBorderHeight * 0.5f);

		auto innerBorderHeight		= outerBorderHeight * 0.98f;
		auto innerBorderWidth		= width + thumbHeight;
		auto innerBorderXPos		= sliderBottomPosition - (thumbHeight * 0.5f);
		auto innerBorderYPos		= (sliderHeight * 0.5f) - (innerBorderHeight * 0.5f);

		auto pathWidth				= sliderPos > minSliderPos ? (sliderPos - minSliderPos) : (minSliderPos - sliderPos);
		auto pathHeight				= innerBorderHeight * 0.40f;
		auto pathXPos				= sliderPos > minSliderPos ? minSliderPos : sliderPos;
		auto pathYPos				= (sliderHeight * 0.5f) - (pathHeight * 0.5f);

		Rectangle<float> sliderOuterBorder{ outerBorderXPos, outerBorderYPos, outerBorderWidth, outerBorderHeight };
		g.setColour(Colour(0xff78c1de));
		g.drawRoundedRectangle(sliderOuterBorder, 3.0f, 2.0f);

		Rectangle<float> sliderInnerBorder{ innerBorderXPos, innerBorderYPos, innerBorderWidth, innerBorderHeight };
		g.setColour(Colour(0x402d91b9));
		g.fillRoundedRectangle(sliderInnerBorder, 1.0f);

		Rectangle<float> sliderPath{ pathXPos , pathYPos, pathWidth, pathHeight };
		g.setColour(Colour(0x505ab9d8));
		g.fillRoundedRectangle(sliderPath, 3.0f);

		Rectangle<float> sliderThumb{ thumbXPos, thumbYPos, thumbWidth, thumbHeight };
		g.setColour(Colour(0x80cce8ff));
		g.fillRoundedRectangle(sliderThumb, 5.0f);

		for (int i = 1; i <= width * 0.25; i++)
		{
			auto number = i * 4;
			g.setColour(Colour(0x60c8f0f9));
			Rectangle<float> line{ sliderBottomPosition + number * 1.0f, sliderPath.getY(), 0.5f, pathHeight };
			g.fillRect(line);
		}
	}
}

Slider::SliderLayout MySliderLookAndFeel::getSliderLayout(Slider& slider)
{
	Slider::SliderLayout layout;

	if (slider.isVertical() || slider.getSliderStyle() == Slider::SliderStyle::IncDecButtons)
	{
		auto window = slider.getLocalBounds().reduced(slider.getHeight() * 0.06f);

		layout.sliderBounds = window.removeFromTop(window.getHeight() * 0.85f).reduced(window.getHeight() * 0.04);
		layout.textBoxBounds = slider.getLocalBounds().removeFromBottom(window.getHeight() * 0.8);

		return layout;
	}
	if (slider.isHorizontal())
	{
		auto window = slider.getLocalBounds().reduced(slider.getWidth() * 0.06f);

		layout.sliderBounds = window.removeFromLeft(window.getWidth() * 0.85);
		window.setPosition(window.getX() + window.getWidth() / 2.5, window.getY());
		layout.textBoxBounds = window.expanded(0, window.getHeight() * 0.95).removeFromLeft(window.getWidth() * 0.9);

		return layout;
	}
}

 void MySliderLookAndFeel::drawLabel(Graphics& g, Label& label)
 {
	 auto yPos		= static_cast<float>(label.getLocalBounds().getY());
	 auto height	= static_cast<float>(label.getLocalBounds().getHeight());
	 auto centre	= static_cast<float>(label.getLocalBounds().getCentreX());
	 auto xPos		= static_cast<float>(label.getLocalBounds().getX());
	 auto width		= static_cast<float>(label.getLocalBounds().getWidth());

	 auto font = getSevenSegmentFont().withHeight(height * 0.85);

	if(isVertical)
	{
		auto xPosAdjusted	= static_cast<float>(centre - (width * 0.12));
		auto widthAdjusted	= static_cast<float>(width * 0.24);
		
		Rectangle<float> labelRectangle{ xPosAdjusted, yPos, widthAdjusted, height };
		
		g.setColour(Colour(0xff78c1de));
		g.setFont(font);
		g.drawFittedText(label.getText(), xPosAdjusted, yPos, widthAdjusted, height, Justification::centred, 1, 0.0f);
		g.setColour(Colour(0xff78c1de));
		g.drawRoundedRectangle(labelRectangle, 5.0f, 2.0f);
	}
	if(!isVertical)
	{
		Rectangle<float> labelRectangle{ xPos, yPos, width, height };

		g.setColour(Colour(0xff78c1de));
		g.setFont(font);
		g.drawFittedText(label.getText(), xPos, yPos, width, height, Justification::centred, 1, 0.0f);
		g.setColour(Colour(0xff78c1de));
		g.drawRoundedRectangle(labelRectangle, 5.0f, 2.0f);
	}
 }

 Button* MySliderLookAndFeel::createSliderButton(Slider&, bool isIncrement)
 {
	auto button = new TextButton(isIncrement ? "PLUS" : "MINUS", String());
	button->setLookAndFeel(&buttonLookAndFeel);
	return button;
 }