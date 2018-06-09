#include "ColorBlocks.hpp"

#include "timer/timer.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#define MIN(a, b) (a <= b ? a : b)
#define MAX(a, b) (a >= b ? a : b)

template <class T>
struct MinimumContainer
{
	int maxCapacity = 0;
	timer shrinkTimer;
	int shrinkToCapacity = 0;
	bool shrinkTimerStarted = false;
	timer internalTimer;
	bool hasStartedTimer = false;

	const int SHRINK_DELAY = 3.f;

	typename std::map<T, float> recentValues;

	void Update(std::vector<T>& activeValues)
	{
		if (!hasStartedTimer)
		{
			internalTimer.start();
			hasStartedTimer = true;
		}

		float currentTime = internalTimer.getTime();

		// Update recent values
		for (T value : activeValues)
		{
			recentValues[value] = currentTime;
		}

		// Remove values which haven't been played in a while
		for (typename std::map<T, float>::iterator it = recentValues.begin();
		     it != recentValues.end();)
		{
			if (currentTime - it->second >= SHRINK_DELAY)
				it = recentValues.erase(it);
			else
				++it;
		}

		maxCapacity = recentValues.size();
	}

	int GetRelativeIndex(T value)
	{
		int recentIndex = 0;
		for (std::pair<T, float> recentValue : recentValues)
		{
			if (recentValue.first == value)
				break;
			++recentIndex;
		}

		return recentIndex;
	}
};

struct ColorBlocksData
{
	MinimumContainer<int> activeTracks;
	MinimumContainer<Note> activeNotes[MAX_TRACKS];
};

static ColorBlocksData s_ColorBlocks;

static const int s_colorScheme[][3] = {
    {46, 17, 45}, {84, 0, 50}, {130, 3, 51}, {201, 40, 62}, {240, 67, 58}};

#define COLOR(index) \
	s_colorScheme[(index)][0], s_colorScheme[(index)][1], s_colorScheme[(index)][2], 255

// Why don't these work?
//#define COLORARRAY_SIZE(array) sizeof(array) / sizeof(array[0]) / sizeof(array[0][0])

void Visualization_ColorBlocksUpdate(window& win, SongState& song)
{
	win.setBackgroundColor(COLOR(0));

	// Update notes and tracks MinimumContainers
	std::vector<int> activeTracks;
	for (int trackIndex = 0; trackIndex < MAX_TRACKS; ++trackIndex)
	{
		SongTrack& currentTrack = song.GetTrack(trackIndex);
		if (!currentTrack.notesPlaying.empty())
			activeTracks.push_back(trackIndex);

		// Update recent notes
		std::vector<Note> recentNotes;
		for (std::pair<Note, Velocity> note : currentTrack.notesPlaying)
		{
			recentNotes.push_back(note.first);
		}

		s_ColorBlocks.activeNotes[trackIndex].Update(recentNotes);
	}
	s_ColorBlocks.activeTracks.Update(activeTracks);

	for (int trackIndex = 0; trackIndex < MAX_TRACKS; ++trackIndex)
	{
		SongTrack& currentTrack = song.GetTrack(trackIndex);
		float rectWidth =
		    win.getWidth() / MAX(1, s_ColorBlocks.activeNotes[trackIndex].maxCapacity);
		float rectHeight = win.getHeight() / MAX(1, s_ColorBlocks.activeTracks.maxCapacity);

		int trackRecentIndex = s_ColorBlocks.activeTracks.GetRelativeIndex(trackIndex);

		for (std::pair<Note, Velocity> note : currentTrack.notesPlaying)
		{
			// Determine this note's index relative to other recently played notes
			int noteRecentIndex =
			    s_ColorBlocks.activeNotes[trackIndex].GetRelativeIndex(note.first);

			sf::RenderWindow* sfWin = win.getBase();
			sf::RectangleShape rectangle;
			rectangle.setSize(sf::Vector2f(rectWidth, rectHeight));

			// This color code is still wrong
			// Exclude background color
			int colorIndex = (noteRecentIndex % (5 - 1)) + 1;
			rectangle.setFillColor(sf::Color(COLOR(colorIndex)));
			
			rectangle.setPosition(noteRecentIndex * rectWidth, trackRecentIndex * rectHeight);
			sfWin->draw(rectangle);
		}
	}
}
