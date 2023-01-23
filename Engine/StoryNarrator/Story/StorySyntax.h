#pragma once

#include <string>

namespace Narrator
{
	namespace Runtime
	{
		class StorySyntax
		{
		public:
			static const std::string KNOT_DECLARATION;
			static const std::string CHOICE_DECLARATION;
			static const std::string DIVERT_DECLARATION;
			static const std::string END_DECLARATION;
			static const std::string DONE_DECLARATION;
		};
	}
}