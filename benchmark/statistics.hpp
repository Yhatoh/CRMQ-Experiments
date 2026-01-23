#pragma once

#include <fstream>
#include <string>

#include "benchmark.hpp"

struct construction_stats {
    static const std::string csv_header;

    std::string algorithm;
    double time;
    size_t space;
    double bpe;

    construction_stats(const std::string &algorithm, const double time, const size_t space, const double bpe) : 
                        algorithm(algorithm), time(time), space(space), bpe(bpe) {}

};

const std::string construction_stats::csv_header = "algorithm,time,space,bpe";

struct queries_stats {
    static const std::string csv_header;

    std::string algorithm;
    double time;
    double bpe;

    queries_stats(const std::string &algorithm, const double time, const double bpe) : 
                    algorithm(algorithm), time(time), bpe(bpe) {}
};

const std::string queries_stats::csv_header = "algorithm,range,time,bpe";