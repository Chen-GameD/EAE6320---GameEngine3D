#pragma once

#include "Node.h"
#include "GraphHelper.h"

#include <string>

#include "../JSON/Includes.h"

namespace Narrator
{
	namespace Runtime
	{
		class UniOutFlowNode : public virtual Node
		{

		public:

			UniOutFlowNode(TNodeType i_Type = TNodeType::kNodeBase, TOutFlowType i_OutFlowType = TOutFlowType::kUniOutFlow);
			virtual ~UniOutFlowNode();

			//Parser Methods
			void SetNextNode(Narrator::Runtime::Node* i_NextNode);
			Narrator::Runtime::Node* GetNextNode();

			virtual void ToJSON(nlohmann::json& nodeObject) override;
			
			void FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph) override;

		protected:

			Narrator::Runtime::Node* m_NextNode;

		private:

		};
	}
}