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
	static String ArpSlotId{ "arpSlot" };
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
	static String ArpSlotName{ "Arp Slot" };
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

	static String Slot1{ "Slot 1" };
	static String Slot2{ "Slot 2" };
	static String Slot3{ "Slot 3" };
	static String Slot4{ "Slot 4" };

	const StringArray ArpSlotChoices
	{
		Slot1,
		Slot2,
		Slot3,
		Slot4
	};

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
	enum slot { slotOne, slotTwo, slotThree, slotFour };
}