#include "Notes.h"
#include "Extensions.h"

Notes::Notes(LatchMode& latchMode, LatchLock& latchLock) :
currentNoteIndex(-1), numberOfNotesToPlay(0), lastNoteValue(0), lastNoteWasNoteOn(false), currentPlayMode(Enums::playMode::up), samplesFromLastNoteOnUntilBufferEnds(0),
noteLengthInSamples(0), noteOffOccursInSameBufferAsLastNoteOn(false), latchMode(latchMode), latchLock(latchLock), latchIsEnabled(false)
{
	toPlay.clear();
	toPlay.reserve(100);
	toPlayLatchMode.clear();
	toPlayLatchMode.reserve(200);
}

Notes::~Notes()
{
}

void Notes::ProcessBuffer(MidiBuffer& midiMessages)
{
	latchMode.Set();
	latchLock.Set();
	UpdateNotesToPlay();

	MidiMessage message;
	int ignore;

	for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(message, ignore);)
	{
		if (message.isNoteOn())
		{
			toPlay.push_back(message.getNoteNumber());

			TransposeIsEnabled() ? transposeNotes(toPlayLatchMode, message.getNoteNumber()) : toPlayLatchMode.push_back(message.getNoteNumber());
		}
		else if (message.isNoteOff())
		{
			erase(toPlay, message.getNoteNumber());
		}
	}
}

bool Notes::TransposeIsEnabled() const
{
	return (latchMode.IsEnabled() && latchLock.state == Enums::latchLock::locked && !toPlayLatchMode.empty());
}

void Notes::UpdateNotesToPlay()
{
	const auto latchModeStateHasChanged = latchMode.stateHasChanged;
	toPlayLatchMode = latchModeStateHasChanged ? toPlay : toPlayLatchMode;
}

void Notes::SortNotesToPlay()
{
	if (currentPlayMode != Enums::playMode::played)
	{
		std::sort(toPlay.begin(), toPlay.end());

		std::sort(toPlayLatchMode.begin(), toPlayLatchMode.end());
	}
}

int Notes::SetLastNoteValue()
{
	latchIsEnabled = latchMode.IsEnabled();
	return latchIsEnabled ? toPlayLatchMode[currentNoteIndex] : toPlay[currentNoteIndex];
}

void Notes::GetNumberOfNotesToPlay()
{
	latchIsEnabled = latchMode.IsEnabled();
	numberOfNotesToPlay = latchIsEnabled ? toPlayLatchMode.size() : toPlay.size();
}

bool Notes::AnyNotesToPlay()
{
	latchIsEnabled = latchMode.IsEnabled();
	return latchIsEnabled ? !toPlayLatchMode.empty() : !toPlay.empty();
}

bool Notes::ShouldAddNoteOn()
{
	return AnyNotesToPlay() && !lastNoteWasNoteOn;
}

bool Notes::ShouldAddNoteOff() const
{
	return (samplesFromLastNoteOnUntilBufferEnds >= noteLengthInSamples && lastNoteWasNoteOn);
}

void Notes::InitializeNoteIndex()
{
	GetNumberOfNotesToPlay();

	const auto lastIndexOfNotesToPlay = numberOfNotesToPlay - 1;

	switch (currentPlayMode)
	{
	case Enums::playMode::up: currentNoteIndex = -1;
		break;
	case Enums::playMode::down: currentNoteIndex = lastIndexOfNotesToPlay;
		break;
	case Enums::playMode::random: currentNoteIndex = Random::getSystemRandom().nextInt(Range<int>(0, numberOfNotesToPlay));
		break;
	case Enums::playMode::played: currentNoteIndex = -1;
		break;
	}
}

void Notes::UpdateNoteValue()
{
	GetNumberOfNotesToPlay();

	switch (currentPlayMode)
	{
	case Enums::playMode::up: currentNoteIndex = (currentNoteIndex + 1) % numberOfNotesToPlay;
		break;
	case Enums::playMode::down: currentNoteIndex = inRange(currentNoteIndex, 0, numberOfNotesToPlay) ? (currentNoteIndex - 1) : numberOfNotesToPlay - 1;
		break;
	case Enums::playMode::random: currentNoteIndex = Random::getSystemRandom().nextInt(Range<int>(0, numberOfNotesToPlay));
		break;
	case Enums::playMode::played: currentNoteIndex = (currentNoteIndex + 1) % numberOfNotesToPlay;
		break;
	}

	lastNoteValue = SetLastNoteValue();
}

void Notes::AddNotes(MidiBuffer& midiMessages, int numberOfSamplesInBuffer, int noteOnOffset)
{
	if (ShouldAddNoteOn())
	{
		UpdateNoteValue();

		AddNoteOnToBuffer(midiMessages, noteOnOffset);
		samplesFromLastNoteOnUntilBufferEnds = numberOfSamplesInBuffer - noteOnOffset;
	}

	if (ShouldAddNoteOff())
	{
		noteOffOccursInSameBufferAsLastNoteOn = true;
		const auto offsetForNoteOff = CalculateOffsetForNoteOff(noteOnOffset, numberOfSamplesInBuffer);
		AddNoteOffToBuffer(midiMessages, offsetForNoteOff);
	}
}

void Notes::AddNoteOnToBuffer(MidiBuffer& midiMessages, const int offset)
{
	midiMessages.addEvent(MidiMessage::noteOn(1, lastNoteValue, (uint8)127), offset);
	lastNoteWasNoteOn = true;
}

void Notes::AddNoteOffToBuffer(MidiBuffer& midiMessages, const int offset)
{
	midiMessages.addEvent(MidiMessage::noteOff(1, lastNoteValue), offset);
	lastNoteWasNoteOn = false;
}

int Notes::CalculateOffsetForNoteOff(int numberOfSamplesInBuffer, int noteOnOffset) const
{
	const auto lastSampleInCurrentBuffer = numberOfSamplesInBuffer - 1;
	int offset;

	if (noteOffOccursInSameBufferAsLastNoteOn)
	{
		offset = noteOnOffset + noteLengthInSamples;
	}
	else
	{
		offset = numberOfSamplesInBuffer - (samplesFromLastNoteOnUntilBufferEnds - noteLengthInSamples);
	}

	return jmin(offset, lastSampleInCurrentBuffer);
}
