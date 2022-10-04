/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

// Entry Point
//============

DeclareMainEntryFrag
{
	// Output solid white
	o_color = VECTOR4(
		// RGB (color)
		1.0, 1.0, 1.0,
		// Alpha (opacity)
		1.0 );
	/*if (cos(g_elapsedSecondCount_simulationTime) < 0)
		o_color.r = -cos(g_elapsedSecondCount_simulationTime);
	else
		o_color.r = cos(g_elapsedSecondCount_simulationTime);
	if (sin(g_elapsedSecondCount_simulationTime) < 0)
		o_color.g = -sin(g_elapsedSecondCount_simulationTime / 2);
	else
		o_color.g = sin(g_elapsedSecondCount_simulationTime / 2);*/

	o_color.r = sin(g_elapsedSecondCount_simulationTime) + 1 / 2;
	o_color.g = cos(g_elapsedSecondCount_simulationTime * 2) + 1 / 2;
	o_color.b = sin(g_elapsedSecondCount_simulationTime * 3) + 1 / 2;
}
