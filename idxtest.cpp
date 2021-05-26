#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string> 
#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream
#include <math.h>
#include <time.h>
#include <filesystem>

#define MMODE 2
#define MLONG 0
#define MREP 218
#define MSTART 0
#define MRANDRANGE 200
#define MRANDBASE 100
#define CHECKFLAG 1 //これが1の場合は，結果が全部本当に正しいかをファイルをひらきながら全部チェックする
#define DEBUGFLAG 0 //これが1なら，デバッグを表示しながら検索を行う
//MMODE = 
// 0 : query.txtの中の文字列を照合
// 1 : qnameで指定されているファイルの中からランダムに MLONG 文字の文字列を選択し，
//     MREP 回実行する．
// 2 : 指定されているファイルの中のMSTARTからMLONG文字の文字列を選択肢，照合する

char qname[] = "../datalist/english.001.2"; //ちゃんとここも変える
char wname[] = "index"; //index file name
int checkans[1000];
int checkpz[5];
int occart[10]; //-1,-1,X,ofc,-1,-1,-1
//Xの-ofsに出現し，左はroundLまで，右はroundRまで測定済
int ancl[MREP][5];
int occart2[30][10]; //高さ30に対して(左0,左1,分割番号,ofs,右1,右2,右3,測定内容)
//測定内容 0 = なし, 1 = B,2 = AB,3 = BC,4 = ABC
int coredata[10]; //Aの判定,Aの開始位置,Aの長さ,Bの判定[1],Bの開始位置,Bの長さ,Cの判定,Cの開始位置,Cの長さ,
double ancd[MREP];
int dcs[18]; //[i] = starting position of floor-i's first purse in the Dictionary
int dc2s[18]; //[i] = starting number of floor-i's first number in the Dictionary
int dbg = 0; //for debuging
int loops =5; //the number of applying GCIS to the input file
int ocofs = 0;
int *m;
unsigned int *NS;
int NSsize;
int transk = 0;
int fround = 0;

int core = 0;

void alldr(FILE** fp,unsigned int *D,int *D_2,int *D_3,int i,int round,long long int qsize){
     if(transk>qsize-1){return;}
    int rofs = 0;if(round>1){rofs=dc2s[round-1];}
    int inrofs = 0;if(round>2){inrofs=dc2s[round-2];}
        if(round>1){
            if(D_3[i]<=MSTART - transk){
                if(DEBUGFLAG==1){
                    if(MSTART - transk < 1500){
                            printf("\n[%d]",i);
                            printf("%d(+%d [%d])\n",transk,D_3[i],i);
                    //printf("\ncut(%d)[+%d(%d)]",i,D_3[i],transk);
                    }
                }
                transk+=D_3[i];
                return;}
            else{
                if(DEBUGFLAG==1){printf("\n[open %d(+%d)]",i,D_3[i]);}
            }
        }
        //printf("\n[rofs = %d(%d)]",rofs,round);
        //printf("\n%d[%x] > %d[%d(%d)]:",round,i,i+rofs,D_2[i],D_3[i]);
        for(int j=D_2[i];j<D_2[i+1];j++){
            if(round>1){alldr(fp,D,D_2,D_3,D[j]+inrofs,round-1,qsize);}
            else{
                if(MSTART >transk){transk++;continue;} //１小さい場合はコンテ
                int chr = fgetc(*fp);transk++;
                //printf("%c",chr);
                if(transk>qsize-1){printf("\nここまで完璧");return;}
                if(chr!=D[j]){printf("[%d(%c) != %d(%c)]",chr,chr,D[j],D[j]);exit(2);}
            }
        }
}

void gcisopen(unsigned int *D,int *D_2,int *D_3,int i){
    int round = 0;
    for(int u = 0;u<loops+1;u++){
        if(i>=dc2s[u]){round++;}
        else{break;}
    }
    printf("\nround = %d\n[%d] : ",round,i);
    int rofs = 0;if(round>1){rofs=dc2s[round-1];}
    int inrofs = 0;if(round>2){inrofs=dc2s[round-2];}
        for(int j=D_2[i];j<D_2[i+1];j++){
            if(round==1){printf("%c",D[j]+inrofs);}
            else{printf(" %d",D[j]+inrofs);}
        }
        printf("\n");
        return;
}



int main()
{
    long long int sps = 0;
    long long int spsmax = 0;
    FILE *fp;
    
    unsigned int *D;
    int *D_2,*D_3;
    int i = 0;
    int msize = 10000;
    int pn = 0;
    int round = 0;
    
    m = (int*)malloc(4*msize);
    //msize = queryload(m);
    dcs[0] = 0;dc2s[0] = 0; 

    unsigned char chr;
    unsigned int a;
    
    fp = fopen(wname, "r");
    int flag = 0; //0:loops,1:dcs,2:dc2s,3:D,4:D_2,5:D_3,6:NS
    int w = 1;int tm1;
    while(flag<8){
        fscanf(fp,"%c",&chr);a = chr;
        fscanf(fp,"%c",&chr);a += chr*256;
        fscanf(fp,"%c",&chr);a += chr*65536;
        fscanf(fp,"%c",&chr);a += chr*65536*256;
        //if(w<30){printf("\n[%d]:%u",flag,a);}
        if(flag==7){NS[w]=a;w++;if(w==tm1){flag++;printf("\n-finishing load index-\n");}}
        if(flag==6){tm1=a;NS = (unsigned int*)malloc(4*tm1);flag++;}
        if(flag==5){D_3[w]=a;w++;if(w==dc2s[loops]){flag++;w=0;}}
        if(flag==4){D_2[w]=a;w++;if(w==dc2s[loops]+1){flag++;w=0;}}
        if(flag==3){D[w]=a;w++;if(w==dcs[loops]){flag++;w=0;}}
        if(flag==2){dc2s[w]=a;if(w==loops){flag++;w=0;D_2 = (int*)malloc(4*dc2s[loops]+4);D_3 = (int*)malloc(4*dc2s[loops]+4);}else{w++;}}
        if(flag==1){dcs[w]=a;if(w==loops){flag++;w=1;D = (unsigned int*)malloc(4*dcs[loops]);}else{w++;}}
        if(flag==0){loops=a;flag++;}
        //fseek(fp,16,SEEK_CUR);

    }
    fclose(fp);
    printf("\nNS = %d,%d,%d ...\n",NS[0],NS[1],NS[2]);
    long long int qsize = std::filesystem::file_size(qname);
    if(MLONG>0){qsize = MLONG+MSTART;}
    fp = fopen(qname,"r");
    fseek(fp,MSTART,SEEK_SET);
    printf("[%d]からチェック",MSTART);
    for(int i = 0;i<loops+1;i++){
        printf("loops[%d] = %d\n",i,dc2s[i]);
    }
    for(int i = 0;i<tm1;i++){
            if(transk<MSTART && transk+1500 >MSTART){printf("\n<%d : %d>",i,NS[i]+dc2s[loops-1]);}
            alldr(&fp,D,D_2,D_3,NS[i]+dc2s[loops-1],loops,qsize); 
        }
    for(int i = 0;i<1000;i++){
        scanf("%d", &pn);
        gcisopen(D,D_2,D_3,pn);
    }
    exit(0);

    free(NS); 
    free(D); 
    free(D_2); 
    free(D_3); 
    return 0;
}
