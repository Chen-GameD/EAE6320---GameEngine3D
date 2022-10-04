// Includes
//=========

#include "Graphics.h"

#include "cView.h"

#include <Engine/Concurrency/cEvent.h>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/UserOutput/UserOutput.h>
#include "sContext.h"

#include "cMesh.h"
#include "cEffect.h"

// Static Data
//============

namespace
{
	// Static View
	eae6320::Graphics::cView s_view;

	// Constant buffer object
	eae6320::Graphics::cConstantBuffer s_constantBuffer_frame(eae6320::Graphics::ConstantBufferTypes::Frame);
	eae6320::Graphics::cConstantBuffer s_constantBuffer_drawCall(eae6320::Graphics::ConstantBufferTypes::DrawCall);

	// Submission Data
	//----------------

	// This struct's data is populated at submission time;
	// it must cache whatever is necessary in order to render a frame
	/*struct sDataRequiredToRenderAFrame
	{
		eae6320::Graphics::ConstantBufferFormats::sFrame constantData_frame;
	};*/
	// In our class there will be two copies of the data required to render a frame:
	//	* One of them will be in the process of being populated by the data currently being submitted by the application loop thread
	//	* One of them will be fully populated and in the process of being rendered from in the render thread
	// (In other words, one is being produced while the other is being consumed)
	eae6320::sDataRequiredToRenderAFrame s_dataRequiredToRenderAFrame[2];
	auto* s_dataBeingSubmittedByApplicationThread = &s_dataRequiredToRenderAFrame[0];
	auto* s_dataBeingRenderedByRenderThread = &s_dataRequiredToRenderAFrame[1];
	// The following two events work together to make sure that
	// the main/render thread and the application loop thread can work in parallel but stay in sync:
	// This event is signaled by the application loop thread when it has finished submitting render data for a frame
	// (the main/render thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenAllDataHasBeenSubmittedFromApplicationThread;
	// This event is signaled by the main/render thread when it has swapped render data pointers.
	// This means that the renderer is now working with all the submitted data it needs to render the next frame,
	// and the application loop thread can start submitting data for the following frame
	// (the application loop thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenDataForANewFrameCanBeSubmittedFromApplicationThread;
}

// Interface
//==========

// Submission
//-----------

void eae6320::Graphics::SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	auto& constantData_frame = s_dataBeingSubmittedByApplicationThread->constantData_frame;
	constantData_frame.g_elapsedSecondCount_systemTime = i_elapsedSecondCount_systemTime;
	constantData_frame.g_elapsedSecondCount_simulationTime = i_elapsedSecondCount_simulationTime;
}

void eae6320::Graphics::SubmitBackBufferColor(const float r, const float g, const float b, const float a)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	auto& backBufferColor = s_dataBeingSubmittedByApplicationThread->backBufferColor;
	backBufferColor.R = r;
	backBufferColor.G = g;
	backBufferColor.B = b;
	backBufferColor.A = a;
}

void eae6320::Graphics::SubmitGameObjectData(GameObjectData*& i_gameObjectArray, size_t i_numOfGameObject, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	EAE6320_ASSERT(i_numOfGameObject <= sDataRequiredToRenderAFrame::maxNumOfGameObject);
	for (size_t i = 0; i < i_numOfGameObject; i++)
	{
		s_dataBeingSubmittedByApplicationThread->gameObjectArray[i] = &i_gameObjectArray[i];
		s_dataBeingSubmittedByApplicationThread->constantData_drawCall[i].g_transform_localToWorld = s_dataBeingSubmittedByApplicationThread->gameObjectArray[i]->m_RigidBodyState.PredictFutureTransform(i_elapsedSecondCount_sinceLastSimulationUpdate);
	}
	s_dataBeingSubmittedByApplicationThread->numOfGameObject = i_numOfGameObject;
}

void eae6320::Graphics::SubmitCamera(Camera& i_camera, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	i_camera.m_RigidBodyState.position = i_camera.m_RigidBodyState.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
	i_camera.m_RigidBodyState.orientation = i_camera.m_RigidBodyState.PredictFutureOrientation(i_elapsedSecondCount_sinceLastSimulationUpdate);
	auto& constantData_frame = s_dataBeingSubmittedByApplicationThread->constantData_frame;
	constantData_frame.g_transform_worldToCamera = Math::cMatrix_transformation::CreateWorldToCameraTransform(i_camera.m_RigidBodyState.orientation, i_camera.m_RigidBodyState.position);
	constantData_frame.g_transform_cameraToProjected = Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(i_camera.m_verticalFieldOfView_inRadians, i_camera.m_aspectRatio, i_camera.m_z_nearPlane, i_camera.m_z_farPlane);
}

