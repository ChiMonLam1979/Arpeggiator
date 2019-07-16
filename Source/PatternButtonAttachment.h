#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ArpMode.h"
#include "SlotButton.h"

class PatternButtonAttachment : public TextButton,
								private AudioProcessorValueTreeState::Listener
{
public:

	PatternButtonAttachment(
		AudioProcessorValueTreeState& treeState,
		const String& pID,
		SlotButton& slot1Button,
		SlotButton& slot2Button,
		SlotButton& slot3Button,
		SlotButton& slot4Button);

	~PatternButtonAttachment();

	bool PatternModeIsOn();

private:

	void parameterChanged(const String& parameterID, float newValue) override;

	void DisableSlotButtons();

	void EnableSlotButtons();

	Enums::patternMode state{ Enums::patternsOff };
	AudioProcessorValueTreeState& treeState;
	String parameterID;
	SlotButton& slot1Button;
	SlotButton& slot2Button;
	SlotButton& slot3Button;
	SlotButton& slot4Button;
};
