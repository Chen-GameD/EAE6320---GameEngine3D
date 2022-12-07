#pragma once

#include <cstdint>

#if defined( EAE6320_PLATFORM_WINDOWS )
#include <Engine/Windows/Includes.h>
#endif

namespace eae6320
{
	namespace Graphics
	{
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
	}
}