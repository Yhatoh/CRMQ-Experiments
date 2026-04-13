#include <iostream>
#include <chrono>
#include <vector>
#include <type_traits>
#include <fstream>
#include <cmath>
#include <map>

#include "benchmark.hpp"
#include "cxxopts.hpp"
#include "utils.hpp"

#include "../competitors/CRMQ/libsais/include/libsais64.h"
#include "../competitors/CRMQ/libsais/include/libsais.h"



int main(int argc, char* argv[]) {
    const std::string input_sequence = argv[1];

    std::ifstream file(input_sequence, std::ios::in | std::ios::binary);
    
    if(!file) throw std::runtime_error("Unable to open the file");

    std::string s((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    const size_t n = s.size();
    std::cout << n << std::endl;
    int64_t *lcp_ptr;

    // build the suffix array and the permuted lcp array
    uint8_t *p = reinterpret_cast<uint8_t*>(s.data());
    int64_t *sa = new int64_t[n];
    int64_t *plcp = new int64_t[n];
    lcp_ptr = new int64_t[n];

    if(libsais64(p, sa, n, 0, NULL) != 0) throw std::runtime_error("SA construction failed");

    if(libsais64_plcp(p, sa, plcp, n) != 0) throw std::runtime_error("PLCP array construction failed");
    
    if(libsais64_lcp(plcp, sa, lcp_ptr, n) != 0) throw std::runtime_error("LCP array construction failed");

    std::ofstream output_file(input_sequence + ".lcp", std::ios::binary);

    output_file.write(reinterpret_cast<const char*>(&n), sizeof(size_t));
    output_file.write(reinterpret_cast<const char*>(lcp_ptr), n * sizeof(int64_t));
    output_file.close();
    file.close();

    return 0;
}
