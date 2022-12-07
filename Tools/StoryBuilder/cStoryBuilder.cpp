// Includes
//=========

#include "cStoryBuilder.h"

#include <Tools/AssetBuildLibrary/Functions.h>
#include <Engine/Platform/Platform.h>
#include <Engine/StoryNarrator/Includes.h>
#include <External/JSON/Includes.h>

#include <fstream>

// Inherited Implementation
//=========================

// Build
//------

eae6320::cResult eae6320::Assets::cStoryBuilder::Build(const std::vector<std::string>& i_arguments)
{
	auto result = Results::Success;

	Narrator::Runtime::Story newStory = Narrator::Runtime::Story::Parse(m_path_source, m_path_target);

	if (newStory.IsValid())
	{
		
	}
	else
	{
		result = Results::Failure;
		OutputErrorMessageWithFileInfo(m_path_source, "Error while Building Story");
	}

	return result;
}
