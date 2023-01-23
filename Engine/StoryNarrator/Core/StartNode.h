#pragma once

#include "UniOutFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class StartNode : public UniOutFlowNode
		{

		public:

			StartNode();
			~StartNode();

			void ToJSON(nlohmann::json& nodeObject) override;

			void FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph) override;

		};
	}
}