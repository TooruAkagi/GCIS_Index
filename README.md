This folder contains programs for creating and testing gcis-nep and gcis-uni.
First, move to this folder and execute make.

in terminal, 

$cd GCIS_Pattern_Matching_master
$make

Successful if the following files are generated without any error : 

bldA bldB schA schB genpattern


-----bldA--------------------------------------------------
You can make the index file of gcis-nep with the following command :
$ ./bldA
Then put the name of input file you want the index. After running, "index" will be generated.


you can edit variants in source file "gcis.cpp", to some settings

[DEBUGFLAG]
if 1, how this algorithm process while construction is written in terminal

[OVERTRANSFORM]
if over than 0, GCIS will repeat construction in (OVERTRANSFORM) times even if index size is larger than the one finishing in previous construction.

[TRANSTIMES]
GCIS finishes construction at most (TRANSTIMES) times.

-----schA--------------------------------------------------
You can test pattern matching of gcis-nep with the following command :
$./schA
Before that, you must do $./bldA and make "index" first.

you can edit variants in source file "idx_nep.cpp", to some settings

[MMODE]
use to select query.
0 : open "query.txt" in this folder, and regard it as a pattern and search it.
1 : open qname(variants defined in line 26), and select a position in it regularly, and search  
the pattern of length [MRANDBASE] ~ [MRANDBASE]+[MRANDRANGE]. repeat [MREP] times.
2 : open qname, and search the pattern of [MLONG], starts at [MSTART].
3 : same of 1, but a starting position is selected randomly.
4 : open "query.txt" in this folder, and regard it as a sequence of 10 patterns, and search it.
5 : not used

[CHECKFLAG]
if 1, program will checks the occurrence (open qname, and seek the locations GCIS answered)

[DEBUGFLAG]
if 1, all of the process of pattern matching is written in terminal.



-----bldB--------------------------------------------------

You can make the index file of gcis-uni with the following command :
$./bldB
Before that, you must do $./bldA and make "index" first.
After running, "codeindex" will be generated.


-----schB--------------------------------------------------

You can test pattern matching of gcis-uni with the following command :
$./schB


you can edit variants in source file "idx_uni.cpp", to some settings
[MMODE]
use to select query.
0 : open "query.txt" in this folder, and regard it as a pattern and search it.
1 : open qname(variants defined in line 26), and select a position in it regularly, and search  
the pattern of length [MRANDBASE] ~ [MRANDBASE]+[MRANDRANGE]. repeat [MREP] times.
2 : open qname, and search the pattern of [MLONG], starts at [MSTART].
3 : same of 1, but a starting position is selected randomly.
4 : open "query.txt" in this folder, and regard it as a sequence of 10 patterns, and search it.
5 : not used

[CHECKFLAG]
if 1, program will checks the occurrence (open qname, and seek the locations GCIS answered)

[DEBUGFLAG]
if 1, all of the process of pattern matching is written in terminal.


-----genpattern--------------------------------------------------

You can make a pattern file of your dataset with the following command : 
$./genpattern

"query_automake.txt" will be generated in this folder, and patterns are written in it.

you can edit variants in source file "makequery.cpp", to some settings

[MLONG]
the length of patterns
[MREP]
the number of patterns written in wname(defined in line 15)

pattern is generated in qname(defined in line 14)




