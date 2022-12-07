#pragma once

#include <cstdint>

//Forward Declarations
namespace Narrator {
	namespace Runtime {
		class Node;
		class Graph;
	}
}

namespace Narrator
{
	namespace Runtime
	{
		class GraphHelper
		{

		public:
			static Narrator::Runtime::Node* GetNode(const Narrator::Runtime::Graph* i_Graph, const std::uint32_t i_NodeID);
		};
	}
}