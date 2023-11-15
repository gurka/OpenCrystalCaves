/*
https://moddingwiki.shikadi.net/wiki/Crystal_Caves_Sound_format
*/
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <find_steam_game.h>
#include <SDL.h>

#define SND_FILENAME_FMT "CC1-{}.SND"
#define GOG_ID "1207665273"
#define GAME_NAME "Crystal Caves"

#define SAMPLE_RATE 44100
#define AUDIO_FMT AUDIO_S16
#define AUDIO_CHANNELS 2

struct Sound
{
	uint16_t priority;
	uint16_t unknown;
	uint16_t vibrate;
	uint32_t unknown2;
	int16_t data[300];
};

std::filesystem::path get_sound_path(const int idx)
{
	// TODO: use <format> in C++20
	const auto filename = std::string(SND_FILENAME_FMT).replace(std::string(SND_FILENAME_FMT).find("{}"), std::string("{}").length(), std::to_string(idx + 1));
  // Try CWD first
  if (std::filesystem::exists(filename))
  {
    return std::filesystem::path(filename);
  }
  // Try GoG
  char buf[4096];
  fsg_get_gog_game_path(buf,
#ifdef _WIN32
                        GOG_ID
#else
                        GAME_NAME
#endif
  );
  if (buf[0])
  {
	  const auto gogfilename = std::filesystem::path(buf)
#ifdef __APPLE__
      / "game"
#endif
      / filename;
	  if (std::filesystem::exists(gogfilename))
	  {
		return std::filesystem::path(gogfilename);
	  }
  }
  // TODO: Try steam
  return std::filesystem::path();
}

struct SoundData {
	SDL_AudioSpec spec;
	std::vector<Sound> sounds;
	int sound_index = -1;
	int counter = 0;
	int freq_counter = 0;
	int index = -1;
	int dc = 64;
	
	const Sound& sound() const
	{
		return sounds[sound_index];
	}
	
	int16_t freq() const
	{
		return sound().data[index];
	}

	int16_t amp() const
	{
		if (freq() == 0 || (index % sound().vibrate) == 0)
		{
			return 0;
		}
		return dc;
	}
	
	void tick()
	{
		if (counter == 0)
		{
			nextfreq();
		}
		else
		{
			if (freq_counter == 0)
			{
				freq_counter = sound().data[index];
				dc *= -1;
			}
			counter--;
			freq_counter--;
		}
	}

	void nextfreq()
	{
		if (is_end())
		{
			return;
		}
		index++;
		freq_counter = 0;
		counter = 1400;
	}

	bool is_end() const
	{
		return sound_index == -1 || index >= 299 || counter == -1;
	}

	void play(const int _sound_index)
	{
		sound_index = _sound_index;
		std::cout << "Playing sound " << sound_index << "\n";
		index = -1;
		freq_counter = 0;
		counter = 1400;
	}
};

void callback(void* userdata, Uint8* stream, int len)
{
	SoundData* sdata = reinterpret_cast<SoundData*>(userdata);
	for (int i = 0; i < len; i++)
	{
		sdata->tick();
		// Generate square wave at frequency
		stream[i] = sdata->amp();
	}
}

char itoc(int i)
{
	if (i < 10)
	{
		return (char)('0' + i);
	}
	i -= 10;
	if (i < 26)
	{
		return (char)('a' + i);
	}
	i -= 26;
	if (i < 26)
	{
		return (char)('A' + i);
	}
	return '?';
}
int keytoi(const SDL_Keycode c, const bool shift)
{
	int d = 0;
	if (c >= SDLK_0 && c <= SDLK_9)
	{
		return c - SDLK_0 + d;
	}
	d += 10;
	if (shift)
	{
		d += 26;
	}
	if (c >= SDLK_a && c <= SDLK_z)
	{
		return c - SDLK_a + d;
	}
	return -1;
}

int main()
{
	SoundData sdata;
	for (int i = 0; ; i++)
	{
		const auto path = get_sound_path(i);
		if (path.empty())
		{
			break;
		}
		std::cout << "Reading sound file at " << path << "\n";
		std::ifstream input{path, std::ios::binary};
		Sound sound;
		while (input.read(reinterpret_cast<char*>(&sound), sizeof sound))
		{
			std::cout << +sound.priority << "," << +sound.vibrate << "," << +sound.unknown << "," << +sound.unknown2 << "\n";
			sdata.sounds.push_back(sound);
		}
	}
	std::cout << "Read " << sdata.sounds.size() << " sounds\n";
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << "\n";
		return 1;
	}
	
	// TODO: select sound
	SDL_AudioSpec want;
	want.freq = SAMPLE_RATE;
	want.format = AUDIO_FMT;
	want.channels = AUDIO_CHANNELS;
	want.samples = 4096;
	want.callback = callback;
	want.userdata = &sdata;
	const auto device = SDL_OpenAudioDevice(nullptr,
		   0,
		   &want,
		   &sdata.spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	if (device == 0)
	{
		std::cout << "Could open audio: " << SDL_GetError() << "\n";
		return 1;
	}
	
	SDL_PauseAudioDevice(device, 0);
	
	std::cout << "Play sound by pressing 0 - " << itoc((int)sdata.sounds.size()) << "\n";

	bool quit = false;
	bool keydown = false;
	while (!quit)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (!keydown && event.type == SDL_KEYDOWN)
			{
				keydown = true;
				const bool shift = !!(SDL_GetModState() & KMOD_SHIFT);
				const int index = keytoi(event.key.keysym.sym, shift);
				if (index >= 0 && index < (int)sdata.sounds.size())
				{
					sdata.play(index);
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				keydown = false;
			}
		}
		SDL_Delay(10);
	}
	
	SDL_CloseAudioDevice(device);

  return 0;
}
