#include "MultiOutFlowNode.h"
#include "GraphHelper.h"

Narrator::Runtime::MultiOutFlowNode::MultiOutFlowNode(TNodeType i_Type /*= TNodeType::kMultiFlow*/,
	TOutFlowType i_OutFlowType /*= TOutFlowType::kMultiOutFlow*/)
	: Narrator::Runtime::Node(i_Type)
{
	m_OutFlowType = i_OutFlowType;
}

Narrator::Runtime::MultiOutFlowNode::~MultiOutFlowNode()
{

}

void Narrator::Runtime::MultiOutFlowNode::AddOutFlowNode(Narrator::Runtime::Node* i_NextNode)
{
	if (i_NextNode)
	{
		std::uint32_t nodeID = i_NextNode->GetID();
		if (m_OutFlowNodeMap.find(nodeID) == m_OutFlowNodeMap.end())
		{
			//Node with nodeID not found, add the new Node to the Map
			m_OutFlowNodeMap.insert(std::pair<std::uint32_t, Narrator::Runtime::Node*>(nodeID, i_NextNode));
		}
		else
		{
			m_OutFlowNodeMap[nodeID] = i_NextNode;
		}
	}
}

void Narrator::Runtime::MultiOutFlowNode::RemoveOutFlowNode(Narrator::Runtime::Node* i_NextNode)
{
	if (i_NextNode)
	{
		std::uint32_t nodeID = i_NextNode->GetID();
		if (m_OutFlowNodeMap.find(nodeID) == m_OutFlowNodeMap.end())
		{
			//Node with nodeID not found
			return;
		}
		else
		{
			m_OutFlowNodeMap.erase(nodeID);
		}
	}
}

void Narrator::Runtime::MultiOutFlowNode::RemoveOutFlowNode(std::uint32_t i_NextNodeID)
{
	if (m_OutFlowNodeMap.find(i_NextNodeID) == m_OutFlowNodeMap.end())
	{
		//Node with nodeID not found
		return;
	}
	else
	{
		m_OutFlowNodeMap.erase(i_NextNodeID);
	}
}

std::map<std::uint32_t, Narrator::Runtime::Node*> Narrator::Runtime::MultiOutFlowNode::GetOutFlowNodeMap() const
{
	return m_OutFlowNodeMap;
}

void Narrator::Runtime::MultiOutFlowNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	
	nlohmann::json outFlowNodeArray = nlohmann::json::array();
	for (std::map<std::uint32_t, Narrator::Runtime::Node*>::iterator mapIterator = m_OutFlowNodeMap.begin();
		mapIterator != m_OutFlowNodeMap.end();
		mapIterator++)
	{
		nlohmann::json outFlowNode = nlohmann::json::object();
		outFlowNode["id"] = mapIterator->second->GetID();
		outFlowNode["type"] = mapIterator->second->GetType();
		
		outFlowNodeArray.emplace_back(outFlowNode);
	}
	nodeObject["out_flow_nodes"] = outFlowNodeArray;
}

void Narrator::Runtime::MultiOutFlowNode::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	Narrator::Runtime::Node::FromJSON(nodeObject, i_Graph);
	if (nodeObject.contains("out_flow_nodes"))
	{
		const nlohmann::json outFlowNodeArray = nodeObject["out_flow_nodes"];
		for (const nlohmann::json& outFlowNodeObject : outFlowNodeArray)
		{
			const std::uint32_t nodeID = outFlowNodeObject["id"];
			Narrator::Runtime::Node* outFlowNode = Narrator::Runtime::GraphHelper::GetNode(i_Graph, nodeID);
			m_OutFlowNodeMap.insert(std::pair<std::uint32_t, Narrator::Runtime::Node*>(nodeID, outFlowNode));
		}
	}
	else
	{
		//TODO: #NarratorToDo 
	}
}

/*
void Narrator::Runtime::MultiOutFlowNode::FromJSON(const nlohmann::json& nodeObject, const std::vector<Narrator::Runtime::Node*> i_OutFlowNodeList)
{
	Narrator::Runtime::Node::FromJSON(nodeObject);
	for (const Narrator::Runtime::Node* outFlowNode : i_OutFlowNodeList)
	{
		const std::uint32_t nodeID = outFlowNode->GetID();
		m_OutFlowNodeMap.insert(std::pair<>(nodeID, outFlowNode))
	}
}
*/

