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

#define MAX_TRACKS 16

class SongState
{
	RtMidiIn MidiIn;

	void SetNoteOn(MidiNote* noteMessage);
	void SetNoteOff(MidiNote* noteMessage);

public:
	SongTrack Tracks[MAX_TRACKS];
	SongTrack& GetTrack(Channel channel);

	SongState();
	~SongState() = default;

	void Update();
};
