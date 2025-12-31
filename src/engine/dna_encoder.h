#pragma once
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cstdint>

namespace engine {
class DNAEncoder final {
public:
	explicit DNAEncoder(int L = 0) { reset(L); }
	void reset(int L);
	static inline uint8_t base_code(char b) {
		switch (b) {
			case 'A': case 'a': return 0u;
			case 'C': case 'c': return 1u;
			case 'G': case 'g': return 2u;
			case 'T': case 't': return 3u;
			default: return 0u;
		}
	}
	inline void set(int i, uint8_t v) {
		v &= 0x3u;
		uint64_t bitpos = uint64_t(2) * uint64_t(i);
		size_t w = size_t(bitpos >> 6);
		uint32_t off = uint32_t(bitpos & 63ull);
		if (off <= 62) {
			uint64_t mask = 0x3ull << off;
			data_[w] = (data_[w] & ~mask) | (uint64_t(v) << off);
		} else {
			uint64_t loMask = 0x1ull << 63;
			uint64_t hiMask = 0x1ull;
			data_[w] = (data_[w] & ~loMask) | (uint64_t(v & 0x1u) << 63);
			data_[w + 1] = (data_[w + 1] & ~hiMask)
					| (uint64_t((v >> 1) & 0x1u));
		}
	}

    void encode(const std::string& bases);
    const std::vector<uint64_t>& data() const { return data_; }
    int length() const { return L_; }


	inline void set_base_code(int i, char b) { set(i, base_code(b)); }
	void show(std::ostream&) const;
    std::string to_string() const;

private:
	int L_ { 0 };
	std::vector<uint64_t> data_ { };
};

}