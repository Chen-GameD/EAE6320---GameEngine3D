#include "KnotNode.h"

Narrator::Runtime::KnotNode::KnotNode()
	:Narrator::Runtime::Node(TNodeType::kKnot),
	Narrator::Runtime::UniInFlowNode(TNodeType::kKnot),
	Narrator::Runtime::UniOutFlowNode(TNodeType::kKnot)
{
	m_Type = TNodeType::kKnot;
}

Narrator::Runtime::KnotNode::~KnotNode()
{

}

void Narrator::Runtime::KnotNode::SetName(const std::string& i_Name)
{
	Narrator::Runtime::Node::SetName(i_Name);
}

void Narrator::Runtime::KnotNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::UniInFlowNode::ToJSON(nodeObject);
	Narrator::Runtime::UniOutFlowNode::ToJSON(nodeObject);
}

void Narrator::Runtime::KnotNode::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	Narrator::Runtime::Node::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::UniInFlowNode::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::UniOutFlowNode::FromJSON(nodeObject, i_Graph);
}

std::string Narrator::Runtime::KnotNode::ToString()
{
	return std::to_string(GetID()) + " : " + "=== " + GetName();
}
