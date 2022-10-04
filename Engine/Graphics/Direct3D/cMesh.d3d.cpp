// Includes
//=========

#include "Includes.h"
#include "../cMesh.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>

#include <list>

// Implementation
//===============

// Initialize / Clean Up
//----------------------

uint16_t* ConvertIndexArrayToD3DVersion(uint16_t i_indexArray[], int indexCount)
{
	uint16_t* retIndexArray = i_indexArray;
	for (int i = 0; i < indexCount;)
	{
		int j = i;
		retIndexArray[j] = i_indexArray[j];
		int temp = i_indexArray[j + 1];
		retIndexArray[j + 1] = i_indexArray[j + 2];
		retIndexArray[j + 2] = temp;
		i += 3;
	}

	return retIndexArray;
}

eae6320::cResult eae6320::Graphics::cMesh::InitializeGeometry(eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[], uint16_t i_indexArray[], int vertexCount, int indexCount)
{
	// how much memory the member variables take
	Logging::OutputMessage("m_vertexFormat takes : %d:", sizeof(m_vertexFormat));
	Logging::OutputMessage("m_vertexBuffer takes : %d:", sizeof(m_vertexBuffer));
	Logging::OutputMessage("m_indexBuffer takes : %d:", sizeof(m_indexBuffer));
	Logging::OutputMessage("m_indexCountToRender takes : %d:", sizeof(m_indexCountToRender));
	Logging::OutputMessage("m_refer takes : %d:", sizeof(m_referenceCount));

	auto result = eae6320::Results::Success;

	auto* const direct3dDevice = eae6320::Graphics::sContext::g_context.direct3dDevice;
	EAE6320_ASSERT(direct3dDevice);

	// Vertex Format
	{
		if (!(result = eae6320::Graphics::cVertexFormat::Load(eae6320::Graphics::eVertexType::Mesh, m_vertexFormat,
			"data/Shaders/Vertex/vertexInputLayout_mesh.shader")))
		{
			EAE6320_ASSERTF(false, "Can't initialize geometry without vertex format");
			return result;
		}
	}
	// Vertex Buffer
	{
		auto dataBufferSize = sizeof(i_vertexData[0]) * vertexCount;
		EAE6320_ASSERT(dataBufferSize <= std::numeric_limits<decltype(D3D11_BUFFER_DESC::ByteWidth)>::max());
		auto dataBufferDescription = [dataBufferSize]
		{
			D3D11_BUFFER_DESC bufferDescription{};

			bufferDescription.ByteWidth = static_cast<unsigned int>(dataBufferSize);
			bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
			bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
			bufferDescription.MiscFlags = 0;
			bufferDescription.StructureByteStride = 0;	// Not used

			return bufferDescription;
		}();

		const auto dataInitialData = [i_vertexData]
		{
			D3D11_SUBRESOURCE_DATA initialData{};

			initialData.pSysMem = i_vertexData;
			// (The other data members are ignored for non-texture buffers)

			return initialData;
		}();

		const auto dataResult_create = direct3dDevice->CreateBuffer(&dataBufferDescription, &dataInitialData, &m_vertexBuffer);
		if (FAILED(dataResult_create))
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, "3D object vertex buffer creation failed (HRESULT %#010x)", dataResult_create);
			eae6320::Logging::OutputError("Direct3D failed to create a 3D object vertex buffer (HRESULT %#010x)", dataResult_create);
			return result;
		}

		// Index Buffer
		uint16_t* indexArray = ConvertIndexArrayToD3DVersion(i_indexArray, indexCount);

		auto indexBufferSize = sizeof(i_indexArray[0]) * indexCount;
		EAE6320_ASSERT(indexBufferSize <= std::numeric_limits<decltype(D3D11_BUFFER_DESC::ByteWidth)>::max());
		m_indexCountToRender = indexCount;
		auto indexBufferDescription = [indexBufferSize]
		{
			D3D11_BUFFER_DESC bufferDescription{};

			bufferDescription.ByteWidth = static_cast<unsigned int>(indexBufferSize);
			bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
			bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
			bufferDescription.MiscFlags = 0;
			bufferDescription.StructureByteStride = 0;	// Not used

			return bufferDescription;
		}();

		const auto initialData = [indexArray]
		{
			D3D11_SUBRESOURCE_DATA initialData{};

			initialData.pSysMem = indexArray;
			// (The other data members are ignored for non-texture buffers)

			return initialData;
		}();

		const auto indexResult_create = direct3dDevice->CreateBuffer(&indexBufferDescription, &initialData, &m_indexBuffer);
		if (FAILED(indexResult_create))
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, "3D object index buffer creation failed (HRESULT %#010x)", indexResult_create);
			eae6320::Logging::OutputError("Direct3D failed to create a 3D object index buffer (HRESULT %#010x)", indexResult_create);
			return result;
		}

	}

	return result;
}

