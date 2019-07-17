#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class LatchLock : public AudioProcessorValueTreeState::Listener
{
public:

	LatchLock();
	~LatchLock();

	bool IsEnabled() const;

private:

	Enums::latchLock state { Enums::latchLock::unlocked };

	void parameterChanged(const String& parameterID, float newValue) override;
};