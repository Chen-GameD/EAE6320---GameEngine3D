// Includes
//=========

#include "cMesh.h"

#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Logging/Logging.h>
#include <External/Lua/Includes.h>
#include <new>

// Vertex parse
//========================

eae6320::cResult LoadTableValues_vertex_values_sigle_line(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sVertex_mesh& i_vertexData)
{
	auto result = eae6320::Results::Success;

	// sign x:
	{
		lua_pushinteger(&io_luaState, 1);
		lua_gettable(&io_luaState, -2);
		eae6320::cScopeGuard scopeGuard_pop_x([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});
		i_vertexData.x = (float)lua_tonumber(&io_luaState, -1);
	}

	// sign y:
	{
		lua_pushinteger(&io_luaState, 2);
		lua_gettable(&io_luaState, -2);
		eae6320::cScopeGuard scopeGuard_pop_y([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});
		i_vertexData.y = (float)lua_tonumber(&io_luaState, -1);
	}

	// sign z:
	{
		lua_pushinteger(&io_luaState, 3);
		lua_gettable(&io_luaState, -2);
		eae6320::cScopeGuard scopeGuard_pop_z([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});
		i_vertexData.z = (float)lua_tonumber(&io_luaState, -1);
	}

	return result;
}

eae6320::cResult LoadTableValues_vertex_values(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sVertex_mesh*& i_vertexData, int& vertexCount)
{
	auto result = eae6320::Results::Success;

	vertexCount = (int)luaL_len(&io_luaState, -1);
	i_vertexData = (struct eae6320::Graphics::VertexFormats::sVertex_mesh*)malloc(sizeof(eae6320::Graphics::VertexFormats::sVertex_mesh) * vertexCount);
	for (int i = 1; i <= vertexCount; ++i)
	{
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);
		eae6320::cScopeGuard scopeGuard_popTexturePath([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});

		if (lua_istable(&io_luaState, -1))
		{
			if (!(result = LoadTableValues_vertex_values_sigle_line(io_luaState, i_vertexData[i - 1])))
			{
				return result;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			return result;
		}
	}

	return result;
}

eae6320::cResult LoadTableValues_index_values(lua_State& io_luaState, uint16_t*& i_indexArray, int& indexCount)
{
	auto result = eae6320::Results::Success;

	indexCount = (int)luaL_len(&io_luaState, -1);
	i_indexArray = (uint16_t*)malloc(sizeof(uint16_t) * indexCount);
	for (int i = 1; i <= indexCount; ++i)
	{
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);
		eae6320::cScopeGuard scopeGuard_popTexturePath([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});

		i_indexArray[i - 1] = (int)lua_tonumber(&io_luaState, -1);
	}

	return result;
}

