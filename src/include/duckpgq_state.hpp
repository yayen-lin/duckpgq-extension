#pragma once

#include "duckpgq/common.hpp"
#include "duckdb/common/case_insensitive_map.hpp"

#include <duckpgq/core/utils/compressed_sparse_row.hpp>

namespace duckdb {

class DuckPGQState : public ClientContextState {
public:
	explicit DuckPGQState() {};

	// Likely used to set up any internal tables necessary for handling property graph data.
	static void InitializeInternalTable(ClientContext &context);

	// Overrides a method that might handle cleanup or finalization after a query executes.
	void QueryEnd() override;

	// Methods to retrieve property graph structures or Compressed Sparse Row (CSR) representations by ID.
	CreatePropertyGraphInfo *GetPropertyGraph(const string &pg_name);
	CSR *GetCSR(int32_t id);

	// Manage property graph data processing.
	void RetrievePropertyGraphs(const shared_ptr<Connection> &context);
	void ProcessPropertyGraphs(unique_ptr<MaterializedQueryResult> &property_graphs, bool is_vertex);
	void PopulateEdgeSpecificFields(unique_ptr<DataChunk> &chunk, idx_t row_idx, PropertyGraphTable &table);
	static void ExtractListValues(const Value &list_value, vector<string> &output);
	void RegisterPropertyGraph(const shared_ptr<PropertyGraphTable> &table, const string &graph_name, bool is_vertex);

public:
	unique_ptr<ParserExtensionParseData> parse_data;
	unordered_map<int32_t, unique_ptr<ParsedExpression>> transform_expression;
	int32_t match_index = 0;

	//! Property graphs that are registered
	case_insensitive_map_t<unique_ptr<CreateInfo>> registered_property_graphs;

	//! Used to build the CSR data structures required for path-finding queries
	std::unordered_map<int32_t, unique_ptr<CSR>> csr_list;
	std::mutex csr_lock;
	std::unordered_set<int32_t> csr_to_delete;
};

} // namespace duckdb
