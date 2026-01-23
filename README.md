# CRMQ Experiments

## Requirements

* `CMake >= 3.25`

## Build the Project

```bash
git clone git@github.com:Yhatoh/CRMQ-Experiments.git
cd CRMQ-Experiments/
git submodule update --init --recursive

# Build competitors
cd competitors/cct/libsais/
cmake . && make -j8
cd ../bit_vector/
cmake . && make -j8

# Build main project
cd ../../../
mkdir build
cd build/
cmake .. -DCMAKE_BUILD_TYPE=Release && make -j8
```

## Generate Queries

To generate queries for the benchmark, run:

```bash
./benchmark/generator -p <path_to_folder> -s <output_filename> -n <number>
```

Arguments:

* `-p`: folder where the query file will be saved
* `-s`: name of the output file
* `-n`: number of queries to generate (sizes: 10, 100, 1k, 10k, 100k, 1M, 10M)

## Run the Benchmark

```bash
cd build/
./benchmark/benchmark -s <dataset> -q <queries> -n <num_queries> -b <build_results_file> -t <query_results_file>
```

Arguments:

* `-s`: dataset file (can be plain text or a binary LCP array of `int64_t`)
* `-q`: binary file with queries (must be generated with the generator above)
* `-n`: number of queries in the query file
* `-b`: output file for build-time results (time + space)
* `-t`: output file for query-time results (avg time + space)

