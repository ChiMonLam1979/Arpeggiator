#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PatternButtonAttachment.h"

class SlotButton : public TextButton
{
public:

	SlotButton(const String& buttonName, std::unique_ptr<PatternButtonAttachment>& patternButton);
	~SlotButton();

	void clicked() override;

	const String buttonName;

private:

	int state{ 0 };

	void SetState();

	void IgnoreClick();

	std::unique_ptr<PatternButtonAttachment>& patternButton;
};
