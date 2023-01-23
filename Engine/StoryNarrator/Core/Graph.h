#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <string>

#include "../JSON/Includes.h"

//Forward Declarations
namespace Narrator {
	namespace Runtime {
		class Node;
		class Edge;
	}
}

namespace Narrator
{
	namespace Runtime
	{
		class Graph
		{

			friend class GraphHelper;

		public:

			Graph();
			virtual ~Graph();

		protected:

			virtual void ToJSON(nlohmann::json& jsonRoot) = 0;

			virtual void FromJSON(const nlohmann::json& jsonRoot) = 0;

			virtual void Traverse() = 0;

			void AddEdge(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_DestinationNode);

			void AddToNodeMap(Narrator::Runtime::Node* i_NodeToAdd);
			void AddToEdgeMap(Narrator::Runtime::Edge* i_EdgeToAdd);
			void AddToAdjacencyListMap(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_AdjacentNode);
			void AddNodeLink(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_TargetNode);

			bool HasRedirectionNode(const std::string& i_RedirectionName);
			Narrator::Runtime::Node* GetRedirectionNode(const std::string& i_RedirectionName);
			void AddToRedirectionNodeMap(const std::string& i_RedirectionName, Narrator::Runtime::Node* i_RedirectionNode);

			bool HasSubGraphStartNode(const std::string& i_SubGraphStartNodeName);
			Narrator::Runtime::Node* GetSubGraphStartNode(const std::string& i_SubGraphStartNodeName);
			void AddToSubGraphStartNodeMap(const std::string& i_SubGraphStartNodeName, Narrator::Runtime::Node* i_SubGraphStartNode);


			std::map<std::uint32_t, Narrator::Runtime::Node*> m_NodeMap;
			std::map<std::uint64_t, Narrator::Runtime::Edge*> m_EdgeMap;

			std::map<std::uint32_t, std::vector<Narrator::Runtime::Node*>> m_AdjacencyListMap;

			//Map of All Redirection Nodes
			std::map<std::string, Narrator::Runtime::Node*> m_RedirectionNodeMap;
			//Map of All SubGraphs' Start Nodes(Section/Knots)
			std::map<std::string, Narrator::Runtime::Node*> m_SubGraphStartNodeMap;

		private:

		};
	}
}