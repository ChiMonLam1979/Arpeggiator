#include "../JuceLibraryCode/JuceHeader.h"
#include "Arpeggiator.h"

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Arpeggiator();
}
