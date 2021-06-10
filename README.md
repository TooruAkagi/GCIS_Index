# 🗂️  GCIS Index

GCIS index is a grammar-compressed full-text self-index capable of locating the occurrences of patterns. The index consists of two flavors: `gcis_nep` and `gcis_uni`, which use internally different representations (see the below reference for details).


## 🚀Complete Test Run

A complete test-run of our index is done with the following lines:

```
git clone https://github.com/TooruAkagi/GCIS_Index.git
cd GCIS_Index
make
./genpattern -i inputsample.txt -o query.txt -l 100 -r 10
#gcis_nep
./gcis_nep_build -i inputsample.txt -o inputsample.gcis.nep.index
./gcis_nep_locate -i inputsample.gcis.nep.index -q  query.txt -m 4 -l 100 -r 10
#gcis uni
./gcis_uni_build -i inputsample.txt -o inputsample.gcis.nep.index
./gcis_uni_locate -i inputsample.gcis.nep.index -q  query.txt -m 4 -l 100 -r 10
```

In what follows, we demystify the meaning of the above lines.


## ⚙️ Compilation

```
git clone https://github.com/TooruAkagi/GCIS_Index.git
cd GCIS_Index
make
```

Required Software:
 - `g++` compiler
 - `make`

Obtained Programs:
 - `gcis_nep_build` : builds gcis-nep index
 - `gcis_nep_locate`: locates the occurrences of a pattern with gcis-nep index
 - `gcis_uni_build` : builds gcis-uni index
 - `gcis_uni_locate` : locates the occurrences of a pattern with gcis-uni


## 🏗️ Index Construction
Use either `gcis_nep_build` or `gcis_uni_build` for building our index data structure.

Parameters:  `-i text-input [-o index-outputfile]`
 - `-i text-input` : the text input
 - `-o index-outputfile`: optional name of the index file. Defaults to `index`.
`


## 🖨️ Generate a Pattern File
Our indexes use a pattern file as input for a locate query.
You can either create manually such a file, use the original text input as pattern file, or generate a pattern file with our handy tool `genpattern`, which works as follows:

Syntax: `genpattern -i inputfile [-o output-patternfile] [-l length] [-r number]`

Parameters:
- `-i`: the input text from which to extract the patterns
- `-r`: the number of patterns to extract (defaults to `10`)
- `-l`: the length of each pattern (defaults to `100`)
- `-o`: output pattern file name (defaults to `query_automake.txt`)


## 🔎 Locate Query
After index construction we can perform a locate query with a given pattern file. Note that we must use the same gcis-variant (nep or uni) for indexing and querying, otherwise we obtain a segmentation fault.
The respective programs are `gcis_nep_locate` and `gcis_uni_locate`, which share the following syntax:

Syntax: `-q patternfile [-m mode] [-i indexfile] [-r number] [-l length] [-s start]`

The most important flag is `-m` for specifying how the patternfile `-q` should be treated. 
Modes 1, 2 and 3 assume that `patternfile` is the same as the text file on which the index has been built

Mode `-m` (defaults to `3`):
- 0 : treat `patternfile` as a single pattern string
- 1 : extracts randomly pattern from `patternfile` with a fixed initial random seed 
- 2 : extracts a single pattern at `-s` with length `-l`
- 3 : same of `1`, but the starting position `-s` is selected randomly
- 4 : regard `patternfile` as a consecutive sequence of `-r` patterns, and query each of them


Other Parameters:
- `-q`: the input text from which to read the patterns
- `-r`: the number of patterns (defaults to `10`)
- `-l`: the length of each pattern (defaults to `100`)
- `-s`: start position in `patternfile` from where to read patterns
- `-i`: the filename of the index (defauls to `index`) 


## 🎌 Compile Flags
Our source files use the following preprocessor flags that can change the behavior of the execution as follows:

#### `#define DEBUGFLAG`
If enabled (set to `1`) the program works in verbose mode.

### Index Construction Compile Flags
- `#define OVERTRANSFORM`: Specifies a lower bound on the number of recursions of the GCIS factorization. Usually, our algorithm stops the recursion whenever the resulting index file size would get larger when performing an additional recursion step. A non-zero number `n` will prevent this behavior for the first `n` recursion steps.
- `#define TRANSTIMES`: Specifies an upper bound on the number of recursions of the GCIS factorization. This lets the factorization prematurely end when `TRANSTIMES`
recursions are reached. 



### Locate Compile Flags
- `#define CHECKFLAG`: If enabled, the program validates each reported occurrence by **assuming that the given query file is actually the text on which the index has been built. This flag makes therefore only sense for the modes 1,2, or 3.**


## Limitations
 - tested only with byte alphabet
 - tested with Arch Linux and gcc version 11.1.0


## 📚 References
- Tooru Akagi, Dominik Köppl, Yuto Nakashima, Shunsuke Inenaga, Hideo Bannai, Masayuki Takeda: [Grammar Index By Induced Suffix Sorting. CoRR abs/2105.13744 (2021)](https://arxiv.org/abs/2105.13744)
- Daniel Saad Nogueira Nunes, Felipe A. Louza, Simon Gog, Mauricio Ayala-Rincón, Gonzalo Navarro: [A Grammar Compression Algorithm Based on Induced Suffix Sorting. DCC 2018: 42-51](https://doi.org/10.1109/DCC.2018.00012)
