#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ArpMode.h"

class PatternButtonAttachment : public TextButton,
								private AudioProcessorValueTreeState::Listener
{
public:

	PatternButtonAttachment(AudioProcessorValueTreeState& treeState, const String& pID);
	~PatternButtonAttachment();

	bool PatternModeIsOn();

private:

	void parameterChanged(const String& parameterID, float newValue) override;

	Enums::patternMode state{ Enums::patternsOff };
	AudioProcessorValueTreeState& treeState;
	String parameterID;
};
