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
		currentNoteIndex = 0;
		lastNoteValue = -1;
		lastNoteWasNoteOn = false;
		rate = sampleRate;
		samplesUntilBufferEnds = 0;
		noteDivisionFactor = 1;
		noteDivisionFactorChanged = false;
		noteOffRequiredThisBuffer = false;
	}

	void releaseResources() override {}

	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
	{
		AudioPlayHead* playHead = getPlayHead();
		if (playHead == nullptr) return;

		AudioPlayHead::CurrentPositionInfo positionInfo;
		playHead->getCurrentPosition(positionInfo);

		const auto beatsPerMinute = positionInfo.bpm;																// quarterNotesPerMinute
		const auto beatsPerSecond = beatsPerMinute / 60;															// quarterNotesPerSecond
		const auto samplesPerBeat = rate / beatsPerSecond;															// number of samples per beat/quarternote is samples per sec / beats per second
		const auto samplesPerNoteDivision = samplesPerBeat / noteDivisionFactor;									// set note division
		const auto noteLengthInSamples = static_cast<int> (std::ceil(samplesPerNoteDivision * *lengthFactor));		// set note length
		const auto numberOfSamplesInBuffer = buffer.getNumSamples();

		//work out which notes will occur in the buffer
		const double partsPerQuarterNoteStart = positionInfo.ppqPosition;
		const double partsPerNoteDivisionStart = partsPerQuarterNoteStart * noteDivisionFactor;
		const double partsPerNoteDivisionEnd = partsPerNoteDivisionStart + (numberOfSamplesInBuffer / samplesPerNoteDivision);
		const int partsPerNoteDivisionStartAsInt = std::ceil(partsPerNoteDivisionStart);
		const int partsPerNoteDivisionEndAsInt = std::floor(partsPerNoteDivisionEnd);

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

		auto currentNoteDivisionFactor = noteDivisionDictionary[noteDivision->getCurrentChoiceName()];

		noteDivisionFactorChanged = noteDivisionFactor != currentNoteDivisionFactor;
		if (noteDivisionFactorChanged)
		{
			noteDivisionFactor = currentNoteDivisionFactor;
		}

		if (positionInfo.isPlaying)
		{
			noteOffRequiredThisBuffer = samplesUntilBufferEnds >= noteLengthInSamples;
			if ((noteOffRequiredThisBuffer || noteDivisionFactorChanged) && lastNoteWasNoteOn)
			{
				auto offsetForNoteOff = jmin((numberOfSamplesInBuffer - (samplesUntilBufferEnds - noteLengthInSamples)), numberOfSamplesInBuffer - 1);
				midiMessages.addEvent(MidiMessage::noteOff(1, lastNoteValue), offsetForNoteOff);
				lastNoteWasNoteOn = false;
			}

			// for each note transition in the buffer...
			for (int i = partsPerNoteDivisionStartAsInt; i <= partsPerNoteDivisionEndAsInt; ++i)
			{
				//work out the exact sample where the note occurs
				const int offset = (int)samplesPerNoteDivision * (i - partsPerNoteDivisionStart);

				if (notes.size() > 0)
				{
					currentNoteIndex = (currentNoteIndex + 1) % notes.size();
					lastNoteValue = notes[currentNoteIndex];
					midiMessages.addEvent(MidiMessage::noteOn(1, lastNoteValue, (uint8)127), offset);

					lastNoteWasNoteOn = true;
					samplesUntilBufferEnds = numberOfSamplesInBuffer - offset;
				}

				noteOffRequiredThisBuffer = samplesUntilBufferEnds >= noteLengthInSamples;
				if (noteOffRequiredThisBuffer)
				{
					const auto OffsetForNoteOff = jmin((offset + noteLengthInSamples), numberOfSamplesInBuffer - 1);
					midiMessages.addEvent(MidiMessage::noteOff(1, lastNoteValue), OffsetForNoteOff);
					lastNoteWasNoteOn = false;
				}
			}

			samplesUntilBufferEnds = (samplesUntilBufferEnds + numberOfSamplesInBuffer);	// add next buffer size
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
	bool noteDivisionFactorChanged;
	bool lastNoteWasNoteOn;
	bool noteOffRequiredThisBuffer;
	int currentNoteIndex;
	int lastNoteValue;
	int samplesUntilBufferEnds;
	double rate;
	SortedSet<int> notes;
	std::map<juce::String, int> noteDivisionDictionary = { {"1/4 note", 1}, {"1/16 note", 4}, {"1/32 note", 8} };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};
