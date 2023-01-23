#include "DecisionNode.h"

Narrator::Runtime::DecisionNode::DecisionNode()
	:Narrator::Runtime::Node(TNodeType::kDecision),
	Narrator::Runtime::UniInFlowNode(TNodeType::kDecision),
	Narrator::Runtime::MultiOutFlowNode(TNodeType::kDecision)
{

}

Narrator::Runtime::DecisionNode::~DecisionNode()
{

}

std::string Narrator::Runtime::DecisionNode::ToString()
{
	return GetName() + "\t:DN:";
}

std::uint32_t Narrator::Runtime::DecisionNode::GetDecisionPathCount()
{
	std::uint32_t pathCount = static_cast<uint32_t>(m_OutFlowNodeMap.size());
	return pathCount;
}

void Narrator::Runtime::DecisionNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::UniInFlowNode::ToJSON(nodeObject);
	Narrator::Runtime::MultiOutFlowNode::ToJSON(nodeObject);
}

void Narrator::Runtime::DecisionNode::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	Narrator::Runtime::Node::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::UniInFlowNode::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::MultiOutFlowNode::FromJSON(nodeObject, i_Graph);
}
