#pragma once

#include "UniInFlowNode.h"
#include "UniOutFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class ChoiceNode : public UniInFlowNode, public UniOutFlowNode
		{

		public:

			ChoiceNode(const std::uint32_t i_Index, const std::string& i_Text);
			ChoiceNode();
			~ChoiceNode();

			std::string ToString() override;

			std::uint32_t GetChoiceIndex() const;
			void SetChoiceIndex(const std::uint32_t i_Index);

			std::string GetChoiceText() const;
			void SetChoiceText(const std::string& i_Text);

			void ToJSON(nlohmann::json& nodeObject) override;

			void FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph) override;

		protected:

			std::uint32_t m_Index;
			std::string m_Text;

		};
	}
}