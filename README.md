This folder contains programs for creating and testing gcis-nep and gcis-uni.
First, move to this folder and execute make.

in terminal, 

$cd GCIS_Pattern_Matching_master
$make

Successful if the following files are generated without any error : 

gcis gcis_uni idx_nep idx_uni genpattern

running it with -h shows some help.

-----gcis_nep--------------------------------------------------
You can make the index file of gcis-nep with the following command :
$ ./gcis_nep -i <inputfile>
Then put the name of input file you want the index. After running, "index" will be generated.
To set the name of outputfile, type
$ ./gcis_nep -i <inputfile> -o <outputfile>


you can edit variants in source file "gcis.cpp", to some settings
[DEBUGFLAG]
if 1, how this algorithm process while construction is written in terminal

[OVERTRANSFORM]
if over than 0, GCIS will repeat construction in (OVERTRANSFORM) times even if index size is larger than the one finishing in previous construction.

[TRANSTIMES]
GCIS finishes construction at most (TRANSTIMES) times.

-----idx_nep--------------------------------------------------
You can test pattern matching of gcis-nep with the following command :
$ ./idx_nep -q <queryfile>
Before that, you must do $./gcis_nep and make "index" first.

Default indexfile is "index". To set the indexfile, type
$ ./idx_nep -q <queryfile> -i <indexfile>

There are some modes about query, shown in detail.
Default mode is 3. To set the mode, type
$ ./idx_nep -q <queryfile> -i <indexfile> -m <mode>

You can set <repetation> with -r, <length> with -l, respectively.
They are used in mode 1 & 3 & 4.

For example,
$ ./idx_nep -q inputsample.txt -m 3 -r 10 -l 100
is search 10 ramdom patterns of length 100 in inputsample.txt .

you can edit variants in source file "idx_nep.cpp", to some settings
[MMODE]
use to select query.
0 : open <queryfile> in this folder, and regard it as a pattern and search it.
1 : open <queryfile>, and select a position in it regularly, and search  
the pattern of length <length>. repeat <repetation> times.
2 : open <queryfile>, and search the pattern of <length>, starts at [MSTART].
3 : equal to mode 1, but a starting position is selected randomly.
4 : open <queryfile> in this folder, and regard it as a sequence of <repetation> patterns, and search it.
5 : not used

[CHECKFLAG]
if 1, program will checks the occurrence (open qname, and seek the locations GCIS answered)

[DEBUGFLAG]
if 1, all of the process of pattern matching is written in terminal.



-----gcis_uni--------------------------------------------------

You can make the index file of gcis-uni with the following command :
$./gcis_uni
Before that, you must do $./gcis_nep and make "index" first.
After running, "codeindex" will be generated.

You can set the name of inputfile by using option -i .
$ ./gcis_uni -i <inputfile>
You can set the name of outputfile by using option -o .
$ ./gcis_uni -o <outputfile>

Default inputfile is "index", 
Default outputfile is "codeindex".(it works with no option.)

-----idx_uni--------------------------------------------------

You can test pattern matching of gcis-uni with the following command :
$./idx_uni -q <queryfile>

All Option(-i -o -m -r -l) is same as idx_nep. For example,
$ ./idx_nep -q inputsample.txt -m 3 -r 10 -l 100
searches 10 ramdom patterns of length 100 in inputsample.txt .

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
$ ./genpattern -i <inputfile>
"query_automake.txt" will be generated in this folder, and patterns are written in it.

You can set the name of output by using option -o instead of "query_automake.txt".
$ ./genpattern -i <inputfile> -o <outputfile>
You can set the number of query by using option -r.
10 is used by default.(defined 'MREP' in this code.)
$ ./genpattern -i <inputfile> -o <outputfile> -r <times>
You can set the length of pattern by using option -l. 
100 is used by default.(defined 'MLONG' in this code.)
$ ./genpattern -i <inputfile> -o <outputfile> -l <length>

For example, 
$ ./genpattern -i inputsample.txt -o query.txt -l 100 -r 10
writes 10 ramdom patterns of length 100 from inputsample.txt to query.txt.

you can edit variants in source file "makequery.cpp", to some settings

[MLONG]
the length of patterns ( default <length> )
[MREP]
the number of patterns written in <outputfile> ( default <times> )




