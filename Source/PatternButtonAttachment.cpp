#include "ArpMode.h"
#include "PatternButtonAttachment.h"

PatternButtonAttachment::PatternButtonAttachment(
	AudioProcessorValueTreeState& treeState,
	const String& pID,
	SlotButton& slot1Button,
	SlotButton& slot2Button,
	SlotButton& slot3Button,
	SlotButton& slot4Button,
	Slider& slot1PlayCountButtons,
	Slider& slot2PlayCountButtons,
	Slider& slot3PlayCountButtons,
	Slider& slot4PlayCountButtons,
	Slider& slot1OrderButtons,
	Slider& slot2OrderButtons,
	Slider& slot3OrderButtons,
	Slider& slot4OrderButtons)
	: treeState(treeState),
	parameterID(pID),
	slot1Button(slot1Button),
	slot2Button(slot2Button),
	slot3Button(slot3Button),
	slot4Button(slot4Button),
	slot1PlayCountButtons(slot1PlayCountButtons),
	slot2PlayCountButtons(slot2PlayCountButtons),
	slot3PlayCountButtons(slot3PlayCountButtons),
	slot4PlayCountButtons(slot4PlayCountButtons),
	slot1OrderButtons(slot1OrderButtons),
	slot2OrderButtons(slot2OrderButtons),
	slot3OrderButtons(slot3OrderButtons),
	slot4OrderButtons(slot4OrderButtons)
{
	treeState.addParameterListener(pID, this);
}

PatternButtonAttachment::~PatternButtonAttachment()
{
}

bool PatternButtonAttachment::PatternModeIsOn()
{
	return state == Enums::patternMode::patternsOn;
}

void PatternButtonAttachment::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	state = static_cast<Enums::patternMode>(choice);

	if(state == Enums::patternsOn)
	{
		DisableSlotButtons();
	}
	else
	{
		EnableSlotButtons();
	}
}

void PatternButtonAttachment::DisableSlotButtons()
{
	slot1Button.setEnabled(false);
	slot2Button.setEnabled(false);
	slot3Button.setEnabled(false);
	slot4Button.setEnabled(false);
	slot1PlayCountButtons.setEnabled(false);
	slot2PlayCountButtons.setEnabled(false);
	slot3PlayCountButtons.setEnabled(false);
	slot4PlayCountButtons.setEnabled(false);
	slot1OrderButtons.setEnabled(false);
	slot2OrderButtons.setEnabled(false);
	slot3OrderButtons.setEnabled(false);
	slot4OrderButtons.setEnabled(false);
}

void PatternButtonAttachment::EnableSlotButtons()
{
	slot1Button.setEnabled(true);
	slot2Button.setEnabled(true);
	slot3Button.setEnabled(true);
	slot4Button.setEnabled(true);
	slot1PlayCountButtons.setEnabled(true);
	slot2PlayCountButtons.setEnabled(true);
	slot3PlayCountButtons.setEnabled(true);
	slot4PlayCountButtons.setEnabled(true);
	slot1OrderButtons.setEnabled(true);
	slot2OrderButtons.setEnabled(true);
	slot3OrderButtons.setEnabled(true);
	slot4OrderButtons.setEnabled(true);
}
