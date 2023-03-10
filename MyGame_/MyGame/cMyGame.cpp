// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Audio/AudioSystem.h>

// Static Data
//============

namespace
{
	//static game object data
	eae6320::GameObjectData* m_gameObjectData_submit = nullptr;
	size_t m_numOfGameObject_submit = 0;
	eae6320::GameObjectData* m_gameObjectData_1 = new eae6320::GameObjectData[3];
	eae6320::GameObjectData* m_gameObjectData_2 = new eae6320::GameObjectData[3];

	//static camera
	eae6320::Camera m_camera;
}

// Inherited Implementation
//=========================

// Run
//----

eae6320::AudioSystem::cAudio myAudio;

void eae6320::cMyGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_camera.m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
	m_gameObjectData_1[0].m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
	m_gameObjectData_1[1].m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
	m_gameObjectData_1[2].m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
	m_gameObjectData_2[0].m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
	m_gameObjectData_2[1].m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
	m_gameObjectData_2[2].m_RigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
	// Controller
	if (UserInput::IsKeyPressed('W') ||
		UserInput::IsKeyPressed('S') ||
		UserInput::IsKeyPressed('A') ||
		UserInput::IsKeyPressed('D'))
	{
		if (UserInput::IsKeyPressed('W'))
		{
			m_gameObjectData_1[0].m_RigidBodyState.velocity = Math::sVector(0, 1, 0);
			m_gameObjectData_2[0].m_RigidBodyState.velocity = Math::sVector(0, 1, 0);
		}
		if (UserInput::IsKeyPressed('S'))
		{
			m_gameObjectData_1[0].m_RigidBodyState.velocity = Math::sVector(0, -1, 0);
			m_gameObjectData_2[0].m_RigidBodyState.velocity = Math::sVector(0, -1, 0);
		}
		if (UserInput::IsKeyPressed('A'))
		{
			m_gameObjectData_1[0].m_RigidBodyState.velocity = Math::sVector(-1, 0, 0);
			m_gameObjectData_2[0].m_RigidBodyState.velocity = Math::sVector(-1, 0, 0);
		}
		if (UserInput::IsKeyPressed('D'))
		{
			m_gameObjectData_1[0].m_RigidBodyState.velocity = Math::sVector(1, 0, 0);
			m_gameObjectData_2[0].m_RigidBodyState.velocity = Math::sVector(1, 0, 0);
		}
	}
	else
	{
		m_gameObjectData_1[0].m_RigidBodyState.velocity = Math::sVector(0, 0, 0);
		m_gameObjectData_2[0].m_RigidBodyState.velocity = Math::sVector(0, 0, 0);
	}

	// Test
	if (UserInput::IsKeyPressed('T'))
	{
		//eae6320::AudioSystem::PauseAudio();
		//myAudio.IsPlaying();
		//eae6320::AudioSystem::Play(1);
		myAudio.PlayIndependent();
	}

	if (UserInput::IsKeyPressed('P'))
	{
		//eae6320::AudioSystem::PauseAudio();
		//myAudio.PauseAudio();
		myAudio.PauseAudio();
	}

	if (UserInput::IsKeyPressed('R'))
	{
		//eae6320::AudioSystem::PauseAudio();
		//myAudio.PauseAudio();
		myAudio.ResumeAudio();
	}

	if (UserInput::IsKeyPressed('O'))
	{
		//eae6320::AudioSystem::PauseAudio();
		//myAudio.SubmitAudioSource();
		//myAudio.Play(1);
		//eae6320::AudioSystem::Play(1);
		myAudio.Play();
	}

	if (UserInput::IsKeyPressed('I'))
	{
		//eae6320::AudioSystem::PauseAudio();
		//myAudio.SubmitAudioSource();
		//myAudio.Play(2);
		//eae6320::AudioSystem::Play(2);
		myAudio.SetVolume(100);
	}


	// Is the user pressing the Space key?
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		// Switch GameObjectData
		if (m_gameObjectData_submit == m_gameObjectData_1)
		{
			m_gameObjectData_submit = m_gameObjectData_2;
			m_numOfGameObject_submit = 3;
		}
		else
		{
			m_gameObjectData_submit = m_gameObjectData_1;
			m_numOfGameObject_submit = 3;
		}
	}

	if (UserInput::IsKeyPressed('Q'))
	{
		m_numOfGameObject_submit = 1;
	}
	else
	{
		m_numOfGameObject_submit = 3;
	}

	// Camera movement
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up) || 
		UserInput::IsKeyPressed(UserInput::KeyCodes::Down) || 
		UserInput::IsKeyPressed(UserInput::KeyCodes::Left) || 
		UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
		{
			m_camera.m_RigidBodyState.velocity = Math::sVector(0, 0, 0.1f);
		}
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
		{
			m_camera.m_RigidBodyState.velocity = Math::sVector(0, 0, -0.1f);
		}
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
		{
			m_camera.m_RigidBodyState.velocity = Math::sVector(-0.1f, 0, 0);
		}
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
		{
			m_camera.m_RigidBodyState.velocity = Math::sVector(0.1f, 0, 0);
		}
	}
	else
	{ 
		m_camera.m_RigidBodyState.velocity = Math::sVector(0, 0, 0);
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	int a = sizeof(eae6320::Graphics::cMesh);
	int b = sizeof(eae6320::Graphics::cEffect);
	int c = sizeof(eae6320::sDataRequiredToRenderAFrame);
	int d = sizeof(eae6320::sDataRequiredToRenderAFrame::gameObjectArray);

	auto result = Results::Success;

	// Initialize the shading data
	{
		const char* vertexShaderAddress_1 = "data/Shaders/Vertex/standard.shader";
		const char* fragmentShaderAddress_1 = "data/Shaders/Fragment/myShader_1.shader";
		if (!(result = eae6320::Graphics::cEffect::CreateEffect(m_gameObjectData_1[0].m_Effect, vertexShaderAddress_1, fragmentShaderAddress_1)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}

		const char* vertexShaderAddress_2 = "data/Shaders/Vertex/standard.shader";
		const char* fragmentShaderAddress_2 = "data/Shaders/Fragment/myShader_2.shader";
		if (!(result = eae6320::Graphics::cEffect::CreateEffect(m_gameObjectData_1[1].m_Effect, vertexShaderAddress_2, fragmentShaderAddress_2)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}

		const char* vertexShaderAddress_5 = "data/Shaders/Vertex/standard.shader";
		const char* fragmentShaderAddress_5 = "data/Shaders/Fragment/myShader_2.shader";
		if (!(result = eae6320::Graphics::cEffect::CreateEffect(m_gameObjectData_1[2].m_Effect, vertexShaderAddress_5, fragmentShaderAddress_5)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}

		const char* vertexShaderAddress_3 = "data/Shaders/Vertex/standard.shader";
		const char* fragmentShaderAddress_3 = "data/Shaders/Fragment/myShader_3.shader";
		if (!(result = eae6320::Graphics::cEffect::CreateEffect(m_gameObjectData_2[0].m_Effect, vertexShaderAddress_3, fragmentShaderAddress_3)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}

		const char* vertexShaderAddress_4 = "data/Shaders/Vertex/standard.shader";
		const char* fragmentShaderAddress_4 = "data/Shaders/Fragment/myShader_4.shader";
		if (!(result = eae6320::Graphics::cEffect::CreateEffect(m_gameObjectData_2[1].m_Effect, vertexShaderAddress_4, fragmentShaderAddress_4)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}

		const char* vertexShaderAddress_6 = "data/Shaders/Vertex/standard.shader";
		const char* fragmentShaderAddress_6 = "data/Shaders/Fragment/myShader_4.shader";
		if (!(result = eae6320::Graphics::cEffect::CreateEffect(m_gameObjectData_2[2].m_Effect, vertexShaderAddress_6, fragmentShaderAddress_6)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
	}

	// Initialize the geometry
	{
		if (!(result = eae6320::Graphics::cMesh::CreateMeshWithBinaryFile(m_gameObjectData_1[0].m_Mesh, "data/meshes/Mesh_1.lua")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
		}

		if (!(result = eae6320::Graphics::cMesh::CreateMeshWithBinaryFile(m_gameObjectData_1[1].m_Mesh, "data/meshes/Mesh_2.lua")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
		}

		if (!(result = eae6320::Graphics::cMesh::CreateMeshWithBinaryFile(m_gameObjectData_1[2].m_Mesh, "data/meshes/Mesh_3.lua")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
		}

		if (!(result = eae6320::Graphics::cMesh::CreateMeshWithBinaryFile(m_gameObjectData_2[0].m_Mesh, "data/meshes/Mesh_1.lua")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
		}

		if (!(result = eae6320::Graphics::cMesh::CreateMeshWithBinaryFile(m_gameObjectData_2[1].m_Mesh, "data/meshes/Mesh_2.lua")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
		}

		if (!(result = eae6320::Graphics::cMesh::CreateMeshWithBinaryFile(m_gameObjectData_2[2].m_Mesh, "data/meshes/Mesh_3.lua")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
		}
	}

	// Initialize the camera
	{
		m_camera.m_RigidBodyState.position.x = 0;
		m_camera.m_RigidBodyState.position.y = 0;
		m_camera.m_RigidBodyState.position.z = 10;
	}

	// Initialize the Object
	{
		m_gameObjectData_1[1].m_RigidBodyState.position = Math::sVector(0, -1, 0);
		m_gameObjectData_2[1].m_RigidBodyState.position = Math::sVector(0, -1, 0);

		m_gameObjectData_1[2].m_RigidBodyState.position = Math::sVector(-1, 1, 0);
		m_gameObjectData_2[2].m_RigidBodyState.position = Math::sVector(-1, 1, 0);
	}

	m_gameObjectData_submit = m_gameObjectData_1;
	m_numOfGameObject_submit = 3;

	Logging::OutputMessage("MyGame is initialized!");

	//eae6320::AudioSystem::SubmitAudioToPlay();
	//myAudio.SubmitAudioToPlay();

	//myAudio.PauseAudio();
	//myAudio.SubmitAudioSource();

	myAudio.CreateAudioData("data/audios/Mixdown.wav", "TestAudio", 1000, true);

	return result;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	Logging::OutputMessage("MyGame is cleaned up!");
	for (int i = 0; i < 3; i++)
	{
		if (m_gameObjectData_1[i].m_Mesh)
		{
			m_gameObjectData_1[i].m_Mesh->DecrementReferenceCount();
			m_gameObjectData_1[i].m_Mesh = nullptr;
		}
		if (m_gameObjectData_1[i].m_Effect)
		{
			m_gameObjectData_1[i].m_Effect->DecrementReferenceCount();
			m_gameObjectData_1[i].m_Effect = nullptr;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (m_gameObjectData_2[i].m_Mesh)
		{
			m_gameObjectData_2[i].m_Mesh->DecrementReferenceCount();
			m_gameObjectData_2[i].m_Mesh = nullptr;
		}
		if (m_gameObjectData_2[i].m_Effect)
		{
			m_gameObjectData_2[i].m_Effect->DecrementReferenceCount();
			m_gameObjectData_2[i].m_Effect = nullptr;
		}
	}

	return Results::Success;
}

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	// Submit background color
	Graphics::SubmitBackBufferColor(1.0f, 1.0f, 0.0f, 1.0f);

	// Submit game object data
	Graphics::SubmitGameObjectData(m_gameObjectData_submit, m_numOfGameObject_submit, i_elapsedSecondCount_sinceLastSimulationUpdate);

	// Submit camera
	Graphics::SubmitCamera(m_camera, i_elapsedSecondCount_sinceLastSimulationUpdate);
	
	//myAudio.CreateAudioData("data/audios/Test.mp3", "TestAudio");
	myAudio.SubmitAudioSource();
	

	//eae6320::AudioSystem::SubmitAudioSource();
}
