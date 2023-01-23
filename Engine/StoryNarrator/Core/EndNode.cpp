#include "EndNode.h"

Narrator::Runtime::EndNode::EndNode()
	:Narrator::Runtime::Node(TNodeType::kEnd),
	Narrator::Runtime::MultiInFlowNode(TNodeType::kEnd)
{
	m_Name = "End";
}

Narrator::Runtime::EndNode::~EndNode()
{

}

void Narrator::Runtime::EndNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::MultiInFlowNode::ToJSON(nodeObject);
}

void Narrator::Runtime::EndNode::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	Narrator::Runtime::Node::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::MultiInFlowNode::FromJSON(nodeObject, i_Graph);
}

/*

void Narrator::Runtime::EndNode::FromJSON(const nlohmann::json& nodeObject)
{

}
*/
