#pragma once
#include <grpcpp/grpcpp.h>

#include "sequence.grpc.pb.h"
#include "../infra/db.h"

namespace api {

class SequenceServiceImpl final : public dnacoder::SequenceService::Service {
public:
  explicit SequenceServiceImpl(infra::Db& db) : db_(db) {}

  grpc::Status AnalyzeSequences(
      grpc::ServerContext* context,
      const dnacoder::AnalyzeRequest* request,
      grpc::ServerWriter<dnacoder::AnalyzeReply>* writer) override;

private:
  infra::Db& db_;
};

} // namespace api
