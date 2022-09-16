/*
	This file declares the external interface for the mesh system
*/

#ifndef EAE6320_GRAPHICS_CMESH_H
#define EAE6320_GRAPHICS_CMESH_H

// Includes
//=========

#include "VertexFormats.h"

#ifdef EAE6320_PLATFORM_GL
	#include "OpenGL/Includes.h"
#elif EAE6320_PLATFORM_D3D
	#include "cVertexFormat.h"
	#include "sContext.h"
	#include "cConstantBuffer.h"
#endif

#include <vector>


namespace eae6320
{
	namespace Graphics
	{
		class cMesh
		{
			// Interface
			//==========

		public:

			//Temporarily default triangleCount = 2, vertexCountPerTriangle = 3, so vertexCount = triangleCount * vertexCountPerTriangle = 6;
			//This will change in future...
#ifdef EAE6320_PLATFORM_GL
			eae6320::cResult InitializeGeometry(eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[6]);
#elif EAE6320_PLATFORM_D3D
			eae6320::cResult InitializeGeometry(eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[], uint16_t i_indexArray[]);
#endif

			eae6320::cResult CleanUp();

			void DrawGeometry();

		private:
			// indexCountToRender size
			unsigned int m_indexCountToRender = 0;

#ifdef EAE6320_PLATFORM_GL
			// Geometry Data for OpenGL
			//--------------

			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;
			//An indext array encapsulates the index data as well as the index input layout
			Gluint m_indexBufferId = 0;
#elif EAE6320_PLATFORM_D3D
			// Geometry Data for D3D
			//--------------

			eae6320::Graphics::cVertexFormat* m_vertexFormat = nullptr;

			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* m_vertexBuffer = nullptr;
			// An index buffer holds the data for each triangle
			ID3D11Buffer* m_indexBuffer = nullptr;
#endif
		};
	}
}

#endif	// EAE6320_GRAPHICS_CMESH_H
