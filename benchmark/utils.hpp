#pragma once

#include <cassert>
#include <fstream>
#include <vector>
#include <map>

using query_type = std::pair<size_t, size_t>;

template<typename T>
void write_data(const std::vector<T> &values, const std::string &path) {
    std::ofstream file(path, std::ios::binary);

    if (!file) throw std::runtime_error("Unable to open the output file");

    size_t n = values.size();

    file.write(reinterpret_cast<const char*>(&n), sizeof(n));

    file.write(reinterpret_cast<const char*>(values.data()), n * sizeof(T));

    file.close();
}

template<typename T>
std::vector<T> read_data(const std::string &path) {
    std::ifstream file(path, std::ios::binary);

    if (!file) throw std::runtime_error("Unable to open the input file");
    
    size_t n;

    file.read(reinterpret_cast<char*>(&n), sizeof(n));

    T* data = new T[n];

    file.read(reinterpret_cast<char*>(data), n * sizeof(T));

    file.close();

    std::vector<T> values(data, data + n);

    return values;
}

std::map<size_t, std::vector<query_type>> read_and_group_queries(const size_t queries, const std::string &path) {
    std::ifstream file(path, std::ios::binary);

    if (!file) throw std::runtime_error("Unable to open the input file");
    
    std::map<size_t, std::vector<query_type>> grouped_queries;

    size_t total_queries;

    file.read(reinterpret_cast<char*>(&total_queries), sizeof(total_queries));

    for(size_t i = 0; i < (total_queries / queries); ++i) {
        query_type* data = new query_type[queries];
        file.read(reinterpret_cast<char*>(data), queries * sizeof(query_type));
        const size_t range = data[0].second - data[0].first + 1;
        grouped_queries.emplace(range, std::vector<query_type>(data, data + queries));
    }

    return grouped_queries;
}