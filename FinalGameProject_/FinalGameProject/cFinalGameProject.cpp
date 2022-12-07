// Includes
//=========

#include "cFinalGameProject.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Audio/AudioSystem.h>

#include <random>

// Static Data
//============

namespace
{
	//static camera
	eae6320::Camera m_camera;
}

// Inherited Implementation
//=========================

// Run
//----

eae6320::AudioSystem::cAudio myAudio;

#if defined( EAE6320_PLATFORM_D3D )
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// Simple helper function to load an image into a DX11 texture with common settings
bool eae6320::cFinalGameProject::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	auto& g_context = eae6320::Graphics::sContext::g_context;
	auto* const direct3dDevice = g_context.direct3dDevice;
	direct3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	direct3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}
#elif defined( EAE6320_PLATFORM_GL )
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Simple helper function to load an image into a OpenGL texture with common settings
bool eae6320::cFinalGameProject::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}
#endif

void eae6320::cFinalGameProject::UpdateDialogue()
{
	if (story.canRead())
	{
		std::string ParseText = story.Read();

		CurrentText = story.Read();
		bool ret = LoadTextureFromFile("data/images/Pic1.jpg", &my_texture, &my_image_width, &my_image_height);
		IM_ASSERT(ret);
	}
	else
	{
		MeetChoice = true;
	}
}

void eae6320::cFinalGameProject::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

void eae6320::cFinalGameProject::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_camera.m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
}

void eae6320::cFinalGameProject::UpdateSimulationBasedOnInput()
{
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		UpdateDialogue();
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cFinalGameProject::Initialize()
{
	auto result = Results::Success;

	RegisterOnImGuiRenderUI(std::bind(&cFinalGameProject::RenderUI, this));

#if defined( EAE6320_PLATFORM_D3D )
	
#elif defined( EAE6320_PLATFORM_GL )
	// Create a OpenGL texture identifier
	glGenTextures(1, &my_texture);
	glBindTexture(GL_TEXTURE_2D, my_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	bool ret = LoadTextureFromFile("data/images/Pic1.jpg", &my_texture, &my_image_width, &my_image_height);
#endif

	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	config.SizePixels = 36.0f;
	MyFont = io.Fonts->AddFontDefault(&config);

	result  = InitializeStory();

	// Initialize the camera
	{
		m_camera.m_RigidBodyState.position.x = 0;
		m_camera.m_RigidBodyState.position.y = 0;
		m_camera.m_RigidBodyState.position.z = 10;
	}

	Logging::OutputMessage("FinalGameProject is initialized!");

	myAudio.CreateAudioData("data/audios/Mixdown.wav", "TestAudio", 1000, true);

	return result;
}

eae6320::cResult eae6320::cFinalGameProject::CleanUp()
{
	Logging::OutputMessage("FinalGameProject is cleaned up!");

	return Results::Success;
}

void eae6320::cFinalGameProject::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	// Submit background color
	Graphics::SubmitBackBufferColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Submit game object data
	//Graphics::SubmitGameObjectData(m_gameObjectData_submit, m_numOfGameObject_submit, i_elapsedSecondCount_sinceLastSimulationUpdate);

	// Submit camera
	Graphics::SubmitCamera(m_camera, i_elapsedSecondCount_sinceLastSimulationUpdate);
	
	//myAudio.CreateAudioData("data/audios/Test.mp3", "TestAudio");
	myAudio.SubmitAudioSource();
	

	//eae6320::AudioSystem::SubmitAudioSource();
}

void eae6320::cFinalGameProject::RenderUI()
{
	/*bool show_demo_window = true;

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);*/

	{
		ImGui::Begin("Test");

#if defined( EAE6320_PLATFORM_D3D )
		ImGui::Image((void*)my_texture, ImVec2(float(my_image_width), float(my_image_height)));
#elif defined( EAE6320_PLATFORM_GL )
		if (my_texture)
		{
			ImVec2 wsize = ImGui::GetWindowSize();
			ImGui::Image((void*)(intptr_t)my_texture, ImVec2(float(my_image_width), float(my_image_height)));
		}
			
#endif
		ImGui::Text("%s", CurrentText.c_str());

		if (MeetChoice)
		{
			std::vector<std::string> choices = story.GetChoices();
			size_t choiceCount = choices.size();
			if (choiceCount > 0)
			{
				size_t choiceIndex = 0;
				for (const std::string& choiceText : choices)
				{
					if (ImGui::Button(choiceText.c_str()))
					{
						story.SelectChoice((std::uint32_t)choiceIndex);
						MeetChoice = false;
						UpdateDialogue();
					}
					choiceIndex++;
				}
			}
			else
			{
				//End of Story
			}
		}
		ImGui::End();
	}
}

eae6320::cResult eae6320::cFinalGameProject::InitializeStory()
{
	auto result = eae6320::Results::Success;

	story.FromJSONFile("data/stories/MyStory.storyasset");

	return result;
}
