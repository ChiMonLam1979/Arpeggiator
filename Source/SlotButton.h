#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SlotButton : public TextButton
{
public:

	SlotButton(const String& buttonName);
	~SlotButton();

	void clicked() override;

	const String buttonName;

private:

	int state{ 0 };
};
