#pragma once

#include "MultiInFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class EndNode : public MultiInFlowNode
		{

		public:

			EndNode();
			~EndNode();

			void ToJSON(nlohmann::json& nodeObject) override;

			void FromJSON(const nlohmann::json& nodeObject, const Narrator::Runtime::Graph* i_Graph) override;

		};
	}
}