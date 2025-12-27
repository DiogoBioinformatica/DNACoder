#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <pqxx/pqxx>  // PostgreSQL C++ library


namespace infra {
struct SequenceRow {
    int64_t id;
    std::string name;
    std::string bases;
};

class Db {
public:
    explicit Db(const std::string &conninfo);
    std::vector<SequenceRow> fetchSequences();

private:
    pqxx::connection conn_;
};


}