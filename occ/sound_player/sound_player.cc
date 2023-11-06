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
};

void callback(void* userdata, Uint8* stream, int len)
{
	const SoundData* sdata = reinterpret_cast<const SoundData*>(userdata);
	for (int i = 0; i < len; i++)
	{
		// Square wave
		if ((i % 512) < 256)
		{
			stream[i] = 0;
		}
		else
		{
			stream[i] = 64;
		}
	}
}

int main()
{
	std::vector<Sound> sounds;
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
			sounds.push_back(sound);
		}
	}
	std::cout << "Read " << sounds.size() << " sounds\n";
	
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << "\n";
		return 1;
	}
	
	SoundData sdata;
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
	
	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		SDL_Delay(10);
	}
	
	SDL_CloseAudioDevice(device);

  return 0;
}
