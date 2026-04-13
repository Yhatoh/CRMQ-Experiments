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
./benchmark/generator -p <path_to_folder> -s <output_filename> -n <dataset_size> -q <number_of_queries>
```

Arguments:

* `-p`: folder where the query file will be saved
* `-s`: name of the output file
* `-n`: the dataset size
* `-q`: number of queries to generate (sizes: 10, 100, 1k, 10k, 100k, 1M, 10M)

## Run the Benchmark

```bash
cd build/
./benchmark/benchmark -s <dataset> -q <queries> -n <num_queries> -b <build_results_file> -t <query_results_file>
```

Arguments:

* `-s`: dataset file (can be plain text or a binary LCP array of `int64_t`)
* `-q`: binary file containing queries (must be generated using the generator above)
* `-n`: number of queries in the query file
* `-b`: output file for build-time results (time + space)
* `-t`: output file for query-time results (average time + space)

## Generate LCP file

Optionally, you can compute the LCP array of a text file and save it for use in the benchmark.

```
cd build/
./benchmark/lcp_builder <text_file>
```

The only argument is the text file. The executable will generate a binary file of `int64_t` named `<text_file>.lcp`.

## Queries Info

To obtain information about the number of queries solved using the sparse table, first add the following flag when building:

```
-DCMAKE_BUILD_TYPE=Information
```

This will compile the benchmark to test only our solution (`CRMQ`) and will output the following metrics:

* `r` – range
* `qst` – number of queries solved by the sparse table
* `qbp` – number of queries not solved by the sparse table
* `%qst` – percentage of queries solved by the sparse table
* `%qbp` – percentage of queries not solved by the sparse table

The output will be displayed in the following format:

```
r,qst,qbp,%qst,%qbp
```

## Credits

If you use this project in a scientific article, please cite the following paper:

```
@inproceedings{CarmonaL26,
    author = {Gabriel Carmona and Filippo Lari},
    title = {Compressing Highly Repetitive Binary Trees with an Application to Range Minimum Queries},
    booktitle = {Proc. 24th Symposium on Experimental Algorithms},
    year = {2026},
}
```
