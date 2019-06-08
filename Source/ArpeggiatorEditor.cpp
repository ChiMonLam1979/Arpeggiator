#include "ArpeggiatorEditor.h"

ArpeggiatorEditor::ArpeggiatorEditor(Arpeggiator& p) : AudioProcessorEditor(&p), processor(p)
{
	setResizable(true, false);
	setSize(200, 200);

	noteDivisionComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::NoteDivisionId, noteDiviosnComboBox);
	playModeComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::ArpPlayModeId, playModeComboBox);
	latchModeComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::LatchModeId, latchModeComboBox);
	latchLockComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::LatchLockId, latchLockComboBox);

	noteDiviosnComboBox.addItemList(ParamterChoices::noteDivisionChoices, 1);
	playModeComboBox.addItemList(ParamterChoices::playModeChoices, 1);
	latchModeComboBox.addItemList(ParamterChoices::latchModeChoices, 1);
	latchLockComboBox.addItemList(ParamterChoices::latchLockChoices, 1);

	addAndMakeVisible(noteDiviosnComboBox);
	addAndMakeVisible(playModeComboBox);
	addAndMakeVisible(latchModeComboBox);
	addAndMakeVisible(latchLockComboBox);
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
	latchLockComboBox.setBounds(localBounds);
}
