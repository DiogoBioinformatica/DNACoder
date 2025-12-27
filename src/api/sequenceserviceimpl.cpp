#include "sequenceserviceimpl.h"
#include "../engine/analyze.h"
#include <mutex>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

namespace api {

grpc::Status SequenceServiceImpl::AnalyzeSequences(
    grpc::ServerContext* context,
    const dnacoder::AnalyzeRequest* request,
    grpc::ServerWriter<dnacoder::AnalyzeReply>* writer) {

  const bool include_bases = request->include_bases();
  auto rows = db_.fetchSequences(); // id=1 fixo no db.cpp

  std::mutex write_mtx;

  tbb::parallel_for(
    tbb::blocked_range<size_t>(0, rows.size(), 1),
    [&](const tbb::blocked_range<size_t>& r) {
      for (size_t i = r.begin(); i < r.end(); ++i) {
        if (context->IsCancelled()) return;

        const auto& s = rows[i];
        auto a = engine::analyzeBase(s.bases);

        dnacoder::AnalyzeReply reply;
        reply.set_id(s.id);
        reply.set_name(s.name);
        reply.set_length(a.length);
        reply.set_checksum(a.checksum);
        reply.set_gccontent(a.gccontent);
        if (include_bases) reply.set_bases(s.bases);

        std::lock_guard<std::mutex> lk(write_mtx);
        writer->Write(reply);
      }
    });

  return grpc::Status::OK;
}

} // namespace api
