#include "UniInFlowNode.h"
#include "GraphHelper.h"

Narrator::Runtime::UniInFlowNode::UniInFlowNode(TNodeType i_Type /*= TNodeType::kUniFlow*/,
	TInFlowType i_InFlowType /*= TInFlowType::kUniInFlow*/)
	:Narrator::Runtime::Node(i_Type),
	m_PreviousNode(nullptr)
{
	m_InFlowType = i_InFlowType;
}

Narrator::Runtime::UniInFlowNode::~UniInFlowNode()
{

}

void Narrator::Runtime::UniInFlowNode::SetPreviousNode(Narrator::Runtime::Node* i_NextNode)
{
	m_PreviousNode = i_NextNode;
}

Narrator::Runtime::Node* Narrator::Runtime::UniInFlowNode::GetPreviousNode()
{
	return m_PreviousNode;
}

void Narrator::Runtime::UniInFlowNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	
	nlohmann::json previousNode = nlohmann::json::object();
	previousNode = nullptr;
	if (m_PreviousNode)
	{
		previousNode["id"] = m_PreviousNode->GetID();
		previousNode["type"] = m_PreviousNode->GetType();
	}
	nodeObject["previous_node"] = previousNode;
}

void Narrator::Runtime::UniInFlowNode::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	Narrator::Runtime::Node::FromJSON(nodeObject, i_Graph);
	if (nodeObject.contains("previous_node"))
	{
		nlohmann::json previousNodeJSON = nodeObject["previous_node"];
		std::uint32_t nodeID = previousNodeJSON["id"];
		m_PreviousNode = Narrator::Runtime::GraphHelper::GetNode(i_Graph, nodeID);
	}
	else
	{
		m_PreviousNode = nullptr;
	}
}
