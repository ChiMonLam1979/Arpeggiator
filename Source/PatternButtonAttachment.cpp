#include "ArpMode.h"
#include "PatternButtonAttachment.h"

PatternButtonAttachment::PatternButtonAttachment(AudioProcessorValueTreeState& treeState, const String& pID) : treeState(treeState), parameterID(pID)
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
}