ここにはGCIS-indexの検索方法をテストするための
簡単なプログラムを保管しています．
まずはフォルダーをダウンロードして，

$cd GCIS_Pattern_Matching_master
$make

を実行してください．以下の5つのファイルが生成されれば成功です．

gcis gcis_uni idx_nep idx_uni genpattern

それぞれのプログラムの実行時に -h をオプションで付けると，
ヘルプが表示されます．

-----gcis_nep--------------------------------------------------
GCISの文法を直接保存したデータを作成します．
$ ./gcis_nep -i <inputfile>
で実行できます．

-o オプションで，出力ファイル名を指定できます．
初期設定では"index"というファイルに出力します．
$ ./gcis_nep -i <inputfile> -o <outputfile>


gcis_nep.cpp のヘッダー直後には3つの可変パラメータがあります．

[DEBUGFLAG]
1なら，GCISのプロセスを表示します．

[OVERTRANSFORM]
出力ファイルのサイズが大きくなっても，指定された回数
GCISの変換を繰り返します．

[TRANSTIMES]
出力ファイルのサイズが小さくなっても，指定された回数で
GCISの変換を中止します．

-----idx_nep--------------------------------------------------
gcis_nepで生成したindexファイルを使って，照合を実行します．
$ ./idx_nep -q <queryfile>

-i オプションで，indexファイルの名前を指定できます．
初期設定では"index"になっています．
$ ./idx_nep -q <queryfile> -i <indexfile>

-m オプションで，照合のモードを指定できます．
初期設定では"3"になっています．モードの詳細は下にあります．
$ ./idx_nep -q <queryfile> -i <indexfile> -m <mode>

照合するクエリの総数を -r，照合する文字列の長さを -l のオプションで指定できます．
You can set <repetation> with -r, <length> with -l, respectively.
プログラムの #define パラメータで，それぞれ MREP / MLONG という名前で初期値が指定されています．未指定の場合はその数値が使われます．

[例]
$ ./idx_nep -q inputsample.txt -m 3 -r 10 -l 100
長さ100のパターンを,inputsample.txtからランダムに10個探して，照合します．

[モードについて]
-m で指定できる照合のモードについて説明します．
0 : <queryfile>  に書かれている内容が，1つのクエリです．
1 : <queryfile>  を開き，[MREP]回，その中からランダムに文字列を選択し，照合します．
2 : <queryfile>  を開き，[MSTART]から始まる長さ[MLONG]のパターンを検索します．(1回のみ)
3 : モード1 を，srand()関数を実行してから行います．
4 : <queryfile>  を開き，それを[MREP]個のパターンの列として，[MREP]回 文字列を照合します．
5 : 未使用

[CHECKFLAG]
1の場合，得られた出現位置を，<queryfile> を開き，実際に確認します．モード1,2,3で利用できます．間違っていた場合はexitで停止します．

[DEBUGFLAG]
1の場合，照合のプロセスを表示します．



-----gcis_uni--------------------------------------------------

gcis_nepで作成したindexファイルをコーディングして出力します．
$./gcis_uni

-i オプションで，indexファイルの名前を指定できます．
初期設定では"index"になっています．
$ ./gcis_uni -i <inputfile>

-o オプションで，出力ファイルの名前を指定できます．
初期設定では"codeindex"になっています．
$ ./gcis_uni -o <outputfile>

-----idx_uni--------------------------------------------------

gcis_uniで作成したファイルから照合を行います．
$./idx_uni -q <queryfile>

※全てのオプションはidx_nepと同じです．詳しくはあちらを参照してください．

-----genpattern--------------------------------------------------

ファイルの中の文字列をランダムに選択し，クエリファイルを出力します．
$ ./genpattern -i <inputfile>
"query_automake.txt" will be generated in this folder, and patterns are written in it.

-o オプションで，出力ファイルの名前を指定できます．
初期設定では "query_automake.txt" になっています．
$ ./genpattern -i <inputfile> -o <outputfile>


生成するクエリの総数を -r，生成するパターンの長さを -l のオプションで指定できます．
$ ./genpattern -i <inputfile> -o <outputfile> -r <times>
$ ./genpattern -i <inputfile> -o <outputfile> -l <length>
プログラムの #define パラメータで，それぞれ MREP / MLONG という名前で初期値が指定されています．未指定の場合はその数値が使われます．

[例]
$ ./genpattern -i inputsample.txt -o query.txt -l 100 -r 10
inputsample.txtから10種類，長さ100の文字列を選択してquery.txtに保存します．



以上で説明は終わります．
自由に個人で改造して利用してください．