eae6320::cResult LoadTableValues_vertex(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sVertex_mesh*& i_vertexData, int& vertexCount)
{
	auto result = eae6320::Results::Success;

	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the "vertexData" table will be at -1:
	constexpr auto* const key = "vertexData";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	eae6320::cScopeGuard scopeGuard_popParameters([&io_luaState]
		{
			lua_pop(&io_luaState, 1);
		});
	if (lua_istable(&io_luaState, -1))
	{
		if (!(result = LoadTableValues_vertex_values(io_luaState, i_vertexData, vertexCount)))
		{
			return result;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		EAE6320_ASSERTF(false, "Invalid File!");
		return result;
	}

	return result;
}

eae6320::cResult LoadTableValues_index(lua_State& io_luaState, uint16_t*& i_indexArray, int& indexCount)
{
	auto result = eae6320::Results::Success;

	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the "indexArray" table will be at -1:
	constexpr auto* const key = "indexArray";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	eae6320::cScopeGuard scopeGuard_popParameters([&io_luaState]
		{
			lua_pop(&io_luaState, 1);
		});
	if (lua_istable(&io_luaState, -1))
	{
		if (!(result = LoadTableValues_index_values(io_luaState, i_indexArray, indexCount)))
		{
			return result;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		EAE6320_ASSERTF(false, "Invalid File!");
		return result;
	}

	return result;
}

eae6320::cResult LoadTableValues(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sVertex_mesh*& i_vertexData, uint16_t*& i_indexArray, int& vertexCount, int& indexCount)
{
	auto result = eae6320::Results::Success;

	if (!(result = LoadTableValues_vertex(io_luaState, i_vertexData, vertexCount)))
	{
		return result;
	}
	if (!(result = LoadTableValues_index(io_luaState, i_indexArray, indexCount)))
	{
		return result;
	}

	return result;
}

eae6320::cResult LoadAsset(const char* const i_path, eae6320::Graphics::VertexFormats::sVertex_mesh*& i_vertexData, uint16_t*& i_indexArray, int& vertexCount, int& indexCount)
{
	auto result = eae6320::Results::Success;

	// Create a new Lua state
	lua_State* luaState = nullptr;
	eae6320::cScopeGuard scopeGuard_onExit([&luaState]
		{
			if (luaState)
			{
				// If I haven't made any mistakes
				// there shouldn't be anything on the stack
				// regardless of any errors
				EAE6320_ASSERT(lua_gettop(luaState) == 0);

				lua_close(luaState);
				luaState = nullptr;
			}
		});
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			result = eae6320::Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Out of memory!");
			return result;
		}
	}

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	const auto stackTopBeforeLoad = lua_gettop(luaState);
	{
		const auto luaResult = luaL_loadfile(luaState, i_path);
		if (luaResult != LUA_OK)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, "Load the asset file fail!");
			// Pop the error message
			lua_pop(luaState, 1);
			return result;
		}
	}
	// Execute the "chunk", which should load the asset
	// into a table at the top of the stack
	{
		constexpr int argumentCount = 0;
		constexpr int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		constexpr int noMessageHandler = 0;
		const auto luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
		if (luaResult == LUA_OK)
		{
			// A well-behaved asset file will only return a single value
			const auto returnedValueCount = lua_gettop(luaState) - stackTopBeforeLoad;
			if (returnedValueCount == 1)
			{
				// A correct asset file _must_ return a table
				if (!lua_istable(luaState, -1))
				{
					result = eae6320::Results::InvalidFile;
					EAE6320_ASSERTF(false, "The file return value is not a table!");
					// Pop the returned non-table value
					lua_pop(luaState, 1);
					return result;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				EAE6320_ASSERTF(false, "Invalid File!");
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
				return result;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			EAE6320_ASSERTF(false, "Invalid File!");
			// Pop the error message
			lua_pop(luaState, 1);
			return result;
		}
	}

	// If this code is reached the asset file was loaded successfully,
	// and its table is now at index -1
	eae6320::cScopeGuard scopeGuard_popAssetTable([luaState]
		{
			lua_pop(luaState, 1);
		});
	result = LoadTableValues(*luaState, i_vertexData, i_indexArray, vertexCount, indexCount);

	return result;
}

eae6320::cResult ReadNestedTableValues(const char* const i_path, eae6320::Graphics::VertexFormats::sVertex_mesh*& i_vertexData, uint16_t*& i_indexArray, int& vertexCount, int& indexCount)
{
	auto result = eae6320::Results::Success;

	if (!(result = LoadAsset(i_path, i_vertexData, i_indexArray, vertexCount, indexCount)))
	{
		return result;
	}

	return result;
}

// Create mesh
//====================

eae6320::cResult eae6320::Graphics::cMesh::CreateMeshWithLuaFile(cMesh*& o_mesh, const char* const i_path)
{
	auto result = Results::Success;

	eae6320::Graphics::VertexFormats::sVertex_mesh* vertexData = nullptr;
	uint16_t* indexArray = nullptr;
	int vertexCount = 0;
	int indexCount = 0;

	// Parse lua file
	ReadNestedTableValues(i_path, vertexData, indexArray, vertexCount, indexCount);

	if (!(result = CreateMesh(o_mesh, vertexData, indexArray, vertexCount, indexCount)))
	{
		EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
	}

	free(vertexData);
	free(indexArray);
	return result;
}

eae6320::cResult eae6320::Graphics::cMesh::CreateMesh(cMesh*& o_mesh, eae6320::Graphics::VertexFormats::sVertex_mesh i_vertexData[], uint16_t i_indexArray[], int vertexCount, int indexCount)
{
	auto result = Results::Success;

	cMesh* newMesh = nullptr;
	cScopeGuard scopeGuard([&o_mesh, &result, &newMesh]
		{
			if (result)
			{
				EAE6320_ASSERT(newMesh != nullptr);
				o_mesh = newMesh;
			}
			else
			{
				if (newMesh)
				{
					newMesh->DecrementReferenceCount();
					newMesh = nullptr;
				}
				o_mesh = nullptr;
			}
		});

	// Allocate a new mesh
	{
		newMesh = new (std::nothrow) cMesh();
		if (!newMesh)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the mesh");
			Logging::OutputError("Failed to allocate memory for the mesh");
			return result;
		}
	}

	// Initialize the geometry
	if (!(result = newMesh->InitializeGeometry(i_vertexData, i_indexArray, vertexCount, indexCount)))
	{
		EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
		return result;
	}

	return result;
}

eae6320::Graphics::cMesh::~cMesh()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}