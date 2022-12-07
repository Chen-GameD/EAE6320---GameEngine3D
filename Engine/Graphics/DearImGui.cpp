#include "DearImGui.h"
#include "sContext.h"

#include <Engine/Asserts/Asserts.h>

eae6320::cResult eae6320::Graphics::DearImGui::InitializeImGui(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;

	// Create context
	if (!(result = CreateImGuiContext()))
	{
		EAE6320_ASSERTF(false, "Can't create ImGui context");
		return result;
	}

	// Initialize ImGui IO
	if (!(result = InitializeImGuiIO()))
	{
		EAE6320_ASSERTF(false, "Can't initialize ImGui IO");
		return result;
	}

	// Initialize ImGui Style
	InitializeImGuiStyle();

	// Initialize platform-specific / Renderer backend specific ImGui context
	if (!(result = InitializeImGuiPlatformRendererBackend(i_initializationParameters)))
	{
		EAE6320_ASSERTF(false, "Can't initialize platform-specific / Renderer backend specific ImGui context");
		return result;
	}

	// Load ImGui Fonts
	if (!(result = LoadImGuiFonts()))
	{
		EAE6320_ASSERTF(false, "Can't load ImGui Fonts");
		return result;
	}

	return result;
}

void eae6320::Graphics::DearImGui::RenderImGuiFrame()
{
	// Rendering
	ImGui::Render();
}

eae6320::cResult eae6320::Graphics::DearImGui::CreateImGuiContext()
{
	auto result = Results::Success;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	return result;
}

eae6320::cResult eae6320::Graphics::DearImGui::InitializeImGuiIO()
{
	auto result = Results::Success;

	ImGuiIO& ImGui_IO = ImGui::GetIO(); (void)ImGui_IO;
	ImGui_IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//ImGui_IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui_IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//ImGui_IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	ImGui_IO.WantCaptureMouse = true;
	ImGui_IO.WantCaptureKeyboard = true;
	//ImGui_IO.ConfigViewportsNoAutoMerge = true;
	//ImGui_IO.ConfigViewportsNoTaskBarIcon = true;
	//ImGui_IO.ConfigViewportsNoDefaultParent = true;
	//ImGui_IO.ConfigDockingAlwaysTabBar = true;
	//ImGui_IO.ConfigDockingTransparentPayload = true;
	//ImGui_IO.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	//ImGui_IO.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

	return result;
}

void eae6320::Graphics::DearImGui::InitializeImGuiStyle()
{
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();

	/*ImGuiIO& ImGui_IO = ImGui::GetIO();
	if (ImGui_IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}*/
}

eae6320::cResult eae6320::Graphics::DearImGui::LoadImGuiFonts()
{
	auto result = Results::Success;

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	return result;
}

void eae6320::Graphics::DearImGui::RenderImGuiUI()
{
	
}
