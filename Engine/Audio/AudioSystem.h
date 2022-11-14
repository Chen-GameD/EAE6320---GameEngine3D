/*
	This file declares the external interface for the audio system
*/

#ifndef EAE6320_AUDIOSYSTEM_H
#define EAE6320_AUDIOSYSTEM_H

// Includes
//=========

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <Engine/Results/Results.h>

#include <iostream>
#include <string>

namespace eae6320
{
	namespace AudioSystem
	{
        // Interface
        //==========

        cResult Initialize();
        cResult CleanUp();


        // Audio Class
        //============
		class cAudio
		{
		private:
            // Audio file path
			std::string filePath = "";

            // Audio nick name
            std::string audioName = "";

            // Audio's index in s_audioDataReadyToPlay
            // The reason I do this is because one audio file maybe play at the same time but with a different audio volume
            // So the user needs to create a new cAudio instance
            // This new instance's filePath is the same but with different indexCount, refCount and volume
            //size_t indexCount = 0;

            // If the user want to use the same audio and same volume to play multiple times at the same time
            // Then the user does not need to create a new instance but just add 1 in this refCount
            //size_t refCount = 0;

            // Audio's volume
            size_t volume = 1000;

        public:
            // Interface
            //==========

            void CreateAudioData(std::string i_filePath = "", std::string i_audioName = "", size_t i_volume = 1000);

            void SubmitAudioSource();

            // Play the audio: 
            // param:
            // * isLoop: If this is true, will loop play this audio, if this is false, just play once.
            void Play(bool isLoop);

            void PauseAudio();

            bool IsPlaying();

		public:

            // Constructor
            cAudio(std::string i_filePath = "", std::string i_audioName = "", size_t i_volume = 1000) : filePath(i_filePath), audioName(i_audioName), volume(i_volume) {}

            // Ban default constructor
            //cAudio() = delete;



            //cAudio(){}

            //cAudio() : lenght(0), volume(1000), balance(0)
            //{

            //}

            //cAudio(std::string filename) : volume(1000), balance(0)
            //{
            //    //Open(filename);
            //}

            //~cAudio()
            //{
            //    //Close();
            //}

            /*void Open(std::string filename)
            {
                Close();
                this->filename = filename;
                std::string msg = "open \"" + filename + "\" type mpegvideo alias " + filename;
                mciSendString(TEXT(msg.c_str()), NULL, 0, 0);
                lenght = Lenght(true);
            }

            void Play(int pos = 0)
            {
                char* buff = new char[10];
                std::string msg = "play " + filename + " from " + itoa(pos, buff, 10);
                mciSendString(msg.c_str(), NULL, 0, 0);
                delete[] buff;
            }

            void Pause()
            {
                std::string msg = "pause " + filename;
                mciSendString(msg.c_str(), NULL, 0, 0);
            }

            void Resume()
            {
                std::string msg = "resume " + filename;
                mciSendString(msg.c_str(), NULL, 0, 0);
            }

            void Stop()
            {
                std::string msg = "stop " + filename;
                mciSendString(msg.c_str(), NULL, 0, 0);
            }

            void Close()
            {
                std::string msg = "close " + filename;
                mciSendString(msg.c_str(), NULL, 0, 0);
            }

            int Lenght(bool refresh = false)
            {
                if (refresh)
                {
                    char* buff = new char[128];
                    std::string msg = "status " + filename + " length";
                    mciSendString(msg.c_str(), buff, 128, 0);
                    int t = atoi(buff);
                    delete[] buff;
                    return t;
                }
                else
                {
                    return lenght;
                }
            }*/

		};
	}
}




#endif // !EAE6320_AUDIOSYSTEM_H



