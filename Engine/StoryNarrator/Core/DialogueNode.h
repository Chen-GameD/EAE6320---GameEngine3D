#pragma once

#include "UniInFlowNode.h"
#include "UniOutFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class DialogueNode : public UniInFlowNode, public UniOutFlowNode
		{

		public:

			DialogueNode(const std::string& i_Text);
			DialogueNode();
			~DialogueNode();

			std::string ToString() override;

			std::string GetText() const;
			void SetText(const std::string& i_Text);

			void ToJSON(nlohmann::json& nodeObject) override;

			void FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph) override;

		protected:

			std::string m_Text;
		};
	}
}