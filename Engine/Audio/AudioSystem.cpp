#include "AudioSystem.h"

#include <Engine/Logging/Logging.h>

#include <vector>

//std::string filename;

// Static Data
//============

namespace
{
	// Structure of Audio list to play
	struct sIndependentAudioListDataStucture
	{
		std::vector<eae6320::AudioSystem::cAudio*> m_AudioList;

		size_t indexCount = 0;
	};

	// Static member that audio system care about.
	// Needs to initialize at the begainning.
	// User can submit new audio source at anytime except initialize.
	sIndependentAudioListDataStucture s_audioDataReadyToPlay;
}

// Interface
//==========

void eae6320::AudioSystem::cAudio::CreateAudioData(std::string i_filePath, std::string i_audioName, size_t i_volume, bool i_isLoop)
{
	if (i_filePath != "" && i_audioName != "" && i_volume)
	{
		filePath = i_filePath;
		audioName = i_audioName;
		volume = i_volume;
		isLoop = i_isLoop;
		inIndex = 0;
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

void eae6320::AudioSystem::cAudio::Play()
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

void eae6320::AudioSystem::cAudio::PlayIndependent()
{
	// Check independent list
	// If there is no audio being played, turn off the audio and free the device for others to use
	std::string t_audioName;
	for (size_t i = inIndex; i > 0; i--)
	{
		t_audioName = audioName + "_" + std::to_string(i);
		if (IsPlaying_WithName(t_audioName))
		{
			break;
		}
		else
		{
			// Close this audio
			if (CloseAudio_WithName(t_audioName) == Results::Success)
				inIndex--;
		}
	}

	// Init a new device for new audio
	inIndex++;
	t_audioName = audioName + "_" + std::to_string(inIndex);

	// Open new device
	MCIERROR mciError;
	std::string mciCommendString = "open " + filePath + " type mpegvideo alias " + t_audioName;
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();
	mciError = mciSendString(mciCommend, NULL, 0, 0);

	WCHAR errorMessage[128];
	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		Logging::OutputMessage("Submit audio %s failed : %s:", t_audioName, errorMessage);
	}

	// Play new device
	mciCommendString = "play " + t_audioName;
	temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	mciCommend = temp.c_str();
	mciError = mciSendString(mciCommend, NULL, 0, 0);

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		Logging::OutputMessage("Play audio %s failed : %s:", t_audioName, errorMessage);
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

void eae6320::AudioSystem::cAudio::SetVolume(size_t i_volume)
{
	// Check the i_volume legality, 
	// if i_volume < 1, then i_volume = 1
	// if i_volume > 1000, then i_volume = 1000
	if (i_volume < 1 || i_volume > 1000)
	{
		volume = i_volume < 1 ? 1 : 1000;
	}
	else
	{
		volume = i_volume;
	}

	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "setaudio " + audioName + " volume to " + std::to_string(volume);
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();

	mciError = mciSendString(mciCommend, NULL, 0, 0);

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		if (errorMessage)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			Logging::OutputMessage("Set audio volume failed : %s:", errorMessage);
		}
	}

	// Set all independent audio to the same volume
	if (inIndex > 0)
	{
		for (size_t i = inIndex; i > 0; i--)
		{
			std::string t_audioName = audioName + "_" + std::to_string(i);
			mciCommendString = "setaudio " + t_audioName + " volume to " + std::to_string(volume);
			temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
			mciCommend = temp.c_str();

			mciError = mciSendString(mciCommend, NULL, 0, 0);

			if (mciError)
			{
				mciGetErrorString(mciError, errorMessage, 128);
				if (errorMessage)
				{
					mciGetErrorString(mciError, errorMessage, 128);
					Logging::OutputMessage("Set audio %s volume failed : %s:", t_audioName, errorMessage);
				}
			}
		}
	}
}

bool eae6320::AudioSystem::cAudio::IsPlaying()
{
	return IsPlaying_WithName(audioName);
}

bool eae6320::AudioSystem::cAudio::IsPlaying_WithName(std::string i_audioName)
{
	bool retValue = false;

	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "status " + i_audioName + " mode";
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();
	WCHAR status[128];

	mciError = mciSendString(mciCommend, status, 128, 0);

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
	const char myChar[8] = {(char)status[0], (char)status[1], (char)status[2], (char)status[3], (char)status[4], (char)status[5], (char)status[6], (char)status[7]};
	
	retValue = strcmp(myChar, "playing") == 0;

	return retValue;
}

eae6320::cResult eae6320::AudioSystem::cAudio::CloseAudio()
{
	eae6320::cResult retValue = Results::Success;

	// Check independent audio, close them all
	for (size_t i = inIndex; i > 0; i--)
	{
		std::string t_audioName = audioName + "_" + std::to_string(i);
		retValue = CloseAudio_WithName(t_audioName);

		if (retValue == Results::Failure)
			return retValue;
	}

	// Close audio
	retValue = CloseAudio_WithName(audioName);

	return retValue;
}

eae6320::cResult eae6320::AudioSystem::cAudio::CloseAudio_WithName(std::string i_audioName)
{
	eae6320::cResult retValue = Results::Success;

	MCIERROR mciError;
	WCHAR errorMessage[128];
	std::string mciCommendString = "close " + i_audioName;
	std::wstring temp = std::wstring(mciCommendString.begin(), mciCommendString.end());
	LPCWSTR mciCommend = temp.c_str();

	mciError = mciSendString(mciCommend, NULL, 64, 0);

	if (mciError)
	{
		mciGetErrorString(mciError, errorMessage, 128);
		if (errorMessage)
		{
			mciGetErrorString(mciError, errorMessage, 128);
			Logging::OutputMessage("Close audio %s failed : %s:", i_audioName, errorMessage);
		}

		retValue = Results::Failure;
	}

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