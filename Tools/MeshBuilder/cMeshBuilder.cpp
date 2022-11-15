// Includes
//=========

#include "cMeshBuilder.h"

#include <Tools/AssetBuildLibrary/Functions.h>
#include <External/Lua/Includes.h>
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Asserts/Asserts.h>
#include <Tools/AssetBuildLibrary/Functions.h>

#include <fstream> 


// Inherited Implementation
//=========================

eae6320::cResult LoadTableValues_vertex_values_sigle_line(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sVertex_mesh& i_vertexData, const char* m_path_source)
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

eae6320::cResult LoadTableValues_vertex_values(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sVertex_mesh*& i_vertexData, uint16_t& vertexCount, const char* m_path_source)
{
	auto result = eae6320::Results::Success;

	const auto temp_vertexCount = luaL_len(&io_luaState, -1);
	vertexCount = (uint16_t)temp_vertexCount;
	if (vertexCount != temp_vertexCount)
	{
		eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source,
			"The number of vertexData is too large!");
		//EAE6320_ASSERTF(false, "The number of vertexData is too large!");
		return eae6320::Results::Failure;
	}

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
			if (!(result = LoadTableValues_vertex_values_sigle_line(io_luaState, i_vertexData[i - 1], m_path_source)))
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

eae6320::cResult LoadTableValues_index_values(lua_State& io_luaState, uint16_t*& i_indexArray, uint16_t& indexCount, const char* m_path_source)
{
	auto result = eae6320::Results::Success;

	const auto temp_indexCount = luaL_len(&io_luaState, -1);
	indexCount = (uint16_t)temp_indexCount;
	if (indexCount != temp_indexCount)
	{
		eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source,
			"The number of indexArray is too large!");
		//EAE6320_ASSERTF(false, "The number of indexArray is too large!");
		return eae6320::Results::Failure;
	}

	i_indexArray = (uint16_t*)malloc(sizeof(uint16_t) * indexCount);
	for (int i = 1; i <= indexCount; ++i)
	{
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);
		eae6320::cScopeGuard scopeGuard_popTexturePath([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});

		i_indexArray[i - 1] = (uint16_t)lua_tonumber(&io_luaState, -1);
	}

	return result;
}

eae6320::cResult LoadTableValues_vertex(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sVertex_mesh*& i_vertexData, uint16_t& vertexCount, const char* m_path_source)
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
		if (!(result = LoadTableValues_vertex_values(io_luaState, i_vertexData, vertexCount, m_path_source)))
		{
			return result;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source,
			"Invalid File!");
		//EAE6320_ASSERTF(false, "Invalid File!");
		return result;
	}

	return result;
}

eae6320::cResult LoadTableValues_index(lua_State& io_luaState, uint16_t*& i_indexArray, uint16_t& indexCount, const char* m_path_source)
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
		if (!(result = LoadTableValues_index_values(io_luaState, i_indexArray, indexCount, m_path_source)))
		{
			return result;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source,
			"Invalid File!");
		//EAE6320_ASSERTF(false, "Invalid File!");
		return result;
	}

	return result;
}

eae6320::cResult LoadTableValues(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sVertex_mesh*& i_vertexData, uint16_t*& i_indexArray, uint16_t& vertexCount, uint16_t& indexCount, const char* m_path_source)
{
	auto result = eae6320::Results::Success;

	if (!(result = LoadTableValues_vertex(io_luaState, i_vertexData, vertexCount, m_path_source)))
	{
		return result;
	}
	if (!(result = LoadTableValues_index(io_luaState, i_indexArray, indexCount, m_path_source)))
	{
		return result;
	}

	return result;
}

eae6320::cResult LoadAsset(const char* const i_path, eae6320::Graphics::VertexFormats::sVertex_mesh*& i_vertexData, uint16_t*& i_indexArray, uint16_t& vertexCount, uint16_t& indexCount)
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
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,
				"Out of memory!");
			//EAE6320_ASSERTF(false, "Out of memory!");
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
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,
				"Load the asset file fail!");
			//EAE6320_ASSERTF(false, "Load the asset file fail!");
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
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,
						"The file return value is not a table!");
					//EAE6320_ASSERTF(false, "The file return value is not a table!");
					// Pop the returned non-table value
					lua_pop(luaState, 1);
					return result;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,
					"Invalid File!");
				//EAE6320_ASSERTF(false, "Invalid File!");
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
				return result;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,
				"Invalid File!");
			//EAE6320_ASSERTF(false, "Invalid File!");
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
	result = LoadTableValues(*luaState, i_vertexData, i_indexArray, vertexCount, indexCount, i_path);

	return result;
}

eae6320::cResult ReadNestedTableValues(const char* const i_path, eae6320::Graphics::VertexFormats::sVertex_mesh*& i_vertexData, uint16_t*& i_indexArray, uint16_t& vertexCount, uint16_t& indexCount)
{
	auto result = eae6320::Results::Success;

	if (!(result = LoadAsset(i_path, i_vertexData, i_indexArray, vertexCount, indexCount)))
	{
		return result;
	}

	return result;
}

// Build
//------

eae6320::cResult eae6320::Assets::cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{
	eae6320::cResult result = Results::Success;

	eae6320::Graphics::VertexFormats::sVertex_mesh* vertexData = nullptr;
	uint16_t* indexArray = nullptr;
	uint16_t vertexCount = 0;
	uint16_t indexCount = 0;

	// Parse lua file
	ReadNestedTableValues(m_path_source, vertexData, indexArray, vertexCount, indexCount);

	std::ofstream outfile(m_path_target, std::ofstream::binary);

	const char* buffer = nullptr;

	// Write vertexCount to binary file
	buffer = reinterpret_cast<const char*>(&vertexCount);
	outfile.write(buffer, sizeof(vertexCount));

	// Write vertexData to binary file
	buffer = reinterpret_cast<const char*>(vertexData);
	outfile.write(buffer, sizeof(eae6320::Graphics::VertexFormats::sVertex_mesh) * vertexCount);

	// Write indexCount to binary file
	buffer = reinterpret_cast<const char*>(&indexCount);
	outfile.write(buffer, sizeof(indexCount));

	// Write indexArray to binary file
	buffer = reinterpret_cast<const char*>(indexArray);
	outfile.write(buffer, sizeof(uint16_t) * indexCount);

	/*if (!(result = Platform::CopyFileW(m_path_source, m_path_target, false, true)))
	{
		eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Fail to copy mesh file!");
	}*/

	outfile.close();

	return result;
}
