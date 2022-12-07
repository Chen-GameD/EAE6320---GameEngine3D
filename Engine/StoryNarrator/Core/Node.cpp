#include "Node.h"
// #include "Graph.h"
#include <iostream>

std::uint32_t Narrator::Runtime::Node::m_CurrentIndex = 0;

std::string Narrator::Runtime::Node::ToString()
{
	return GetName();
}

size_t Narrator::Runtime::Node::GetTotalNodeCount()
{
	return m_CurrentIndex;
}

size_t Narrator::Runtime::Node::GetMaxNodeCount()
{
	return std::numeric_limits<uint32_t>::max();
}

void Narrator::Runtime::Node::ResetNodeCount()
{
	m_CurrentIndex = 0;
}

Narrator::Runtime::Node::Node(TNodeType i_Type /*= TNodeType::kNone*/)
	: m_ID(m_CurrentIndex++),
	m_Type(i_Type),
	m_InFlowType(TInFlowType::kNone),
	m_OutFlowType(TOutFlowType::kNone),
	m_IsVisited(false)
{
	m_Name = std::to_string(m_ID);
}

Narrator::Runtime::Node::~Node()
{

}

Narrator::Runtime::TNodeType Narrator::Runtime::Node::GetType() const
{
	return m_Type;
}

std::string Narrator::Runtime::Node::GetName() const
{
	if (m_Name.empty())
	{
		return std::to_string(m_ID);
	}
	return m_Name;
}

std::uint32_t Narrator::Runtime::Node::GetID() const
{
	return m_ID;
}

Narrator::Runtime::TInFlowType Narrator::Runtime::Node::GetInFlowType() const
{
	return m_InFlowType;
}

Narrator::Runtime::TOutFlowType Narrator::Runtime::Node::GetOutFlowType() const
{
	return m_OutFlowType;
}

bool Narrator::Runtime::Node::IsVisited() const
{
	return m_IsVisited;
}

void Narrator::Runtime::Node::MarkAsVisited()
{
	m_IsVisited = true;
}

void Narrator::Runtime::Node::ToJSON(nlohmann::json& nodeObject)
{
	nodeObject["id"] = m_ID;
	nodeObject["name"] = m_Name;
	nodeObject["type"] = m_Type;
	nodeObject["in_flow_type"] = m_InFlowType;
	nodeObject["out_flow_type"] = m_OutFlowType;
}

void Narrator::Runtime::Node::FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph)
{
	//already existing ID should match the node
	const std::uint32_t nodeID = nodeObject["id"];
	if (m_ID == nodeID)
	{
		m_ID = nodeObject["id"];
		m_Name = nodeObject["name"];
		m_Type = nodeObject["type"];
		m_InFlowType = nodeObject["in_flow_type"];
		m_OutFlowType = nodeObject["out_flow_type"];

	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError
	}
}

void Narrator::Runtime::Node::SetName(const std::string& i_Name)
{
	m_Name = i_Name;
}

std::vector<std::uint64_t> Narrator::Runtime::Node::GetParseLineNumberList() const
{
	return m_ParseLineNumberList;
}

void Narrator::Runtime::Node::AddParseLineNumber(std::uint64_t i_ParseLineNumber)
{
	m_ParseLineNumberList.emplace_back(i_ParseLineNumber);
}
