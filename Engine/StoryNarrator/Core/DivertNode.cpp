#include "DivertNode.h"
#include "GraphHelper.h"

Narrator::Runtime::DivertNode::DivertNode()
	:Narrator::Runtime::Node(TNodeType::kDivert),
	Narrator::Runtime::MultiInFlowNode(TNodeType::kDivert),
	Narrator::Runtime::UniOutFlowNode(TNodeType::kDivert),
	m_TargetNode(nullptr)
{
	m_NextNode = nullptr;
}

Narrator::Runtime::DivertNode::~DivertNode()
{

}

std::string Narrator::Runtime::DivertNode::GetTargetNodeName() const
{
	return m_TargetNodeName;
}

void Narrator::Runtime::DivertNode::SetTargetNodeName(std::string i_TargetNodeName)
{
	m_TargetNodeName = i_TargetNodeName;
}

Narrator::Runtime::Node* Narrator::Runtime::DivertNode::GetTargetNode() const
{
	return m_TargetNode;
}

void Narrator::Runtime::DivertNode::SetTargetNode(Narrator::Runtime::Node* i_TargetNode)
{
	m_TargetNode = i_TargetNode;
}

void Narrator::Runtime::DivertNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::MultiInFlowNode::ToJSON(nodeObject);
	Narrator::Runtime::UniOutFlowNode::ToJSON(nodeObject);
	
	nlohmann::json target = nlohmann::json::object();
	target["name"] = m_TargetNodeName;
	
	nlohmann::json targetNode = nlohmann::json::object();
	targetNode["id"] = m_TargetNode->GetID();
	targetNode["type"] = m_TargetNode->GetType();
	target["node"] = targetNode;

	nodeObject["target"] = target;
}

void Narrator::Runtime::DivertNode::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	Narrator::Runtime::Node::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::MultiInFlowNode::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::UniOutFlowNode::FromJSON(nodeObject, i_Graph);

	if(nodeObject.contains("target"))
	{
		const nlohmann::json targetObject = nodeObject["target"];
		m_TargetNodeName = targetObject["name"];

		const nlohmann::json targetNodeObject = targetObject["node"];
		const std::uint32_t nodeID = targetNodeObject["id"];
		m_TargetNode = Narrator::Runtime::GraphHelper::GetNode(i_Graph, nodeID);
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError
	}
}

/*
void Narrator::Runtime::DivertNode::FromJSON(const nlohmann::json& nodeObject)
{

}
*/
std::string Narrator::Runtime::DivertNode::ToString()
{
	return GetName() + " : ->" + m_TargetNodeName;
}
