#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include "UniOutFlowNode.h"
#include "MultiOutFlowNode.h"
#include "UniInFlowNode.h"
#include "MultiInFlowNode.h"

Narrator::Runtime::Graph::Graph()
{

}

Narrator::Runtime::Graph::~Graph()
{

}

void Narrator::Runtime::Graph::AddEdge(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_DestinationNode)
{
	if (i_SourceNode == nullptr)
	{
		//TODO: #NarratorToDoAssert Error Log / Assert
		return;
	}

	Narrator::Runtime::Edge* newEdge = new Narrator::Runtime::Edge(i_SourceNode, i_DestinationNode);
	AddToNodeMap(i_SourceNode);
	AddToNodeMap(i_DestinationNode);
	AddToEdgeMap(newEdge);
	AddToAdjacencyListMap(i_SourceNode, i_DestinationNode);
	AddNodeLink(i_SourceNode, i_DestinationNode);
}

void Narrator::Runtime::Graph::AddToNodeMap(Narrator::Runtime::Node* i_NodeToAdd)
{
	if (i_NodeToAdd)
	{
		std::uint32_t nodeID = i_NodeToAdd->GetID();
		if (m_NodeMap.find(nodeID) == m_NodeMap.end())
		{
			//Node Does not Exist
			m_NodeMap.insert(std::pair<uint32_t, Narrator::Runtime::Node*>(nodeID, i_NodeToAdd));
		}
		else
		{
			//TODO: #NarratorToDoLog Add Log that the nodeID is replaced
			m_NodeMap[nodeID] = i_NodeToAdd;
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert Error Log / Assert
	}
}

void Narrator::Runtime::Graph::AddToEdgeMap(Narrator::Runtime::Edge* i_EdgeToAdd)
{
	if (i_EdgeToAdd)
	{
		std::uint64_t edgeID = i_EdgeToAdd->GetID();
		if (m_EdgeMap.find(edgeID) == m_EdgeMap.end())
		{
			//Edge Does not Exist
			m_EdgeMap.insert(std::pair<uint64_t, Narrator::Runtime::Edge*>(i_EdgeToAdd->GetID(), i_EdgeToAdd));
		}
		else
		{
			//TODO: #NarratorToDoLog Add Log that the edgeID is replaced
			m_EdgeMap[edgeID] = i_EdgeToAdd;
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert Error Log / Assert
	}
}

void Narrator::Runtime::Graph::AddToAdjacencyListMap(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_AdjacentNode)
{
	if (i_SourceNode != nullptr && i_AdjacentNode != nullptr)
	{
		std::uint32_t sourceNodeID = i_SourceNode->GetID();
		std::map<uint32_t, std::vector<Narrator::Runtime::Node*>>::iterator mapIterator = m_AdjacencyListMap.find(sourceNodeID);
		if (mapIterator == m_AdjacencyListMap.end())
		{
			//AdjacencyList Not found for the Source Node

			//Create a new Adjacency List and add the adjacent Node
			std::vector<Narrator::Runtime::Node*> adjacencyList;
			adjacencyList.emplace_back(i_AdjacentNode);

			m_AdjacencyListMap.insert(std::pair<uint32_t, std::vector<Narrator::Runtime::Node*>>(sourceNodeID, adjacencyList));
		}
		else
		{
			mapIterator->second.emplace_back(i_AdjacentNode);
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert Error Log / Assert
	}
}

void Narrator::Runtime::Graph::AddNodeLink(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_TargetNode)
{
	switch (i_SourceNode->GetOutFlowType())
	{
	case Narrator::Runtime::TOutFlowType::kNone:
	{

	}
	break;
	case Narrator::Runtime::TOutFlowType::kUniOutFlow:
	{
		Narrator::Runtime::UniOutFlowNode* flowNode = dynamic_cast<Narrator::Runtime::UniOutFlowNode*>(i_SourceNode);
		if (flowNode)
		{
			flowNode->SetNextNode(i_TargetNode);
		}
		else
		{
			//TODO: #NarratorToDoAssert Should Not be null
		}
	}
	break;
	case Narrator::Runtime::TOutFlowType::kMultiOutFlow:
	{

		Narrator::Runtime::MultiOutFlowNode* flowNode = dynamic_cast<Narrator::Runtime::MultiOutFlowNode*>(i_SourceNode);
		if (flowNode)
		{
			flowNode->AddOutFlowNode(i_TargetNode);
		}
		else
		{
			//TODO: #NarratorToDoAssert Should Not be null
		}
	}
	break;
	}

	switch (i_TargetNode->GetInFlowType())
	{
	case Narrator::Runtime::TInFlowType::kNone:
	{

	}
	break;
	case Narrator::Runtime::TInFlowType::kUniInFlow:
	{
		Narrator::Runtime::UniInFlowNode* flowNode = dynamic_cast<Narrator::Runtime::UniInFlowNode*>(i_TargetNode);
		if (flowNode)
		{
			flowNode->SetPreviousNode(i_SourceNode);
		}
		else
		{
			//TODO: #NarratorToDoAssert Should Not be null
		}
	}
	break;
	case Narrator::Runtime::TInFlowType::kMultiInFlow:
	{
		Narrator::Runtime::MultiInFlowNode* flowNode = dynamic_cast<Narrator::Runtime::MultiInFlowNode*>(i_TargetNode);
		if (flowNode)
		{
			flowNode->AddInFlowNode(i_SourceNode);
		}
		else
		{
			//TODO: #NarratorToDoAssert Should Not be null
		}
	}
	break;
	}
}

bool Narrator::Runtime::Graph::HasRedirectionNode(const std::string& i_RedirectionName)
{
	std::map<std::string, Narrator::Runtime::Node*>::iterator mapIterator = m_RedirectionNodeMap.find(i_RedirectionName);
	if (mapIterator != m_RedirectionNodeMap.end())
	{
		//Present
		return true;
	}
	else
	{
		return false;
	}
}

bool Narrator::Runtime::Graph::HasSubGraphStartNode(const std::string& i_SubGraphStartNodeName)
{
	std::map<std::string, Narrator::Runtime::Node*>::iterator mapIterator = m_SubGraphStartNodeMap.find(i_SubGraphStartNodeName);
	if (mapIterator != m_SubGraphStartNodeMap.end())
	{
		//Present
		return true;
	}
	else
	{
		return false;
	}
}

Narrator::Runtime::Node* Narrator::Runtime::Graph::GetSubGraphStartNode(const std::string& i_SubGraphStartNodeName)
{
	return m_SubGraphStartNodeMap[i_SubGraphStartNodeName];
}

Narrator::Runtime::Node* Narrator::Runtime::Graph::GetRedirectionNode(const std::string& i_RedirectionName)
{
	return m_RedirectionNodeMap[i_RedirectionName];
}

void Narrator::Runtime::Graph::AddToRedirectionNodeMap(const std::string& i_RedirectionName, Narrator::Runtime::Node* i_RedirectionNode)
{
	std::map<std::string, Narrator::Runtime::Node*>::iterator mapIterator = m_RedirectionNodeMap.find(i_RedirectionName);
	if (mapIterator != m_RedirectionNodeMap.end())
	{
		//Present
		return;
	}
	else
	{
		//Add to Map
		m_RedirectionNodeMap.insert(std::pair<std::string, Narrator::Runtime::Node*>(i_RedirectionName, i_RedirectionNode));
	}
}

void Narrator::Runtime::Graph::AddToSubGraphStartNodeMap(const std::string& i_SubGraphStartNodeName, Narrator::Runtime::Node* i_SubGraphStartNode)
{
	std::map<std::string, Narrator::Runtime::Node*>::iterator mapIterator = m_SubGraphStartNodeMap.find(i_SubGraphStartNodeName);
	if (mapIterator != m_SubGraphStartNodeMap.end())
	{
		//Present
		return;
	}
	else
	{
		//Add To Map
		m_SubGraphStartNodeMap.insert(std::pair<std::string, Narrator::Runtime::Node*>(i_SubGraphStartNodeName, i_SubGraphStartNode));
	}
}
