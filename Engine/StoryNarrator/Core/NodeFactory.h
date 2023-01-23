#pragma once

#include "NodeType.h"
#include "Node.h"
#include <cstdint>

namespace Narrator
{
	namespace Runtime
	{
		class NodeFactory
		{

		public:

			static Narrator::Runtime::Node* Create(const Narrator::Runtime::TNodeType i_NodeType);

		};
	}
}