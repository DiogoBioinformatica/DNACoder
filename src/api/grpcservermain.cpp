#include <grpcpp/grpcpp.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../infra/db.h"
#include "sequenceserviceimpl.h"

static std::string getenv_or_default(const char* name, const std::string& def) {
  const char* v = std::getenv(name);
  return v ? std::string(v) : def;
}

int main(int argc, char** argv) {
  std::string grpc_addr = "0.0.0.0:50051";
  std::string conninfo =
      getenv_or_default("DNACODER_DB_CONN",
                        "host=/var/run/postgresql dbname=dnacoder user=postgres");

  if (argc > 1) grpc_addr = argv[1];
  if (argc > 2) conninfo = argv[2];

  try {
    infra::Db db(conninfo);
    api::SequenceServiceImpl service(db);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(grpc_addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    auto server = builder.BuildAndStart();
    std::cout << "gRPC listening on " << grpc_addr << "\n";
    std::cout << "Postgres conninfo: " << conninfo << "\n";
    server->Wait();
  } catch (const std::exception& ex) {
    std::cerr << "Fatal: " << ex.what() << "\n";
    return 1;
  }

  return 0;
}
