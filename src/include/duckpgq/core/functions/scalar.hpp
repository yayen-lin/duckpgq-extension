#pragma once
#include "duckpgq/common.hpp"

namespace duckdb {

// These are scalar functions (Lower-Level Helpers),
// they typically process row-by-row or compute single values.
// Many of these are building blocks for the table functions
struct CoreScalarFunctions {
	static void Register(ExtensionLoader &loader) {

		// Compute path length
		RegisterCheapestPathLengthScalarFunction(loader); // this 1

		// Iterative path computation
		RegisterIterativeLengthScalarFunction(loader); // this 3
		RegisterIterativeLength2ScalarFunction(loader);
		RegisterIterativeLengthBidirectionalScalarFunction(loader);
		RegisterLocalClusteringCoefficientScalarFunction(loader);
		RegisterPageRankScalarFunction(loader);
		RegisterWeaklyConnectedComponentScalarFunction(loader);

		// Access CSR data structures
		RegisterGetCSRWTypeScalarFunction(loader);

		// Create CSR graph representation
		RegisterCSRCreationScalarFunctions(loader);
		RegisterCSRDeletionScalarFunction(loader);

		// Check if nodes are reachable
		RegisterReachabilityScalarFunction(loader); // this 4

		// Find shortest path
		RegisterShortestPathScalarFunction(loader); // this 2
	}

private:
	static void RegisterCheapestPathLengthScalarFunction(ExtensionLoader &loader);
	static void RegisterCSRCreationScalarFunctions(ExtensionLoader &loader);
	static void RegisterCSRDeletionScalarFunction(ExtensionLoader &loader);
	static void RegisterGetCSRWTypeScalarFunction(ExtensionLoader &loader);
	static void RegisterIterativeLengthScalarFunction(ExtensionLoader &loader);
	static void RegisterIterativeLength2ScalarFunction(ExtensionLoader &loader);
	static void RegisterIterativeLengthBidirectionalScalarFunction(ExtensionLoader &loader);
	static void RegisterLocalClusteringCoefficientScalarFunction(ExtensionLoader &loader);
	static void RegisterReachabilityScalarFunction(ExtensionLoader &loader);
	static void RegisterShortestPathScalarFunction(ExtensionLoader &loader);
	static void RegisterWeaklyConnectedComponentScalarFunction(ExtensionLoader &loader);
	static void RegisterPageRankScalarFunction(ExtensionLoader &loader);
};

} // namespace duckdb
