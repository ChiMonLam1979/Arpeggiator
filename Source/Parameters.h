#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

namespace IDs
{
	static String noteDivisionId{ "noteDivision" };
	static String arpPlayModeId{ "arpPlayMode" };
	static String noteLengthId{ "noteLength" };
	static String latchModeId{ "latchMode" };
	static String latchLockId{ "latchLock" };
	static String noteShiftId{ "noteShift" };
	static String swingPercentageId{ "swingAmount" };
}

namespace ParameterNames
{
	static String noteDivisionName{ "Note Division" };
	static String arpPlayModeName{ "Play Mode" };
	static String noteLengthName{ "Note Length" };
	static String latchModeName{ "Latch Mode" };
	static String latchLockName{ "Latch Lock" };
	static String noteShiftName{ "Note Shift" };
	static String swingPercentageName{ "Swing Amount" };
}

namespace ParamterChoices
{
	static String playModeUp{ "Up" };
	static String playModeDown{ "Down" };
	static String playModeRandom{ "Random" };
	static String playModePlayed{ "Played" };

	static String quarterNoteDivision{ "1/4 Note" };
	static String eighthNoteDivision{ "1/8 Note" };
	static String eighthNoteTripletDivision{ "1/8 Note Triplet" };
	static String sixteenthNoteDivision{ "1/16 Note" };
	static String sixteenthNoteTripletDivision{ "1/16 Note Triplet" };
	static String thirtySecondNoteDivision{ "1/32 Note" };
	static String thirtySecondNoteTripletDivision{ "1/32 Note Triplet" };

	static String latchModeOff{ "Latch Off" };
	static String latchModeOn{ "Latch On" };
	static String latchLockOff{ "Latch Lock Off" };
	static String latchLockOn{ "Latch Lock On" };
}

namespace Enums
{
	enum latchMode { off, on };
	enum playMode { up, down, random, played };
}