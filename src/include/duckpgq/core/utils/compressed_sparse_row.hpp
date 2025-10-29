//===----------------------------------------------------------------------===//
//                         DuckPGQ
//
// duckpgq/core/utils/compressed_sparse_row.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once
#include "duckdb/function/function.hpp"

#include "duckdb/parser/expression/cast_expression.hpp"
#include "duckdb/parser/expression/function_expression.hpp"
#include "duckdb/parser/expression/subquery_expression.hpp"
#include "duckdb/parser/query_node/set_operation_node.hpp"

#include "duckdb/parser/expression/columnref_expression.hpp"
#include "duckdb/parser/property_graph_table.hpp"
#include "duckdb/parser/query_node/select_node.hpp"
#include "duckdb/parser/tableref/joinref.hpp"
#include "duckpgq/common.hpp"

namespace duckdb {

class CSR {
public:
	CSR() = default;
	~CSR() {
		delete[] v;
	}

	// For vertex i, edges are in e[v[i]] to e[v[i+1]-1]
	atomic<int64_t> *v {};      // Vertex offsets (where each vertex's edges start)
	vector<int64_t> e;          // Edge destinations (destination vertices)
	vector<int64_t> edge_ids;   // Original edge IDs
	vector<int64_t> w;          // Integer weights
	vector<double> w_double;    // Double weights

	bool initialized_v = false;
	bool initialized_e = false;
	bool initialized_w = false;

	size_t vsize {};

	string ToString() const;
};

/****************** metadata/context that gets passed around when CSR-related functions are called ******************/
struct CSRFunctionData : FunctionData {
	CSRFunctionData(ClientContext &context, int32_t id, const LogicalType &weight_type);
	unique_ptr<FunctionData> Copy() const override;
	bool Equals(const FunctionData &other_p) const override;
	static unique_ptr<FunctionData> CSRVertexBind(ClientContext &context, ScalarFunction &bound_function,
	                                              vector<unique_ptr<Expression>> &arguments);
	static unique_ptr<FunctionData> CSREdgeBind(ClientContext &context, ScalarFunction &bound_function,
	                                            vector<unique_ptr<Expression>> &arguments);
	static unique_ptr<FunctionData> CSRBind(ClientContext &context, ScalarFunction &bound_function,
	                                        vector<unique_ptr<Expression>> &arguments);

	ClientContext &context;         // Database connection context
	const int32_t id;               // CSR id reference
	const LogicalType weight_type;  // weights type
};

/********************************************* CSR BindReplace functions *********************************************/
unique_ptr<CommonTableExpressionInfo> CreateUndirectedCSRCTE(const shared_ptr<PropertyGraphTable> &edge_table,
                                                             const unique_ptr<SelectNode> &select_node);

// Takes an edge table (with src, dst columns)
// Returns a SQL CTE query
// DuckDB executes that CTE, produces CSR arrays
// CSR arrays stored in memory
// BellmanFord reads from those arrays
unique_ptr<CommonTableExpressionInfo> CreateDirectedCSRCTE(
	const shared_ptr<PropertyGraphTable> &edge_table,
	const string &prev_binding,  // src
	const string &edge_binding,  // edge
	const string &next_binding); // dst

/************************************************* Helper functions *************************************************/
// Create the edge array
unique_ptr<CommonTableExpressionInfo> MakeEdgesCTE(const shared_ptr<PropertyGraphTable> &edge_table);

// Create the vertex offset array
unique_ptr<SubqueryExpression> CreateDirectedCSRVertexSubquery(
	const shared_ptr<PropertyGraphTable> &edge_table,
	const string &binding);

unique_ptr<SubqueryExpression> CreateUndirectedCSRVertexSubquery(
	const shared_ptr<PropertyGraphTable> &edge_table,
	const string &binding);

unique_ptr<SelectNode> CreateOuterSelectEdgesNode();

unique_ptr<SelectNode> CreateOuterSelectNode(unique_ptr<FunctionExpression> create_csr_edge_function);

// Set up joins and groupings
unique_ptr<JoinRef> GetJoinRef(
	const shared_ptr<PropertyGraphTable> &edge_table,
	const string &edge_binding,
	const string &prev_binding,
	const string &next_binding);

unique_ptr<SubqueryExpression> GetCountTable(
	const shared_ptr<PropertyGraphTable> &table,
	const string &table_alias,
	const string &primary_key);

// Set up joins and groupings
void SetupSelectNode(
	unique_ptr<SelectNode> &select_node,
	const shared_ptr<PropertyGraphTable> &edge_table,
	bool reverse = false);

unique_ptr<SubqueryRef> CreateCountCTESubquery();
unique_ptr<SubqueryExpression> GetCountUndirectedEdgeTable();
unique_ptr<SubqueryExpression> GetCountEdgeTable(const shared_ptr<PropertyGraphTable> &edge_table);

} // namespace duckdb
