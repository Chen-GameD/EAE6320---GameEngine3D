#pragma once

#include <cstdint>

#include "../JSON/Includes.h"

namespace Narrator
{
	namespace Runtime
	{
		enum class TNodeType : std::uint16_t
		{
			kNodeBase = 0,
			//Start Node
			kStart,
			//End Node
			kEnd,
			//Text Content Node (Dialogues)
			kDialogue,
			//Link Node
			kDivert,
			//Section Node
			kKnot,
			//Choice Node
			kChoice,
			//Decision Node
			kDecision
		};

		enum class TInFlowType : std::uint16_t
		{
			kNone = 0,
			//Single In Flow Link
			kUniInFlow,
			//Multiple In Flow Links
			kMultiInFlow
		};

		enum class TOutFlowType : std::uint16_t
		{
			kNone = 0,
			//Single Out Flow Link
			kUniOutFlow,
			//Multiple Out Flow Links
			kMultiOutFlow
		};

		// map TNodeType values to JSON as strings
		NLOHMANN_JSON_SERIALIZE_ENUM(Narrator::Runtime::TNodeType, {
			{Narrator::Runtime::TNodeType::kNodeBase, "kNodeBase"},
			{Narrator::Runtime::TNodeType::kStart, "kStart"},
			{Narrator::Runtime::TNodeType::kEnd, "kEnd"},
			{Narrator::Runtime::TNodeType::kDialogue, "kDialogue"},
			{Narrator::Runtime::TNodeType::kDivert, "kDivert"},
			{Narrator::Runtime::TNodeType::kKnot, "kKnot"},
			{Narrator::Runtime::TNodeType::kChoice, "kChoice"},
			{Narrator::Runtime::TNodeType::kDecision, "kDecision"}
			})

		// map TInFlowType values to JSON as strings
		NLOHMANN_JSON_SERIALIZE_ENUM(Narrator::Runtime::TInFlowType, {
			{Narrator::Runtime::TInFlowType::kNone, "kNone"},
			{Narrator::Runtime::TInFlowType::kUniInFlow, "kUniInFlow"},
			{Narrator::Runtime::TInFlowType::kMultiInFlow, "kMultiInFlow"}
			})

		// map TOutFlowType values to JSON as strings
		NLOHMANN_JSON_SERIALIZE_ENUM(Narrator::Runtime::TOutFlowType, {
			{Narrator::Runtime::TOutFlowType::kNone, "kNone"},
			{Narrator::Runtime::TOutFlowType::kUniOutFlow, "kUniOutFlow"},
			{Narrator::Runtime::TOutFlowType::kMultiOutFlow, "kMultiOutFlow"}
			})

	}
}