#include "Story.h"
#include "StorySyntax.h"
#include "ParseMetaData.h"

#include "../Core/StartNode.h"
#include "../Core/EndNode.h"
#include "../Core/Edge.h"
#include "../Core/DecisionNode.h"
#include "../Core/DialogueNode.h"
#include "../Core/DivertNode.h"
#include "../Core/KnotNode.h"
#include "../Core/ChoiceNode.h"
#include "../Core/Graph.h"
#include "../Utils/StringUtils.h"

#include "../JSON/Includes.h"
#include "../Core/NodeFactory.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <queue>
#include <iomanip>

Narrator::Runtime::Story::Story()
	: Narrator::Runtime::Graph(),
	m_IsParseErrorsPresent(false),
	m_StartNode(nullptr),
	m_EndNode(nullptr),
	m_CurrentNode(nullptr),
	m_LastDecisionNode(nullptr),
	m_LastChoiceNode(nullptr),
	m_IsAValidKnotSection(false),
	m_canRead(false)
{
	Narrator::Runtime::Node::ResetNodeCount();
	Narrator::Runtime::Edge::ResetEdgeCount();

	m_StartNode = new Narrator::Runtime::StartNode();
	AddToNodeMap(m_StartNode);
	m_EndNode = new Narrator::Runtime::EndNode();
	AddToNodeMap(m_EndNode);

	m_CurrentNode = m_StartNode;

}

Narrator::Runtime::Story::~Story()
{

}

bool Narrator::Runtime::Story::canRead()
{
	return m_canRead;
}

