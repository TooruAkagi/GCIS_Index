#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string> 
#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream
#include <math.h>
#include <time.h>
#include <filesystem>

#define MMODE 3
#define MLONG 2500
#define MREP 10
#define CHECKFLAG 1 //これが1の場合は，結果が全部本当に正しいかをファイルをひらきながら全部チェックする
#define DEBUGFLAG 0 //これが1なら，デバッグを表示しながら検索を行う
//MMODE = 
// 0 : query.txtの中の文字列を照合
// 1 : qnameで指定されているファイルの中からランダムに MLONG 文字の文字列を選択し，
//     MREP 回実行する．
// 2 : 指定されているファイルの中のMSTARTからMLONG文字の文字列を選択肢，照合する

char qname[] = "../datalist/fib41"; //ちゃんとここも変える
char wname[] = "../esp-index-I-master/src/makingquery.txt"; //index file name
char wname2[] = "../FM-Index-master/mkquery";
char wname3[] = "pt2.txt";
char ptlist[MREP][MLONG];


int qrmload(int c,int jo){ //mの中に，english.001.2 の中からc文字をランダムに選択して保存する
    char chr;FILE *fp;
    long long int qsize = std::filesystem::file_size(qname); //qsizeには，ファイルのサイズが書かれているはず
    fp = fopen(qname, "r"); // open file or return null
    if(fp == NULL) {
        printf("%s such file doesn't exist!\n", qname);
        exit(2);
        return -1;
    }
    int i = 0;
    int jswt = 0;
    int rlen = rand()%(qsize-c);
    fseek(fp,rlen,SEEK_SET); //指定したシークまで移動する
    //printf("\n[%lld]rlen[%d]から[%d]文字(%lld)\n[",qsize,rlen,c,qsize);
    for(i=0;i<c;i++) { //c文字
        chr = fgetc(fp);
        ptlist[jo][i] = chr;
        if(chr<0){ptlist[jo][i]=chr + 256;}
        //if(ptlist[jo][i]==10){jswt=1;} //改行コードだったら-1
        //printf("%c",ptlist[jo][i]);
    }
    //printf("]");
    if(jswt==0){
        for(int i = 0;i<MLONG;i++){
            printf("%c",ptlist[jo][i]);
        }
        printf("\n");
    }
    return jswt;
}

int testrr(){
    std::string input_file = "../esp-index-I-master/src/makingquery.txt";
    std::ifstream ifs(input_file.c_str());
    ifs.seekg(0,std::fstream::end);//ファイル末尾を探す
    uint eofPos = ifs.tellg();//ファイル末尾インデクスを取得
    ifs.clear();//先頭にもどるために一度clear()をかける。これをしないと次のseekg()でコケるときがある。
    ifs.seekg(0,std::fstream::beg);//ファイル先頭に戻る
    uint begPos = ifs.tellg();//ファイル先頭インデクスを取得
    uint filsize = eofPos - begPos;//末尾-先頭でファイルサイズを計算
printf("filsize = %d,",filsize);
    std::string line;
    for(int i =0;i<100;i++){
        char chr = ifs.get();
        line = line + chr;
        printf("%c",line[i]);
    }
    //std::cout << line << std::endl;
    exit(1);
    return 0;
}



int main()
{
    srand((unsigned int)time(NULL));
    //testrr();
    const char p1[2] = "\n";
    printf("[%d %d]\n\n",p1[0],p1[1]);
    for(int i = 0;i<MREP;i++){
        int t = qrmload(MLONG,i);
        if(t!=0){i--;}
    }
    FILE *fp;
        fp = fopen(wname2, "w"); //esp-index用
        for(int jo = 0;jo<MREP;jo++){for(int i = 0;i<MLONG;i++){fprintf(fp,"%c",ptlist[jo][i]);}}
        fclose(fp); // open file or return null
        fp = fopen(wname, "w"); //FM-index用
        for(int jo = 0;jo<MREP;jo++){for(int i = 0;i<MLONG;i++){fprintf(fp,"%c",ptlist[jo][i]);}}
        fclose(fp); // open file or return null
        fp = fopen(wname3, "w");
        fprintf(fp,"#numberOfFrases::%d",MREP); //lzindex用に送る
        for(int jo = 0;jo<MREP;jo++){
            fprintf(fp,"\n       %d      %d ",jo,MLONG);
            for(int i = 0;i<MLONG;i++){
                fprintf(fp,"%c",ptlist[jo][i]);}
        }
        fclose(fp); // open file or return null
    printf("長さ%dのパターンを作って以下のファイルに保存:\n\n%s\n%s\n%s\n",MLONG,wname,wname2,wname3);
    return 0;
}
