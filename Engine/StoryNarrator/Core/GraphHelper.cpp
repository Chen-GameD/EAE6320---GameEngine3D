#include "GraphHelper.h"
#include "Node.h"
#include "Graph.h"

Narrator::Runtime::Node* Narrator::Runtime::GraphHelper::GetNode(const Narrator::Runtime::Graph* i_Graph, const std::uint32_t i_NodeID)
{
	//TODO: #NarratorToDoAssert Graph Not Null
	return i_Graph->m_NodeMap.at(i_NodeID);
}

