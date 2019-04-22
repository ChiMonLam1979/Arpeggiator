#pragma once

class Arpeggiator  : public AudioProcessor
{
public:
    Arpeggiator()
        : AudioProcessor (BusesProperties()
			.withInput("Input", AudioChannelSet::stereo(), true)
		) // add no audio buses at all
    {
        addParameter (speed = new AudioParameterFloat ("speed", "Arpeggiator Speed", 0.0, 1.0, 0.5));
    }

    ~Arpeggiator() {}

    void prepareToPlay (double sampleRate, int) override
    {
        notes.clear();
        currentNote = 0;
        lastNoteValue = -1;
        time = 0.0;
        rate = static_cast<float> (sampleRate);
    }

    void releaseResources() override {}

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
    {
		AudioPlayHead* playHead = getPlayHead();
		if (playHead == nullptr) return;

		AudioPlayHead::CurrentPositionInfo positionInfo;
		playHead->getCurrentPosition(positionInfo);

		auto bpm = positionInfo.bpm;				//bpm is quarterNotesPerMinute
		auto bps = bpm / 60;						//bps is quarterNotesPerSecond
		auto samplesPerBeat = rate / bps;			//number of samples per beat/quarternote is samples per sec / beats per second

        // set note duration
		auto noteDuration = static_cast<int> (std::ceil(samplesPerBeat * 0.25f ));		// for a 16th note

		auto numSamples = buffer.getNumSamples();	// number of samples in each buffer


        MidiMessage message;
        int ignore;

        for (MidiBuffer::Iterator it (midiMessages); it.getNextEvent (message, ignore);)
        {
			if (message.isNoteOn())
			{
				notes.add(message.getNoteNumber());
			}
			else if (message.isNoteOff())
			{
				notes.removeValue(message.getNoteNumber());
			}
        }

        midiMessages.clear();

        if ((time + numSamples) >= noteDuration)
        {
            auto offset = jmax (0, jmin ((int) (noteDuration - time), numSamples - 1));

            if (lastNoteValue > 0)
            {
                midiMessages.addEvent (MidiMessage::noteOff (1, lastNoteValue), offset);
                lastNoteValue = -1;
            }

            if (notes.size() > 0) // if there are notes in 'notes' coolection
            {
                currentNote = (currentNote + 1) % notes.size();  // advance to next note in collection
                lastNoteValue = notes[currentNote];  // set last note to current note
                midiMessages.addEvent (MidiMessage::noteOn  (1, lastNoteValue, (uint8) 127), offset); // add last note to buffer at sample pos = offset
            }

        }

        time = (time + numSamples) % noteDuration;
    }

    bool isMidiEffect() const override                     { return true; }

    AudioProcessorEditor* createEditor() override          { return new GenericAudioProcessorEditor (this); }
    bool hasEditor() const override                        { return true; }

    const String getName() const override                  { return "Arpeggiator"; }

    bool acceptsMidi() const override                      { return true; }
    bool producesMidi() const override                     { return true; }
    double getTailLengthSeconds() const override           { return 0; }

    int getNumPrograms() override                          { return 1; }
    int getCurrentProgram() override                       { return 0; }
    void setCurrentProgram (int) override                  {}
    const String getProgramName (int) override             { return {}; }
    void changeProgramName (int, const String&) override   {}

    void getStateInformation (MemoryBlock& destData) override
    {
        MemoryOutputStream (destData, true).writeFloat (*speed);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        speed->setValueNotifyingHost (MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat());
    }

private:
    AudioParameterFloat* speed;
    int currentNote, lastNoteValue;
    int time;
    float rate;
    SortedSet<int> notes;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Arpeggiator)
};
