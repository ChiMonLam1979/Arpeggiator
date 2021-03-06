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

	patternModeRadioGroup = std::make_unique<ChoiceParameterRadioGroup>(processor.treeState, IDs::PatternModeId, ChoiceParameterRadioGroup::orientation::horizontal);
	patternButtonAttachment = std::make_unique<PatternButtonAttachment>(
		processor.treeState,
		IDs::PatternModeId,
		slot1Button,
		slot2Button,
		slot3Button,
		slot4Button,
		slot1PlayCountButtons,
		slot2PlayCountButtons,
		slot3PlayCountButtons,
		slot4PlayCountButtons,
		slot1OrderButtons,
		slot2OrderButtons,
		slot3OrderButtons,
		slot4OrderButtons);

	slot1OrderButtonsAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::Slot1OrderId, slot1OrderButtons);
	slot2OrderButtonsAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::Slot2OrderId, slot2OrderButtons);
	slot3OrderButtonsAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::Slot3OrderId, slot3OrderButtons);
	slot4OrderButtonsAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::Slot4OrderId, slot4OrderButtons);

	slot1OrderButtons.addListener(&slotPlayOrder);
	slot2OrderButtons.addListener(&slotPlayOrder);
	slot3OrderButtons.addListener(&slotPlayOrder);
	slot4OrderButtons.addListener(&slotPlayOrder);

	slot1ButtonAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.treeState, IDs::Slot1Id, slot1Button);
	slot2ButtonAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.treeState, IDs::Slot2Id, slot2Button);
	slot3ButtonAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.treeState, IDs::Slot3Id, slot3Button);
	slot4ButtonAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.treeState, IDs::Slot4Id, slot4Button);

	slot1PlayCountButtonsAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::Slot1PlayCountId, slot1PlayCountButtons);
	slot2PlayCountButtonsAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::Slot2PlayCountId, slot2PlayCountButtons);
	slot3PlayCountButtonsAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::Slot3PlayCountId, slot3PlayCountButtons);
	slot4PlayCountButtonsAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::Slot4PlayCountId, slot4PlayCountButtons);

	setResizable(false, false);
	setSize(600, 650);

	noteLengthSlider.setTextBoxStyle(Slider::TextBoxRight, true, 80, 30);
	noteLengthSlider.setRange(0.0, 1.0, 0.01);
	noteLengthSlider.textFromValueFunction = nullptr;
	noteLengthSlider.setNumDecimalPlacesToDisplay(2);
	noteLengthSlider.setLookAndFeel(&sliderLookAndFeel);
	noteLengthSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);

	swingFactorSlider.setTextBoxStyle(Slider::TextBoxRight, true, 80, 30);
	swingFactorSlider.setRange(0.0, 1.0);
	swingFactorSlider.textFromValueFunction = nullptr;
	swingFactorSlider.setNumDecimalPlacesToDisplay(2);
	swingFactorSlider.setLookAndFeel(&sliderLookAndFeel);
	swingFactorSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);

	noteShiftSlider.setTextBoxStyle(Slider::TextBoxRight, true, 80, 30);
	noteShiftSlider.setRange(-32.0f, 32.0f, 0.25);
	noteShiftSlider.setNumDecimalPlacesToDisplay(1);
	noteShiftSlider.setLookAndFeel(&sliderLookAndFeel);
	noteShiftSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	noteShiftSlider.onValueChange = [this] { noteShiftButtonsSlider.setValue(noteShiftSlider.getValue()); };

	noteShiftButtonsSlider.setTextBoxStyle(Slider::NoTextBox, true, 80, 30);
	noteShiftButtonsSlider.setRange(-32.0f, 32.0f, 0.25);
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
	slotPlayCountLabel.setLookAndFeel(&labelLookAndFeel);
	slotOrderLabel.setLookAndFeel(&labelLookAndFeel);
	patternModeLabel.setLookAndFeel(&labelLookAndFeel);

	slot1Button.setClickingTogglesState(true);
	slot1Button.setLookAndFeel(&slotButtonLookAndFeel);
	slot2Button.setClickingTogglesState(true);
	slot2Button.setLookAndFeel(&slotButtonLookAndFeel);
	slot3Button.setClickingTogglesState(true);
	slot3Button.setLookAndFeel(&slotButtonLookAndFeel);
	slot4Button.setClickingTogglesState(true);
	slot4Button.setLookAndFeel(&slotButtonLookAndFeel);

	slot1PlayCountButtons.setTextBoxStyle(Slider::TextBoxRight, true, 50, 30);
	slot1PlayCountButtons.setRange(0.0f, 4.0f, 1.0f);
	slot1PlayCountButtons.setNumDecimalPlacesToDisplay(0);
	slot1PlayCountButtons.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	slot1PlayCountButtons.setLookAndFeel(&slotSliderLookAndFeel);

	slot2PlayCountButtons.setTextBoxStyle(Slider::TextBoxRight, true, 50, 30);
	slot2PlayCountButtons.setRange(0.0f, 4.0f, 1.0f);
	slot2PlayCountButtons.setNumDecimalPlacesToDisplay(0);
	slot2PlayCountButtons.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	slot2PlayCountButtons.setLookAndFeel(&slotSliderLookAndFeel);

	slot3PlayCountButtons.setTextBoxStyle(Slider::TextBoxRight, true, 50, 30);
	slot3PlayCountButtons.setRange(0.0f, 4.0f, 1.0f);
	slot3PlayCountButtons.setNumDecimalPlacesToDisplay(0);
	slot3PlayCountButtons.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	slot3PlayCountButtons.setLookAndFeel(&slotSliderLookAndFeel);

	slot4PlayCountButtons.setTextBoxStyle(Slider::TextBoxRight, true, 50, 30);
	slot4PlayCountButtons.setRange(0.0f, 4.0f, 1.0f);
	slot4PlayCountButtons.setNumDecimalPlacesToDisplay(0);
	slot4PlayCountButtons.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	slot4PlayCountButtons.setLookAndFeel(&slotSliderLookAndFeel);

	slot1OrderButtons.setTextBoxStyle(Slider::TextBoxRight, true, 50, 30);
	slot1OrderButtons.setRange(1.0f, 4.0f, 1.0f);
	slot1OrderButtons.setNumDecimalPlacesToDisplay(0);
	slot1OrderButtons.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	slot1OrderButtons.setLookAndFeel(&slotSliderLookAndFeel);
	
	slot2OrderButtons.setTextBoxStyle(Slider::TextBoxRight, true, 50, 30);
	slot2OrderButtons.setRange(1.0f, 4.0f, 1.0f);
	slot2OrderButtons.setNumDecimalPlacesToDisplay(0);
	slot2OrderButtons.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	slot2OrderButtons.setLookAndFeel(&slotSliderLookAndFeel);
	
	slot3OrderButtons.setTextBoxStyle(Slider::TextBoxRight, true, 50, 30);
	slot3OrderButtons.setRange(1.0f, 4.0f, 1.0f);
	slot3OrderButtons.setNumDecimalPlacesToDisplay(0);
	slot3OrderButtons.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	slot3OrderButtons.setLookAndFeel(&slotSliderLookAndFeel);
	
	slot4OrderButtons.setTextBoxStyle(Slider::TextBoxRight, true, 50, 30);
	slot4OrderButtons.setRange(1.0f, 4.0f, 1.0f);
	slot4OrderButtons.setNumDecimalPlacesToDisplay(0);
	slot4OrderButtons.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	slot4OrderButtons.setLookAndFeel(&slotSliderLookAndFeel);

	addAndMakeVisible(noteLengthSlider);
	addAndMakeVisible(swingFactorSlider);
	addAndMakeVisible(noteShiftSlider);
	addAndMakeVisible(noteShiftButtonsSlider);
	addAndMakeVisible(noteDivisionRadioGroup.get());
	addAndMakeVisible(arpModeRadioGroup.get());
	addAndMakeVisible(latchModeRadioGroup.get());
	addAndMakeVisible(latchLockRadioGroup.get());
	addAndMakeVisible(patternModeRadioGroup.get());
	addAndMakeVisible(noteDivisionLabel);
	addAndMakeVisible(arpModeLabel);
	addAndMakeVisible(latchModeLabel);
	addAndMakeVisible(latchLockLabel);
	addAndMakeVisible(noteShiftLabel);
	addAndMakeVisible(swingFactorLabel);
	addAndMakeVisible(noteLengthLabel);
	addAndMakeVisible(slotPlayCountLabel);
	addAndMakeVisible(slotOrderLabel);
	addAndMakeVisible(patternModeLabel);
	addAndMakeVisible(slot1Button);
	addAndMakeVisible(slot2Button);
	addAndMakeVisible(slot3Button);
	addAndMakeVisible(slot4Button);
	addAndMakeVisible(slot1PlayCountButtons);
	addAndMakeVisible(slot2PlayCountButtons);
	addAndMakeVisible(slot3PlayCountButtons);
	addAndMakeVisible(slot4PlayCountButtons);
	addAndMakeVisible(slot1OrderButtons);
	addAndMakeVisible(slot2OrderButtons);
	addAndMakeVisible(slot3OrderButtons);
	addAndMakeVisible(slot4OrderButtons);
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
										makeLabelBoxItem(latchLockLabel).withFlex(1),
										makeLabelBoxItem(patternModeLabel).withFlex(1)
									});

	FlexBox latchModeButtonsBox;
	latchModeButtonsBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	latchModeButtonsBox.items.addArray({ 
										makeButtonBoxItem(*latchModeRadioGroup).withFlex(1),
										makeButtonBoxItem(*latchLockRadioGroup).withFlex(1),
										makeButtonBoxItem(*patternModeRadioGroup).withFlex(1)
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

	FlexBox slotButtonsBox;
	slotButtonsBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	slotButtonsBox.items.addArray({
									makeButtonBoxItem(slot1Button).withFlex(1),
									makeButtonBoxItem(slot2Button).withFlex(1),
									makeButtonBoxItem(slot3Button).withFlex(1),
									makeButtonBoxItem(slot4Button).withFlex(1)
	});

	FlexBox slotRepeatLabelBox;
	slotRepeatLabelBox.items.addArray({ makeSlotLabelBoxItem(slotPlayCountLabel).withFlex(1) });

	FlexBox slotRepeatButtonsBox;
	slotRepeatButtonsBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	slotRepeatButtonsBox.items.addArray({
										makeSlotIncBoxItem(slot1PlayCountButtons).withFlex(1),
										makeSlotIncBoxItem(slot2PlayCountButtons).withFlex(1),
										makeSlotIncBoxItem(slot3PlayCountButtons).withFlex(1),
										makeSlotIncBoxItem(slot4PlayCountButtons).withFlex(1),
		});

	FlexBox slotOrderLabelBox;
	slotOrderLabelBox.items.addArray({ makeSlotLabelBoxItem(slotOrderLabel).withFlex(1) });

	FlexBox slotOrderButtonsBox;
	slotOrderButtonsBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	slotOrderButtonsBox.items.addArray({
										makeSlotIncBoxItem(slot1OrderButtons).withFlex(1),
										makeSlotIncBoxItem(slot2OrderButtons).withFlex(1),
										makeSlotIncBoxItem(slot3OrderButtons).withFlex(1),
										makeSlotIncBoxItem(slot4OrderButtons).withFlex(1),
		});

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
								FlexItem(slotOrderLabelBox).withFlex(0.4),
								FlexItem(slotOrderButtonsBox).withFlex(0.5),
								FlexItem(slotRepeatLabelBox).withFlex(0.4),
								FlexItem(slotRepeatButtonsBox).withFlex(0.5),
								FlexItem(slotButtonsBox).withFlex(1),
	});

	masterBox.performLayout(window);
}

static constexpr float buttonHeight = 100.0f;
static constexpr float sliderHeight = 100.0f;
static constexpr float sliderButtonHeight = 100.0f;
static constexpr float labelHeight = 100.0f;
static constexpr float slotLabelHeight = 100.0f;
static constexpr float slotIncButtonHeight = 100.0f;


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

FlexItem ArpeggiatorEditor::makeSlotIncBoxItem(Component& component)
{
	return FlexItem(component).withMaxHeight(slotIncButtonHeight).withFlex(1);
}

FlexItem ArpeggiatorEditor::makeSlotLabelBoxItem(Component& component)
{
	return FlexItem(component).withMaxHeight(slotLabelHeight).withFlex(1);
}