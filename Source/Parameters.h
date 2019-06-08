#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

namespace IDs
{
	static String NoteDivisionId{ "noteDivision" };
	static String ArpPlayModeId{ "arpPlayMode" };
	static String NoteLengthId{ "noteLength" };
	static String LatchModeId{ "latchMode" };
	static String LatchLockId{ "latchLock" };
	static String NoteShiftId{ "noteShift" };
	static String SwingFactorId{ "swingAmount" };
	static String TreeStateId{ "treeState" };
}

namespace ParameterNames
{
	static String NoteDivisionName{ "Note Division" };
	static String ArpPlayModeName{ "Play Mode" };
	static String NoteLengthName{ "Note Length" };
	static String LatchModeName{ "Latch Mode" };
	static String LatchLockName{ "Latch Lock" };
	static String NoteShiftName{ "Note Shift" };
	static String SwingFactorName{ "Swing Amount" };
}

namespace ParamterChoices
{
	static String Up{ "Up" };
	static String Down{ "Down" };
	static String Random{ "Random" };
	static String Played{ "Played" };

	static String QuarterNoteDivision{ "1/4 Note" };
	static String EighthNoteDivision{ "1/8 Note" };
	static String EighthNoteTripletDivision{ "1/8 Note Triplet" };
	static String SixteenthNoteDivision{ "1/16 Note" };
	static String SixteenthNoteTripletDivision{ "1/16 Note Triplet" };
	static String ThirtySecondNoteDivision{ "1/32 Note" };
	static String ThirtySecondNoteTripletDivision{ "1/32 Note Triplet" };

	static String Off{ "Off" };
	static String On{ "On" };

	const StringArray NoteDivisionChoices
	{
		QuarterNoteDivision,
		EighthNoteDivision,
		EighthNoteTripletDivision,
		SixteenthNoteDivision,
		SixteenthNoteTripletDivision,
		ThirtySecondNoteDivision,
		ThirtySecondNoteTripletDivision
	};

	static std::map<juce::String, float> NoteDivisionDictionary
	{
		{ QuarterNoteDivision, 1.0f },
		{ EighthNoteDivision, 2.0f },
		{ EighthNoteTripletDivision, 3.0f },
		{ SixteenthNoteDivision, 4.0f },
		{ SixteenthNoteTripletDivision, 6.0f },
		{ ThirtySecondNoteDivision, 8.0f },
		{ ThirtySecondNoteTripletDivision, 12.0f }
	};

	const StringArray PlayModeChoices
	{
		Up,
		Down,
		Random,
		Played,
	};

	const StringArray LatchLockChoices
	{
		Off,
		On
	};

	const StringArray LatchModeChoices
	{
		Off,
		On
	};
}

namespace ParameterRanges
{

	const NormalisableRange<float> NoteLengthRange
	{
		0.0f,
		1.0f
	};

	const NormalisableRange<float> SwingFactorRange
	{
		0.0f,
		1.0f
	};
}

namespace Enums
{
	enum latchMode { off, on };
	enum playMode { up, down, random, played };
	enum latchLock { unlocked, locked };
}