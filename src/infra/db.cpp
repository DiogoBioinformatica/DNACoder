#include <stdexcept>

#include "db.h"

namespace infra {
Db::Db(const std::string &conninfo) : conn_(conninfo) {
    if (!conn_.is_open()) {
        throw std::runtime_error("Failed to open database connection");
    }
}

std::vector<SequenceRow> Db::fetch_sequences() {
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec("SELECT id, definition AS name, origin AS bases FROM sequence");
    txn.commit();

    std::vector<SequenceRow> sequences;
    sequences.reserve(res.size());

    for (const auto& row : res) {
        SequenceRow seq;
        seq.id = row["id"].as<int64_t>();
        seq.name = row["name"].as<std::string>();
        seq.bases = row["bases"].as<std::string>();
        sequences.push_back(std::move(seq));
    }
    return sequences;
}

}