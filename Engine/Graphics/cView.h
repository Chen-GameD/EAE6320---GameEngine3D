/*
	This file declares the external interface for the view system
*/

#ifndef EAE6320_GRAPHICS_CVIEW_H
#define EAE6320_GRAPHICS_CVIEW_H

// Includes
//=========

#include "Graphics.h"

#ifdef EAE6320_PLATFORM_D3D
#include <Engine/Results/Results.h>
#include <d3d11.h>
#endif

#include "cConstantBuffer.h"

namespace eae6320
{
	namespace Graphics
	{
		class cView
		{
			// Interface
			//==========

		public:
			eae6320::cResult InitializeViews(const sInitializationParameters& i_initializationParameters);
			void CleanUp();

			void ClearImageBuffer(sDataRequiredToRenderAFrame* s_dataBeingRenderedByRenderThread);
			void ClearDepthBuffer();
			void UpdateFrameConstantBuffer(cConstantBuffer& s_constantBuffer_frame, sDataRequiredToRenderAFrame* s_dataBeingRenderedByRenderThread);
			void SwapFrontBuffer();

		private:
#ifdef EAE6320_PLATFORM_D3D
			// In Direct3D "views" are objects that allow a texture to be used a particular way:
			// A render target view allows a texture to have color rendered to it
			ID3D11RenderTargetView* m_renderTargetView = nullptr;
			// A depth/stencil view allows a texture to have depth rendered to it
			ID3D11DepthStencilView* m_depthStencilView = nullptr;
#endif // EAE6320_PLATFORM_D3
		};
	}
}

#endif	// EAE6320_GRAPHICS_CVIEW_H
