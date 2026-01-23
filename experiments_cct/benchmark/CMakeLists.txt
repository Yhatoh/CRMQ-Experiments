add_executable(generator generator.cpp)
target_link_libraries(generator cxxopts)

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark cxxopts cct_lib succinct_rmq_lib hyperrmq_lib)

add_executable(lcp_builder lcp_builder.cpp)
target_link_libraries(lcp_builder cxxopts cct_lib succinct_rmq_lib hyperrmq_lib)

add_executable(info_tree info_tree.cpp)
target_link_libraries(info_tree cxxopts cct_lib succinct_rmq_lib hyperrmq_lib)
