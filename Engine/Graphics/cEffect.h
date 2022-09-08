/*
	This file declares the external interface for the effect system
*/

#ifndef EAE6320_GRAPHICS_CEFFECT_H
#define EAE6320_GRAPHICS_CEFFECT_H

// Includes
//=========

#include "cShader.h"
#include "cRenderState.h"

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
#ifdef EAE6320_PLATFORM_GL
			void BindShadingData();
			eae6320::cResult CleanUp();
#elif EAE6320_PLATFORM_D3D
			void BindShadingData(ID3D11DeviceContext* i_direct3dImmediateContext);
			void CleanUp();
#endif // EAE6320_PLATFORM_GL

			eae6320::cResult InitializeShadingData();

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