eae6320::cResult eae6320::Graphics::WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds)
{
	return Concurrency::WaitForEvent(s_whenDataForANewFrameCanBeSubmittedFromApplicationThread, i_timeToWait_inMilliseconds);
}

eae6320::cResult eae6320::Graphics::SignalThatAllDataForAFrameHasBeenSubmitted()
{
	return s_whenAllDataHasBeenSubmittedFromApplicationThread.Signal();
}

// Render
//-------

void UpdateDrawCallConstantBuffer(eae6320::Graphics::ConstantBufferFormats::sDrawCall& i_constantData_drawCall)
{
	s_constantBuffer_drawCall.Update(&i_constantData_drawCall);
}

void eae6320::Graphics::RenderFrame()
{
	/*int a = sizeof(s_dataRequiredToRenderAFrame);
	int b = sizeof(s_constantBuffer_frame);
	int c = sizeof(s_constantBuffer_drawCall);
	int d = sizeof(s_view);
	int e = 0;*/

	// Wait for the application loop to submit data to be rendered
	{
		if (Concurrency::WaitForEvent(s_whenAllDataHasBeenSubmittedFromApplicationThread))
		{
			// Switch the render data pointers so that
			// the data that the application just submitted becomes the data that will now be rendered
			std::swap(s_dataBeingSubmittedByApplicationThread, s_dataBeingRenderedByRenderThread);
			// Once the pointers have been swapped the application loop can submit new data
			if (!s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Signal())
			{
				EAE6320_ASSERTF(false, "Couldn't signal that new graphics data can be submitted");
				Logging::OutputError("Failed to signal that new render data can be submitted");
				UserOutput::Print("The renderer failed to signal to the application that new graphics data can be submitted."
					" The application is probably in a bad state and should be exited");
				return;
			}
		}
		else
		{
			EAE6320_ASSERTF(false, "Waiting for the graphics data to be submitted failed");
			Logging::OutputError("Waiting for the application loop to submit data to be rendered failed");
			UserOutput::Print("The renderer failed to wait for the application to submit data to be rendered."
				" The application is probably in a bad state and should be exited");
			return;
		}
	}

	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	s_view.ClearImageBuffer(s_dataBeingRenderedByRenderThread);

	// In addition to the color buffer there is also a hidden image called the "depth buffer"
	// which is used to make it less important which order draw calls are made.
	// It must also be "cleared" every frame just like the visible color buffer.
	s_view.ClearDepthBuffer();

	//// Update the frame constant buffer
	s_view.UpdateFrameConstantBuffer(s_constantBuffer_frame, s_dataBeingRenderedByRenderThread);

	for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->numOfGameObject; i++)
	{
		// Update the draw call constant buffer
		UpdateDrawCallConstantBuffer(s_dataBeingRenderedByRenderThread->constantData_drawCall[i]);
		// Bind the shading data
		s_dataBeingRenderedByRenderThread->gameObjectArray[i]->m_Effect->BindShadingData();
		// Draw the geometry
		s_dataBeingRenderedByRenderThread->gameObjectArray[i]->m_Mesh->DrawGeometry();
	}

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (or "swapped" with the "front buffer", which is the image that is actually being displayed)
	s_view.SwapFrontBuffer();

	// After all of the data that was submitted for this frame has been used
	// you must make sure that it is all cleaned up and cleared out
	// so that the struct can be re-used (i.e. so that data for a new frame can be submitted to it)
	{
		// (At this point in the class there isn't anything that needs to be cleaned up)
		//dataRequiredToRenderFrame	// TODO

		for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->numOfGameObject; i++)
		{
			if (s_dataBeingRenderedByRenderThread->gameObjectArray[i])
			{
				s_dataBeingRenderedByRenderThread->gameObjectArray[i] = nullptr;
			}
		}

		s_dataBeingRenderedByRenderThread->numOfGameObject = 0;
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;

	// Initialize the platform-specific context
	if (!(result = sContext::g_context.Initialize(i_initializationParameters)))
	{
		EAE6320_ASSERTF(false, "Can't initialize Graphics without context");
		return result;
	}
	// Initialize the platform-independent graphics objects
	{
		if (result = s_constantBuffer_frame.Initialize())
		{
			// There is only a single frame constant buffer that is reused
			// and so it can be bound at initialization time and never unbound
			s_constantBuffer_frame.Bind(
				// In our class both vertex and fragment shaders use per-frame constant data
				static_cast<uint_fast8_t>(eShaderType::Vertex) | static_cast<uint_fast8_t>(eShaderType::Fragment));
		}
		else
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without frame constant buffer");
			return result;
		}

		if (result = s_constantBuffer_drawCall.Initialize())
		{
			s_constantBuffer_drawCall.Bind(static_cast<uint_fast8_t>(eShaderType::Vertex) | static_cast<uint_fast8_t>(eShaderType::Fragment));
		}
		else
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without drawCall constant buffer");
			return result;
		}
	}
	// Initialize the events
	{
		if (!(result = s_whenAllDataHasBeenSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without event for when data has been submitted from the application thread");
			return result;
		}
		if (!(result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled,
			Concurrency::EventState::Signaled)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without event for when data can be submitted from the application thread");
			return result;
		}
	}
	// Initialize the views
	{
		if (!(result = s_view.InitializeViews(i_initializationParameters)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the views");
			return result;
		}
	}

	return result;
}

eae6320::cResult eae6320::Graphics::CleanUp()
{
	auto result = Results::Success;

	s_view.CleanUp();
	
	if (s_dataBeingSubmittedByApplicationThread)
	{
		for (size_t i = 0; i < s_dataBeingSubmittedByApplicationThread->numOfGameObject; i++)
		{
			if (s_dataBeingSubmittedByApplicationThread->gameObjectArray[i])
			{
				if (s_dataBeingSubmittedByApplicationThread->gameObjectArray[i]->m_Mesh)
				{
					s_dataBeingSubmittedByApplicationThread->gameObjectArray[i]->m_Mesh->DecrementReferenceCount();
					s_dataBeingSubmittedByApplicationThread->gameObjectArray[i]->m_Mesh = nullptr;
				}
			}
			if (s_dataBeingSubmittedByApplicationThread->gameObjectArray[i])
			{
				if (s_dataBeingSubmittedByApplicationThread->gameObjectArray[i]->m_Effect)
				{
					s_dataBeingSubmittedByApplicationThread->gameObjectArray[i]->m_Effect->DecrementReferenceCount();
					s_dataBeingSubmittedByApplicationThread->gameObjectArray[i]->m_Effect = nullptr;
				}
			}
		}
	}

	if (s_dataBeingRenderedByRenderThread)
	{
		for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->numOfGameObject; i++)
		{
			if (s_dataBeingRenderedByRenderThread->gameObjectArray[i])
			{
				if (s_dataBeingRenderedByRenderThread->gameObjectArray[i]->m_Mesh)
				{
					s_dataBeingRenderedByRenderThread->gameObjectArray[i]->m_Mesh->DecrementReferenceCount();
					s_dataBeingRenderedByRenderThread->gameObjectArray[i]->m_Mesh = nullptr;
				}
			}
			if (s_dataBeingRenderedByRenderThread->gameObjectArray[i])
			{
				if (s_dataBeingRenderedByRenderThread->gameObjectArray[i]->m_Effect)
				{
					s_dataBeingRenderedByRenderThread->gameObjectArray[i]->m_Effect->DecrementReferenceCount();
					s_dataBeingRenderedByRenderThread->gameObjectArray[i]->m_Effect = nullptr;
				}
			}
		}
	}

	{
		const auto result_constantBuffer_frame = s_constantBuffer_frame.CleanUp();
		if (!result_constantBuffer_frame)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = result_constantBuffer_frame;
			}
		}

		const auto result_constantBuffer_drawCall = s_constantBuffer_drawCall.CleanUp();
		if (!result_constantBuffer_drawCall)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = result_constantBuffer_drawCall;
			}
		}
	}

	{
		const auto result_context = sContext::g_context.CleanUp();
		if (!result_context)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = result_context;
			}
		}
	}

	return result;
}