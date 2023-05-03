//
// Created by jinhai on 23-3-14.
//


#pragma once

#include "common/types/internal_types.h"
#include "main/query_context.h"
#include "executor/physical_operator.h"

namespace infinity {

class PhysicalDropSchema final : public PhysicalOperator {
public:
    explicit
    PhysicalDropSchema(SharedPtr<String> schema_name,
                       ConflictType conflict_type,
                       u64 id)
                       : PhysicalOperator(PhysicalOperatorType::kDropSchema, nullptr, nullptr, id),
                        schema_name_(std::move(schema_name)),
                        conflict_type_(conflict_type) {
    }

    ~PhysicalDropSchema() override = default;

    void
    Init() override;

    void
    Execute(SharedPtr<QueryContext>& query_context) override;

    inline SharedPtr<Vector<String>>
    GetOutputNames() const final {
        return output_names_;
    }

    inline SharedPtr<Vector<DataType>>
    GetOutputTypes() const final {
        return output_types_;
    }

    inline SharedPtr<String>
    schema_name() const {
        return schema_name_;
    }

    inline ConflictType
    conflict_type() const {
        return conflict_type_;
    }

private:
    SharedPtr<String> schema_name_{};
    ConflictType conflict_type_{ConflictType::kInvalid};

    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<DataType>> output_types_{};
};

}

