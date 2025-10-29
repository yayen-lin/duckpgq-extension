#pragma once
#include "duckpgq/common.hpp"

namespace duckdb {

// These are table-valued functions (High-Level Graph Operations), also what we want to port to GPU,
// they return entire result sets (like tables). These are the main graph algorithms:
struct CoreTableFunctions {
	static void Register(ExtensionLoader &loader) {
		RegisterCreatePropertyGraphTableFunction(loader);
		RegisterDescribePropertyGraphTableFunction(loader);
		RegisterDropPropertyGraphTableFunction(loader);

		// Measure clustering for nodes
		RegisterLocalClusteringCoefficientTableFunction(loader);

		// Pattern matching queries (like "find all paths from A to B")
		RegisterMatchTableFunction(loader);

		// Compute PageRank for all nodes in a graph
		RegisterPageRankTableFunction(loader);

		// Scan property graph data
		RegisterScanTableFunctions(loader);
		RegisterSummarizePropertyGraphTableFunction(loader);

		// Find connected components
		RegisterWeaklyConnectedComponentTableFunction(loader);
	}

private:
	static void RegisterCreatePropertyGraphTableFunction(ExtensionLoader &loader);
	static void RegisterMatchTableFunction(ExtensionLoader &loader);
	static void RegisterDropPropertyGraphTableFunction(ExtensionLoader &loader);
	static void RegisterDescribePropertyGraphTableFunction(ExtensionLoader &loader);
	static void RegisterLocalClusteringCoefficientTableFunction(ExtensionLoader &loader);
	static void RegisterScanTableFunctions(ExtensionLoader &loader);
	static void RegisterWeaklyConnectedComponentTableFunction(ExtensionLoader &loader);
	static void RegisterPageRankTableFunction(ExtensionLoader &loader);
	static void RegisterSummarizePropertyGraphTableFunction(ExtensionLoader &loader);
};

} // namespace duckdb
