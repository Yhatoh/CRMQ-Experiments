#pragma once

#include <vector>
#include <string>

#include "cct/include/crmq.hpp"

template<typename T, size_t threshold = 8>
class CRMQWrapper : public CRMQ<T, threshold> {

    uint64_t n;

public:

    CRMQWrapper(std::vector<T> &data) : n(data.size()), CRMQ<T, threshold>(data) {}

    inline size_t query(const size_t i, const size_t j) {
        return CRMQ<T, threshold>::query(i, j);
    }

    static constexpr std::string name() {
        return "CRMQ_" + std::to_string(threshold);
    }

    inline double bpe() {
        return double(this->size()) / double(n); 
    }

};
