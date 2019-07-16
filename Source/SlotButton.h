#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SlotButton : public TextButton
{
public:

	SlotButton(const String& buttonName);
	~SlotButton();

	void clicked() override;

private:

	const String buttonName;
	int state{ 0 };
};