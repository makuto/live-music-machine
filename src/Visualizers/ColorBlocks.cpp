#include "ColorBlocks.hpp"

void Visualization_ColorBlocksUpdate(window& win, SongState& song)
{
	SongTrack& track1 = song.GetTrack(0);

	// std::cout << track1.notesPlaying.size() << " notes playing\n";

	if (!track1.notesPlaying.empty())
	{
		std::cout << "Played a ntoe\n";
		win.setBackgroundColor(155, 0, 122, 255);
	}
	else
		win.setBackgroundColor(155, 155, 0, 255);
}
