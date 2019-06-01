#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class NoteDivionHandler
{
public:

	NoteDivionHandler();
	~NoteDivionHandler();

	AudioParameterChoice* GetNoteDivisionParameter() const;

	void UpdateNoteDivision();

	float noteDivisionFactor = 1.0f;
	bool noteDivisionChanged;

private:

	AudioParameterChoice* noteDivision = new AudioParameterChoice
	{
		IDs::noteDivisionId, ParameterNames::noteDivisionName,
		{
			ParamterChoices::quarterNoteDivision,
			ParamterChoices::eighthNoteDivision,
			ParamterChoices::eighthNoteTripletDivision,
			ParamterChoices::sixteenthNoteDivision,
			ParamterChoices::sixteenthNoteTripletDivision,
			ParamterChoices::thirtySecondNoteDivision,
			ParamterChoices::thirtySecondNoteTripletDivision
		},
		0
	};

	std::map<juce::String, float> noteDivisionDictionary =
	{
		{ ParamterChoices::quarterNoteDivision, 1.0f },
		{ ParamterChoices::eighthNoteDivision, 2.0f },
		{ ParamterChoices::eighthNoteTripletDivision, 3.0f },
		{ ParamterChoices::sixteenthNoteDivision, 4.0f },
		{ ParamterChoices::sixteenthNoteTripletDivision, 6.0f },
		{ ParamterChoices::thirtySecondNoteDivision, 8.0f },
		{ ParamterChoices::thirtySecondNoteTripletDivision, 12.0f }
	};
};
