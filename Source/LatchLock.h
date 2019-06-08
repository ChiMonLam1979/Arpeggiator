#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class LatchLock : public AudioProcessorValueTreeState::Listener
{
public:

	LatchLock();
	~LatchLock();

	//void Set();

	Enums::latchLock state { Enums::latchLock::unlocked };

private:

	void parameterChanged(const String& parameterID, float newValue) override;
};