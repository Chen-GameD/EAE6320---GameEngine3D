#include "ChoiceNode.h"

Narrator::Runtime::ChoiceNode::ChoiceNode(const std::uint32_t i_Index, const std::string& i_Text)
	:Narrator::Runtime::Node(TNodeType::kChoice),
	Narrator::Runtime::UniInFlowNode(TNodeType::kChoice),
	Narrator::Runtime::UniOutFlowNode(TNodeType::kChoice),
	m_Index(i_Index),
	m_Text(i_Text)
{

}

Narrator::Runtime::ChoiceNode::ChoiceNode()
	:Narrator::Runtime::Node(TNodeType::kChoice),
	Narrator::Runtime::UniInFlowNode(TNodeType::kChoice),
	Narrator::Runtime::UniOutFlowNode(TNodeType::kChoice),
	m_Index(0)
{

}

Narrator::Runtime::ChoiceNode::~ChoiceNode()
{

}


std::string Narrator::Runtime::ChoiceNode::ToString()
{
	return GetName() + " : " + m_Text;
}

std::uint32_t Narrator::Runtime::ChoiceNode::GetChoiceIndex() const
{
	return m_Index;
}

void Narrator::Runtime::ChoiceNode::SetChoiceIndex(const std::uint32_t i_Index)
{
	m_Index = i_Index;
}

std::string Narrator::Runtime::ChoiceNode::GetChoiceText() const
{
	return m_Text;
}

void Narrator::Runtime::ChoiceNode::SetChoiceText(const std::string& i_Text)
{
	m_Text = i_Text;
}

void Narrator::Runtime::ChoiceNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::UniInFlowNode::ToJSON(nodeObject);
	Narrator::Runtime::UniOutFlowNode::ToJSON(nodeObject);

	nlohmann::json choice = nlohmann::json::object();
	choice["id"] = m_Index;
	choice["text"] = m_Text;
	nodeObject["choice"] = choice;
}

void Narrator::Runtime::ChoiceNode::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	Narrator::Runtime::Node::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::UniInFlowNode::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::UniOutFlowNode::FromJSON(nodeObject, i_Graph);

	const nlohmann::json choiceObject = nodeObject["choice"];
	m_Index = choiceObject["id"];
	m_Text = choiceObject["text"];
}
