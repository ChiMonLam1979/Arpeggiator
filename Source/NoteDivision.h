#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class NoteDivision
{
public:

	NoteDivision();
	~NoteDivision();

	AudioParameterChoice* GetParameter() const;

	void Set();

	float currentFactor;
	bool stateChanged;

	AudioParameterChoice* noteDivisions = new AudioParameterChoice
	{
		IDs::NoteDivisionId, ParameterNames::NoteDivisionName,
		{
			ParamterChoices::QuarterNoteDivision,
			ParamterChoices::EighthNoteDivision,
			ParamterChoices::EighthNoteTripletDivision,
			ParamterChoices::SixteenthNoteDivision,
			ParamterChoices::SixteenthNoteTripletDivision,
			ParamterChoices::ThirtySecondNoteDivision,
			ParamterChoices::ThirtySecondNoteTripletDivision
		},
		0
	};

	std::map<juce::String, float> noteDivisionDictionary =
	{
		{ ParamterChoices::QuarterNoteDivision, 1.0f },
		{ ParamterChoices::EighthNoteDivision, 2.0f },
		{ ParamterChoices::EighthNoteTripletDivision, 3.0f },
		{ ParamterChoices::SixteenthNoteDivision, 4.0f },
		{ ParamterChoices::SixteenthNoteTripletDivision, 6.0f },
		{ ParamterChoices::ThirtySecondNoteDivision, 8.0f },
		{ ParamterChoices::ThirtySecondNoteTripletDivision, 12.0f }
	};

private:

	//AudioParameterChoice* noteDivisions = new AudioParameterChoice
	//{
	//	IDs::NoteDivisionId, ParameterNames::NoteDivisionName,
	//	{
	//		ParamterChoices::QuarterNoteDivision,
	//		ParamterChoices::EighthNoteDivision,
	//		ParamterChoices::EighthNoteTripletDivision,
	//		ParamterChoices::SixteenthNoteDivision,
	//		ParamterChoices::SixteenthNoteTripletDivision,
	//		ParamterChoices::ThirtySecondNoteDivision,
	//		ParamterChoices::ThirtySecondNoteTripletDivision
	//	},
	//	0
	//};

	//std::map<juce::String, float> noteDivisionDictionary =
	//{
	//	{ ParamterChoices::QuarterNoteDivision, 1.0f },
	//	{ ParamterChoices::EighthNoteDivision, 2.0f },
	//	{ ParamterChoices::EighthNoteTripletDivision, 3.0f },
	//	{ ParamterChoices::SixteenthNoteDivision, 4.0f },
	//	{ ParamterChoices::SixteenthNoteTripletDivision, 6.0f },
	//	{ ParamterChoices::ThirtySecondNoteDivision, 8.0f },
	//	{ ParamterChoices::ThirtySecondNoteTripletDivision, 12.0f }
	//};
};
