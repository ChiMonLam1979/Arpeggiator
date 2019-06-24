#include "ArpeggiatorEditor.h"

ArpeggiatorEditor::ArpeggiatorEditor(Arpeggiator& p) : AudioProcessorEditor(&p), processor(p)
{
	noteLengthSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::NoteLengthId, noteLengthSlider);
	swingFactorSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::SwingFactorId, swingFactorSlider);
	noteShiftSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::NoteShiftId, noteShiftSlider);
	noteShiftSliderButtonsAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::NoteShiftId, noteShiftButtonsSlider);

	noteDivisionRadioGroup = std::make_unique<ChoiceParameterRadioGroup>(processor.treeState, IDs::NoteDivisionId, ChoiceParameterRadioGroup::orientation::horizontal);
	arpModeRadioGroup = std::make_unique<ChoiceParameterRadioGroup>(processor.treeState, IDs::ArpModeId, ChoiceParameterRadioGroup::orientation::horizontal);
	latchModeRadioGroup = std::make_unique<ChoiceParameterRadioGroup>(processor.treeState, IDs::LatchModeId, ChoiceParameterRadioGroup::orientation::horizontal);
	latchLockRadioGroup = std::make_unique<ChoiceParameterRadioGroup>(processor.treeState, IDs::LatchLockId, ChoiceParameterRadioGroup::orientation::horizontal);
	arpSlotRadioGroup = std::make_unique<ChoiceParameterRadioGroup>(processor.treeState, IDs::ArpSlotId, ChoiceParameterRadioGroup::orientation::horizontal);

	setResizable(false, false);
	setSize(550, 800);

	noteLengthSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 30);
	noteLengthSlider.setRange(0.0, 1.0, 0.01);
	noteLengthSlider.textFromValueFunction = nullptr;
	noteLengthSlider.setNumDecimalPlacesToDisplay(2);
	noteLengthSlider.setLookAndFeel(&sliderLookAndFeel);
	noteLengthSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);

	swingFactorSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 30);
	swingFactorSlider.setRange(0.0, 1.0);
	swingFactorSlider.textFromValueFunction = nullptr;
	swingFactorSlider.setNumDecimalPlacesToDisplay(2);
	swingFactorSlider.setLookAndFeel(&sliderLookAndFeel);
	swingFactorSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);

	noteShiftSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 30);
	noteShiftSlider.setRange(-32.0f, 32.0f, 0.5);
	noteShiftSlider.setNumDecimalPlacesToDisplay(1);
	noteShiftSlider.setLookAndFeel(&sliderLookAndFeel);
	noteShiftSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	noteShiftSlider.onValueChange = [this] { noteShiftButtonsSlider.setValue(noteShiftSlider.getValue()); };

	noteShiftButtonsSlider.setTextBoxStyle(Slider::NoTextBox, true, 80, 30);
	noteShiftButtonsSlider.setRange(-32.0f, 32.0f, 0.5);
	noteShiftButtonsSlider.setNumDecimalPlacesToDisplay(1);
	noteShiftButtonsSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	noteShiftButtonsSlider.setLookAndFeel(&sliderLookAndFeel);
	noteShiftButtonsSlider.onValueChange = [this] { noteShiftSlider.setValue(noteShiftButtonsSlider.getValue()); };

	noteDivisionLabel.setLookAndFeel(&labelLookAndFeel);
	arpModeLabel.setLookAndFeel(&labelLookAndFeel);
	latchModeLabel.setLookAndFeel(&labelLookAndFeel);
	latchLockLabel.setLookAndFeel(&labelLookAndFeel);
	noteShiftLabel.setLookAndFeel(&labelLookAndFeel);
	swingFactorLabel.setLookAndFeel(&labelLookAndFeel);
	noteLengthLabel.setLookAndFeel(&labelLookAndFeel);

	addAndMakeVisible(noteLengthSlider);
	addAndMakeVisible(swingFactorSlider);
	addAndMakeVisible(noteShiftSlider);
	addAndMakeVisible(noteShiftButtonsSlider);
	addAndMakeVisible(noteDivisionRadioGroup.get());
	addAndMakeVisible(arpModeRadioGroup.get());
	addAndMakeVisible(latchModeRadioGroup.get());
	addAndMakeVisible(latchLockRadioGroup.get());
	addAndMakeVisible(arpSlotRadioGroup.get());
	addAndMakeVisible(noteDivisionLabel);
	addAndMakeVisible(arpModeLabel);
	addAndMakeVisible(latchModeLabel);
	addAndMakeVisible(latchLockLabel);
	addAndMakeVisible(noteShiftLabel);
	addAndMakeVisible(swingFactorLabel);
	addAndMakeVisible(noteLengthLabel);
}

ArpeggiatorEditor::~ArpeggiatorEditor()
{
}

void ArpeggiatorEditor::paint(Graphics& g)
{
	g.fillAll(Colours::black);
}

