#pragma once

// Includes
//=========

#include <Tools/AssetBuildLibrary/iBuilder.h>

#include <Engine/Graphics/Configuration.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class cStoryBuilder final : public iBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			cResult Build( const std::vector<std::string>& i_arguments ) final;

			// Implementation
			//===============

		private:	

		};
	}
}
