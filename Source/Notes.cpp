#include "Notes.h"
#include "Extensions.h"

Notes::Notes(LatchMode& latchMode, LatchLock& latchLock, ArpMode& arpMode, SlotController& slotController)
: latchMode(latchMode), latchLock(latchLock), arpMode(arpMode), slotController(slotController)
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
	AssignLatchedNotes();
	SortNotes();

	slotController.currentNotes = latchMode.IsEnabled() ? notesLatched : notes;

	if (!AnyNotesToPlay() || slotController.patternModeHasChanged)
	{
		InitializeNoteIndex();
		slotController.patternModeHasChanged = false;
	}
}

bool Notes::TransposeIsEnabled() const
{
	return (latchMode.IsEnabled() && latchLock.IsEnabled() && !notesLatched.empty());
}

void Notes::AssignLatchedNotes()
{
	notesLatched = latchMode.StateChanged() ? notes : notesLatched;
}

void Notes::SortNotes()
{
	if (arpMode.state != Enums::arpMode::played)
	{
		std::sort(notes.begin(), notes.end());

		std::sort(notesLatched.begin(), notesLatched.end());
	}
}

void Notes::GetNumberOfNotes()
{
	if(slotController.patternModeIsOn)
	{
		numberOfNotesToPlay = slotController.patternToPlay.size();
	}
	else
	{
		numberOfNotesToPlay = latchMode.IsEnabled() ? notesLatched.size() : notes.size();
	}
}

bool Notes::AnyNotesToPlay() const
{
	if (slotController.patternModeIsOn)
	{
		return  !slotController.patternToPlay.empty();
	}
	else
	{
		return latchMode.IsEnabled() ? !notesLatched.empty() : !notes.empty();
	}
}

void Notes::InitializeNoteIndex()
{
	GetNumberOfNotes();

	const auto lastIndexOfNotesToPlay = numberOfNotesToPlay - 1;

	switch (arpMode.state)
	{
	case Enums::arpMode::up: currentNoteIndex = -1;
		break;
	case Enums::arpMode::down: currentNoteIndex = lastIndexOfNotesToPlay;
		break;
	case Enums::arpMode::random: numberOfNotesToPlay > 0 ? currentNoteIndex = Random::getSystemRandom().nextInt(Range<int>(0, numberOfNotesToPlay)) : -1;
		break;
	case Enums::arpMode::played: currentNoteIndex = -1;
		break;
	}
}

void Notes::UpdateNoteValue()
{
	GetNumberOfNotes();

	switch (arpMode.state)
	{
	case Enums::arpMode::up: currentNoteIndex = (currentNoteIndex + 1) % numberOfNotesToPlay;
		break;
	case Enums::arpMode::down: currentNoteIndex = inRange(currentNoteIndex, 0, numberOfNotesToPlay) ? (currentNoteIndex - 1) : numberOfNotesToPlay - 1;
		break;
	case Enums::arpMode::random: currentNoteIndex = Random::getSystemRandom().nextInt(Range<int>(0, numberOfNotesToPlay));
		break;
	case Enums::arpMode::played: currentNoteIndex = (currentNoteIndex + 1) % numberOfNotesToPlay;
		break;
	}

	lastNoteValue = SetLastNoteValue();
}

int Notes::SetLastNoteValue()
{
	if(slotController.patternModeIsOn)
	{
		return slotController.patternToPlay[currentNoteIndex];
	}
	else
	{
		return latchMode.IsEnabled() ? notesLatched[currentNoteIndex] : notes[currentNoteIndex];
	}
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

bool Notes::ShouldAddNoteOn() const
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