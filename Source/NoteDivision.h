#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameters.h"

class NoteDivision : public AudioProcessorValueTreeState::Listener
{
public:

	NoteDivision();
	~NoteDivision();

	void Set();

	float currentFactor  { 1.0f };
	float selectedFactor { 1.0f };
	bool stateChanged    { false };

private:

	AudioParameterChoice* noteDivisions = new AudioParameterChoice
	{
		IDs::NoteDivisionId, ParameterNames::NoteDivisionName, ParamterChoices::noteDivisionChoices,
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

	void parameterChanged(const String& parameterID, float newValue) override;

};
