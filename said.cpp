#include<iostream>
#include<stdlib.h>
#include<string> 
#include <fstream>  
#include <sstream> 
#include "sais.hxx"
#include <time.h>
#include <filesystem>


#define MLONG 100
#define MREP 20
#define MMAXOCC 100 //10-100くらい出現する長さ10の文字列を探す
#define MMINOCC 90
#define SAMODE 1 //0ならsaを作って保存する．1なら読む

char fname[] = "../datalist/english.001.2";
char qname[] = "query.txt";
char wname[] = "../esp-index-I-master/src/makingquery.txt"; //index file name
char wname2[] = "../FM-Index-master/mkquery";
char wname3[] = "../r-index-master/build/ptax";
unsigned char ptlist[MREP][MLONG];
unsigned char *m;
int lps = 0;
int occmax = 0;
int occmin = 99999999;
int limit = 10000000;

int match(unsigned char *T,int s,int mlen){
    //compare if m and T(starting from s) match or mismatch
    //printf("(%d)",s);
    for(int i = 0;i<mlen;i++){
        //printf("%c",T[s+i]);
        if(T[s+i]!=ptlist[lps][i]){
            if(T[s+i]>ptlist[lps][i]){return 1;}else{return -1;}
        }
    }
    //printf("\n[%d is occurance]",s);
    return 0;
}

int sasearch(int *sa,unsigned char *T,int mlen,int n,int mod){
    //sasearch[mod 0]--sasearch[mod 1] = the range of occurance 
    //printf("search start(%d %d)",mlen,n);
    int posbt = -1;
    int posup = n;
    int pos;
    int rs = -1;
    while(1){
        if(posup-posbt<2){break;}
        pos = (posbt+posup)/2;
        //printf("\n[%d]",pos);
        int k = match(T,sa[pos],mlen);
        if(k==1){posup = pos;}
        if(k==-1){posbt = pos;}
        if(k==0){
            rs = pos;
            if(mod == 0){posup = pos;} //辞書じゅん最大
            if(mod == 1){posbt = pos;} //辞書じゅん最小
        }
    }
    //printf("\nreturn : %d\n",rs);
    return rs;
}

int qrmload(int c,int jo){ //mの中に，english.001.2 の中からc文字をランダムに選択して保存する
    char chr;FILE *fp;
    long long int fsize = std::filesystem::file_size(fname); //qsizeには，ファイルのサイズが書かれているはず
    fp = fopen(fname, "r"); // open file or return null
    if(fp == NULL) {
        printf("%s such file doesn't exist!\n", fname);
        exit(2);
        return -1;
    }
    int i = 0;
    int jswt = 0;
    int rlen = rand()%(fsize-c);
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
    return jswt;
}

int write6(int *sa,long long int samax){
    FILE *fp3;
    fp3 = fopen("suffixarray", "w"); // open file or return null
    fwrite(sa, sizeof(int),samax, fp3); //2:dc2s
    fclose(fp3); // open file or return null
    return 0;
}

int load6(int *sa,long long int samax){
    FILE *fp3;
    fp3 = fopen("suffixarray", "r");
    int flag = 0; //0:loops,1:dcs,2:dc2s,3:D,4:D_2,5:D_3,6:NS
    int w = 0;int a;
    unsigned char chr;
    while(flag<1){
        chr = fgetc(fp3);a = chr;
        chr = fgetc(fp3);a += chr*256;
        chr = fgetc(fp3);a += chr*65536;
        chr = fgetc(fp3);a += chr*65536*256;
        if(flag==0){sa[w]=a;w++;if(w==samax){flag++;printf("\n-finishing load index-\n");}}
    }
    fclose(fp3);
    return 0;
}

