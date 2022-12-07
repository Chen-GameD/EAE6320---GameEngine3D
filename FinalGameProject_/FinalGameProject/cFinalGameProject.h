/*
	This class is your specific game
*/

#ifndef EAE6320_CFINALGAMEPROJECT_H
#define EAE6320_CFINALGAMEPROJECT_H

// Includes
//=========

#include <Engine/Application/iApplication.h>
#include <Engine/Results/Results.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/StoryNarrator/Includes.h>
#include <External/imgui/Includes.h>

#if defined( EAE6320_PLATFORM_D3D )
#include <Engine/Graphics/Direct3D/Includes.h>
#elif defined( EAE6320_PLATFORM_GL )
#include <Engine/Graphics/OpenGl/Includes.h>
#endif

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif

// Class Declaration
//==================


namespace eae6320
{
	class cFinalGameProject final : public Application::iApplication
	{
		// Inherited Implementation
		//=========================

		void SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate);

		void RenderUI();

		void UpdateDialogue();

	private:

		// Configuration
		//--------------

		virtual void GetDefaultInitialResolution(uint16_t& o_width, uint16_t& o_height) const
		{
			o_width = 1600;
			o_height = 1000;
		}

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		const char* GetMainWindowName() const final
		{
			return "CMY's Game"
				" -- "
#if defined( EAE6320_PLATFORM_D3D )
				"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
#else
				" -- Release"
#endif
			;
		}
		// Window classes are almost always identified by name;
		// there is a unique "ATOM" associated with them,
		// but in practice Windows expects to use the class name as an identifier.
		// If you don't change the name below
		// your program could conceivably have problems if it were run at the same time on the same computer
		// as one of your classmate's.
		// You don't need to worry about this for our class,
		// but if you ever ship a real project using this code as a base you should set this to something unique
		// (a generated GUID would be fine since this string is never seen)
		const char* GetMainWindowClassName() const final { return "CMY's Main Window Class"; }
		// The following three icons are provided:
		//	* IDI_EAEGAMEPAD
		//	* IDI_EAEALIEN
		//	* IDI_VSDEFAULT_LARGE / IDI_VSDEFAULT_SMALL
		// If you want to try creating your own a convenient website that will help is: http://icoconvert.com/
		const WORD* GetLargeIconId() const final { static constexpr WORD iconId_large = IDI_EAEGAMEPAD; return &iconId_large; }
		const WORD* GetSmallIconId() const final { static constexpr WORD iconId_small = IDI_EAEGAMEPAD; return &iconId_small; }
#endif

		// Run
		//----

		void UpdateBasedOnInput() final;
		void UpdateSimulationBasedOnInput();
		void UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate);

		// Initialize / Clean Up
		//----------------------

		cResult Initialize() final;
		cResult CleanUp() final;

		eae6320::cResult InitializeStory();

#if defined( EAE6320_PLATFORM_D3D )
		bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
		ID3D11ShaderResourceView* my_texture = NULL;
#elif defined( EAE6320_PLATFORM_GL )
		bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
		GLuint my_texture = 0;
#endif

		Narrator::Runtime::Story story;
		std::string CurrentText = "";
		bool MeetChoice = false;
		int my_image_width = 0;
		int my_image_height = 0;
		ImFont* MyFont;
	};
}

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		namespace Application
		{
			// You can add specific results for your game here:
			//	* The System should always be Application
			//	* The __LINE__ macro is used to make sure that every result has a unique ID.
			//		That means, however, that all results _must_ be defined in this single file
			//		or else you could have two different ones with equal IDs.
			//	* Note that you can define multiple Success codes.
			//		This can be used if the caller may want to know more about how a function succeeded.
			constexpr cResult ExampleResult( IsFailure, eSystem::Application, __LINE__, Severity::Default );
		}
	}
}

#endif	// EAE6320_CMYGAME_H
