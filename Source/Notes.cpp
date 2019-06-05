#include "Notes.h"
#include "Extensions.h"

Notes::Notes(LatchMode& latchMode, LatchLock& latchLock, ArpPlayMode& playmode) : latchMode(latchMode), latchLock(latchLock), playMode(playmode)
{
	notes.clear();
	notes.reserve(100);
	notesLatched.clear();
	notesLatched.reserve(200);
}

Notes::~Notes()
{
}

void Notes::ProcessBuffer(MidiBuffer& midiMessages)
{
	PrepareToProcess();

	MidiMessage message;
	int ignore;

	for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(message, ignore);)
	{
		if (message.isNoteOn())
		{
			notes.push_back(message.getNoteNumber());

			TransposeIsEnabled() ? transposeNotes(notesLatched, message.getNoteNumber()) : notesLatched.push_back(message.getNoteNumber());
		}
		else if (message.isNoteOff())
		{
			erase(notes, message.getNoteNumber());
		}
	}
}

void Notes::PrepareToProcess()
{
	latchMode.Set();
	latchLock.Set();
	playMode.Set();
	currentPlayMode = playMode.currentState;
	AssignLatchedNotes();
	SortNotes();

	if (playMode.stateHasChanged || !AnyNotesToPlay())
	{
		InitializeNoteIndex();
	}
}

bool Notes::TransposeIsEnabled() const
{
	return (latchMode.IsEnabled() && latchLock.state == Enums::latchLock::locked && !notesLatched.empty());
}

void Notes::AssignLatchedNotes()
{
	notesLatched = latchMode.stateHasChanged ? notes : notesLatched;
}

void Notes::SortNotes()
{
	if (currentPlayMode != Enums::playMode::played)
	{
		std::sort(notes.begin(), notes.end());

		std::sort(notesLatched.begin(), notesLatched.end());
	}
}

void Notes::GetNumberOfNotes()
{
	latchIsEnabled = latchMode.IsEnabled();
	numberOfNotesToPlay = latchIsEnabled ? notesLatched.size() : notes.size();
}

bool Notes::AnyNotesToPlay()
{
	latchIsEnabled = latchMode.IsEnabled();
	return latchIsEnabled ? !notesLatched.empty() : !notes.empty();
}

void Notes::InitializeNoteIndex()
{
	GetNumberOfNotes();

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
	GetNumberOfNotes();

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

int Notes::SetLastNoteValue()
{
	latchIsEnabled = latchMode.IsEnabled();
	return latchIsEnabled ? notesLatched[currentNoteIndex] : notes[currentNoteIndex];
}

void Notes::AddNotes(MidiBuffer& midiMessages, int noteOnOffset)
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
		const auto offsetForNoteOff = CalculateOffsetForNoteOff(noteOnOffset);
		AddNoteOffToBuffer(midiMessages, offsetForNoteOff);
	}
}

bool Notes::ShouldAddNoteOn()
{
	return AnyNotesToPlay() && !lastNoteWasNoteOn;
}

bool Notes::ShouldAddNoteOff() const
{
	return (samplesFromLastNoteOnUntilBufferEnds >= noteLengthInSamples && lastNoteWasNoteOn);
}

void Notes::AddNoteOff(MidiBuffer& midiMessages)
{
	noteOffOccursInSameBufferAsLastNoteOn = false;
	const auto offsetForNoteOff = CalculateOffsetForNoteOff();
	AddNoteOffToBuffer(midiMessages, offsetForNoteOff);
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

int Notes::CalculateOffsetForNoteOff(int noteOnOffset) const
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

void Notes::Reset()
{
	notes.clear();

	notesLatched.clear();

	InitializeNoteIndex();
}