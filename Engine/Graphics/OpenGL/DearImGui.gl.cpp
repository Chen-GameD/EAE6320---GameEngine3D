#include "../DearImGui.h"

#include "../sContext.h"

#include <External/imgui/backends/imgui_impl_win32.h>
#include <External/imgui/backends/imgui_impl_opengl3.h>

eae6320::cResult eae6320::Graphics::DearImGui::InitializeImGuiPlatformRendererBackend(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;

	const char* glsl_version = "#version 130";

	ImGui_ImplWin32_Init(i_initializationParameters.mainWindow);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return result;
}

eae6320::cResult eae6320::Graphics::DearImGui::CreateImGuiFrame()
{
	auto result = Results::Success;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return result;
}

eae6320::cResult eae6320::Graphics::DearImGui::RenderImGui_DrawData()
{
	auto result = Results::Success;

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& ImGui_IO = ImGui::GetIO();

	// Update and Render additional Platform Windows
	if (ImGui_IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	return result;
}

eae6320::cResult eae6320::Graphics::DearImGui::CleanUpImGui()
{
	auto result = Results::Success;

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return result;
}

