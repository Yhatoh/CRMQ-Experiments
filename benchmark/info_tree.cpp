#include <iostream>
#include <chrono>
#include <vector>
#include <type_traits>
#include <fstream>
#include <cmath>
#include <map>

#include <sdsl/int_vector.hpp>

#include "benchmark.hpp"
#include "cxxopts.hpp"
#include "utils.hpp"

#include "../competitors/CRMQ/libsais/include/libsais64.h"
#include "../competitors/CRMQ/libsais/include/libsais.h"
#include "../competitors/crmq_wrapper.hpp"

template< typename T >
sdsl::bit_vector build_bp_sequence(std::vector<T> &data) {
  // this encloses everything into an "()"
  // todo: can we avoid to change the array? this does not
  // work if the input already contains such a value
  data.push_back(std::numeric_limits<T>::min());

  const size_t n = data.size();
  sdsl::bit_vector bp(2 * n, 0);

  std::stack<size_t> st;
  size_t pos = 2 * n;
  for(int64_t i = n - 1; i >= 0; i--) {
    while (!st.empty() && data[st.top()] >= data[i]) {
      st.pop();
      bp[--pos] = 1;
    }
    st.push(i);
    bp[--pos] = 0;
  }
  while (!st.empty()) {
    st.pop();
    bp[--pos] = 1;
  }

  data.pop_back();

  return bp;
}

int main(int argc, char* argv[]) {

    const std::string input_sequence = argv[1];

    int64_t *lcp_ptr;
    std::vector<int64_t> data;

    std::ifstream file(input_sequence, std::ios::in | std::ios::binary);
    
    if(!file) throw std::runtime_error("Unable to open the file");

    data = read_data<int64_t>(input_sequence);

    sdsl::bit_vector bp = build_bp_sequence<int64_t>(data);

    size_t height = 0;
    size_t depth = 0;
    for(size_t i = 0; i < bp.size(); i++) {
      if(bp[i] == 0 && depth == 0) {
        std::cout << "danger danger" << std::endl;
      }
      depth += (bp[i] ? 1 : -1);
      height = std::max(depth, height);
    }

    std::cout << "Nodes        : " << bp.size() / 2  << " check: " << data.size() << std::endl;
    std::cout << "Height       : " << height << std::endl;

    std::cout << "Buildingg scst" << std::endl;
    scst<> cbp(bp, 2);
    std::cout << "Stats" << std::endl;
//    uint64_t maxi = 0;
//    for(uint64_t i = bp.size() / 2; i > 0; i--) {
//      maxi = std::max(maxi, cbp.test(i));
//    }
//    std::cout << "Max subtrees : " << cbp.max_trees() << std::endl;
//    std::cout << "Max recursion: " << maxi << std::endl;
    auto res = cbp.average_length_chain();
    auto res_subsize = cbp.average_subtree_size();
    std::cout << res.first << ","  << res.second << "," << res_subsize.first << "," <<  res_subsize.second << std::endl;
    return 0;
}
