#include "AudioSystem.h"

#include <Engine/Logging/Logging.h>

#include <vector>

//std::string filename;

// Static Data
//============

namespace
{
	// Structure of Audio list to play
	struct sAudioListDataStucture
	{
		std::vector<eae6320::AudioSystem::cAudio*> m_AudioList;

		size_t indexCount = 0;
	};

	// Static member that audio system care about.
	// Needs to initialize at the begainning.
	// User can submit new audio source at anytime except initialize.
	sAudioListDataStucture s_audioDataReadyToPlay;
}

// Interface
//==========

void eae6320::AudioSystem::cAudio::CreateAudioData(std::string i_filePath, std::string i_audioName, size_t i_volume)
{
	if (i_filePath != "" && i_audioName != "" && i_volume)
	{
		filePath = i_filePath;
		audioName = i_audioName;
		volume = i_volume;
	}
	else
	{
		Logging::OutputMessage("Audio path or name is illegal!");
	}
}

void eae6320::AudioSystem::cAudio::SubmitAudioSource()
{
	MCIERROR mciError;

	//mciError = mciSendString(TEXT("open data/audios/Mixdown.wav type mpegvideo alias TEST1"), NULL, 0, 0);
	//mciError = mciSendString(TEXT("open data/audios/Mixdown.wav type mpegvideo alias TEST2"), NULL, 0, 0);
	//mciError = mciSendString(TEXT("open data/audios/Test.mp3 type mpegvideo alias TEST1"), NULL, 0, 0);

	std::string mciCommendString = "open " + filePath + " type mpegvideo alias " + audioName;
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();;
	//mciError = mciSendString(mciCommend, NULL, 0, 0);
	mciError = mciSendString(mciCommend, NULL, 0, 0);

	WCHAR errorMessage[128];
	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		Logging::OutputMessage("Submit audio failed : %s:", errorMessage);
	}
}

void eae6320::AudioSystem::cAudio::Play(bool isLoop)
{
	//filename = "data / audios / Test.mp3";

	//PlaySound(TEXT("data/audios/Mixdown.wav"), NULL, SND_FILENAME | SND_ASYNC);
	//result = _stprintf_s
	//mciSendString(TEXT("open data/audios/Test.mp3 alias TEST"), NULL, 0, 0);
	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "play " + audioName;
	LPCWSTR mciCommend;
	if (isLoop)
	{
		mciCommendString += " repeat";
		std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
		mciCommend = temp.c_str();
		mciError = mciSendString(mciCommend, NULL, 0, 0);
		//mciError = mciSendString(TEXT("play TestAudio repeat"), NULL, 0, 0);
	}
	else
	{
		std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
		mciCommend = temp.c_str();
		mciError = mciSendString(mciCommend, NULL, 0, 0);
	}

	//mciError = mciSendString(TEXT("play TESTt repeat"), NULL, 0, 0);
	if (mciError)
	{
		//UINT mciErrorLen = MCI_ERROR_LENGTH;
		//LPWSTR* textBuf = ;
		mciGetErrorString(mciError, errorMessage, 128);
		if (errorMessage)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			Logging::OutputMessage("Play audio failed : %s:", errorMessage);
		}
	}
}

void eae6320::AudioSystem::cAudio::PauseAudio()
{
	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "pause " + audioName;
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();

	mciError = mciSendString(mciCommend, NULL, 0, 0);

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		if (errorMessage)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			Logging::OutputMessage("Pause audio failed : %s:", errorMessage);
		}
	}
}

bool eae6320::AudioSystem::cAudio::IsPlaying()
{
	bool retValue = false;

	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "status " + audioName + " mode";
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();
	char status[128];

	mciError = mciSendString(mciCommend, (LPWSTR)status, 64, 0);

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		if (errorMessage)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			Logging::OutputMessage("Call IsPlaying() failed : %s:", errorMessage);
		}
		return retValue;
	}
	//char myChar[128] = (char)status;
	retValue = strcmp(status, "playing") == 0;

	return retValue;
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::AudioSystem::Initialize()
{
	auto result = Results::Success;

	return result;
}

eae6320::cResult eae6320::AudioSystem::CleanUp()
{
	auto result = Results::Success;

	return result;
}