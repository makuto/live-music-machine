#pragma once

/* Midi Commands
 From http://www.songstuff.com/recording/article/midi_message_format/
Note Off 	8n 	Note Number 	Velocity
Note On 	9n 	Note Number 	Velocity
Polyphonic Aftertouch 	An 	Note Number 	Pressure
Control Change 	Bn 	Controller Number 	Data
Program Change 	Cn 	Program Number 	Unused
Channel Aftertouch 	Dn 	Pressure 	Unused
Pitch Wheel 	En 	LSB 	MSB
*/
enum class MidiCommandType : unsigned char
{
	NoteOff = 8,
	NoteOn = 9,
	Aftertouch = 0xA,
	ControlChange = 0xB,
	ProgramChange = 0xC,
	ChannelAftertouch = 0xD,
	PitchWheel = 0xE,
};

typedef unsigned char Channel;
typedef unsigned char Note;
typedef unsigned char Velocity;

struct MidiCommand
{
	// Do NOT reorder these
	Channel channel : 4;
	MidiCommandType command : 4;

	void Print();
};

struct MidiNote
{
	// Do NOT reorder these
	MidiCommand status;
	Note note : 8;
	Velocity velocity : 8;

	void Print();
};

void TestRtMidi();
