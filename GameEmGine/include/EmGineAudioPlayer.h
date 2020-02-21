#pragma once
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <vector>
#include <string>

typedef FMOD::System AudioSystem;
typedef FMOD::Sound Audio;
typedef FMOD::Channel AudioChannel;
typedef FMOD::ChannelGroup AudioChannelGroup;
typedef FMOD_VECTOR FMOD_VEC3;
typedef unsigned int uint;

struct Listener
{
	FMOD_VEC3 pos = {0,0,0},
		vel = {0,0,0},
		up = {0,1,0},
		forward = {1,0,0};
};

struct AudioControle
{
	~AudioControle()
	{
		//delete sound;
		//delete channel;
	}
	Audio* sound;
	AudioChannel* channel;
	Listener* listener;
	std::string tag;
};

class EmGineAudioPlayer
{
public:
	//MUST be called before using any other function 
	static void init(int channels = 36);

	/*
	disables the audio system, closing and clearing all open audio instances.
	NOTE:
	EmGineAudioPlayer::init() must be called before any other calls can be made
	*/
	static void disable();

	//creates an audio instance that is stored in memory	
	static bool createAudio(const char* file, std::string tag);

	//creates an audio instance that is read from disk (recommended for large audio files)	
	static bool createAudioStream(const char* file, std::string tag);

	//plays a single audio channel created by EmGineAudioPlayer::createAudio/AudioStream()	
	static void play(bool loop = false, bool newInstance = false, uint index = (m_controle->size() - 1),
		uint from = 0, uint to = 0, FMOD_TIMEUNIT unit = FMOD_TIMEUNIT_MS);

	static void play(std::string tag, bool loop = false, bool newInstance = false,
		uint from = 0, uint to = 0, FMOD_TIMEUNIT unit = FMOD_TIMEUNIT_MS);

	//plays all existing audio channels created by EmGineAudioPlayer::createAudio/AudioStream()	
	static void playAll(bool loop = false, uint from = 0, uint to = 0, FMOD_TIMEUNIT unit = FMOD_TIMEUNIT_MS);

	//pauses an audio channel at specified index.
	static void pause(uint index = (m_controle->size() - 1));
	static void pause(std::string tag = "");

	//pauses all audio channels	
	static void pauseAll();

	//stops audio channel at specified index
	static void stop(uint index = (m_controle->size() - 1));
	static void stop(std::string tag = "");

	//stops all audio channels
	static void stopAll();

	//mutes audio channel at specified index
	static void mute(uint index = (m_controle->size() - 1));
	static void mute(std::string tag = "");

	//mutes all audio channels
	static void muteAll();

	/*
	checks if audio channel at specified index has stopped playing
	NOTE:
	audio is not considered off until audio has reached it's end
	or EmGineAudioPlayer::stop/stopAll() is called
	*/
	static bool isStoped(uint index = (m_controle->size() - 1));
	static bool isStoped(std::string tag = "");

	/*
	checks if audio channel at specified index has been paused
	NOTE:
	audio is not considered off until audio has reached it's end
	or EmGineAudioPlayer::pause/pauseAll() is called
	*/
	static bool isPaused(uint index = (m_controle->size() - 1));
	static bool isPaused(std::string tag = "");

	static uint getPosition(uint index = (m_controle->size() - 1), FMOD_TIMEUNIT type = FMOD_TIMEUNIT_MS);
	static uint getPosition(FMOD_TIMEUNIT type, std::string tag = "");

	//gets the amount of audio channels created
	static uint size();

	/*
	sets audio volume at specified index with normal volume levels ranging from 0 -> 1.
	NOTE:
	*levels below 0 will invert sound.
	*increasing level above the normal level may result in distortion.
	*/
	static void setVolume(float vol, uint index = (m_controle->size() - 1));
	static void setVolume(std::string tag, float vol);


	/*
	sets the maximum volume levels for all audio channels ranging from 0 -> 1.
	NOTE:
	*levels below 0 will invert sound.
	*increasing level above the normal level may result in distortion.
	*/
	static void setMasterVolume(float vol);

	static AudioSystem * getAudioSystem();

	static AudioChannelGroup * getMasterChannelGroup();

	static std::vector<AudioControle*> * getAudioControle();

	//required for certain functionality (i.e. audio cleanup,3D sound...)
	static void update();

private:

	static void cleanup();
	static void printError(FMOD_RESULT, const char* where = "");
	static FMOD_RESULT __stdcall cleanUpCallback(FMOD_CHANNELCONTROL * chanCtrl, FMOD_CHANNELCONTROL_TYPE ctrlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, void* commandData1, void* commandData2);

	static uint stopIndex;

	static AudioSystem * m_system;
	static AudioChannelGroup * m_mainChannelGroup;
	static std::vector<AudioControle*> * m_controle;
	//static std::vector<Audio*>* m_sounds;
};

