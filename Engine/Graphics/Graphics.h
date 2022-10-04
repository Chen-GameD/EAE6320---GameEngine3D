/*
	This file declares the external interface for the graphics system
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H

// Includes
//=========

#include "Configuration.h"

#include <cstdint>
#include <Engine/Results/Results.h>
#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Math/Functions.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif

#include "ConstantBufferFormats.h"

// Interface
//==========

namespace eae6320
{
	struct sDataBackBufferFormat
	{
		float R = 1.0f;
		float G = 1.0f;
		float B = 1.0f;
		float A = 1.0f;
	};

	namespace Graphics
	{
		class cMesh;
		class cEffect;
	}

	struct Camera
	{
		Physics::sRigidBodyState m_RigidBodyState;
		float m_verticalFieldOfView_inRadians = Math::ConvertDegreesToRadians(45);
		float m_aspectRatio = 1.0f;
		float m_z_nearPlane = 0.1f;
		float m_z_farPlane = 100;
	};
	
	struct GameObjectData
	{
		eae6320::Graphics::cMesh* m_Mesh = nullptr;
		eae6320::Graphics::cEffect* m_Effect = nullptr;
		//rigidbody
		eae6320::Physics::sRigidBodyState m_RigidBodyState;
	};

	struct sDataRequiredToRenderAFrame
	{
		// Static const variables
		// -----------------------
		// The max number of game object that graphics can render at a frame, 
		// This can be change due to different platform,
		// But we'll stay with this for this project.
		static const uint16_t maxNumOfGameObject = 1000;

		// Struct Members
		// -----------------------
		eae6320::Graphics::ConstantBufferFormats::sFrame constantData_frame;
		eae6320::Graphics::ConstantBufferFormats::sDrawCall constantData_drawCall[maxNumOfGameObject];

		// Background color data
		eae6320::sDataBackBufferFormat backBufferColor;

		// A game object array that holds all gameobject's data
		GameObjectData* gameObjectArray[maxNumOfGameObject];

		// Number of game object that will be render at this frame
		size_t numOfGameObject;
	};

	namespace Graphics
	{
		// Submission
		//-----------

		// These functions should be called from the application (on the application loop thread)

		// As the class progresses you will add your own functions for submitting data,
		// but the following is an example (that gets called automatically)
		// of how the application submits the total elapsed times
		// for the frame currently being submitted
		void SubmitElapsedTime( const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime );

		// Submit back color data
		// The constant passed in is four float type data, from left to right are r, g, b, a 
		// r:red, 
		// g:green, 
		// b:blue, 
		// a:alpha
		void SubmitBackBufferColor(const float r, const float g, const float b, const float a);

		// Submit game object data
		// i_gameObjectArray should be all the (mesh, effect) pair that will be render at next frame
		// i_numOfGameObject will be the number of the (mesh, effect) pair
		void SubmitGameObjectData(GameObjectData*& i_gameObjectArray, size_t i_numOfGameObject, const float i_elapsedSecondCount_sinceLastSimulationUpdate);

		void SubmitCamera(Camera& i_camera, const float i_elapsedSecondCount_sinceLastSimulationUpdate);

		// When the application is ready to submit data for a new frame
		// it should call this before submitting anything
		// (or, said another way, it is not safe to submit data for a new frame
		// until this function returns successfully)
		cResult WaitUntilDataForANewFrameCanBeSubmitted( const unsigned int i_timeToWait_inMilliseconds );
		// When the application has finished submitting data for a frame
		// it must call this function
		cResult SignalThatAllDataForAFrameHasBeenSubmitted();

		// Render
		//-------

		// This is called (automatically) from the main/render thread.
		// It will render a submitted frame as soon as it is ready
		// (i.e. as soon as SignalThatAllDataForAFrameHasBeenSubmitted() has been called)
		void RenderFrame();

		// Initialize / Clean Up
		//----------------------

		struct sInitializationParameters
		{
#if defined( EAE6320_PLATFORM_WINDOWS )
			HWND mainWindow = NULL;
	#if defined( EAE6320_PLATFORM_D3D )
			uint16_t resolutionWidth = 0, resolutionHeight = 0;
	#elif defined( EAE6320_PLATFORM_GL )
			HINSTANCE thisInstanceOfTheApplication = NULL;
	#endif
#endif
		};

		cResult Initialize( const sInitializationParameters& i_initializationParameters );
		cResult CleanUp();
	}
}

#endif	// EAE6320_GRAPHICS_H
