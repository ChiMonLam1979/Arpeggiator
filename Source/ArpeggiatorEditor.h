#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ChoiceParameterRadioGroup.cpp"
#include "Arpeggiator.h"
#include "MyLabelLookAndFeel.h"
#include "MySliderLookAndFeel.h"
#include "SlotButton.h"
#include "MySlotButtonLookAndFeel.h"

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
	Label arpModeLabel{ ParameterNames::ArpModeName };
	Label latchModeLabel{ ParameterNames::LatchModeName };
	Label latchLockLabel{ ParameterNames::LatchLockName };
	Label noteShiftLabel{ ParameterNames::NoteShiftName };
	Label swingFactorLabel{ ParameterNames::SwingFactorName };
	Label noteLengthLabel{ ParameterNames::NoteLengthName };

	SlotButton slot1Button{ ParameterNames::Slot1Name };
	SlotButton slot2Button{ ParameterNames::Slot2Name };
	SlotButton slot3Button{ ParameterNames::Slot3Name };
	SlotButton slot4Button{ ParameterNames::Slot4Name };

	MyLabelLookAndFeel labelLookAndFeel;
	MySliderLookAndFeel sliderLookAndFeel;
	MyTextButtonLookAndFeel textButtonLookAndFeel;
	MySlotButtonLookAndFeel slotButtonLookAndFeel;

	static FlexItem makeButtonBoxItem(Component& component);
	static FlexItem makeSliderBoxItem(Component& component);
	static FlexItem makeSliderButtonBoxItem(Component& component);
	static FlexItem makeLabelBoxItem(Component& component);

public:

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteLengthSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> swingFactorSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteShiftSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteShiftSliderButtonsAttachment;

	std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> slot1ButtonAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> slot2ButtonAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> slot3ButtonAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> slot4ButtonAttachment;

	std::unique_ptr<ChoiceParameterRadioGroup> noteDivisionRadioGroup;
	std::unique_ptr<ChoiceParameterRadioGroup> arpModeRadioGroup;
	std::unique_ptr<ChoiceParameterRadioGroup> latchModeRadioGroup;
	std::unique_ptr<ChoiceParameterRadioGroup> latchLockRadioGroup;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArpeggiatorEditor)
};
