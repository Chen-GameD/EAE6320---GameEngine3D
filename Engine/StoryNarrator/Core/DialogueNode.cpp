#include "DialogueNode.h"

Narrator::Runtime::DialogueNode::DialogueNode(const std::string& i_Text)
	:Narrator::Runtime::Node(TNodeType::kDialogue),
	Narrator::Runtime::UniInFlowNode(TNodeType::kDialogue),
	Narrator::Runtime::UniOutFlowNode(TNodeType::kDialogue),
	m_Text(i_Text)
{

}

Narrator::Runtime::DialogueNode::DialogueNode()
	:Narrator::Runtime::Node(TNodeType::kDialogue),
	Narrator::Runtime::UniInFlowNode(TNodeType::kDialogue),
	Narrator::Runtime::UniOutFlowNode(TNodeType::kDialogue)
{

}

Narrator::Runtime::DialogueNode::~DialogueNode()
{

}

std::string Narrator::Runtime::DialogueNode::ToString()
{
	return GetName() + " : " + m_Text;
}

std::string Narrator::Runtime::DialogueNode::GetText() const
{
	return m_Text;
}

void Narrator::Runtime::DialogueNode::SetText(const std::string& i_Text)
{
	m_Text = i_Text;
}

void Narrator::Runtime::DialogueNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::UniInFlowNode::ToJSON(nodeObject);
	Narrator::Runtime::UniOutFlowNode::ToJSON(nodeObject);

	nlohmann::json dialogue = nlohmann::json::object();
	dialogue["text"] = m_Text;
	nodeObject["dialogue"] = dialogue;
}

void Narrator::Runtime::DialogueNode::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	Narrator::Runtime::Node::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::UniInFlowNode::FromJSON(nodeObject, i_Graph);
	Narrator::Runtime::UniOutFlowNode::FromJSON(nodeObject, i_Graph);

	const nlohmann::json dialogueObject = nodeObject["dialogue"];
	m_Text = dialogueObject["text"];
}
