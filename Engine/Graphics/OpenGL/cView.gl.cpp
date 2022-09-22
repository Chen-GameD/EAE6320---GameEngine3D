// Includes
//=========

#include "../cView.h"

#include "Includes.h"
#include "../sContext.h"

#include <Engine/Asserts/Asserts.h>

// Implementation
//===============

// Initialize / Clean Up
//----------------------
eae6320::cResult eae6320::Graphics::cView::InitializeViews(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;
	return result;
}


void eae6320::Graphics::cView::CleanUp()
{
	//Do noting, for making Platform-Independent Graphics.cpp
}

// ClearImageBuffer / ClearDepthBuffer / UpdateFrameConstantBuffer / SwapFrontBuffer
//----------------------

void eae6320::Graphics::cView::ClearImageBuffer(sDataRequiredToRenderAFrame* s_dataBeingRenderedByRenderThread)
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		// Black is usually used
		{
			auto& backBufferColor = s_dataBeingRenderedByRenderThread->backBufferColor;
			glClearColor(backBufferColor.R, backBufferColor.G, backBufferColor.B, backBufferColor.A);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		{
			constexpr GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
			glClear(clearColor);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
	}
}

void eae6320::Graphics::cView::ClearDepthBuffer()
{
	// In addition to the color buffer there is also a hidden image called the "depth buffer"
	// which is used to make it less important which order draw calls are made.
	// It must also be "cleared" every frame just like the visible color buffer.
	{
		{
			glDepthMask(GL_TRUE);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			constexpr GLclampd clearToFarDepth = 1.0;
			glClearDepth(clearToFarDepth);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		{
			constexpr GLbitfield clearDepth = GL_DEPTH_BUFFER_BIT;
			glClear(clearDepth);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
	}
}

void eae6320::Graphics::cView::UpdateFrameConstantBuffer(cConstantBuffer& s_constantBuffer_frame, sDataRequiredToRenderAFrame* s_dataBeingRenderedByRenderThread)
{
	EAE6320_ASSERT(s_dataBeingRenderedByRenderThread);

	// Update the frame constant buffer
	{
		// Copy the data from the system memory that the application owns to GPU memory
		auto& constantData_frame = s_dataBeingRenderedByRenderThread->constantData_frame;
		s_constantBuffer_frame.Update(&constantData_frame);
	}
}

void eae6320::Graphics::cView::SwapFrontBuffer()
{
	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (or "swapped" with the "front buffer", which is the image that is actually being displayed)
	{
		const auto deviceContext = sContext::g_context.deviceContext;
		EAE6320_ASSERT(deviceContext != NULL);
		const auto glResult = SwapBuffers(deviceContext);
		EAE6320_ASSERT(glResult != FALSE);
	}
}