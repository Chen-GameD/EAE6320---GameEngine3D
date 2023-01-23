#pragma once

#include "../Core/Graph.h"
#include "ParseMetaData.h"

namespace Narrator {
	namespace Runtime {
		class ChoiceNode;
	}
}

namespace Narrator
{
	namespace Runtime
	{
		class Story : public Narrator::Runtime::Graph
		{

		public:

			static Narrator::Runtime::Story Parse(const std::string& i_PathToRead, const std::string& i_PathToWrite, bool i_IsPlayMode = false);

			Story();
			virtual ~Story();

			//Runtime Methods
			bool canRead();
			std::string Read();

			std::vector<std::string> GetChoices();
			void SelectChoice(uint32_t i_ChoiceIndex);

			bool IsValid() const;

			void Play();

			bool FromJSONFile(const std::string& i_JSONFilePath);

		protected:

			bool ToJSONFile(const std::string& i_JSONFilePath);

			void ToJSON(nlohmann::json& jsonRoot) override;
			void FromJSON(const nlohmann::json& jsonRoot) override;

			//Parser Methods
			void Traverse() override;
			void BreadthFirstSearch();

			void AddNode(Narrator::Runtime::Node* i_NodeToAdd);
			void LinkEndNode();

			Narrator::Runtime::Node* GetCurrentNode() const;

			bool HasDivertNode(const std::string& i_DivertName);
			//Always use HasDivertNode to check if it is present before accessing the Node
			Narrator::Runtime::Node* GetDivertNode(const std::string& i_DivertName);
			void AddToDivertNodeMap(const std::string& i_DivertName, Narrator::Runtime::Node* i_DivertNode);
			std::map<std::string, Narrator::Runtime::Node*> GetDivertNodeMap();

			bool HasKnotNode(const std::string& i_KnotName);
			//Always use HasKnotNode to check if it is present before accessing the Node
			Narrator::Runtime::Node* GetKnotNode(const std::string& i_KnotName);
			void AddToKnotNodeMap(const std::string& i_KnotName, Narrator::Runtime::Node* i_KnotNode);
			std::map<std::string, Narrator::Runtime::Node*> GetKnotNodeMap();

			Narrator::Runtime::Node* GetLastDecisionNode() const;
			Narrator::Runtime::Node* CreateDecisionNode();
			void ClearLastDecisionNode();

			bool GetCanRead() const;
			void SetCanRead(bool i_CanRead);

			void ReadChoices(Narrator::Runtime::Node* i_CurrentDecisionNode);

			void AddParseMetaData(Narrator::Parser::TParseMessageType i_MessageType, std::uint64_t i_LineNumber, std::string i_Message);
			std::multimap<std::uint64_t, Narrator::Parser::ParseMetaData> GetParseMetaDataMap() const;
			bool GetIsParseErrorsPresent() const;
			void SetIsParseErrorsPresent(bool i_IsParseErrorsPresent);

			//Parser Meta Data
			//Line Number and Corresponding ParseMetaData Map
			std::multimap<std::uint64_t, Narrator::Parser::ParseMetaData> m_ParseMetaDataMap;
			bool m_IsParseErrorsPresent;

			//Story State
			Narrator::Runtime::Node* m_StartNode;
			Narrator::Runtime::Node* m_EndNode;
			Narrator::Runtime::Node* m_CurrentNode;

			Narrator::Runtime::Node* m_LastDecisionNode;
			Narrator::Runtime::Node* m_LastChoiceNode;
			std::uint32_t m_CurrentChoiceIndex = 0;

			bool m_IsAValidKnotSection;

			//Runtime Story State
			bool m_canRead;
			std::vector<std::string> m_CurrentChoices;

		};
	}
}