eae6320::cResult eae6320::Graphics::cMesh::CleanUp()
{
	auto result = Results::Success;

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
	if (m_vertexFormat)
	{
		m_vertexFormat->DecrementReferenceCount();
		m_vertexFormat = nullptr;
	}
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}
	return result;
}

// Draw
//----------------------

void eae6320::Graphics::cMesh::DrawGeometry()
{
	auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
	EAE6320_ASSERT(direct3dImmediateContext);

	// Bind a specific vertex buffer to the device as a data source
	{
		EAE6320_ASSERT(m_vertexBuffer != nullptr);
		constexpr unsigned int startingSlot = 0;
		constexpr unsigned int vertexBufferCount = 1;
		// The "stride" defines how large a single vertex is in the stream of data
		constexpr unsigned int bufferStride = sizeof(VertexFormats::sVertex_mesh);
		// It's possible to start streaming data in the middle of a vertex buffer
		constexpr unsigned int bufferOffset = 0;
		direct3dImmediateContext->IASetVertexBuffers(startingSlot, vertexBufferCount, &m_vertexBuffer, &bufferStride, &bufferOffset);
	}

	// Bind a specific index buffer to the devive as a index source
	{
		EAE6320_ASSERT(m_indexBuffer);
		constexpr DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT;
		// The indices start at the beginning of the buffer
		constexpr unsigned int offset = 0;
		direct3dImmediateContext->IASetIndexBuffer(m_indexBuffer, indexFormat, offset);
	}

	// Specify what kind of data the vertex buffer holds
	{
		// Bind the vertex format (which defines how to interpret a single vertex)
		{
			EAE6320_ASSERT(m_vertexFormat != nullptr);
			m_vertexFormat->Bind();
		}
		// Set the topology (which defines how to interpret multiple vertices as a single "primitive";
		// the vertex buffer was defined as a triangle list
		// (meaning that every primitive is a triangle and will be defined by three vertices)
		direct3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	// Render triangles from the currently-bound vertex buffer
	//{
	//	// As of this comment only a single triangle is drawn
	//	// (you will have to update this code in future assignments!)
	//	constexpr unsigned int triangleCount = 2;
	//	constexpr unsigned int vertexCountPerTriangle = 3;
	//	constexpr auto vertexCountToRender = triangleCount * vertexCountPerTriangle;
	//	// It's possible to start rendering primitives in the middle of the stream
	//	constexpr unsigned int indexOfFirstVertexToRender = 0;
	//	direct3dImmediateContext->Draw(vertexCountToRender, indexOfFirstVertexToRender);
	//}
	 
	// It's possible to start rendering primitives in the middle of the stream
	{
		constexpr unsigned int indexOfFirstIndexToUse = 0;
		constexpr unsigned int offsetToAddToEachIndex = 0;
		direct3dImmediateContext->DrawIndexed(static_cast<unsigned int>(m_indexCountToRender), indexOfFirstIndexToUse, offsetToAddToEachIndex);
	}
}