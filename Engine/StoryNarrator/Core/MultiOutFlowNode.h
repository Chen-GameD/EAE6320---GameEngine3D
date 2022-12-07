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
		class MultiOutFlowNode : public virtual Node
		{

		public:

			MultiOutFlowNode(TNodeType i_Type = TNodeType::kNodeBase, TOutFlowType i_OutFlowType = TOutFlowType::kMultiOutFlow);
			virtual ~MultiOutFlowNode();

			//Parser Methods
			void AddOutFlowNode(Narrator::Runtime::Node* i_NextNode);
			void RemoveOutFlowNode(Narrator::Runtime::Node* i_NextNode);
			void RemoveOutFlowNode(std::uint32_t i_NextNodeID);
			std::map<std::uint32_t, Narrator::Runtime::Node*> GetOutFlowNodeMap() const;

			virtual void ToJSON(nlohmann::json& nodeObject) override;

			void FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph) override;

		protected:

			std::map<std::uint32_t, Narrator::Runtime::Node*> m_OutFlowNodeMap;

		private:

		};
	}
}