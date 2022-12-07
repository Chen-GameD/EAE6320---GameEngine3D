#pragma once

#include "Node.h"

#include <string>

#include "../JSON/Includes.h"

namespace Narrator
{
	namespace Runtime
	{
		class UniInFlowNode : public virtual Node
		{

		public:

			UniInFlowNode(TNodeType i_Type = TNodeType::kNodeBase, TInFlowType i_InFlowType = TInFlowType::kUniInFlow);
			virtual ~UniInFlowNode();

			//Parser Methods
			void SetPreviousNode(Narrator::Runtime::Node* i_NextNode);
			Narrator::Runtime::Node* GetPreviousNode();

			virtual void ToJSON(nlohmann::json& nodeObject) override;

			void FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph) override;

		protected:

			Narrator::Runtime::Node* m_PreviousNode;

		private:

		};
	}
}