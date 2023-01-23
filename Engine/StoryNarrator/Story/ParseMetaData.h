#pragma once

#include <string>
#include <cstdint>

#include "ParseMessageType.h"

namespace Narrator
{
	namespace Parser
	{
		struct ParseMetaData
		{

		public:

			ParseMetaData(Narrator::Parser::TParseMessageType i_MessageType,
				std::uint64_t i_LineNumber, std::string i_Message) :
				ParseMessageType(i_MessageType),
				LineNumber(i_LineNumber),
				Message(i_Message)
			{
			}

			Narrator::Parser::TParseMessageType GetParseMessageType() const
			{
				return ParseMessageType;
			}

			std::uint64_t GetLineNumber() const
			{
				return LineNumber;
			}

			std::string GetMessage() const
			{
				return Message;
			}

		private:

			ParseMetaData() :
				ParseMessageType(Narrator::Parser::TParseMessageType::kInfo),
				LineNumber(0)
			{
			}

			TParseMessageType ParseMessageType;
			std::uint64_t LineNumber;
			std::string Message;
		};
	}
}