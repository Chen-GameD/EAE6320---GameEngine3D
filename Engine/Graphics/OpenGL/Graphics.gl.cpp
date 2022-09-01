// Includes
//=========

#include "../Graphics.h"

#include "Includes.h"
#include "../cConstantBuffer.h"
#include "../ConstantBufferFormats.h"
#include "../cRenderState.h"
#include "../cShader.h"
#include "../sContext.h"
#include "../VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <new>
#include <utility>

// Static Data
//============

namespace
{
	// Constant buffer object
	eae6320::Graphics::cConstantBuffer s_constantBuffer_frame( eae6320::Graphics::ConstantBufferTypes::Frame );

	// Submission Data
	//----------------

	// This struct's data is populated at submission time;
	// it must cache whatever is necessary in order to render a frame
	struct sDataRequiredToRenderAFrame
	{
		eae6320::Graphics::ConstantBufferFormats::sFrame constantData_frame;
	};
	// In our class there will be two copies of the data required to render a frame:
	//	* One of them will be in the process of being populated by the data currently being submitted by the application loop thread
	//	* One of them will be fully populated and in the process of being rendered from in the render thread
	// (In other words, one is being produced while the other is being consumed)
	sDataRequiredToRenderAFrame s_dataRequiredToRenderAFrame[2];
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

	// Geometry Data
	//--------------

	// A vertex buffer holds the data for each vertex
	GLuint s_vertexBufferId = 0;
	// A vertex array encapsulates the vertex data as well as the vertex input layout
	GLuint s_vertexArrayId = 0;

	// Shading Data
	//-------------

	eae6320::Graphics::cShader* s_vertexShader = nullptr;
	eae6320::Graphics::cShader* s_fragmentShader = nullptr;
	GLuint s_programId = 0;

	eae6320::Graphics::cRenderState s_renderState;
}

// Helper Declarations
//====================

namespace
{
	eae6320::cResult InitializeGeometry();
	eae6320::cResult InitializeShadingData();
}

// Interface
//==========

// Submission
//-----------

void eae6320::Graphics::SubmitElapsedTime( const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime )
{
	EAE6320_ASSERT( s_dataBeingSubmittedByApplicationThread );
	auto& constantData_frame = s_dataBeingSubmittedByApplicationThread->constantData_frame;
	constantData_frame.g_elapsedSecondCount_systemTime = i_elapsedSecondCount_systemTime;
	constantData_frame.g_elapsedSecondCount_simulationTime = i_elapsedSecondCount_simulationTime;
}

eae6320::cResult eae6320::Graphics::WaitUntilDataForANewFrameCanBeSubmitted( const unsigned int i_timeToWait_inMilliseconds )
{
	return Concurrency::WaitForEvent( s_whenDataForANewFrameCanBeSubmittedFromApplicationThread, i_timeToWait_inMilliseconds );
}

eae6320::cResult eae6320::Graphics::SignalThatAllDataForAFrameHasBeenSubmitted()
{
	return s_whenAllDataHasBeenSubmittedFromApplicationThread.Signal();
}

// Render
//-------

