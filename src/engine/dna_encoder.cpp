#include "dna_encoder.h"


namespace engine {
void DNAEncoder::reset(int L) {
  if (L < 0) throw std::invalid_argument("L must be >= 0");
  L_ = L;
  // 2 bits por base => total bits = 2*L
  const uint64_t bits = 2ull * uint64_t(L_);
  const size_t words = size_t((bits + 63ull) / 64ull);
  data_.assign(words, 0ull);
}

void DNAEncoder::encode(const std::string& bases) {
  reset(static_cast<int>(bases.size()));
  for (int i = 0; i < L_; ++i) {
    set_base_code(i, bases[static_cast<size_t>(i)]);
  }
}

void DNAEncoder::show(std::ostream& os) const {
    os << "words=" << data_.size() << "  [";
    for (size_t i = 0; i < data_.size(); ++i) {
        os << "0x" << std::hex << std::setw(16) << std::setfill('0') << data_[i];
        if (i + 1 < data_.size())
        	os << " ";
    }
    os << std::dec << "]\n";
}

std::string DNAEncoder::to_string() const {
    std::ostringstream ss;
    show(ss);
    return ss.str();
}

}