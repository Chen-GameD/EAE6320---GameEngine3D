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

			static cResult CreateEffect(cEffect*& o_effect, const char* i_vertexShaderAddress, const char* i_fragmentShaderAddress);

			//eae6320::cResult InitializeShadingData(const char* i_vertexShaderAddress, const char* i_fragmentShaderAddress);

			void BindShadingData();

			//eae6320::cResult CleanUp();

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cEffect)

		private:

			eae6320::cResult InitializeShadingData(const char* i_vertexShaderAddress, const char* i_fragmentShaderAddress);

			eae6320::cResult CleanUp();

		private:
			// Shading Data
			//-------------

			eae6320::Graphics::cShader* m_vertexShader = nullptr;
			eae6320::Graphics::cShader* m_fragmentShader = nullptr;

			eae6320::Graphics::cRenderState m_renderState;
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()

#ifdef EAE6320_PLATFORM_GL
			GLuint m_programId = 0;
#endif // EAE6320_PLATFORM_GL

			//Reference Counting
			

			//Constructor / Destructor
			cEffect() = default;
			~cEffect();
		};
	}
}

#endif	// EAE6320_GRAPHICS_CEFFECT_H
