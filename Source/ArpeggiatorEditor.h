#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Arpeggiator.h"

class ArpeggiatorEditor : public AudioProcessorEditor
{
public:

	ArpeggiatorEditor(Arpeggiator&);
	~ArpeggiatorEditor();

	void paint(Graphics&) override;
	void resized() override;

private:

	Arpeggiator& processor;

	ComboBox noteDiviosnComboBox;
	ComboBox playModeComboBox;

public:

	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> noteDivisionComboBoxAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> playModeComboBoxAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArpeggiatorEditor)
};
