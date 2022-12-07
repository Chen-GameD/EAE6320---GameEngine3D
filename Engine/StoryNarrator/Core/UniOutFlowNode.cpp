#include "UniOutFlowNode.h"

Narrator::Runtime::UniOutFlowNode::UniOutFlowNode(TNodeType i_Type /*= TNodeType::kUniFlow*/,
	TOutFlowType i_OutFlowType /*= TOutFlowType::kUniOutFlow*/)
	:Narrator::Runtime::Node(i_Type),
	m_NextNode(nullptr)
{
	m_OutFlowType = i_OutFlowType;
}

Narrator::Runtime::UniOutFlowNode::~UniOutFlowNode()
{

}

void Narrator::Runtime::UniOutFlowNode::SetNextNode(Narrator::Runtime::Node* i_NextNode)
{
	m_NextNode = i_NextNode;
}

Narrator::Runtime::Node* Narrator::Runtime::UniOutFlowNode::GetNextNode()
{
	return m_NextNode;
}

void Narrator::Runtime::UniOutFlowNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	
	nlohmann::json nextNode = nlohmann::json::object();
	nextNode = nullptr;
	if (m_NextNode)
	{
		nextNode["id"] = m_NextNode->GetID();
		nextNode["type"] = m_NextNode->GetType();
	}
	nodeObject["next_node"] = nextNode;
}

void Narrator::Runtime::UniOutFlowNode::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	Narrator::Runtime::Node::FromJSON(nodeObject, i_Graph);
	if (nodeObject.contains("next_node"))
	{
		nlohmann::json nextNodeObject = nodeObject["next_node"];
		std::uint32_t nodeID = nextNodeObject["id"];
		m_NextNode = Narrator::Runtime::GraphHelper::GetNode(i_Graph, nodeID);
	}
	else
	{
		m_NextNode = nullptr;
	}
}
