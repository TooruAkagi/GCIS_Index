This folder contains programs for creating and testing gcis-nep and gcis-uni.
First, move to this folder and execute make.

in terminal, 

$cd GCIS_Pattern_Matching_master
$make

Successful if the following files are generated without any error : 

bldA bldB schA schB genpattern


You can make the index file of gcis-nep with the following command :
$ ./bldA
Then put the name of input file you want the index. After running, "index" will be generated.

You can test pattern matching of gcis-nep with the following command :
$./schA
Before that, you must do $./bldA and make "index" first.

You can make the index file of gcis-uni with the following command :
$./bldB
Before that, you must do $./bldA and make "index" first.
After running, "codeindex" will be generated.

You can test pattern matching of gcis-uni with the following command :
$./schB

You can make a pattern file of your dataset with the following command : 
$./genpattern
