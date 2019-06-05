#include "ArpeggiatorEditor.h"

ArpeggiatorEditor::ArpeggiatorEditor(Arpeggiator& p) : AudioProcessorEditor(&p), processor(p)
{
	setResizable(true, false);
	setSize(200, 200);

	noteDivisionComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::NoteDivisionId, noteDiviosnComboBox);
	playModeComboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::ArpPlayModeId, playModeComboBox);

	noteDiviosnComboBox.addItemList(ParamterChoices::noteDivisionChoices, 1);
	playModeComboBox.addItemList(ParamterChoices::playModeChoices, 1);

	addAndMakeVisible(noteDiviosnComboBox);
	addAndMakeVisible(playModeComboBox);
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
	noteDiviosnComboBox.setBounds(localBounds.removeFromBottom(100));
	playModeComboBox.setBounds(localBounds);
}
