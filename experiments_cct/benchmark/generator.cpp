#include <iostream>
#include <vector>

#include "generator.hpp"
#include "cxxopts.hpp"

int main(int argc, char* argv[]) {

    cxxopts::Options options("Generator", "Generates all the synthetic datasets used in our experiments");

    options.add_options()
        ("p,path", "Where to save the generated datasets", cxxopts::value<std::string>())
        ("h,help", "Print the usage instructions")
        ("n,numbers", "Numbers", cxxopts::value<uint64_t>())
        ("s,save", "Name save file", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << "\n";
        return 0;
    }

    const std::string path = result["path"].as<std::string>();
    const std::string save = result["save"].as<std::string>();

    const size_t n = result["numbers"].as<uint64_t>();
    const size_t q = 100;

    std::cout << "creating queries n: " << n << " q: " << q << std::endl;

    std::vector<size_t> ranges = {10, 100, 1000, 10000, 100000, 1000000, 1000000};
    std::vector<query_type> queries = generate_queries(n, q, ranges);

    write_data<query_type>(queries, path+"/"+save);
    
    return 0;
}
