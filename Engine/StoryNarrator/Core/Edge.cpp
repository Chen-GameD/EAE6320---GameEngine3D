#include "Edge.h"
#include "Node.h"

std::uint64_t Narrator::Runtime::Edge::m_CurrentIndex = 0;

std::uint64_t Narrator::Runtime::Edge::GetTotalEdgeCount()
{
	return m_CurrentIndex;
}

std::uint64_t Narrator::Runtime::Edge::GetMaxEdgeCount()
{
	return std::numeric_limits<uint64_t>::max();
}

void Narrator::Runtime::Edge::ResetEdgeCount()
{
	m_CurrentIndex = 0;
}

Narrator::Runtime::Edge::Edge()
	:m_ID(m_CurrentIndex++),
	m_SourceNode(nullptr),
	m_DestinationNode(nullptr)
{

}

Narrator::Runtime::Edge::Edge(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_DestinationNode)
	:m_ID(m_CurrentIndex++),
	m_SourceNode(i_SourceNode),
	m_DestinationNode(i_DestinationNode)
{

}

Narrator::Runtime::Edge::~Edge()
{

}

std::string Narrator::Runtime::Edge::GetName() const
{
	const std::string edgeIDString = std::to_string(m_ID);
	std::string startNodeIDString("0");
	std::string endNodeIDString("0");

	if (m_SourceNode)
	{
		startNodeIDString = m_SourceNode->GetName();
	}

	if (m_DestinationNode)
	{
		endNodeIDString = m_DestinationNode->GetName();
	}

	const std::string edgeName = edgeIDString + "_" + startNodeIDString + "_" + endNodeIDString;
	return edgeName;
}

std::uint64_t Narrator::Runtime::Edge::GetID() const
{
	return m_ID;
}