void eae6320::Graphics::RenderFrame()
{
	// Wait for the application loop to submit data to be rendered
	{
		if ( Concurrency::WaitForEvent( s_whenAllDataHasBeenSubmittedFromApplicationThread ) )
		{
			// Switch the render data pointers so that
			// the data that the application just submitted becomes the data that will now be rendered
			std::swap( s_dataBeingSubmittedByApplicationThread, s_dataBeingRenderedByRenderThread );
			// Once the pointers have been swapped the application loop can submit new data
			if ( !s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Signal() )
			{
				EAE6320_ASSERTF( false, "Couldn't signal that new graphics data can be submitted" );
				Logging::OutputError( "Failed to signal that new render data can be submitted" );
				UserOutput::Print( "The renderer failed to signal to the application that new graphics data can be submitted."
					" The application is probably in a bad state and should be exited" );
				return;
			}
		}
		else
		{
			EAE6320_ASSERTF( false, "Waiting for the graphics data to be submitted failed" );
			Logging::OutputError( "Waiting for the application loop to submit data to be rendered failed" );
			UserOutput::Print( "The renderer failed to wait for the application to submit data to be rendered."
				" The application is probably in a bad state and should be exited" );
			return;
		}
	}

	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		// Black is usually used
		{
			glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		}
		{
			constexpr GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
			glClear( clearColor );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		}
	}
	// In addition to the color buffer there is also a hidden image called the "depth buffer"
	// which is used to make it less important which order draw calls are made.
	// It must also be "cleared" every frame just like the visible color buffer.
	{
		{
			glDepthMask( GL_TRUE );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
			constexpr GLclampd clearToFarDepth = 1.0;
			glClearDepth( clearToFarDepth );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		}
		{
			constexpr GLbitfield clearDepth = GL_DEPTH_BUFFER_BIT;
			glClear( clearDepth );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		}
	}

	EAE6320_ASSERT( s_dataBeingRenderedByRenderThread );

	// Update the frame constant buffer
	{
		// Copy the data from the system memory that the application owns to GPU memory
		auto& constantData_frame = s_dataBeingRenderedByRenderThread->constantData_frame;
		s_constantBuffer_frame.Update( &constantData_frame );
	}

	// Bind the shading data
	{
		{
			EAE6320_ASSERT( s_programId != 0 );
			glUseProgram( s_programId );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		}
		// Render state
		{
			s_renderState.Bind();
		}
	}
	// Draw the geometry
	{
		// Bind a specific vertex buffer to the device as a data source
		{
			EAE6320_ASSERT( s_vertexArrayId != 0 );
			glBindVertexArray( s_vertexArrayId );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		}
		// Render triangles from the currently-bound vertex buffer
		{
			// The mode defines how to interpret multiple vertices as a single "primitive";
			// a triangle list is defined
			// (meaning that every primitive is a triangle and will be defined by three vertices)
			constexpr GLenum mode = GL_TRIANGLES;
			// As of this comment only a single triangle is drawn
			// (you will have to update this code in future assignments!)
			constexpr unsigned int triangleCount = 1;
			constexpr unsigned int vertexCountPerTriangle = 3;
			constexpr auto vertexCountToRender = triangleCount * vertexCountPerTriangle;
			// It's possible to start rendering primitives in the middle of the stream
			constexpr unsigned int indexOfFirstVertexToRender = 0;
			glDrawArrays( mode, indexOfFirstVertexToRender, vertexCountToRender );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		}
	}

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (or "swapped" with the "front buffer", which is the image that is actually being displayed)
	{
		const auto deviceContext = sContext::g_context.deviceContext;
		EAE6320_ASSERT( deviceContext != NULL );
		const auto glResult = SwapBuffers( deviceContext );
		EAE6320_ASSERT( glResult != FALSE );
	}

	// After all of the data that was submitted for this frame has been used
	// you must make sure that it is all cleaned up and cleared out
	// so that the struct can be re-used (i.e. so that data for a new frame can be submitted to it)
	{
		// (At this point in the class there isn't anything that needs to be cleaned up)
		//dataRequiredToRenderFrame	// TODO
	}
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::Initialize( const sInitializationParameters& i_initializationParameters )
{
	auto result = Results::Success;

	// Initialize the platform-specific context
	if ( !( result = sContext::g_context.Initialize( i_initializationParameters ) ) )
	{
		EAE6320_ASSERTF( false, "Can't initialize Graphics without context" );
		return result;
	}
	// Initialize the platform-independent graphics objects
	{
		if ( result = s_constantBuffer_frame.Initialize() )
		{
			// There is only a single frame constant buffer that is reused
			// and so it can be bound at initialization time and never unbound
			s_constantBuffer_frame.Bind(
				// In our class both vertex and fragment shaders use per-frame constant data
				static_cast<uint_fast8_t>( eShaderType::Vertex ) | static_cast<uint_fast8_t>( eShaderType::Fragment ) );
		}
		else
		{
			EAE6320_ASSERTF( false, "Can't initialize Graphics without frame constant buffer" );
			return result;
		}
	}
	// Initialize the events
	{
		if ( !( result = s_whenAllDataHasBeenSubmittedFromApplicationThread.Initialize( Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled ) ) )
		{
			EAE6320_ASSERTF( false, "Can't initialize Graphics without event for when data has been submitted from the application thread" );
			return result;
		}
		if ( !( result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Initialize( Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled,
			Concurrency::EventState::Signaled ) ) )
		{
			EAE6320_ASSERTF( false, "Can't initialize Graphics without event for when data can be submitted from the application thread" );
			return result;
		}
	}
	// Initialize the shading data
	{
		if ( !( result = InitializeShadingData() ) )
		{
			EAE6320_ASSERTF( false, "Can't initialize Graphics without the shading data" );
			return result;
		}
	}
	// Initialize the geometry
	{
		if ( !( result = InitializeGeometry() ) )
		{
			EAE6320_ASSERTF( false, "Can't initialize Graphics without the geometry data" );
			return result;
		}
	}

	return result;
}