int main(){
    FILE *fp;
    unsigned char *T;
    char chr;
    long long int fsize = std::filesystem::file_size(fname);
    long long int qsize = std::filesystem::file_size(qname);
    int i = 0;
    int *sa;
    int size = fsize;
    fp = fopen(fname, "r"); // open file or return null
    if(fp == NULL) {
        printf("%s such file doesn't exist!\n", fname);
        return -1;
    }
    printf("\nfile %s 's size = %lld byte",fname,fsize);
    T = (unsigned char*)malloc(fsize);
    while((chr = fgetc(fp)) != EOF) {
        T[i] = chr;
        if(chr<0){T[i]=chr + 256;}
        i++;
    }
    fclose(fp); //close textfile and then open query file


    fp = fopen(qname, "r"); // open file or return null
    if(fp == NULL) {
        printf("%s such file doesn't exist!\n", qname);
        return -1;
    }
    m = (unsigned char*)malloc(qsize);
    i=0;
    while((chr = fgetc(fp)) != EOF) {
        m[i] = chr;
        if(chr<0){m[i]=chr + 256;}
        //printf("\n%d(%d)",m[i],i);
        i++;
    }
    //int sa[fsize+10];
    sa = (int*)malloc((fsize+1)*4);
    if(SAMODE==0){
        printf(" (%lld) \nsuffix array作成開始",fsize);
        saisxx(T,sa,size,510); //ここでSuffixArrayを作ります
        write6(sa,fsize); //ここでSuffixArrayを作ります
        printf(" (%lld) \nsuffix array作成完了",fsize);
    }
    if(SAMODE==1){  
        printf(" (%lld) \nsuffix array読み込み開始",fsize);
        load6(sa,fsize); //ここでSuffixArrayを作ります
        printf(" (%lld) \nsuffix array読み込み完了",fsize);
    }
    for(int u=0;u<10;u++){printf("[%d]",sa[u]);}
    clock_t timesb = clock();
    
    int locate = 2;
    if(locate==1){
        int d = sasearch(sa,T,qsize,size,0);
        int d2 = sasearch(sa,T,qsize,size,1);
        for(int j = d-1;j<d2+2;j++){
            printf("\n%d[",sa[j]);
            for(int ko=0;ko<qsize+10;ko++){
                printf("%c",T[sa[j]+ko]);
            }
            printf("]");
            int y = sa[j];
            int k = 0;
            while(y<size && k<25){
                //printf("%c ",T[y]);
                y++;k++;
               //if(k==25){printf("...");}
            }
        }
        printf("\nall occ : %d\n",d2-d+1);
    }
    if(locate==0){
        int d = sasearch(sa,T,qsize,size,0);
        int d2 = sasearch(sa,T,qsize,size,1);
        printf("\ncount : %d\n",d2-d+1);
    }
    if(locate==2){
        srand((unsigned int)time(NULL));
        while(lps<MREP){
            int t = qrmload(MLONG,lps);
            int d = sasearch(sa,T,MLONG,size,0);
            int d2 = sasearch(sa,T,MLONG,size,1);
            if(d2-d+1<= MMAXOCC && d2-d+1>=MMINOCC){
                for(int iy = 0;iy<MLONG;iy++){
                    printf("%c",ptlist[lps][iy]);
                }
                printf("(occ : %d)\n",d2-d+1);lps++;
                /*for(int j = d-1;j<d2+2;j++){
                    printf("\n%d[",sa[j]);
                    for(int ko=0;ko<MLONG+3;ko++){
                        printf("%c",T[sa[j]+ko]);
                    }
                    printf("]");
                }*/
            }
            else{limit--;
                if(d2-d+1 > occmax){occmax = d2-d+1;}
                if(d2-d+1 < occmin){occmin = d2-d+1;}
                if(limit<1){
                    printf("[検索しすぎ]");
                    break;}
            }
        }
        printf("\noccmax:%d\noccmin:%d\nlimit :%d\n",occmax,occmin,limit);
        FILE *fp2;
        fp2 = fopen(wname2, "w");
        for(int jo = 0;jo<MREP;jo++){
            for(int i = 0;i<MLONG;i++){fprintf(fp2,"%c",ptlist[jo][i]);}
        }
        fclose(fp2); // open file or return null
        fp2 = fopen(wname, "w");
        for(int jo = 0;jo<MREP;jo++){
            for(int i = 0;i<MLONG;i++){fprintf(fp2,"%c",ptlist[jo][i]);}
        }
        fclose(fp2); // open file or return null
        fp2 = fopen(wname3, "w");
        fprintf(fp2,"# number=%d length=%d file=%s forbidden=\n",MREP,MLONG,fname);
        for(int jo = 0;jo<MREP;jo++){
            for(int i = 0;i<MLONG;i++){fprintf(fp2,"%c",ptlist[jo][i]);}
        }
        fclose(fp2);
        printf("長さ%dのパターンを作ってファイルに保存:\n",MLONG);
    }
    clock_t timee = clock();
    const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC * 1000.0;


    printf("\ntime %lf[ms]\n", time);
    return 0;
}