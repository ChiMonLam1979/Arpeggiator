#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ChoiceParameterRadioGroup.cpp"
#include "Arpeggiator.h"
#include "MyLabelLookAndFeel.h"
#include "MySliderLookAndFeel.h"
#include "SlotButton.h"
#include "MySlotButtonLookAndFeel.h"
#include "MySlotSliderLookAndFeel.h"
#include "PatternButtonAttachment.h"

class ArpeggiatorEditor : public AudioProcessorEditor
{
public:

	ArpeggiatorEditor(Arpeggiator&);
	~ArpeggiatorEditor();

	void paint(Graphics&) override;
	void resized() override;

private:

	MyLabelLookAndFeel labelLookAndFeel;
	MySliderLookAndFeel sliderLookAndFeel;
	MyTextButtonLookAndFeel textButtonLookAndFeel;
	MySlotButtonLookAndFeel slotButtonLookAndFeel;
	MySlotSliderLookAndFeel slotSliderLookAndFeel;

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
	Label slotRepeatLabel{ ParameterNames::SlotRepeatName };
	Label slotOrderLabel{ ParameterNames::SlotOrderName };
	Label patternModeLabel{ ParameterNames::PatternModeName };

	SlotButton slot1Button{ ParameterNames::Slot1Name };
	SlotButton slot2Button{ ParameterNames::Slot2Name };
	SlotButton slot3Button{ ParameterNames::Slot3Name };
	SlotButton slot4Button{ ParameterNames::Slot4Name };

	Slider slot1RepeatButtons;
	Slider slot2RepeatButtons;
	Slider slot3RepeatButtons;
	Slider slot4RepeatButtons;

	Slider slot1OrderButtons;
	Slider slot2OrderButtons;
	Slider slot3OrderButtons;
	Slider slot4OrderButtons;

	static FlexItem makeButtonBoxItem(Component& component);
	static FlexItem makeSliderBoxItem(Component& component);
	static FlexItem makeSliderButtonBoxItem(Component& component);
	static FlexItem makeLabelBoxItem(Component& component);
	static FlexItem makeSlotIncBoxItem(Component& component);
	static FlexItem makeSlotLabelBoxItem(Component& component);

public:

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteLengthSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> swingFactorSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteShiftSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteShiftSliderButtonsAttachment;

	std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> slot1ButtonAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> slot2ButtonAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> slot3ButtonAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> slot4ButtonAttachment;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slot1RepeatButtonsAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slot2RepeatButtonsAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slot3RepeatButtonsAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slot4RepeatButtonsAttachment;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slot1OrderButtonsAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slot2OrderButtonsAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slot3OrderButtonsAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> slot4OrderButtonsAttachment;

	std::unique_ptr<ChoiceParameterRadioGroup> noteDivisionRadioGroup;
	std::unique_ptr<ChoiceParameterRadioGroup> arpModeRadioGroup;
	std::unique_ptr<ChoiceParameterRadioGroup> latchModeRadioGroup;
	std::unique_ptr<ChoiceParameterRadioGroup> latchLockRadioGroup;

	std::unique_ptr<ChoiceParameterRadioGroup> patternModeRadioGroup;
	std::unique_ptr<PatternButtonAttachment> patternButtonAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArpeggiatorEditor)
};
