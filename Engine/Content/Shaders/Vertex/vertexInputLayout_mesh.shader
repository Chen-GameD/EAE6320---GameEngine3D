/*
	This vertex shader is used to create a Direct3D vertex input layout object
*/

#include <Shaders/shaders.inc>

DeclareMainEntryVert
{
	// The shader program is only used by Direct3D
	o_vertexPosition_projected = VECTOR4( i_vertexPosition_local, 1.0 );
}
