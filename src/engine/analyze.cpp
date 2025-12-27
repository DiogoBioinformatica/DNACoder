#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>

#include "analyze.h"


namespace engine {
struct LocalAgg {
    uint64_t length;
    uint64_t gccount;
    uint64_t checksum;
};

Analysis analyze_base(const std::string& bases) {
    const auto n = bases.size();

    auto agg = tbb::parallel_reduce(
        tbb::blocked_range<size_t>(0, n, 4096),
        LocalAgg{},
        [&](const tbb::blocked_range<size_t>& r, LocalAgg local) -> LocalAgg {
            for (size_t i = r.begin(); i != r.end(); ++i) {
                unsigned char base = static_cast<unsigned char>(bases[i]);
                local.checksum += static_cast<uint8_t>(base);
                if (base == 'G' || base == 'C' || base == 'g' || base == 'c') {
                    local.gccount++;
                }
            }
            return local;
        },
        [](const LocalAgg& a, const LocalAgg& b) -> LocalAgg {
            return LocalAgg{
                a.length + b.length,
                a.gccount + b.gccount,
                a.checksum + b.checksum
            };
        }
    );

    Analysis result;
    result.length = n;
    result.checksum = agg.checksum;
    result.gccontent = n > 0 ? static_cast<double>(agg.gccount) / static_cast<double>(n) : 0.0;
    return result;
}

   
    
}