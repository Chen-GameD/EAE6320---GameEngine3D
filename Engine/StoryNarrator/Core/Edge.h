#pragma once

#include <cstdint>
#include <string>

#include "../JSON/Includes.h"

//Forward Declarations
namespace Narrator {
	namespace Runtime {
		class Node;
	}
}

namespace Narrator
{
	namespace Runtime
	{
		//Edge Class
		class Edge
		{

		public:

			static std::uint64_t GetTotalEdgeCount();
			static std::uint64_t GetMaxEdgeCount();
			static void ResetEdgeCount();

			Edge();
			Edge(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_DestinationNode);
			virtual ~Edge();

			std::string GetName() const;
			std::uint64_t GetID() const;
		protected:

			static std::uint64_t m_CurrentIndex;

			std::uint64_t m_ID;
			std::string m_Name;

			Narrator::Runtime::Node* m_SourceNode;
			Narrator::Runtime::Node* m_DestinationNode;

		private:

		};
	}
}