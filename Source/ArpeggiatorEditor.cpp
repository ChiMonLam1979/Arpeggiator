#include "ArpeggiatorEditor.h"

ArpeggiatorEditor::ArpeggiatorEditor(Arpeggiator& p) : AudioProcessorEditor(&p), processor(p)
{
	setResizable(true, false);
	setSize(350, 350);

	noteDivisionComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::NoteDivisionId, noteDiviosnComboBox);
	playModeComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::ArpPlayModeId, playModeComboBox);
	latchModeComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::LatchModeId, latchModeComboBox);
	latchLockComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::LatchLockId, latchLockComboBox);
	noteLengthSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::NoteLengthId, noteLengthSlider);
	noteLengthSliderButtonsAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::NoteLengthId, noteLengthButtonsSlider);
	swingFactorSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::SwingFactorId, swingFactorSlider);

	noteDiviosnComboBox.addItemList(ParamterChoices::NoteDivisionChoices, 1);
	playModeComboBox.addItemList(ParamterChoices::PlayModeChoices, 1);
	latchModeComboBox.addItemList(ParamterChoices::LatchModeChoices, 1);
	latchLockComboBox.addItemList(ParamterChoices::LatchLockChoices, 1);

	noteLengthSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 30);
	noteLengthSlider.setRange(0.0, 1.0);
	noteLengthSlider.setNumDecimalPlacesToDisplay(2);
	noteLengthSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	noteLengthSlider.onValueChange = [this] { noteLengthButtonsSlider.setValue(noteLengthSlider.getValue()); };

	noteLengthButtonsSlider.setTextBoxStyle(Slider::NoTextBox, true, 80, 30);
	noteLengthButtonsSlider.setRange(0.0, 1.0, 0.1);
	noteLengthButtonsSlider.setNumDecimalPlacesToDisplay(2);
	noteLengthButtonsSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	noteLengthButtonsSlider.onValueChange = [this] { noteLengthSlider.setValue(noteLengthButtonsSlider.getValue()); };

	swingFactorSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 30);
	swingFactorSlider.setRange(0.0, 1.0);
	swingFactorSlider.setNumDecimalPlacesToDisplay(2);
	swingFactorSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);

	addAndMakeVisible(noteDiviosnComboBox);
	addAndMakeVisible(playModeComboBox);
	addAndMakeVisible(latchModeComboBox);
	addAndMakeVisible(latchLockComboBox);
	addAndMakeVisible(noteLengthSlider);
	addAndMakeVisible(noteLengthButtonsSlider);
	addAndMakeVisible(swingFactorSlider);
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
	auto localBounds = getLocalBounds();
	noteDiviosnComboBox.setBounds(localBounds.removeFromBottom(50));
	playModeComboBox.setBounds(localBounds.removeFromBottom(50));
	latchModeComboBox.setBounds(localBounds.removeFromBottom(50));
	latchLockComboBox.setBounds(localBounds.removeFromBottom(50));
	noteLengthSlider.setBounds(localBounds.removeFromBottom(50));
	noteLengthButtonsSlider.setBounds(localBounds.removeFromBottom(50));
	swingFactorSlider.setBounds(localBounds);
}
