#pragma once
#include "duckpgq/common.hpp"

#include <duckpgq_state.hpp>

// - The primary purpose of the duckpgq_parser.hpp header file and its associated structs
//   and functions is to facilitate the parsing of property graph queries sent to DuckDB.
// - The parsing functions translate graph queries into a format DuckDB understands,
//   and the planning functions generate execution plans for those queries.
namespace duckdb {

struct CorePGQParser {
	static void Register(ExtensionLoader &loader) {
		RegisterPGQParserExtension(loader);
	}

private:
	static void RegisterPGQParserExtension(ExtensionLoader &loader);
};

struct DuckPGQParserExtensionInfo : ParserExtensionInfo {
	DuckPGQParserExtensionInfo() : ParserExtensionInfo() {};
	~DuckPGQParserExtensionInfo() override = default;
};

ParserExtensionParseResult duckpgq_parse(ParserExtensionInfo *info, const std::string &query);

ParserExtensionPlanResult duckpgq_plan(ParserExtensionInfo *info, ClientContext &,
                                       unique_ptr<ParserExtensionParseData>);

ParserExtensionPlanResult duckpgq_find_select_statement(SQLStatement *statement, DuckPGQState &duckpgq_state);

ParserExtensionPlanResult duckpgq_handle_statement(SQLStatement *statement, DuckPGQState &duckpgq_state);

void duckpgq_find_match_function(TableRef *table_ref, DuckPGQState &duckpgq_state);

struct DuckPGQParserExtension : ParserExtension {
	DuckPGQParserExtension() : ParserExtension() {
		parse_function = duckpgq_parse;
		plan_function = duckpgq_plan;
		parser_info = make_shared_ptr<DuckPGQParserExtensionInfo>();
	}
};

struct DuckPGQParseData : ParserExtensionParseData {
	unique_ptr<SQLStatement> statement;

	unique_ptr<ParserExtensionParseData> Copy() const override {
		return make_uniq_base<ParserExtensionParseData, DuckPGQParseData>(statement->Copy());
	}

	string ToString() const override {
		return statement->ToString();
	};

	explicit DuckPGQParseData(unique_ptr<SQLStatement> statement) : statement(std::move(statement)) {
	}
};

} // namespace duckdb
