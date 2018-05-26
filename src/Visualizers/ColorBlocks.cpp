#include "ColorBlocks.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

void Visualization_ColorBlocksUpdate(window& win, SongState& song)
{
	for (int trackIndex = 0; trackIndex < MAX_TRACKS; ++trackIndex)
	{
		SongTrack& currentTrack = song.GetTrack(trackIndex);
		if (!currentTrack.notesPlaying.empty())
		{
			win.setBackgroundColor(155, 0, currentTrack.notesPlaying.begin()->first % 256, 255);
			int i = 0;
			for (NoteVelocityMap::iterator note = currentTrack.notesPlaying.begin();
			     note != currentTrack.notesPlaying.end(); ++note)
			{
				sf::RenderWindow* sfWin = win.getBase();
				float rectWidth = 10.f;
				float rectHeight = 128.f;
				sf::RectangleShape rectangle;
				rectangle.setSize(sf::Vector2f(rectWidth, rectHeight));
				rectangle.setFillColor(sf::Color(0, 0, (note->first * 20) % 256, 255));
				rectangle.setPosition(note->first * rectWidth, trackIndex * rectHeight);
				sfWin->draw(rectangle);

				++i;
			}
		}
		else
			win.setBackgroundColor(155, 155, 0, 255);
	}
}
