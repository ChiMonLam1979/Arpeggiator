#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

namespace IDs
{
	static String NoteDivisionId{ "noteDivision" };
	static String ArpModeId{ "arpMode" };
	static String NoteLengthId{ "noteLength" };
	static String LatchModeId{ "latchMode" };
	static String LatchLockId{ "latchLock" };
	static String NoteShiftId{ "noteShift" };
	static String SwingFactorId{ "swingAmount" };
	static String TreeStateId{ "treeState" };
	static String Slot1Id{ "slot1Id" };
	static String Slot2Id{ "slot2Id" };
	static String Slot3Id{ "slot3Id" };
	static String Slot4Id{ "slot4Id" };
	static String Slot1RepeatId{ "slot1RepeatId" };
	static String Slot2RepeatId{ "slot2RepeatId" };
	static String Slot3RepeatId{ "slot3RepeatId" };
	static String Slot4RepeatId{ "slot4RepeatId" };
	static String Slot1OrderId{ "slot1OrderId" };
	static String Slot2OrderId{ "slot2OrderId" };
	static String Slot3OrderId{ "slot3OrderId" };
	static String Slot4OrderId{ "slot4OrderId" };
	static String PatternModeId{ "patternMode" };
}

namespace ParameterNames
{
	static String NoteDivisionName{ "Note Division" };
	static String ArpModeName{ "Arp Mode" };
	static String NoteLengthName{ "Note Length" };
	static String LatchModeName{ "Latch Mode" };
	static String LatchLockName{ "Latch Lock" };
	static String NoteShiftName{ "Note Shift" };
	static String SwingFactorName{ "Swing Amount" };
	static String Slot1Name{ "Slot 1" };
	static String Slot2Name{ "Slot 2" };
	static String Slot3Name{ "Slot 3" };
	static String Slot4Name{ "Slot 4" };
	static String Slot1RepeatName{ "Slot 1 Repeat" };
	static String Slot2RepeatName{ "Slot 2 Repeat" };
	static String Slot3RepeatName{ "Slot 3 Repeat" };
	static String Slot4RepeatName{ "Slot 4 Repeat" };
	static String Slot1OrderName{ "Slot 1 Order" };
	static String Slot2OrderName{ "Slot 2 Order" };
	static String Slot3OrderName{ "Slot 3 Order" };
	static String Slot4OrderName{ "Slot 4 Order" };
	static String SlotRepeatName{ "Slot Repeats" };
	static String SlotOrderName{ "Slot Order" };
	static String PatternModeName{ "Pattern Mode" };
}

namespace ParamterChoices
{
	static String Up{ "Up" };
	static String Down{ "Down" };
	static String Random{ "Random" };
	static String Played{ "Played" };

	static String QuarterNoteDivision{ "1/4" };
	static String EighthNoteDivision{ "1/8" };
	static String EighthNoteTripletDivision{ "1/8 T" };
	static String SixteenthNoteDivision{ "1/16" };
	static String SixteenthNoteTripletDivision{ "1/16 T" };
	static String ThirtySecondNoteDivision{ "1/32" };
	static String ThirtySecondNoteTripletDivision{ "1/32 T" };

	static String LatchOff{ "Latch Off" };
	static String LatchOn{ "Latch On" };
	static String LockOff{ "Lock Off" };
	static String LockOn{ "Lock On" };

	static String PatternModeOff{ "Patterns Off" };
	static String PatternModeOn{ "Patterns On" };

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

	const StringArray ArpModeChoices
	{
		Up,
		Down,
		Random,
		Played,
	};

	const StringArray LatchLockChoices
	{
		LockOff,
		LockOn
	};

	const StringArray LatchModeChoices
	{
		LatchOff,
		LatchOn
	};

	const StringArray PatternModeChoices
	{
		PatternModeOff,
		PatternModeOn,
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
	enum arpMode { up, down, random, played };
	enum latchLock { unlocked, locked };
	enum orientation { horizontal, vertical };
	enum slotAction { clear, save };
	enum patternMode { patternsOff, patternsOn };
}