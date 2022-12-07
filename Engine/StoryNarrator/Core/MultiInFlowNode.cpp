#include "MultiInFlowNode.h"
#include "GraphHelper.h"

Narrator::Runtime::MultiInFlowNode::MultiInFlowNode(TNodeType i_Type /*= TNodeType::kMultiFlow*/,
	TInFlowType i_InFlowType /*= TInFlowType::kMultiInFlow*/)
	: Narrator::Runtime::Node(i_Type)
{
	m_InFlowType = i_InFlowType;
}

Narrator::Runtime::MultiInFlowNode::~MultiInFlowNode()
{

}

void Narrator::Runtime::MultiInFlowNode::AddInFlowNode(Narrator::Runtime::Node* i_NextNode)
{
	if (i_NextNode)
	{
		std::uint32_t nodeID = i_NextNode->GetID();
		if (m_InFlowNodeMap.find(nodeID) == m_InFlowNodeMap.end())
		{
			//Node with nodeID not found, add the new Node to the Map
			m_InFlowNodeMap.insert(std::pair<std::uint32_t, Narrator::Runtime::Node*>(nodeID, i_NextNode));
		}
		else
		{
			m_InFlowNodeMap[nodeID] = i_NextNode;
		}
	}
}

void Narrator::Runtime::MultiInFlowNode::RemoveInFlowNode(Narrator::Runtime::Node* i_NextNode)
{
	if (i_NextNode)
	{
		std::uint32_t nodeID = i_NextNode->GetID();
		if (m_InFlowNodeMap.find(nodeID) == m_InFlowNodeMap.end())
		{
			//Node with nodeID not found
			return;
		}
		else
		{
			m_InFlowNodeMap.erase(nodeID);
		}
	}
}

void Narrator::Runtime::MultiInFlowNode::RemoveInFlowNode(std::uint32_t i_NextNodeID)
{
	if (m_InFlowNodeMap.find(i_NextNodeID) == m_InFlowNodeMap.end())
	{
		//Node with nodeID not found
		return;
	}
	else
	{
		m_InFlowNodeMap.erase(i_NextNodeID);
	}
}

std::map<std::uint32_t, Narrator::Runtime::Node*> Narrator::Runtime::MultiInFlowNode::GetInFlowNodeMap() const
{
	return m_InFlowNodeMap;
}

void Narrator::Runtime::MultiInFlowNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	
	nlohmann::json inFlowNodeArray = nlohmann::json::array();
	for (std::map<std::uint32_t, Narrator::Runtime::Node*>::iterator mapIterator = m_InFlowNodeMap.begin();
		mapIterator != m_InFlowNodeMap.end();
		mapIterator++)
	{
		nlohmann::json inFlowNode = nlohmann::json::object();
		inFlowNode["id"] = mapIterator->second->GetID();
		inFlowNode["type"] = mapIterator->second->GetType();
		
		inFlowNodeArray.emplace_back(inFlowNode);
	}
	nodeObject["in_flow_nodes"] = inFlowNodeArray;
}

void Narrator::Runtime::MultiInFlowNode::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	Narrator::Runtime::Node::FromJSON(nodeObject, i_Graph);
	if (nodeObject.contains("in_flow_nodes"))
	{
		const nlohmann::json inFlowNodeArray = nodeObject["in_flow_nodes"];
		for (const nlohmann::json& inFlowNodeObject : inFlowNodeArray)
		{
			const std::uint32_t nodeID = inFlowNodeObject["id"];
			Narrator::Runtime::Node* inFlowNode = Narrator::Runtime::GraphHelper::GetNode(i_Graph, nodeID);
			m_InFlowNodeMap.insert(std::pair<std::uint32_t, Narrator::Runtime::Node*>(nodeID, inFlowNode));
		}
	}
	else
	{
		//TODO: #NarratorToDo 
	}
}

/*
void Narrator::Runtime::MultiInFlowNode::FromJSON(const nlohmann::json& nodeObject, const std::vector<Narrator::Runtime::Node*> i_InFlowNodeList)
{
	Narrator::Runtime::Node::FromJSON(nodeObject);
	for (const Narrator::Runtime::Node* inFlowNode : i_InFlowNodeList)
	{
		const std::uint32_t nodeID = inFlowNode->GetID();
		m_InFlowNodeMap.insert(std::pair<std::uint32_t, Narrator::Runtime::Node*>(nodeID, inFlowNode))
	}
}
*/

