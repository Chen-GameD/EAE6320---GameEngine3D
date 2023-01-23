#pragma once

#include <cstdint>

namespace Narrator
{
	namespace Parser
	{
		enum class TParseMessageType : std::uint16_t
		{
			kTrace,
			kDebug,
			kInfo,
			kWarning,
			kError,
			kCritical
		};
	}
}