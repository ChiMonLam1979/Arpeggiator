#include "ArpSlot.h"

ArpSlot::ArpSlot()
{
	
}

ArpSlot::~ArpSlot()
{
	
}

void ArpSlot::parameterChanged(const String& parameterID, float newValue)
{
	auto choice = static_cast<int>(newValue);

	state = static_cast<Enums::slot>(choice);
}
