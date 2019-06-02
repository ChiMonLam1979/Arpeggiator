#include "Notes.h"
#include "Extensions.h"

Notes::Notes():numberOfNotesToPlay(0), lastNoteValue(0), lastNoteWasNoteOn(false), latchIsDisabled(false)
{
	toPlay.clear();
	toPlay.reserve(100);
	toPlayLatchMode.clear();
	toPlayLatchMode.reserve(200);
}

Notes::~Notes()
{
}

void Notes::ProcessBuffer(MidiBuffer& midiMessages, bool transposeIsEnabled)
{
	MidiMessage message;
	int ignore;

	for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(message, ignore);)
	{
		if (message.isNoteOn())
		{
			toPlay.push_back(message.getNoteNumber());

			transposeIsEnabled ? transposeNotes(toPlayLatchMode, message.getNoteNumber()) : toPlayLatchMode.push_back(message.getNoteNumber());
		}
		else if (message.isNoteOff())
		{
			erase(toPlay, message.getNoteNumber());
		}
	}
}

void Notes::UpdateNotesToPlay(bool latchModeStateHasChanged)
{
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
	return latchIsDisabled ? toPlay[currentNoteIndex] : toPlayLatchMode[currentNoteIndex];
}

void Notes::GetNumberOfNotesToPlay()
{
	numberOfNotesToPlay = latchIsDisabled ? toPlay.size() : toPlayLatchMode.size();
}

bool Notes::AnyNotesToPlay() const
{
	return latchIsDisabled ? !toPlay.empty() : !toPlayLatchMode.empty();
}

bool Notes::ShouldAddNoteOn() const
{
	return AnyNotesToPlay() && !lastNoteWasNoteOn;
}

bool Notes::ShouldAddNoteOff(int samplesFromLastNoteOnUntilBufferEnds, int noteLengthInSamples) const
{
	return (samplesFromLastNoteOnUntilBufferEnds >= noteLengthInSamples && lastNoteWasNoteOn);
}

void Notes::InitializeNoteIndex()
{
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
