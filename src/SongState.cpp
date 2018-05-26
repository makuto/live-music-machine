#include "SongState.hpp"

SongState::SongState()
{
	// Open MIDI port
	{
		unsigned int numPorts = MidiIn.getPortCount();
		if (!numPorts)
		{
			std::cout << "No MIDI ports available\n";
			return;
		}
		std::cout << numPorts << " MIDI ports available\n";

		MidiIn.openPort(1);
	}
}

// TODO: Just make this a vector with 16 things already? This will be called a lot
SongTrack& SongState::GetTrack(Channel channel)
{
	return Tracks[channel];
}

void SongState::SetNoteOn(MidiNote* noteMessage)
{
	SongTrack& track = GetTrack(noteMessage->status.channel);

	track.notesPlaying[noteMessage->note] = noteMessage->velocity;
}

void SongState::SetNoteOff(MidiNote* noteMessage)
{
	SongTrack& track = GetTrack(noteMessage->status.channel);

	NoteVelocityMap::iterator findIt = track.notesPlaying.find(noteMessage->note);
	if (findIt != track.notesPlaying.end())
	{
		track.notesPlaying.erase(findIt);
	}
}

void SongState::Update()
{
	// Get MIDI messages
	while (true)
	{
		std::vector<unsigned char> message;
		int nBytes;
		double stamp;
		stamp = MidiIn.getMessage(&message);
		if (message.empty())
			break;

		nBytes = message.size();
		if (nBytes && message.size() == 3)
		{
			MidiCommand* command = reinterpret_cast<MidiCommand*>(&message[0]);
			command->Print();

			switch (command->command)
			{
				case MidiCommandType::NoteOn:
				{
					MidiNote* noteOn = reinterpret_cast<MidiNote*>(command);
					SetNoteOn(noteOn);
					break;
				}
				case MidiCommandType::NoteOff:
				{
					MidiNote* noteOff = reinterpret_cast<MidiNote*>(command);
					SetNoteOff(noteOff);
					break;
				}
				default:
					break;
			}

			if (nBytes > 0)
			{
				std::cout << "stamp = " << stamp << std::endl;
			}
		}
	}
}
