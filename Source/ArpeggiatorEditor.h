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
	ComboBox latchModeComboBox;
	ComboBox latchLockComboBox;
	Slider noteLengthSlider;
	Slider noteLengthButtonsSlider;
	Slider swingFactorSlider;

public:

	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> noteDivisionComboBoxAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> playModeComboBoxAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> latchModeComboBoxAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> latchLockComboBoxAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteLengthSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteLengthSliderButtonsAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> swingFactorSliderAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArpeggiatorEditor)
};
