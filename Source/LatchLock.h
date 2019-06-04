#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class LatchLock
{
public:

	LatchLock();
	~LatchLock();

	AudioParameterChoice* GetParameter() const;

	void Set();

	Enums::latchLock state { Enums::latchLock::unlocked };

private:

	AudioParameterChoice* lockModes = new AudioParameterChoice
	{
		IDs::LatchLockId,
		ParameterNames::LatchLockName,
		{
			ParamterChoices::Off,
			ParamterChoices::On
		},
		0
	};
};