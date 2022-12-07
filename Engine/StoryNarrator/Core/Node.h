#pragma once

#include "NodeType.h"

#include <string>
#include <cstdint>
#include <vector>

#include "../JSON/Includes.h"

//Forward Declaration
namespace Narrator
{
	namespace Runtime
	{
		class Graph;
	}
}

namespace Narrator
{
	namespace Runtime
	{
		class Node
		{

		public:

			static size_t GetTotalNodeCount();
			static size_t GetMaxNodeCount();
			static void ResetNodeCount();

			Node(TNodeType i_Type = TNodeType::kNodeBase);
			virtual ~Node();

			virtual std::string ToString();

			TNodeType GetType() const;
			std::string GetName() const;
			std::uint32_t GetID() const;
			TInFlowType GetInFlowType() const;
			TOutFlowType GetOutFlowType() const;

			bool IsVisited() const;
			void MarkAsVisited();

			//ParseMetaData - LineNumber
			std::vector<std::uint64_t> GetParseLineNumberList() const;
			void AddParseLineNumber(const std::uint64_t i_ParseLineNumber);

			virtual void ToJSON(nlohmann::json& nodeObject);

			virtual void FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph);

		protected:

			static std::uint32_t m_CurrentIndex;

			void SetName(const std::string& i_Name);

			std::uint32_t m_ID;
			std::string m_Name;
			TNodeType m_Type;
			TInFlowType m_InFlowType;
			TOutFlowType m_OutFlowType;

			//Traversal Data
			bool m_IsVisited;

			//TODO: #NarratorToDo Save this information elsewhere 
			//possibly in story parse meta data(Node pointer & LineNumber)
			//ParseMetaData - LineNumber
			std::vector<std::uint64_t> m_ParseLineNumberList;

		private:

		};
	}
}