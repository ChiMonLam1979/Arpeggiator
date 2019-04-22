#include "../JuceLibraryCode/JuceHeader.h"
#include "ArpeggiatorTutorial.h"

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Arpeggiator();
}
