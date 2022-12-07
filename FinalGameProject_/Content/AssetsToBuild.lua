--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },

		{ path = "Shaders/Vertex/vertexInputLayout_mesh.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/myShader_1.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/myShader_2.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/myShader_3.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/myShader_4.shader", arguments = { "fragment" } },
	},
	meshes =
	{
		{ path = "Meshes/Mesh_1.lua"},
		{ path = "Meshes/Mesh_2.lua"},
		{ path = "Meshes/Mesh_3.lua"},
	},
	audios =
	{
		{ path = "Audios/Mixdown.mp3"},
		{ path = "Audios/Mixdown.wav"},
		{ path = "Audios/Test.mp3"},
	},
	stories =
	{
		"Stories/test_story.txt",
		"Stories/MyStory.txt",
	},
}
