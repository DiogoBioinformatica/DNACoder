#include <grpcpp/grpcpp.h>
#include <iostream>
#include <string>

#include "sequence.grpc.pb.h"

int main(int argc, char** argv) {
  std::string target = "127.0.0.1:50051";
  bool include_bases = false;

  if (argc > 1) target = argv[1];
  if (argc > 2) {
    std::string v = argv[2];
    include_bases = (v == "1" || v == "true" || v == "yes");
  }

  auto channel = grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
  auto stub = dnacoder::SequenceService::NewStub(channel);

  dnacoder::AnalyzeRequest req;
  req.set_include_bases(include_bases);

  grpc::ClientContext ctx;
  std::unique_ptr<grpc::ClientReader<dnacoder::AnalyzeReply>> reader(
      stub->AnalyzeSequences(&ctx, req));

  dnacoder::AnalyzeReply reply;
  while (reader->Read(&reply)) {
    std::cout
      << "id=" << reply.id()
      << " name=" << reply.name()
      << " len=" << reply.length()
      << " gc=" << reply.gccontent()
      << " checksum=" << reply.checksum();

    if (include_bases) std::cout << " bases=" << reply.bases();
    std::cout << "\n";
  }

  grpc::Status status = reader->Finish();
  if (!status.ok()) {
    std::cerr << "RPC failed: " << status.error_message()
              << " (code=" << status.error_code() << ")\n";
    return 1;
  }

  return 0;
}
