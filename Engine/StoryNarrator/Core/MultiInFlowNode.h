#pragma once

#include "Node.h"

#include <string>
#include <map>
#include <cstdint>

#include "../JSON/Includes.h"

namespace Narrator
{
	namespace Runtime
	{
		class MultiInFlowNode : public virtual Node
		{

		public:

			MultiInFlowNode(TNodeType i_Type = TNodeType::kNodeBase, TInFlowType i_InFlowType = TInFlowType::kMultiInFlow);
			virtual ~MultiInFlowNode();

			//Parser Methods
			void AddInFlowNode(Narrator::Runtime::Node* i_NextNode);
			void RemoveInFlowNode(Narrator::Runtime::Node* i_NextNode);
			void RemoveInFlowNode(std::uint32_t i_NextNodeID);
			std::map<std::uint32_t, Narrator::Runtime::Node*> GetInFlowNodeMap() const;

			virtual void ToJSON(nlohmann::json& nodeObject) override;

			void FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph) override;

		protected:

			std::map<std::uint32_t, Narrator::Runtime::Node*> m_InFlowNodeMap;

		private:

		};
	}
}