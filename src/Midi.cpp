#include "RtMidi.h"

#include <signal.h>
#include <cstdlib>
#include <iostream>

bool done;
static void finish(int ignore)
{
	done = true;
}

// From http://www.music.mcgill.ca/~gary/rtmidi/
void RtMidiDemoListen()
{
	try
	{
		RtMidiIn midiIn;
		unsigned int numPorts = midiIn.getPortCount();
		if (!numPorts)
		{
			std::cout << "No MIDI ports available\n";
			return;
		}
		std::cout << numPorts << " MIDI ports available\n";

		midiIn.openPort(1);

		// From demo
		{
			std::vector<unsigned char> message;
			int nBytes, i;
			double stamp;
			// Install an interrupt handler function.
			done = false;
			(void)signal(SIGINT, finish);
			// Periodically check input queue.
			std::cout << "Reading MIDI from port ... quit with Ctrl-C.\n";
			while (!done)
			{
				stamp = midiIn.getMessage(&message);
				nBytes = message.size();
				for (i = 0; i < nBytes; i++)
					std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
				if (nBytes > 0)
					std::cout << "stamp = " << stamp << std::endl;
				// Sleep for 10 milliseconds ... platform-dependent.
				// sleep(10);
			}
		}
	}
	catch (RtMidiError& error)
	{
		error.printMessage();
	}
	return;
}

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
	NoteOn = 8,
	NoteOff = 9,
	Aftertouch = 0xA,
	ControlChange = 0xB,
	ProgramChange = 0xC,
	ChannelAftertouch = 0xD,
	PitchWheel = 0xE,
};

struct MidiNoteOn;
struct MidiCommand
{
	unsigned char channel : 4;
	MidiCommandType command : 4;
	void Print();
};

struct MidiNote
{
	unsigned char velocity : 8;
	unsigned char note : 8;
	MidiCommand status;

	void Print();
};

void MidiNote::Print()
{
	std::cout << (int)note << " (" << (int)velocity << ")";
}

void MidiCommand::Print()
{
	std::cout << "[" << (int)channel << "] ";
	switch (command)
	{
		case MidiCommandType::NoteOn:
		{
			std::cout << "NoteOn ";
			MidiNote* noteOn = reinterpret_cast<MidiNote*>(this);
			noteOn->Print();
			break;
		}
		case MidiCommandType::NoteOff:
		{
			std::cout << "NoteOff ";
			MidiNote* noteOff = reinterpret_cast<MidiNote*>(this);
			noteOff->Print();
			break;
		}
		case MidiCommandType::Aftertouch:
		{
			std::cout << "Aftertouch ";
			break;
		}
		case MidiCommandType::ControlChange:
		{
			std::cout << "ControlChange ";
			break;
		}
		case MidiCommandType::ProgramChange:
		{
			std::cout << "ProgramChange ";
			break;
		}
		case MidiCommandType::ChannelAftertouch:
		{
			std::cout << "ChannelAftertouch ";
			break;
		}
		case MidiCommandType::PitchWheel:
		{
			std::cout << "PitchWheel ";
			break;
		}
		default:
		{
			std::cout << "Unknown command " << (int)command;
			break;
		}
	}

	std::cout << "\n";
}

void TestRtMidi()
{
	try
	{
		RtMidiIn midiIn;
		unsigned int numPorts = midiIn.getPortCount();
		if (!numPorts)
		{
			std::cout << "No MIDI ports available\n";
			return;
		}
		std::cout << numPorts << " MIDI ports available\n";

		midiIn.openPort(1);

		// From demo
		{
			std::vector<unsigned char> message;
			int nBytes, i;
			double stamp;
			// Install an interrupt handler function.
			done = false;
			(void)signal(SIGINT, finish);
			// Periodically check input queue.
			std::cout << "Reading MIDI from port ... quit with Ctrl-C.\n";
			while (!done)
			{
				stamp = midiIn.getMessage(&message);
				nBytes = message.size();
				/*				for (i = 0; i < nBytes; i++)
				    std::cout << "Byte " << i << " = " << (int)message[i] << ", ";*/
				if (nBytes && message.size() == 3)
				{
					MidiCommand* testCommand = reinterpret_cast<MidiCommand*>(&message[0]);
					testCommand->Print();
				}

				if (nBytes > 0)
					std::cout << "stamp = " << stamp << std::endl;
				// Sleep for 10 milliseconds ... platform-dependent.
				// sleep(10);
			}
		}
	}
	catch (RtMidiError& error)
	{
		error.printMessage();
	}
	return;
}