void ArpeggiatorEditor::resized()
{
	auto window = getLocalBounds();

	FlexBox noteDivisionLabelBox;
	noteDivisionLabelBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	noteDivisionLabelBox.items.addArray({ makeLabelBoxItem(noteDivisionLabel).withFlex(1) });

	FlexBox noteDivionButtons;
	noteDivionButtons.justifyContent = FlexBox::JustifyContent::spaceBetween;
	noteDivionButtons.items.addArray({ makeButtonBoxItem(*noteDivisionRadioGroup).withFlex(1) });

	FlexBox arpModeLabelBox;
	arpModeLabelBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	arpModeLabelBox.items.addArray({ makeLabelBoxItem(arpModeLabel).withFlex(1) });

	FlexBox arpModeButtonsBox;
	arpModeButtonsBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	arpModeButtonsBox.items.addArray({ makeButtonBoxItem(*arpModeRadioGroup).withFlex(1) });

	FlexBox latchModeLabelBox;
	latchModeLabelBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	latchModeLabelBox.items.addArray({ 
										makeLabelBoxItem(latchModeLabel).withFlex(1),
										makeLabelBoxItem(latchLockLabel).withFlex(1)
									});

	FlexBox latchModeButtonsBox;
	latchModeButtonsBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	latchModeButtonsBox.items.addArray({ 
										makeButtonBoxItem(*latchModeRadioGroup).withFlex(1),
										makeButtonBoxItem(*latchLockRadioGroup).withFlex(1)
									});

	FlexBox noteLengthLabelBox;
	noteLengthLabelBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	noteLengthLabelBox.items.addArray({ makeLabelBoxItem(noteLengthLabel).withFlex(1) });

	FlexBox noteLengthSliderBox;
	noteLengthSliderBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	noteLengthSliderBox.items.addArray({ makeSliderBoxItem(noteLengthSlider).withFlex(1) });

	FlexBox noteShiftLabelBox;
	noteShiftLabelBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	noteShiftLabelBox.items.addArray({ makeLabelBoxItem(noteShiftLabel).withFlex(1) });

	FlexBox noteShiftSliderBox;
	noteShiftSliderBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	noteShiftSliderBox.items.addArray({ makeSliderBoxItem(noteShiftSlider).withFlex(1) });

	FlexBox noteShiftSliderButtonsBox;
	noteShiftSliderButtonsBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	noteShiftSliderButtonsBox.items.addArray({ makeSliderButtonBoxItem(noteShiftButtonsSlider).withFlex(1) });

	FlexBox swingFactorLabelBox;
	swingFactorLabelBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	swingFactorLabelBox.items.addArray({ makeLabelBoxItem(swingFactorLabel).withFlex(1) });

	FlexBox swingFactorSliderBox;
	swingFactorSliderBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	swingFactorSliderBox.items.addArray({ makeSliderBoxItem(swingFactorSlider).withFlex(1) });

	FlexBox arpSlotButtonsBox;
	arpSlotButtonsBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	arpSlotButtonsBox.items.addArray({ makeButtonBoxItem(*arpSlotRadioGroup).withFlex(1) });

	FlexBox masterBox;

	masterBox.flexDirection = FlexBox::Direction::column;

	masterBox.items.addArray({  
								FlexItem(noteDivisionLabelBox).withFlex(0.4),
								FlexItem(noteDivionButtons).withFlex(1),
								FlexItem(arpModeLabelBox).withFlex(0.4),
								FlexItem(arpModeButtonsBox).withFlex(1),
								FlexItem(latchModeLabelBox).withFlex(0.4),
								FlexItem(latchModeButtonsBox).withFlex(1),
								FlexItem(noteLengthLabelBox).withFlex(0.4),
								FlexItem(noteLengthSliderBox).withFlex(1),
								FlexItem(swingFactorLabelBox).withFlex(0.4),
								FlexItem(swingFactorSliderBox).withFlex(1),
								FlexItem(noteShiftLabelBox).withFlex(0.4),
								FlexItem(noteShiftSliderBox).withFlex(1),
								FlexItem(noteShiftSliderButtonsBox).withFlex(1),
								FlexItem(arpSlotButtonsBox).withFlex(1),
	});

	masterBox.performLayout(window);
}

static constexpr float buttonHeight = 100.0f;
static constexpr float sliderHeight = 100.0f;
static constexpr float sliderButtonHeight = 100.0f;
static constexpr float labelHeight = 100.0f;


FlexItem ArpeggiatorEditor::makeButtonBoxItem(Component& component)
{
	return FlexItem(component).withMaxHeight(buttonHeight).withFlex(1);
}

FlexItem ArpeggiatorEditor::makeSliderBoxItem(Component& component)
{
	return FlexItem(component).withMaxHeight(sliderHeight).withFlex(1);
}

FlexItem ArpeggiatorEditor::makeSliderButtonBoxItem(Component& component)
{
	return FlexItem(component).withMaxHeight(sliderButtonHeight).withFlex(1);
}

FlexItem ArpeggiatorEditor::makeLabelBoxItem(Component& component)
{
	return FlexItem(component).withMaxHeight(labelHeight).withFlex(1);
}