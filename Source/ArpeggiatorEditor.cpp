#include "ArpeggiatorEditor.h"

ArpeggiatorEditor::ArpeggiatorEditor(Arpeggiator& p) : AudioProcessorEditor(&p), processor(p)
{
	setResizable(true, false);
	setSize(200, 200);

	comboBoxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, IDs::NoteDivisionId, comboBox);

	const StringArray choices
	{
		ParamterChoices::QuarterNoteDivision,
		ParamterChoices::EighthNoteDivision,
		ParamterChoices::EighthNoteTripletDivision,
		ParamterChoices::SixteenthNoteDivision,
		ParamterChoices::SixteenthNoteTripletDivision,
		ParamterChoices::ThirtySecondNoteDivision,
		ParamterChoices::ThirtySecondNoteTripletDivision
	};

	comboBox.addItemList(choices, 1);

	addAndMakeVisible(comboBox);
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
	comboBox.setBounds(getLocalBounds());
}