std::string Narrator::Runtime::Story::Read()
{
	std::string storyText;

	if (m_CurrentNode)
	{
		Narrator::Runtime::TNodeType currentNodeType = m_CurrentNode->GetType();
		Narrator::Runtime::TOutFlowType outFlowType = m_CurrentNode->GetOutFlowType();

		Narrator::Runtime::Node* nextNode = nullptr;

		switch (outFlowType)
		{
		default:
		case Narrator::Runtime::TOutFlowType::kNone:
		{
			//TODO: #NarratorToDoAssert #RuntimeError
		}
		break;
		case Narrator::Runtime::TOutFlowType::kUniOutFlow:
		{
			Narrator::Runtime::UniOutFlowNode* outFlowNode = dynamic_cast<Narrator::Runtime::UniOutFlowNode*>(m_CurrentNode);
			if (outFlowNode)
			{
				nextNode = outFlowNode->GetNextNode();
			}
			else
			{
				//TODO: #NarratorToDoAssert #RuntimeError
			}
		}
		break;
		case Narrator::Runtime::TOutFlowType::kMultiOutFlow:
		{
			SetCanRead(false);
		}
		break;
		}

		switch (currentNodeType)
		{
		default:
		case Narrator::Runtime::TNodeType::kNodeBase:
		{
			//TODO: #NarratorToDoAssert #RuntimeError
		}
		break;
		case Narrator::Runtime::TNodeType::kStart:
		{
			SetCanRead(true);
			m_CurrentNode = nextNode;
		}
		break;
		case Narrator::Runtime::TNodeType::kEnd:
		{
			SetCanRead(false);
			m_CurrentNode = nextNode;
			storyText = "\n" + std::string("End of Story");
		}
		break;
		case Narrator::Runtime::TNodeType::kDialogue:
		{
			Narrator::Runtime::DialogueNode* dialogueNode = dynamic_cast<Narrator::Runtime::DialogueNode*>(m_CurrentNode);
			if (dialogueNode)
			{
				storyText = dialogueNode->GetText();
				m_CurrentNode = nextNode;
			}
			else
			{
				//TODO: #NarratorToDoAssert #RuntimeError
			}
		}
		break;
		case Narrator::Runtime::TNodeType::kDivert:
		{
			SetCanRead(true);
			m_CurrentNode = nextNode;
			return "" + Read();
		}
		break;
		case Narrator::Runtime::TNodeType::kKnot:
		{
			SetCanRead(true);
			m_CurrentNode = nextNode;
			return "" + Read();
		}
		break;
		case Narrator::Runtime::TNodeType::kChoice:
		{
			SetCanRead(true);
			m_CurrentNode = nextNode;
			return "" + Read();
		}
		break;
		case Narrator::Runtime::TNodeType::kDecision:
		{
			ReadChoices(m_CurrentNode);
			SetCanRead(false);
		}
		break;
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError
	}

	return storyText;
}

std::vector<std::string> Narrator::Runtime::Story::GetChoices()
{
	return m_CurrentChoices;
}

void Narrator::Runtime::Story::SelectChoice(uint32_t i_ChoiceIndex)
{
	if (m_CurrentChoices.size() > 0)
	{
		if (i_ChoiceIndex >= 0 && i_ChoiceIndex < m_CurrentChoices.size())
		{
			Narrator::Runtime::DecisionNode* decisionNode = dynamic_cast<Narrator::Runtime::DecisionNode*>(m_CurrentNode);
			if (decisionNode)
			{
				std::map<std::uint32_t, Narrator::Runtime::Node*> outFlowNodeMap = decisionNode->GetOutFlowNodeMap();
				std::uint32_t mapIndex = 0;
				for (std::map<std::uint32_t, Narrator::Runtime::Node*>::iterator mapIterator = outFlowNodeMap.begin();
					mapIterator != outFlowNodeMap.end();
					mapIterator++, mapIndex++)
				{
					if (mapIndex == i_ChoiceIndex)
					{
						m_CurrentNode = mapIterator->second;
						if (m_CurrentNode)
						{
							m_CurrentChoices.clear();
							SetCanRead(true);
							return;
						}
						else
						{
							//TODO: #NarratorToDoAssert #RuntimeError
						}
					}
				}
			}
			else
			{
				//TODO: #NarratorToDoAssert #RuntimeError
			}
		}
		else
		{
			//TODO: #NarratorToDoAssert #RuntimeError
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError		
	}
}

bool Narrator::Runtime::Story::IsValid() const
{
	return !GetIsParseErrorsPresent();
}

Narrator::Runtime::Story Narrator::Runtime::Story::Parse(const std::string& i_PathToRead, const std::string& i_PathToWrite, bool i_IsPlayMode/*= false*/)
{
	std::map<std::uint64_t, std::string> FileLineMap;

	//Read a text file
	std::ifstream inFile(i_PathToRead);
	std::string line;
	std::uint64_t lineNumber = 0;
	if (inFile.is_open())
	{
		while (std::getline(inFile, line))
		{
			lineNumber++;
			std::istringstream iss(line);
			// 		std::cout << line << std::endl;
			std::string  lineTrim = Narrator::Runtime::StringUtils::TrimCopy(line);
			if (!lineTrim.empty())
			{
				FileLineMap.insert(std::pair<std::uint64_t, std::string>(lineNumber, lineTrim));
			}
		}
		inFile.close();
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError Error opening file
	}

	//Create A Story Graph
	//Narrator::Runtime::Story* story = new Narrator::Runtime::Story();
	Narrator::Runtime::Story story;

	//Parse the Content
	std::uint64_t previousFileLineNumber = 0;
	for (std::map<std::uint64_t, std::string>::iterator fileLineMapIterator = FileLineMap.begin();
		fileLineMapIterator != FileLineMap.end();
		fileLineMapIterator++)
	{
		const std::uint64_t& fileLineNumber = fileLineMapIterator->first;
		const std::string& fileLineString = fileLineMapIterator->second;

		//std::cout << fileLineMapIterator->first << " : " << fileLineString << std::endl;

		if (Narrator::Runtime::StringUtils::StartsWith(fileLineString, Narrator::Runtime::StorySyntax::KNOT_DECLARATION))
		{
			//Get the Knot Name
			std::string knotName;
			std::string tLine = fileLineString;
			Narrator::Runtime::StringUtils::RemoveAll(tLine, Narrator::Runtime::StorySyntax::KNOT_DECLARATION);
			std::string nameToCheck = Narrator::Runtime::StringUtils::TrimCopy(tLine);
			if (nameToCheck.empty())
			{
				std::string message = "Knot name cannot be empty";
				story.AddParseMetaData(Narrator::Parser::TParseMessageType::kError, fileLineNumber, message);
			}
			else
			{
				if (Narrator::Runtime::StringUtils::IsValidKnotName(nameToCheck))
				{
					knotName = nameToCheck;
					//Check the Knot Node is already present
					if (story.HasKnotNode(knotName))
					{
						std::string message = "Redefinition of knot name : " + knotName;
						story.AddParseMetaData(Narrator::Parser::TParseMessageType::kError, fileLineNumber, message);
					}
					else
					{
						if (story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kKnot)
						{
							std::string currentKnotName = story.GetCurrentNode()->GetName();
							std::string message = "At least One Line expected within knot : " + currentKnotName;
							story.AddParseMetaData(Narrator::Parser::TParseMessageType::kError, previousFileLineNumber, message);
						}

						if (!story.m_IsAValidKnotSection)
						{
							std::string message = "Invalid Knot Flow. Missing DONE / END / Divert.";
							story.AddParseMetaData(Narrator::Parser::TParseMessageType::kError, previousFileLineNumber, message);
						}

						//Create Knot Node
						Narrator::Runtime::KnotNode* knotNode = new Narrator::Runtime::KnotNode();
						knotNode->SetName(knotName);
						knotNode->AddParseLineNumber(fileLineNumber);
						story.AddNode(knotNode);
						story.AddToKnotNodeMap(knotName, knotNode);


						//Clear Knot Validity when creating a new Knot
						story.m_IsAValidKnotSection = false;
					}
				}
				else
				{
					std::string message = "Not a valid knot name : " + nameToCheck;
					story.AddParseMetaData(Narrator::Parser::TParseMessageType::kError, fileLineNumber, message);
				}
			}
			story.ClearLastDecisionNode();
		}
		else if (Narrator::Runtime::StringUtils::StartsWith(fileLineString, Narrator::Runtime::StorySyntax::CHOICE_DECLARATION))
		{
			if (story.GetLastDecisionNode() == nullptr)
			{
				if (story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kDivert)
				{
					std::string message = "Unreachable Choice after a divert.";
					story.AddParseMetaData(Narrator::Parser::TParseMessageType::kWarning, fileLineNumber, message);
				}
			}
			//Decision Point
			Narrator::Runtime::DecisionNode* decisionNode = dynamic_cast<Narrator::Runtime::DecisionNode*>(story.CreateDecisionNode());

			if (decisionNode)
			{
				//Cache Decision Node for Knot Validity
				story.m_IsAValidKnotSection = true;

				std::string tLine = fileLineString;
				Narrator::Runtime::StringUtils::RemoveAll(tLine, Narrator::Runtime::StorySyntax::CHOICE_DECLARATION);
				std::string choiceText = Narrator::Runtime::StringUtils::TrimCopy(tLine);
				if (choiceText.empty())
				{
					std::string message = "Choice Text cannot be empty.";
					story.AddParseMetaData(Narrator::Parser::TParseMessageType::kError, fileLineNumber, message);
				}
				else
				{
					std::uint32_t choiceIndex = decisionNode->GetDecisionPathCount();
					//Create Choice Node
					Narrator::Runtime::ChoiceNode* choiceNode = new Narrator::Runtime::ChoiceNode(choiceIndex, choiceText);
					story.AddNode(choiceNode);
					story.AddNodeLink(decisionNode, choiceNode);
				}
			}
			else
			{
				std::cout << "Error while Creating Decision Node" << std::endl;
				std::string message = "Error while Creating Decision Node.";
				story.AddParseMetaData(Narrator::Parser::TParseMessageType::kCritical, fileLineNumber, message);
			}

		}
		else if (Narrator::Runtime::StringUtils::StartsWith(fileLineString, Narrator::Runtime::StorySyntax::DIVERT_DECLARATION))
		{

			if (story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kDivert
				|| story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kEnd)
			{
				std::string message = "Unreachable Divert after a Divert / END / DONE.";
				story.AddParseMetaData(Narrator::Parser::TParseMessageType::kError, fileLineNumber, message);
			}

			std::string tLine = fileLineString;
			//tLine = Narrator::Runtime::StringUtils::RemoveAllSpaces(tLine);

			tLine = Narrator::Runtime::StringUtils::TrimCopy(tLine);
			std::string possibleDivertName = tLine;
			Narrator::Runtime::StringUtils::RemoveAll(possibleDivertName, Narrator::Runtime::StorySyntax::DIVERT_DECLARATION);
			possibleDivertName = Narrator::Runtime::StringUtils::TrimCopy(possibleDivertName);

			if (possibleDivertName.empty())
			{
				std::string message = "The Divert Name should not be empty.";
				story.AddParseMetaData(Narrator::Parser::TParseMessageType::kError, fileLineNumber, message);
			}
			else
			{
				if (Narrator::Runtime::StringUtils::IsValidKnotName(possibleDivertName))
				{
					if (Narrator::Runtime::StringUtils::StartsWithIgnoreCase(possibleDivertName, Narrator::Runtime::StorySyntax::END_DECLARATION))
					{
						//Cache Decision Node for Knot Validity
						story.m_IsAValidKnotSection = true;

						//End Node
						story.LinkEndNode();
					}
					else if (Narrator::Runtime::StringUtils::StartsWithIgnoreCase(possibleDivertName, Narrator::Runtime::StorySyntax::DONE_DECLARATION))
					{
						//Cache Decision Node for Knot Validity
						story.m_IsAValidKnotSection = true;

						//Done Node is a divert node that redirects to the end node by default
						story.LinkEndNode();
					}
					else
					{
						Narrator::Runtime::DivertNode* divertNode = nullptr;

						//Get the Divert Name
						std::string divertName = possibleDivertName;

						//Check the Divert Node is already present
						if (story.HasDivertNode(divertName))
						{
							//Get the existing Divert Node
							Narrator::Runtime::Node* foundNode = story.GetDivertNode(divertName);
							divertNode = dynamic_cast<Narrator::Runtime::DivertNode*>(foundNode);
						}
						else
						{
							//Create Divert Node
							divertNode = new Narrator::Runtime::DivertNode();
							divertNode->SetTargetNodeName(divertName);
						}

						//Add the Node to the story
						if (divertNode)
						{
							divertNode->AddParseLineNumber(fileLineNumber);

							//Cache Decision Node for Knot Validity
							story.m_IsAValidKnotSection = true;

							story.AddNode(divertNode);
							story.AddToDivertNodeMap(divertName, divertNode);
						}
						else
						{
							std::cout << "The Divert Node should not be nullptr" << std::endl;
							std::string message = "The Divert Node should not be nullptr.";
							story.AddParseMetaData(Narrator::Parser::TParseMessageType::kCritical, fileLineNumber, message);
						}
					}
				}
				else
				{
					std::string message = "Not a Valid Divert Name : " + possibleDivertName;
					story.AddParseMetaData(Narrator::Parser::TParseMessageType::kError, fileLineNumber, message);
				}
			}
		}
		else
		{
			if (story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kDivert
				|| story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kEnd)
			{
				std::string message = "Unreachable Dialogue after a divert / END / DONE.";
				story.AddParseMetaData(Narrator::Parser::TParseMessageType::kWarning, fileLineNumber, message);
			}

			//Create Dialogue Node
			Narrator::Runtime::DialogueNode* dialogueNode = new Narrator::Runtime::DialogueNode(fileLineString);
			story.AddNode(dialogueNode);
		}

		previousFileLineNumber = fileLineNumber;
	}


	{
		std::map<std::string, Narrator::Runtime::Node*> divertNodeMap = story.GetDivertNodeMap();
		//Check the Validity of the Divert Nodes
		for (std::map<std::string, Narrator::Runtime::Node*>::iterator divertNodeMapIterator = divertNodeMap.begin();
			divertNodeMapIterator != divertNodeMap.end();
			divertNodeMapIterator++)
		{
			Narrator::Runtime::DivertNode* divertNode = dynamic_cast<Narrator::Runtime::DivertNode*>(divertNodeMapIterator->second);
			if (divertNode)
			{
				std::string divertTargetName = divertNode->GetTargetNodeName();
				if (story.HasKnotNode(divertTargetName))
				{
					Narrator::Runtime::KnotNode* knotNode = dynamic_cast<Narrator::Runtime::KnotNode*>(story.GetKnotNode(divertTargetName));
					story.AddNodeLink(divertNode, knotNode);
					divertNode->SetTargetNode(knotNode);
				}
				else
				{
					std::vector<std::uint64_t> lineNumbers = divertNode->GetParseLineNumberList();
					for (const std::uint64_t& lineNumber : lineNumbers)
					{
						std::string message = "Divert Target not Found : " + divertTargetName;
						story.AddParseMetaData(Narrator::Parser::TParseMessageType::kError, lineNumber, message);
					}
				}
			}
			else
			{
				std::cout << "DivertNode should not be nullptr" << std::endl;
				std::string message = "DivertNode should not be nullptr";
				story.AddParseMetaData(Narrator::Parser::TParseMessageType::kCritical, 0, message);
			}
		}
	}

	{
		std::map<std::string, Narrator::Runtime::Node*> knotNodeMap = story.GetKnotNodeMap();
		//Check the Reachability of the Knot Nodes
		for (std::map<std::string, Narrator::Runtime::Node*>::iterator knotNodeMapIterator = knotNodeMap.begin();
			knotNodeMapIterator != knotNodeMap.end();
			knotNodeMapIterator++)
		{
			Narrator::Runtime::KnotNode* knotNode = dynamic_cast<Narrator::Runtime::KnotNode*>(knotNodeMapIterator->second);
			if (knotNode)
			{
				const std::string& knotNodeName = knotNode->GetName();
				if (!story.HasDivertNode(knotNodeName))
				{
					std::vector<std::uint64_t> lineNumbers = knotNode->GetParseLineNumberList();
					for (const std::uint64_t& lineNumber : lineNumbers)
					{
						std::string message = "Unreachable Knot Section Found : " + knotNodeName;
						story.AddParseMetaData(Narrator::Parser::TParseMessageType::kWarning, lineNumber, message);
					}
				}
			}
			else
			{
				std::cout << "KnotNode should not be nullptr" << std::endl;
				std::string message = "KnotNode should not be nullptr";
				story.AddParseMetaData(Narrator::Parser::TParseMessageType::kCritical, 0, message);
			}
		}
	}


	//Check for Empty Document
	Narrator::Runtime::StartNode* startNode = dynamic_cast<Narrator::Runtime::StartNode*>(story.m_StartNode);
	if (startNode)
	{
		Narrator::Runtime::Node* nextNode = startNode->GetNextNode();
		if (!nextNode)
		{
			std::cout << "Empty Document is invalid." << std::endl;
			std::string message = "Empty Document is invalid.";
			story.AddParseMetaData(Narrator::Parser::TParseMessageType::kCritical, 0, message);
		}
		else
		{
			//Check For loose knots
			if (!story.m_IsAValidKnotSection)
			{
				std::string message = "Invalid Knot Flow. Missing DONE / END / Divert.";
				story.AddParseMetaData(Narrator::Parser::TParseMessageType::kError, previousFileLineNumber, message);
			}
		}
	}


	//Traverse the Graph to validate the flow
	story.Traverse();

	std::multimap<std::uint64_t, Narrator::Parser::ParseMetaData> parseMetaDataMap = story.GetParseMetaDataMap();
	for (std::multimap<std::uint64_t, Narrator::Parser::ParseMetaData>::iterator multiMapIterator = parseMetaDataMap.begin();
		multiMapIterator != parseMetaDataMap.end();
		multiMapIterator++)
	{
		const std::uint64_t lineNumber = multiMapIterator->first;
		const Narrator::Parser::ParseMetaData parseMetaData = multiMapIterator->second;
		const Narrator::Parser::TParseMessageType parseMetaDataType = parseMetaData.GetParseMessageType();
		std::string typeString = "";

		switch (parseMetaDataType)
		{
		default:
			break;
		case Narrator::Parser::TParseMessageType::kTrace:
		{
			typeString = "Trace";
		}
		break;
		case Narrator::Parser::TParseMessageType::kDebug:
		{
			typeString = "Debug";
		}
		break;
		case Narrator::Parser::TParseMessageType::kInfo:
		{
			typeString = "Info";
		}
		break;
		case Narrator::Parser::TParseMessageType::kWarning:
		{
			typeString = "Warning";
		}
		break;
		case Narrator::Parser::TParseMessageType::kError:
		{
			story.SetIsParseErrorsPresent(true);
			typeString = "Error";
		}
		break;
		case Narrator::Parser::TParseMessageType::kCritical:
		{
			story.SetIsParseErrorsPresent(true);
			typeString = "Critical";
		}
		break;
		}

		if (lineNumber != 0)
		{
			std::cout << lineNumber << " : " + typeString + " : " + parseMetaData.GetMessage() << std::endl;
		}
		else
		{
			std::cout << typeString + " : " + parseMetaData.GetMessage() << std::endl;
		}
	}

	//Only Export the JSON file if there are no parsing errors
	if (story.GetIsParseErrorsPresent())
	{
		std::cout << "Resolve the errors in the document." << std::endl;
	}
	else
	{
		story.ToJSONFile(i_PathToWrite);
	}

	return story;
}


void Narrator::Runtime::Story::Play()
{
	Narrator::Runtime::Node* currentNode = m_StartNode;
	while (canRead())
	{
		while (canRead())
		{
			std::string dialogue = Read();
			if (!dialogue.empty())
			{
				std::cout << dialogue << std::endl;
			}
		}

		std::vector<std::string> choices = GetChoices();
		size_t choiceCount = choices.size();
		if (choiceCount > 0)
		{
			size_t choiceIndex = 0;
			for (const std::string& choiceText : choices)
			{
				std::cout << "Choice Index: " << choiceIndex << "\t" << choiceText << std::endl;
				choiceIndex++;
			}

			size_t selectChoiceIndex = 0;			
			do
			{
				std::cout << "Select Choice Index: ";
				std::cin >> selectChoiceIndex;
			} while ((selectChoiceIndex < 0) || (selectChoiceIndex >= choiceCount));				

			
			std::cout << "Selecting Choice Index: " << selectChoiceIndex << std::endl;
			SelectChoice((std::uint32_t)selectChoiceIndex);
		}
		else
		{
			//End of Story
			break;
		}
	}
	//std::cout << "End of Story" << std::endl;
}

bool Narrator::Runtime::Story::ToJSONFile(const std::string& i_JSONFilePath)
{
	std::ofstream fout(i_JSONFilePath.c_str());
	if (fout.is_open())
	{
		nlohmann::json storyJSON;

		ToJSON(storyJSON);

		//Save the File
		fout << std::setw(4) << storyJSON << std::endl;

		fout.close();

		std::cout << "Write file complete" << std::endl;
	}
	else
	{
		std::cout << "Could not write to file" << std::endl;
	}

	return false;
}

bool Narrator::Runtime::Story::FromJSONFile(const std::string& i_JSONFilePath)
{
	std::ifstream fin(i_JSONFilePath.c_str());

	if (fin.is_open())
	{
		const nlohmann::json storyJSON = nlohmann::json::parse(fin);

		FromJSON(storyJSON);

		fin.close();

		std::cout << "Read file complete" << std::endl;
	}
	else
	{
		std::cout << "Could not read to file" << std::endl;
	}

	//TODO: #NarratorToDo Reset Story Read State
	m_CurrentNode = m_StartNode;
	m_CurrentChoices.clear();
	Read();

	return false;
}

bool Narrator::Runtime::Story::GetIsParseErrorsPresent() const
{
	return m_IsParseErrorsPresent;
}

void Narrator::Runtime::Story::SetIsParseErrorsPresent(bool i_IsParseErrorsPresent)
{
	m_IsParseErrorsPresent = i_IsParseErrorsPresent;
}

void Narrator::Runtime::Story::ToJSON(nlohmann::json& jsonRoot)
{
	nlohmann::json nodeArray = nlohmann::json::array();

	//Nodes
	for (std::map<uint32_t, Narrator::Runtime::Node*>::iterator mapIterator = m_NodeMap.begin();
		mapIterator != m_NodeMap.end();
		mapIterator++)
	{
		nlohmann::json nodeObject = nlohmann::json::object();
		mapIterator->second->ToJSON(nodeObject);
		nodeArray.emplace_back(nodeObject);
	}
	jsonRoot["nodes"] = nodeArray;

	nlohmann::json edgeArray = nlohmann::json::array();

	//Edges
	for (std::map<uint64_t, Narrator::Runtime::Edge*>::iterator mapIterator = m_EdgeMap.begin();
		mapIterator != m_EdgeMap.end();
		mapIterator++)
	{
		mapIterator;
	}
	jsonRoot["edges"] = edgeArray;

	nlohmann::json adjacencyListObject = nlohmann::json::object();

	//AdjacencyList
	for (std::map<uint32_t, std::vector<Narrator::Runtime::Node*>>::iterator mapIterator = m_AdjacencyListMap.begin();
		mapIterator != m_AdjacencyListMap.end();
		mapIterator++)
	{
		mapIterator;
	}
	jsonRoot["adjacency_list"] = adjacencyListObject;
}

void Narrator::Runtime::Story::FromJSON(const nlohmann::json& jsonRoot)
{
	if (jsonRoot.is_object())
	{
		const nlohmann::json nodeArray = jsonRoot["nodes"];
		if (nodeArray.is_array())
		{
			for (const nlohmann::json& nodeElement : nodeArray)
			{
				if (nodeElement.is_object())
				{
					if (nodeElement.contains("type"))
					{
						Narrator::Runtime::TNodeType nodeType = nodeElement["type"];
						//Start Node and End Node are created by default constructor
						if ((nodeType != TNodeType::kStart) && (nodeType != TNodeType::kEnd))
						{
							Narrator::Runtime::Node* newNode = NodeFactory::Create(nodeType);
							AddToNodeMap(newNode);
						}
					}
					else
					{
						//TODO: #NarratorToDoAssert #RuntimeError
					}
				}
				else
				{
					//TODO: #NarratorToDoAssert #RuntimeError
				}
			}


		}
		else
		{
			//TODO: #NarratorToDoAssert #RuntimeError
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError
	}

	if (jsonRoot.is_object())
	{
		const nlohmann::json nodeArray = jsonRoot["nodes"];
		if (nodeArray.is_array())
		{
			for (const nlohmann::json& nodeElement : nodeArray)
			{
				if (nodeElement.is_object())
				{
					if (nodeElement.contains("type"))
					{
						Narrator::Runtime::TNodeType nodeType = nodeElement["type"];
						switch (nodeType)
						{
						default:
						case Narrator::Runtime::TNodeType::kNodeBase:
						{
							//TODO: #NarratorToDoAssert #RuntimeError
						}
						break;
						case Narrator::Runtime::TNodeType::kStart:
						{
							m_StartNode->FromJSON(nodeElement, this);
						}
						break;
						case Narrator::Runtime::TNodeType::kEnd:
						{
							m_EndNode->FromJSON(nodeElement, this);
						}
						break;
						case Narrator::Runtime::TNodeType::kDialogue:
						case Narrator::Runtime::TNodeType::kDivert:
						case Narrator::Runtime::TNodeType::kKnot:
						case Narrator::Runtime::TNodeType::kChoice:
						case Narrator::Runtime::TNodeType::kDecision:
						{
							Narrator::Runtime::Node* currentNode = m_NodeMap[nodeElement["id"]];
							currentNode->FromJSON(nodeElement, this);
						}
						break;
						}
					}
					else
					{
						//TODO: #NarratorToDoAssert #RuntimeError
					}
				}
				else
				{
					//TODO: #NarratorToDoAssert #RuntimeError
				}
			}
		}
		else
		{
			//TODO: #NarratorToDoAssert #RuntimeError
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError
	}

	Traverse();

	std::cout << "Read Complete" << std::endl;
}

void Narrator::Runtime::Story::Traverse()
{
	BreadthFirstSearch();
}

void Narrator::Runtime::Story::BreadthFirstSearch()
{
	std::queue<Narrator::Runtime::Node*> nodeQueue;

	for (std::map<uint32_t, Narrator::Runtime::Node*>::iterator mapIterator = m_NodeMap.begin();
		mapIterator != m_NodeMap.end();
		mapIterator++)
	{
		Narrator::Runtime::Node* currentNode = mapIterator->second;
		if (!currentNode->IsVisited())
		{
			nodeQueue.push(currentNode);
		}

		while (!nodeQueue.empty())
		{
			Narrator::Runtime::Node* frontNode = nodeQueue.front();
			frontNode->MarkAsVisited();

			//Print to standard output stream using std::cout
			//std::cout << frontNode->ToString() << std::endl;

			nodeQueue.pop();

			//TODO: #NarratorToDo Check the Previous Valid Nodes and add error checks for invalid flow

			if (frontNode)
			{
				std::vector<Narrator::Runtime::Node*> adjacentList = m_AdjacencyListMap[frontNode->GetID()];

				for (std::vector<Narrator::Runtime::Node*>::iterator adjacentListIterator = adjacentList.begin();
					adjacentListIterator != adjacentList.end();
					adjacentListIterator++)
				{
					Narrator::Runtime::Node* adjacentNode = *adjacentListIterator;
					if (!adjacentNode->IsVisited())
					{
						adjacentNode->MarkAsVisited();
						nodeQueue.push(adjacentNode);
					}
				}
			}
			else
			{
				//TODO: #NarratorToDoAssert Throw Parse Error. This should not be null.
				std::cout << "FrontNode in the queue should not be null" << std::endl;
			}
		}
	}
}

void Narrator::Runtime::Story::AddNode(Narrator::Runtime::Node* i_NodeToAdd)
{
	AddEdge(m_CurrentNode, i_NodeToAdd);
	m_CurrentNode = i_NodeToAdd;
}

void Narrator::Runtime::Story::LinkEndNode()
{
	AddEdge(m_CurrentNode, m_EndNode);
	//TODO: #NarratorToDo #HighPriority If its an END Node is it necessary to make it as CurrentNode?
	m_CurrentNode = m_EndNode;
}

bool Narrator::Runtime::Story::HasDivertNode(const std::string& i_DivertName)
{
	return HasRedirectionNode(i_DivertName);
}

bool Narrator::Runtime::Story::HasKnotNode(const std::string& i_KnotName)
{
	return HasSubGraphStartNode(i_KnotName);
}

Narrator::Runtime::Node* Narrator::Runtime::Story::GetKnotNode(const std::string& i_KnotName)
{
	//TODO: #NarratorToDo Optimize this logic to just return the Node without checking / Return a nullptr if its not present
	if (HasDivertNode(i_KnotName))
	{
		return GetSubGraphStartNode(i_KnotName);
	}
	else
	{
		return nullptr;
	}
}

Narrator::Runtime::Node* Narrator::Runtime::Story::GetCurrentNode() const
{
	return m_CurrentNode;
}

Narrator::Runtime::Node* Narrator::Runtime::Story::GetDivertNode(const std::string& i_DivertName)
{
	//TODO: #NarratorToDo Optimize this logic to just return the Node without checking / Return a nullptr if its not present
	if (HasDivertNode(i_DivertName))
	{
		return GetRedirectionNode(i_DivertName);
	}
	else
	{
		return nullptr;
	}
}

void Narrator::Runtime::Story::AddToDivertNodeMap(const std::string& i_DivertName, Narrator::Runtime::Node* i_DivertNode)
{
	AddToRedirectionNodeMap(i_DivertName, i_DivertNode);
}

std::map<std::string, Narrator::Runtime::Node*> Narrator::Runtime::Story::GetDivertNodeMap()
{
	return m_RedirectionNodeMap;
}

void Narrator::Runtime::Story::AddToKnotNodeMap(const std::string& i_KnotName, Narrator::Runtime::Node* i_KnotNode)
{
	AddToSubGraphStartNodeMap(i_KnotName, i_KnotNode);
}

std::map<std::string, Narrator::Runtime::Node*> Narrator::Runtime::Story::GetKnotNodeMap()
{
	return m_SubGraphStartNodeMap;
}

Narrator::Runtime::Node* Narrator::Runtime::Story::GetLastDecisionNode() const
{
	return m_LastDecisionNode;
}

Narrator::Runtime::Node* Narrator::Runtime::Story::CreateDecisionNode()
{
	if (m_LastDecisionNode == nullptr)
	{
		Narrator::Runtime::DecisionNode* newDecisionNode = new Narrator::Runtime::DecisionNode();
		AddEdge(m_CurrentNode, newDecisionNode);

		m_CurrentNode = newDecisionNode;
		m_LastDecisionNode = newDecisionNode;
	}

	return m_LastDecisionNode;
}

void Narrator::Runtime::Story::ClearLastDecisionNode()
{
	m_LastDecisionNode = nullptr;
}

bool Narrator::Runtime::Story::GetCanRead() const
{
	return m_canRead;
}

void Narrator::Runtime::Story::SetCanRead(bool i_CanRead)
{
	m_canRead = i_CanRead;
}

void Narrator::Runtime::Story::ReadChoices(Narrator::Runtime::Node* i_CurrentDecisionNode)
{
	Narrator::Runtime::DecisionNode* decisionNode = dynamic_cast<Narrator::Runtime::DecisionNode*>(i_CurrentDecisionNode);
	if (decisionNode)
	{
		m_CurrentChoices.clear();

		std::map<std::uint32_t, Narrator::Runtime::Node*> outFlowNodeMap = decisionNode->GetOutFlowNodeMap();

		for (std::map<std::uint32_t, Narrator::Runtime::Node*>::iterator mapIterator = outFlowNodeMap.begin();
			mapIterator != outFlowNodeMap.end();
			mapIterator++)
		{
			Narrator::Runtime::ChoiceNode* choiceNode = dynamic_cast<Narrator::Runtime::ChoiceNode*>(mapIterator->second);
			if (choiceNode)
			{
				std::string choiceText = choiceNode->GetChoiceText();
				m_CurrentChoices.emplace_back(choiceText);
			}
			else
			{
				//TODO: #NarratorToDoAssert #RuntimeError
			}
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError
	}
}

void Narrator::Runtime::Story::AddParseMetaData(Narrator::Parser::TParseMessageType i_MessageType, std::uint64_t i_LineNumber, std::string i_Message)
{
	m_ParseMetaDataMap.insert(std::make_pair(i_LineNumber, Narrator::Parser::ParseMetaData(i_MessageType, i_LineNumber, i_Message)));
}

std::multimap<std::uint64_t, Narrator::Parser::ParseMetaData> Narrator::Runtime::Story::GetParseMetaDataMap() const
{
	return m_ParseMetaDataMap;
}
