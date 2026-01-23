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

#include "../competitors/hyperrmq_wrapper.hpp"
#include "../competitors/crmq_wrapper.hpp"

#include "../competitors/cct/libsais/include/libsais64.h"
#include "../competitors/cct/libsais/include/libsais.h"

template<typename K>
void run_encoding_benchmark(Benchmark<K> &benchmark, const std::string &output_build,
                             const std::string &output_time) {

    benchmark.template run<HyperRMQWrapper<64>>();
    benchmark.template run<HyperRMQWrapper<128>>();
    benchmark.template run<HyperRMQWrapper<256>>();
    benchmark.template run<HyperRMQWrapper<512>>();

    benchmark.template run<CRMQWrapper<int64_t, 32>>();
    benchmark.template run<CRMQWrapper<int64_t, 48>>();
    benchmark.template run<CRMQWrapper<int64_t, 64>>();

    std::ofstream c_output(output_build);
    std::ofstream q_output(output_time);

    benchmark.save(c_output, q_output);

    c_output.close();
    q_output.close();
}


int main(int argc, char* argv[]) {

    cxxopts::Options options("Benchmark", "Benchmarking program for various RMQ implementations");

    options.add_options()
        ("i,indexing", "Run the benchmark using the indexing data structures, if not provided uses the encodings",
            cxxopts::value<bool>()->default_value("false"))
        ("s,sequence", "Path to the input sequence", cxxopts::value<std::string>())
        ("q,queries", "Path to the queries sequence", cxxopts::value<std::string>())
        ("n,num_queries", "Number of queries", cxxopts::value<int>())
        ("b,build", "Where to write the building statistics", cxxopts::value<std::string>())
        ("t,time", "Where to write the queries statistics", cxxopts::value<std::string>())
        ("h,help", "Print usage instructions");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help({""}) << std::endl;
        return 0;
    }

    const bool indexing = result["indexing"].as<bool>();

    const std::string input_sequence = result["sequence"].as<std::string>();
    const std::string input_queries = result["queries"].as<std::string>();

    const int num_queries = result["num_queries"].as<int>();

    const std::string output_build = result["build"].as<std::string>();
    const std::string output_time = result["time"].as<std::string>();
    int64_t *lcp_ptr;
    std::vector<int64_t> data;

    if(input_sequence.find("lcp") == std::string::npos) {
      std::ifstream file(input_sequence, std::ios::in | std::ios::binary);
      
      if(!file) throw std::runtime_error("Unable to open the file");

      std::string s((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

      const size_t n = s.size();

      // build the suffix array and the permuted lcp array
      uint8_t *p = reinterpret_cast<uint8_t*>(s.data());
      int64_t *sa = new int64_t[n];
      int64_t *plcp = new int64_t[n];
      lcp_ptr = new int64_t[n];

      if(libsais64(p, sa, n, 0, NULL) != 0) throw std::runtime_error("SA construction failed");

      if(libsais64_plcp(p, sa, plcp, n) != 0) throw std::runtime_error("PLCP array construction failed");
      
      if(libsais64_lcp(plcp, sa, lcp_ptr, n) != 0) throw std::runtime_error("LCP array construction failed");

      data.assign(lcp_ptr, lcp_ptr + n);
    } else {
      data = read_data<int64_t>(input_sequence);
    }

    std::cout << "finished building the lcp array" << std::endl;

    std::map<size_t, std::vector<query_type>> queries = read_and_group_queries(num_queries, input_queries);

    std::cout << "finished reading the queries" << std::endl;


    Benchmark<int64_t> benchmark(data, queries);

    run_encoding_benchmark(benchmark, output_build, output_time);

    return 0;
}
