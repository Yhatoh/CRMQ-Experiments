#pragma once

#include <vector>
#include <string>

#include "../competitors/hyperrmq/include/hyperrmq/hyper_rmq.hpp"
#include "../competitors/hyperrmq/include/hyperrmq/hypersuccinct_binary_tree.hpp"

template<uint32_t B>
class HyperRMQWrapper {

    /* best configuration from the paper:
        "A Simple Representation of Tree Covering Utilizing Balanced 
        Parentheses and Efficient Implementation of Average-Case Optimal RMQs" Hamada et al. ESA 2024
    */
    using HyperRMQBreadthFirstArithmetic =
        hyperrmq::HyperRMQ<16, hyperrmq::CompressedMicrotreeSplitRankArrayArithmetic<false>>;

    HyperRMQBreadthFirstArithmetic rmq;

    size_t n;

public:

    HyperRMQWrapper(const std::vector<int64_t> &data) : n(data.size()) {
        std::vector<int32_t> data32(data.size());

        std::transform(data.begin(), data.end(), data32.begin(),
                   [](int64_t d) { return static_cast<int32_t>(d); });

        rmq = HyperRMQBreadthFirstArithmetic(data32, B);
    }

    inline size_t query(const size_t i, const size_t j) const {
        return rmq.query(i, j);
    }

    static constexpr std::string name() {
        return "HyperRMQ_" + std::to_string(B);
    }

    inline size_t size() const {
        return rmq.evaluate_memory_consumption();
    }

    inline double bpe() const {
        return double(size()) / double(n);
    }

};