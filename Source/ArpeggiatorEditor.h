#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ChoiceParameterRadioGroup.cpp"
#include "Arpeggiator.h"
#include "MyLabelLookAndFeel.h"
#include "MySliderLookAndFeel.h"

class ArpeggiatorEditor : public AudioProcessorEditor
{
public:

	ArpeggiatorEditor(Arpeggiator&);
	~ArpeggiatorEditor();

	void paint(Graphics&) override;
	void resized() override;

private:

	Arpeggiator& processor;

	Slider noteLengthSlider { ParameterNames::NoteLengthName };
	Slider swingFactorSlider { ParameterNames::SwingFactorName };
	Slider noteShiftSlider { ParameterNames::NoteShiftName };
	Slider noteShiftButtonsSlider;

	Label noteDivisionLabel{ ParameterNames::NoteDivisionName };
	Label playModeLabel{ ParameterNames::ArpPlayModeName };
	Label latchModeLabel{ ParameterNames::LatchModeName };
	Label latchLockLabel{ ParameterNames::LatchLockName };
	Label noteShiftLabel{ ParameterNames::NoteShiftName };
	Label swingFactorLabel{ ParameterNames::SwingFactorName };
	Label noteLengthLabel{ ParameterNames::NoteLengthName };

	MyLabelLookAndFeel labelLookAndFeel;
	MySliderLookAndFeel sliderLookAndFeel;

	static FlexItem makeButtonBoxItem(Component& component);
	static FlexItem makeSliderBoxItem(Component& component);
	static FlexItem makeSliderButtonBoxItem(Component& component);
	static FlexItem makeLabelBoxItem(Component& component);

public:

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteLengthSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> swingFactorSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteShiftSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteShiftSliderButtonsAttachment;

	std::unique_ptr<ChoiceParameterRadioGroup> noteDivisionRadioGroup;
	std::unique_ptr<ChoiceParameterRadioGroup> playModeRadioGroup;
	std::unique_ptr<ChoiceParameterRadioGroup> latchModeRadioGroup;
	std::unique_ptr<ChoiceParameterRadioGroup> latchLockRadioGroup;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArpeggiatorEditor)
};
