#include "SlotButton.h"

SlotButton::SlotButton(const String& buttonName)
: TextButton(buttonName + " SAVE"), buttonName(buttonName)
{
}

SlotButton::~SlotButton()
{
}

void SlotButton::clicked()
{
	if (state == 0)
	{
		state = 1;
		setButtonText(buttonName + " CLEAR");
	}
	else
	{
		state = 0;
		setButtonText(buttonName + " SAVE");
	}
}