eae6320::cResult eae6320::Graphics::CleanUp()
{
	auto result = Results::Success;

	{
		if ( s_vertexArrayId != 0 )
		{
			// Make sure that the vertex array isn't bound
			{
				// Unbind the vertex array
				glBindVertexArray( 0 );
				const auto errorCode = glGetError();
				if ( errorCode != GL_NO_ERROR )
				{
					if ( result )
					{
						result = Results::Failure;
					}
					EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					Logging::OutputError( "OpenGL failed to unbind all vertex arrays before cleaning up geometry: %s",
						reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				}
			}
			constexpr GLsizei arrayCount = 1;
			glDeleteVertexArrays( arrayCount, &s_vertexArrayId );
			const auto errorCode = glGetError();
			if ( errorCode != GL_NO_ERROR )
			{
				if ( result )
				{
					result = Results::Failure;
				}
				EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				Logging::OutputError( "OpenGL failed to delete the vertex array: %s",
					reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			}
			s_vertexArrayId = 0;
		}
		if ( s_vertexBufferId != 0 )
		{
			constexpr GLsizei bufferCount = 1;
			glDeleteBuffers( bufferCount, &s_vertexBufferId );
			const auto errorCode = glGetError();
			if ( errorCode != GL_NO_ERROR )
			{
				if ( result )
				{
					result = Results::Failure;
				}
				EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				Logging::OutputError( "OpenGL failed to delete the vertex buffer: %s",
					reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			}
			s_vertexBufferId = 0;
		}
	}
	if ( s_programId != 0 )
	{
		glDeleteProgram( s_programId );
		const auto errorCode = glGetError();
		if ( errorCode != GL_NO_ERROR )
		{
			if ( result )
			{
				result = eae6320::Results::Failure;
			}
			EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			eae6320::Logging::OutputError( "OpenGL failed to delete the program: %s",
				reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
		}
		s_programId = 0;
	}
	if ( s_vertexShader )
	{
		s_vertexShader->DecrementReferenceCount();
		s_vertexShader = nullptr;
	}
	if ( s_fragmentShader )
	{
		s_fragmentShader->DecrementReferenceCount();
		s_fragmentShader = nullptr;
	}

	{
		const auto result_constantBuffer_frame = s_constantBuffer_frame.CleanUp();
		if ( !result_constantBuffer_frame )
		{
			EAE6320_ASSERT( false );
			if ( result )
			{
				result = result_constantBuffer_frame;
			}
		}
	}
	
	{
		const auto result_context = sContext::g_context.CleanUp();
		if ( !result_context )
		{
			EAE6320_ASSERT( false );
			if ( result )
			{
				result = result_context;
			}
		}
	}

	return result;
}

// Helper Definitions
//===================

namespace
{
	eae6320::cResult InitializeGeometry()
	{
		auto result = eae6320::Results::Success;

		// Create a vertex array object and make it active
		{
			constexpr GLsizei arrayCount = 1;
			glGenVertexArrays( arrayCount, &s_vertexArrayId );
			const auto errorCode = glGetError();
			if ( errorCode == GL_NO_ERROR )
			{
				glBindVertexArray( s_vertexArrayId );
				const auto errorCode = glGetError();
				if ( errorCode != GL_NO_ERROR )
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					eae6320::Logging::OutputError( "OpenGL failed to bind a new vertex array: %s",
						reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					return result;
				}
			}
			else
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				eae6320::Logging::OutputError( "OpenGL failed to get an unused vertex array ID: %s",
					reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				return result;
			}
		}
		// Create a vertex buffer object and make it active
		{
			constexpr GLsizei bufferCount = 1;
			glGenBuffers( bufferCount, &s_vertexBufferId );
			const auto errorCode = glGetError();
			if ( errorCode == GL_NO_ERROR )
			{
				glBindBuffer( GL_ARRAY_BUFFER, s_vertexBufferId );
				const auto errorCode = glGetError();
				if ( errorCode != GL_NO_ERROR )
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					eae6320::Logging::OutputError( "OpenGL failed to bind a new vertex buffer: %s",
						reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					return result;
				}
			}
			else
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				eae6320::Logging::OutputError( "OpenGL failed to get an unused vertex buffer ID: %s",
					reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				return result;
			}
		}
		// Assign the data to the buffer
		{
			constexpr unsigned int triangleCount = 1;
			constexpr unsigned int vertexCountPerTriangle = 3;
			const auto vertexCount = triangleCount * vertexCountPerTriangle;
			eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[vertexCount];
			{
				// OpenGL is right-handed

				vertexData[0].x = 0.0f;
				vertexData[0].y = 0.0f;
				vertexData[0].z = 0.0f;

				vertexData[1].x = 1.0f;
				vertexData[1].y = 0.0f;
				vertexData[1].z = 0.0f;

				vertexData[2].x = 1.0f;
				vertexData[2].y = 1.0f;
				vertexData[2].z = 0.0f;
			}
			constexpr auto bufferSize = sizeof( vertexData[0] ) * vertexCount;
			EAE6320_ASSERT( bufferSize <= std::numeric_limits<GLsizeiptr>::max() );
			glBufferData( GL_ARRAY_BUFFER, static_cast<GLsizeiptr>( bufferSize ), reinterpret_cast<GLvoid*>( vertexData ),
				// In our class we won't ever read from the buffer
				GL_STATIC_DRAW );
			const auto errorCode = glGetError();
			if ( errorCode != GL_NO_ERROR )
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				eae6320::Logging::OutputError( "OpenGL failed to allocate the vertex buffer: %s",
					reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				return result;
			}
		}
		// Initialize vertex format
		{
			// The "stride" defines how large a single vertex is in the stream of data
			// (or, said another way, how far apart each position element is)
			constexpr auto stride = static_cast<GLsizei>( sizeof( eae6320::Graphics::VertexFormats::sVertex_mesh ) );

			// Position (0)
			// 3 floats == 12 bytes
			// Offset = 0
			{
				constexpr GLuint vertexElementLocation = 0;
				constexpr GLint elementCount = 3;
				constexpr GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
				glVertexAttribPointer( vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
					reinterpret_cast<GLvoid*>( offsetof( eae6320::Graphics::VertexFormats::sVertex_mesh, x ) ) );
				const auto errorCode = glGetError();
				if ( errorCode == GL_NO_ERROR )
				{
					glEnableVertexAttribArray( vertexElementLocation );
					const GLenum errorCode = glGetError();
					if ( errorCode != GL_NO_ERROR )
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
						eae6320::Logging::OutputError( "OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
							vertexElementLocation, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
						return result;
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					eae6320::Logging::OutputError( "OpenGL failed to set the POSITION vertex attribute at location %u: %s",
						vertexElementLocation, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					return result;
				}
			}
		}

		return result;
	}

	eae6320::cResult InitializeShadingData()
	{
		auto result = eae6320::Results::Success;

		if ( !( result = eae6320::Graphics::cShader::Load( "data/Shaders/Vertex/standard.shader",
			s_vertexShader, eae6320::Graphics::eShaderType::Vertex ) ) )
		{
			EAE6320_ASSERTF( false, "Can't initialize shading data without vertex shader" );
			return result;
		}
		if ( !( result = eae6320::Graphics::cShader::Load( "data/Shaders/Fragment/standard.shader",
			s_fragmentShader, eae6320::Graphics::eShaderType::Fragment ) ) )
		{
			EAE6320_ASSERTF( false, "Can't initialize shading data without fragment shader" );
			return result;
		}
		{
			constexpr auto renderStateBits = []
			{
				uint8_t renderStateBits = 0;

				eae6320::Graphics::RenderStates::DisableAlphaTransparency( renderStateBits );
				eae6320::Graphics::RenderStates::DisableDepthTesting( renderStateBits );
				eae6320::Graphics::RenderStates::DisableDepthWriting( renderStateBits );
				eae6320::Graphics::RenderStates::DisableDrawingBothTriangleSides( renderStateBits );

				return renderStateBits;
			}();
			if ( !( result = s_renderState.Initialize( renderStateBits ) ) )
			{
				EAE6320_ASSERTF( false, "Can't initialize shading data without render state" );
				return result;
			}
		}

		// Create a program
		eae6320::cScopeGuard scopeGuard_program( [&result]
			{
				if ( !result )
				{
					if ( s_programId != 0 )
					{
						glDeleteProgram( s_programId );
						const auto errorCode = glGetError();
						if ( errorCode != GL_NO_ERROR )
						{
							EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
							eae6320::Logging::OutputError( "OpenGL failed to delete the program: %s",
								reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
						}
						s_programId = 0;
					}
				}
			} );
		{
			EAE6320_ASSERT( s_programId == 0 );
			s_programId = glCreateProgram();
			const auto errorCode = glGetError();
			if ( errorCode != GL_NO_ERROR )
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				eae6320::Logging::OutputError( "OpenGL failed to create a program: %s",
					reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				return result;
			}
			else if ( s_programId == 0 )
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERT( false );
				eae6320::Logging::OutputError( "OpenGL failed to create a program" );
				return result;
			}
		}
		// Attach the shaders to the program
		{
			// Vertex
			{
				EAE6320_ASSERT( ( s_vertexShader != nullptr ) && ( s_vertexShader->m_shaderId != 0 ) );
				glAttachShader( s_programId, s_vertexShader->m_shaderId );
				const auto errorCode = glGetError();
				if ( errorCode != GL_NO_ERROR )
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					eae6320::Logging::OutputError( "OpenGL failed to attach the vertex shader to the program: %s",
						reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					return result;
				}
			}
			// Fragment
			{
				EAE6320_ASSERT( ( s_fragmentShader != nullptr ) && ( s_fragmentShader->m_shaderId != 0 ) );
				glAttachShader( s_programId, s_fragmentShader->m_shaderId );
				const auto errorCode = glGetError();
				if ( errorCode != GL_NO_ERROR )
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					eae6320::Logging::OutputError( "OpenGL failed to attach the fragment shader to the program: %s",
						reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					return result;
				}
			}
		}
		// Link the program
		{
			glLinkProgram( s_programId );
			const auto errorCode = glGetError();
			if ( errorCode == GL_NO_ERROR )
			{
				// Get link info
				// (this won't be used unless linking fails
				// but it can be useful to look at when debugging)
				std::string linkInfo;
				{
					GLint infoSize;
					glGetProgramiv( s_programId, GL_INFO_LOG_LENGTH, &infoSize );
					const auto errorCode = glGetError();
					if ( errorCode == GL_NO_ERROR )
					{
						if ( infoSize > 0 )
						{
							auto* const info = new (std::nothrow) GLchar[infoSize];
							if ( info )
							{
								eae6320::cScopeGuard scopeGuard_info( [info]
									{
										delete [] info;
									} );
								constexpr GLsizei* const dontReturnLength = nullptr;
								glGetProgramInfoLog( s_programId, static_cast<GLsizei>( infoSize ), dontReturnLength, info );
								const auto errorCode = glGetError();
								if ( errorCode == GL_NO_ERROR )
								{
									linkInfo = info;
								}
								else
								{
									result = eae6320::Results::Failure;
									EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
									eae6320::Logging::OutputError( "OpenGL failed to get link info of the program: %s",
										reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
									return result;
								}
							}
							else
							{
								result = eae6320::Results::OutOfMemory;
								EAE6320_ASSERTF( false, "Couldn't allocate memory for the program link info" );
								eae6320::Logging::OutputError( "Failed to allocate memory for the program link info" );
								return result;
							}
						}
					}
					else
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
						eae6320::Logging::OutputError( "OpenGL failed to get the length of the program link info: %s",
							reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
						return result;
					}
				}
				// Check to see if there were link errors
				GLint didLinkingSucceed;
				{
					glGetProgramiv( s_programId, GL_LINK_STATUS, &didLinkingSucceed );
					const auto errorCode = glGetError();
					if ( errorCode == GL_NO_ERROR )
					{
						if ( didLinkingSucceed == GL_FALSE )
						{
							result = eae6320::Results::Failure;
							EAE6320_ASSERTF( false, linkInfo.c_str() );
							eae6320::Logging::OutputError( "The program failed to link: %s",
								linkInfo.c_str() );
							return result;
						}
					}
					else
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
						eae6320::Logging::OutputError( "OpenGL failed to find out if linking of the program succeeded: %s",
							reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
						return result;
					}
				}
			}
			else
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				eae6320::Logging::OutputError( "OpenGL failed to link the program: %s",
					reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				return result;
			}
		}

		return result;
	}
}
