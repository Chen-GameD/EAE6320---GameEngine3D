#pragma once

#include "InitializationParameters.h"

#include <External/imgui/imgui.h>
#include <Engine/Results/Results.h>


namespace eae6320
{
	namespace Graphics
	{
		namespace DearImGui
		{
			cResult InitializeImGui(const sInitializationParameters& i_initializationParameters);
			//Platform / Renderer Backend Specific 
			cResult CleanUpImGui();

			cResult CreateImGuiContext();
			//Platform / Renderer Backend Specific 
			cResult InitializeImGuiPlatformRendererBackend(const sInitializationParameters& i_initializationParameters);

			cResult InitializeImGuiIO();
			void InitializeImGuiStyle();
			cResult LoadImGuiFonts();

			//Platform / Renderer Backend Specific 
			cResult CreateImGuiFrame();
			void RenderImGuiUI();
			void RenderImGuiFrame();
			//Platform / Renderer Backend Specific 
			cResult RenderImGui_DrawData();
		}
	}
}