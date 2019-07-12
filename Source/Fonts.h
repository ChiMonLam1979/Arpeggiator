#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

inline Font getSevenSegmentFont()
{
	return Typeface::createSystemTypefaceFor(BinaryData::_7_Segment_ttf, BinaryData::_7_Segment_ttfSize);
}