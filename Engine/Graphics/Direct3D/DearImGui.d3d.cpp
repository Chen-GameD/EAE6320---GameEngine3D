#include "../DearImGui.h"

#include "../sContext.h"

#include <External/imgui/backends/imgui_impl_win32.h>
#include <External/imgui/backends/imgui_impl_dx11.h>

eae6320::cResult eae6320::Graphics::DearImGui::InitializeImGuiPlatformRendererBackend(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;

	ImGui_ImplWin32_Init(i_initializationParameters.mainWindow);
	ImGui_ImplDX11_Init(eae6320::Graphics::sContext::g_context.direct3dDevice, eae6320::Graphics::sContext::g_context.direct3dImmediateContext);

	return result;
}

eae6320::cResult eae6320::Graphics::DearImGui::CreateImGuiFrame()
{
	auto result = Results::Success;

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return result;
}

eae6320::cResult eae6320::Graphics::DearImGui::RenderImGui_DrawData()
{
	auto result = Results::Success;

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

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
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return result;
}

