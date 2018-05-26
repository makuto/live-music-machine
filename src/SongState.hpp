#pragma once

#include <map>

#include "RtMidi.h"

#include "Midi.hpp"

typedef std::map<Note, Velocity> NoteVelocityMap;

struct SongTrack
{
	Channel channel;

	NoteVelocityMap notesPlaying;
};

class SongState
{
	RtMidiIn MidiIn;

	void SetNoteOn(MidiNote* noteMessage);
	void SetNoteOff(MidiNote* noteMessage);

public:
	SongTrack Tracks[16];
	SongTrack& GetTrack(Channel channel);

	SongState();
	~SongState() = default;

	void Update();
};
