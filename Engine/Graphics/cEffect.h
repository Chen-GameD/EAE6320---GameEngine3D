/*
	This file declares the external interface for the effect system
*/

#ifndef EAE6320_GRAPHICS_CEFFECT_H
#define EAE6320_GRAPHICS_CEFFECT_H

// Includes
//=========

#include "cShader.h"
#include "cRenderState.h"

#ifdef EAE6320_PLATFORM_D3D
	#include "sContext.h"
#endif

namespace
{
//#ifdef EAE6320_PLATFORM_GL
//	GLuint s_programId = 0;
//#endif // EAE6320_PLATFORM_GL
}

namespace eae6320
{
	namespace Graphics
	{
		class cEffect
		{
		public:
			// Interface
			//==========
			eae6320::cResult InitializeShadingData(const char* i_vertexShaderAddress, const char* i_fragmentShaderAddress);

			void BindShadingData();

			eae6320::cResult CleanUp();

		private:
			// Shading Data
			//-------------

			eae6320::Graphics::cShader* m_vertexShader = nullptr;
			eae6320::Graphics::cShader* m_fragmentShader = nullptr;

			eae6320::Graphics::cRenderState m_renderState;

#ifdef EAE6320_PLATFORM_GL
			GLuint m_programId = 0;
#endif // EAE6320_PLATFORM_GL
		};
	}
}

#endif	// EAE6320_GRAPHICS_CEFFECT_H
