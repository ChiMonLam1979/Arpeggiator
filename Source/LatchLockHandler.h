#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class LatchLockHandler
{
public:

	LatchLockHandler();
	~LatchLockHandler();

	AudioParameterChoice* GetLatchLockParameter() const;

	void SetLatchLockMode();

	Enums::latchLock latchLockState;

private:

	AudioParameterChoice* arpLatchLock = new AudioParameterChoice
	{
		IDs::latchLockId,
		ParameterNames::latchLockName,
		{
			ParamterChoices::latchLockOff,
			ParamterChoices::latchLockOn
		},
		0
	};
};