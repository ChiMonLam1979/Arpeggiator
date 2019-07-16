#include "ArpMode.h"
#include "PatternButtonAttachment.h"

PatternButtonAttachment::PatternButtonAttachment(
	AudioProcessorValueTreeState& treeState,
	const String& pID,
	SlotButton& slot1Button,
	SlotButton& slot2Button,
	SlotButton& slot3Button,
	SlotButton& slot4Button) 
	: treeState(treeState),
	parameterID(pID),
	slot1Button(slot1Button),
	slot2Button(slot2Button),
	slot3Button(slot3Button),
	slot4Button(slot4Button)
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
}

void PatternButtonAttachment::EnableSlotButtons()
{
	slot1Button.setEnabled(true);
	slot2Button.setEnabled(true);
	slot3Button.setEnabled(true);
	slot4Button.setEnabled(true);
}
