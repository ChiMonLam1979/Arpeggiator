#pragma once

class Arpeggiator : public AudioProcessor
{
public:

	Arpeggiator()
		: AudioProcessor(BusesProperties()
			.withInput("Input", AudioChannelSet::stereo(), true)
		)
	{
		addParameter(noteDivision = new AudioParameterChoice("divisions", "Note Divisions", { "1/4 note", "1/16 note", "1/32 note" }, 0));
		addParameter(lengthFactor = new AudioParameterFloat("length", "Note Length", 0.1, 0.9, 0.5));
	}

	~Arpeggiator() {}

	void prepareToPlay(double sampleRate, int) override
	{
		notes.clear();
		currentNote = 0;
		lastNoteValue = -1;
		rate = sampleRate;
		samplesSinceNoteOn = 0;
	}

	void releaseResources() override {}

	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
	{
		noteDivisionFactor = noteDivisionDictionary[noteDivision->getCurrentChoiceName()];

		AudioPlayHead* playHead = getPlayHead();
		if (playHead == nullptr) return;

		AudioPlayHead::CurrentPositionInfo positionInfo;
		playHead->getCurrentPosition(positionInfo);

		auto bpm = positionInfo.bpm;															// bpm is quarterNotesPerMinute
		auto bps = bpm / 60;																	// bps is quarterNotesPerSecond
		auto samplesPerBeat = rate / bps;														// number of samples per beat/quarternote is samples per sec / beats per second
		auto samplesPerNoteDivision = samplesPerBeat / noteDivisionFactor;						// set note division
		auto noteLength = static_cast<int> (std::ceil(samplesPerNoteDivision * *lengthFactor));	// set note length

		auto numSamples = buffer.getNumSamples();	// number of samples in each buffer

		//work out which notes will occur in the buffer
		const double ppqBegin = positionInfo.ppqPosition * noteDivisionFactor;
		const double ppqEnd = ppqBegin + (numSamples / samplesPerNoteDivision);
		const int ippqBegin = std::ceil(ppqBegin);
		const int ippqEnd = std::floor(ppqEnd);

		MidiMessage message;
		int ignore;

		for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(message, ignore);)
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

		// ppqPosition is only changing when the transport is playing.
		if (positionInfo.isPlaying)
		{
			if (samplesSinceNoteOn >= noteLength && lastNoteValue > 0)	// check if last note was a note-on. if true we need to add a note off inside this buffer
			{
				auto offsetForNoteOff = jmin((numSamples - (samplesSinceNoteOn - noteLength)), numSamples - 1);
				midiMessages.addEvent(MidiMessage::noteOff(1, lastNoteValue), offsetForNoteOff);
				lastNoteValue = -1;
			}

			// for each note transition in the buffer...
			for (int i = ippqBegin; i <= ippqEnd; ++i)
			{
				//work out the exact sample where the note occurs
				const int offset = (int)samplesPerNoteDivision * (i - ppqBegin);

				if (notes.size() > 0) // if there are notes in 'notes' coolection
				{
					currentNote = (currentNote + 1) % notes.size();  // advance to next note in collection
					lastNoteValue = notes[currentNote];  // set flag that last note was a note-on
					midiMessages.addEvent(MidiMessage::noteOn(1, lastNoteValue, (uint8)127), offset); // add last note to buffer at sample pos = offset

					samplesSinceNoteOn = numSamples - offset;
				}

				if (samplesSinceNoteOn >= noteLength)		// check if note-off should occur within this buffer
				{
					auto OffsetForNoteOff = jmin((offset + noteLength), numSamples - 1);
					midiMessages.addEvent(MidiMessage::noteOff(1, lastNoteValue), OffsetForNoteOff);
					lastNoteValue = -1;		// set flag that last note was a note-off
				}
			}

			samplesSinceNoteOn = (samplesSinceNoteOn + numSamples);	// update elapsed time since note-off 
		}
	}

	bool isMidiEffect() const override { return true; }

	AudioProcessorEditor* createEditor() override { return new GenericAudioProcessorEditor(this); }
	bool hasEditor() const override { return true; }

	const String getName() const override { return "Arpeggiator"; }

	bool acceptsMidi() const override { return true; }
	bool producesMidi() const override { return true; }
	double getTailLengthSeconds() const override { return 0; }

	int getNumPrograms() override { return 1; }
	int getCurrentProgram() override { return 0; }
	void setCurrentProgram(int) override {}
	const String getProgramName(int) override { return {}; }
	void changeProgramName(int, const String&) override {}

	void getStateInformation(MemoryBlock& destData) override
	{
		MemoryOutputStream(destData, true).writeFloat(*noteDivision);
		MemoryOutputStream(destData, true).writeFloat(*lengthFactor);
	}

	void setStateInformation(const void* data, int sizeInBytes) override
	{
		noteDivision->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
		lengthFactor->setValueNotifyingHost(MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
	}

private:

	AudioParameterChoice* noteDivision;
	AudioParameterFloat* lengthFactor;
	int noteDivisionFactor;
	int currentNote, lastNoteValue;
	int samplesSinceNoteOn;
	double rate;
	SortedSet<int> notes;
	std::map<juce::String, int> noteDivisionDictionary = { {"1/4 note", 1}, {"1/16 note", 4}, {"1/32 note", 8} };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};
