#include "NodeFactory.h"
#include "StartNode.h"
#include "EndNode.h"
#include "DivertNode.h"
#include "KnotNode.h"
#include "ChoiceNode.h"
#include "DecisionNode.h"
#include "DialogueNode.h"

Narrator::Runtime::Node* Narrator::Runtime::NodeFactory::Create(const Narrator::Runtime::TNodeType i_NodeType)
{
	switch (i_NodeType)
	{
	default:
	case Narrator::Runtime::TNodeType::kNodeBase:
	{
		//TODO: #NarratorToDoAssert #RuntimeError
	}
	break;
	case Narrator::Runtime::TNodeType::kStart:
	{
		return new Narrator::Runtime::StartNode();
	}
	break;
	case Narrator::Runtime::TNodeType::kEnd:
	{
		return new Narrator::Runtime::EndNode();
	}
	break;
	case Narrator::Runtime::TNodeType::kDialogue:
	{
		return new Narrator::Runtime::DialogueNode();
	}
	break;
	case Narrator::Runtime::TNodeType::kDivert:
	{
		return new Narrator::Runtime::DivertNode();
	}
	break;
	case Narrator::Runtime::TNodeType::kKnot:
	{

		return new Narrator::Runtime::KnotNode();
	}
	break;
	case Narrator::Runtime::TNodeType::kChoice:
	{

		return new Narrator::Runtime::ChoiceNode();
	}
	break;
	case Narrator::Runtime::TNodeType::kDecision:
	{

		return new Narrator::Runtime::DecisionNode();
	}
	break;
	}

	return nullptr;
}
