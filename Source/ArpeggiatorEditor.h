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

	ComboBox comboBox;

public:

	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> comboBoxAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArpeggiatorEditor)
};
