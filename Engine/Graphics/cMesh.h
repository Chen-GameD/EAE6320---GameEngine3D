/*
	This file declares the external interface for the mesh system
*/

#ifndef EAE6320_GRAPHICS_CMESH_H
#define EAE6320_GRAPHICS_CMESH_H

// Includes
//=========

#include "VertexFormats.h"
//#include <Engine/Assets/ReferenceCountedAssets.h>

#ifdef EAE6320_PLATFORM_GL
	#include "OpenGL/Includes.h"
	#include <Engine/Assets/ReferenceCountedAssets.h>
#elif EAE6320_PLATFORM_D3D
	#include "cVertexFormat.h"
	#include "sContext.h"
	#include "cConstantBuffer.h"
#endif


namespace eae6320
{
	namespace Graphics
	{
		class cMesh
		{
			// Interface
			//==========

		public:

			static cResult CreateMeshWithLuaFile(cMesh*& o_mesh, const char* const i_path);

			static cResult CreateMesh(cMesh*& o_mesh, eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[], uint16_t i_indexArray[], int vertexCount, int indexCount);

			//eae6320::cResult InitializeGeometry(eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[], uint16_t i_indexArray[], int vertexCount, int indexCount);

			//eae6320::cResult CleanUp();

			void DrawGeometry();

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cMesh)

		private:

			eae6320::cResult InitializeGeometry(eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[], uint16_t i_indexArray[], int vertexCount, int indexCount);

			eae6320::cResult CleanUp();

		private:
			// indexCountToRender size
			unsigned int m_indexCountToRender = 0;
			//Reference Counting
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()

#ifdef EAE6320_PLATFORM_GL
			// Geometry Data for OpenGL
			//--------------

			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;
			//An indext array encapsulates the index data as well as the index input layout
			GLuint m_indexBufferId = 0;
#elif EAE6320_PLATFORM_D3D
			// Geometry Data for D3D
			//--------------

			eae6320::Graphics::cVertexFormat* m_vertexFormat = nullptr;

			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* m_vertexBuffer = nullptr;
			// An index buffer holds the data for each triangle
			ID3D11Buffer* m_indexBuffer = nullptr;
#endif

			//Constructor / Destructor
			cMesh() = default;
			~cMesh();
		};
	}
}

#endif	// EAE6320_GRAPHICS_CMESH_H
