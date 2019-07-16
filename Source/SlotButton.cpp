#include "SlotButton.h"

SlotButton::SlotButton(const String& buttonName, std::unique_ptr<PatternButtonAttachment>& patternButton)
: TextButton(buttonName + " SAVE"), buttonName(buttonName), patternButton(patternButton)
{
}


SlotButton::~SlotButton()
{
}

void SlotButton::clicked()
{
	if(patternButton->PatternModeIsOn())
	{
		IgnoreClick();
	}
	else
	{
		SetState();
	}
}

void SlotButton::IgnoreClick()
{
	if(state == 0)
	{
		setToggleState(false, dontSendNotification);
	}
	else
	{
		setToggleState(true, dontSendNotification);
	}
}

void SlotButton::SetState()
{
	if (state == 0)
	{
		state = 1;
		setButtonText(buttonName + " CLEAR");
		setToggleState(true, dontSendNotification);
	}
	else
	{
		state = 0;
		setButtonText(buttonName + " SAVE");
		setToggleState(false, dontSendNotification);
	}
}