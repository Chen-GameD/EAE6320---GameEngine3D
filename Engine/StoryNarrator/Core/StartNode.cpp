#include "StartNode.h"

Narrator::Runtime::StartNode::StartNode()
	:Narrator::Runtime::Node(TNodeType::kStart),
	Narrator::Runtime::UniOutFlowNode(TNodeType::kStart)
{
	m_Name = "Start";
}

Narrator::Runtime::StartNode::~StartNode()
{

}

void Narrator::Runtime::StartNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::UniOutFlowNode::ToJSON(nodeObject);
}

void Narrator::Runtime::StartNode::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	Narrator::Runtime::Node::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::UniOutFlowNode::FromJSON(nodeObject, i_Graph);
}
