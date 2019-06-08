#include "ArpeggiatorEditor.h"

ArpeggiatorEditor::ArpeggiatorEditor(Arpeggiator& p) : AudioProcessorEditor(&p), processor(p)
{
	setResizable(true, false);
	setSize(250, 250);

	noteDivisionComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::NoteDivisionId, noteDiviosnComboBox);
	playModeComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::ArpPlayModeId, playModeComboBox);
	latchModeComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::LatchModeId, latchModeComboBox);
	latchLockComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::LatchLockId, latchLockComboBox);
	noteLengthSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, IDs::NoteLengthId, noteLengthSlider);

	noteDiviosnComboBox.addItemList(ParamterChoices::NoteDivisionChoices, 1);
	playModeComboBox.addItemList(ParamterChoices::PlayModeChoices, 1);
	latchModeComboBox.addItemList(ParamterChoices::LatchModeChoices, 1);
	latchLockComboBox.addItemList(ParamterChoices::LatchLockChoices, 1);

	noteLengthSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 30);
	noteLengthSlider.setRange(0.0, 1.0);
	noteLengthSlider.setValue(0.5);
	noteLengthSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	noteLengthSlider.setIncDecButtonsMode(Slider::incDecButtonsNotDraggable);

	addAndMakeVisible(noteDiviosnComboBox);
	addAndMakeVisible(playModeComboBox);
	addAndMakeVisible(latchModeComboBox);
	addAndMakeVisible(latchLockComboBox);
	addAndMakeVisible(noteLengthSlider);
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
	noteLengthSlider.setBounds(localBounds);
}
