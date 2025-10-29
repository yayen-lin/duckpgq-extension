#pragma once

#include "duckpgq/common.hpp"

namespace duckdb {

// likely responsible for binding a SQL statement related to property graph queries.
// It will look at the SQL statement and possibly the provided context and binder to resolve types,
// check for compatibility, and prepare for execution.
BoundStatement duckpgq_bind(ClientContext &context, Binder &binder, OperatorExtensionInfo *info,
                            SQLStatement &statement);

struct DuckPGQOperatorExtension : public OperatorExtension {
	DuckPGQOperatorExtension() : OperatorExtension() {
		Bind = duckpgq_bind;
	}

	std::string GetName() override {
		return "duckpgq_bind";
	}

	unique_ptr<LogicalExtensionOperator> Deserialize(Deserializer &deserializer) override {
		throw InternalException("DuckPGQ operator should not be serialized");
	}
};

} // namespace duckdb
