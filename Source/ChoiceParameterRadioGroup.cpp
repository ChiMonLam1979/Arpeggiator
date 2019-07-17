#include "../JuceLibraryCode/JuceHeader.h"
#include "MyTextButtonLookAndFeel.h"

class ChoiceParameterRadioGroup : public Component,
                                  private Button::Listener,
                                  private AudioProcessorValueTreeState::Listener
{
public:
    enum orientation
    {
        horizontal,
        vertical
    };

    ChoiceParameterRadioGroup (AudioProcessorValueTreeState& s, const String& pID, orientation o)
    : state (s), orientation (o), parameterID (pID)
    {
        auto* choices = dynamic_cast<AudioParameterChoice*> (state.getParameter (parameterID));
        if (choices == nullptr)
        {
            // Parameter not found or is not of type AudioParameterChoice
            jassertfalse;
            return;
        }

        parameter = choices;

        for (auto& text : choices->choices)
        {
            auto button = std::make_unique<TextButton>(text);
            button->setClickingTogglesState (true);
            button->setRadioGroupId (100);
            button->addListener (this);
			button->setLookAndFeel(&textButtonLookAndFeel);
            addAndMakeVisible (button.get());
            buttons.push_back (std::move (button));
        }

        auto index = choices->getIndex();
        if (isPositiveAndBelow (index, buttons.size()))
            buttons [index]->setToggleState (true, sendNotificationSync);
        
        state.addParameterListener (parameterID, this);
    }

    ~ChoiceParameterRadioGroup()
    {
        state.removeParameterListener (parameterID, this);
    }

    void resized() override
    {
        if (buttons.empty())
            return;
        
        auto area = getLocalBounds();
        if (orientation == horizontal)
        {
            auto w = getWidth() / int (buttons.size());
            for (auto& button : buttons)
                button->setBounds (area.removeFromLeft (w));
        }
        else
        {
            auto h = getHeight() / int (buttons.size());
            for (auto& button : buttons)
                button->setBounds (area.removeFromTop (h));
        }
    }

    void buttonClicked (Button* button) override
    {
        if (updating || parameter == nullptr)
            return;

        ScopedValueSetter<bool> u (updating, true);
        
        auto index = std::find_if (buttons.begin(), buttons.end(), [button](const auto& b) {return b.get() == button;} );
        if (index != buttons.end())
            *parameter = int (std::distance (buttons.begin(), index));
    }

    void parameterChanged (const String& pID, float newValue) override
    {
        if (updating || pID != parameterID)
            return;

        ScopedValueSetter<bool> u (updating, true);

        auto index = roundToInt (newValue);
        if (isPositiveAndBelow (index, buttons.size()))
            buttons [index]->setToggleState (true, sendNotificationSync);
    }

private:
	MyTextButtonLookAndFeel textButtonLookAndFeel;
    AudioProcessorValueTreeState& state;
    std::vector<std::unique_ptr<Button>> buttons;
    orientation orientation;
    String parameterID;
    AudioParameterChoice* parameter = nullptr;
    bool updating = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChoiceParameterRadioGroup)
};
