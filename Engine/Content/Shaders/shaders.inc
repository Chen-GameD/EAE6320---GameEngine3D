/*
	This file should be #included by all shaders
*/

// Version Information
//====================

#if defined( EAE6320_PLATFORM_GL )

	// GLSL shaders require the version to be #defined before anything else in the shader
	#version 420

#endif

// Constant Buffers
//=================

#if defined( EAE6320_PLATFORM_D3D )

	#define DeclareConstantBuffer( i_name, i_id ) cbuffer i_name : register( b##i_id )
	#define DeclareDrawCallBuffer( i_name, i_id ) cbuffer i_name : register( b##i_id )
	#define MATRIX4x4 float4x4
	#define VECTOR2 float2
	#define VECTOR3 float3
	#define VECTOR4 float4

#elif defined( EAE6320_PLATFORM_GL )

	#define DeclareConstantBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
	#define DeclareDrawCallBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
	#define MATRIX4x4 mat4
	#define VECTOR2 vec2
	#define VECTOR3 vec3
	#define VECTOR4 vec4
	#define o_vertexPosition_projected gl_Position

#endif

DeclareConstantBuffer( g_constantBuffer_frame, 0 )
{
	MATRIX4x4 g_transform_worldToCamera;
	MATRIX4x4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For alignment
	VECTOR2 g_padding;
};

DeclareDrawCallBuffer(g_constantBuffer_drawCall, 2)
{
  MATRIX4x4 g_transform_localToWorld;
};

#if defined( EAE6320_PLATFORM_D3D )

	#define DeclareMainEntryFrag \
	void main( \
		in const VECTOR4 i_fragmentPosition : SV_POSITION, \
		out VECTOR4 o_color : SV_TARGET \
	)

	#define DeclareMainEntryVert \
	void main(\
		in const VECTOR3 i_vertexPosition_local : POSITION, \
		out VECTOR4 o_vertexPosition_projected : SV_POSITION \
	)

#endif


#if defined( EAE6320_PLATFORM_GL )
	
	#define mul(i_matrix, i_vector) i_matrix * i_vector

	#define DeclareMainEntryFrag out VECTOR4 o_color; \
	void main()

	#define DeclareMainEntryVert layout( location = 0 ) in VECTOR3 i_vertexPosition_local; \
	void main()

#endif