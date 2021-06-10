#include<iostream>
#include<stdlib.h>
#include<string> 
#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream
#include <math.h>
#include <time.h>
#include <filesystem>
#include <cstring> //depends on your enviroment

#define DEBUGFLAG 0 // 1 -> display the process with printf
#define OVERTRANSFORM 0 //the least times of repetation
#define TRANSTIMES 99 //the maximum limit of repeation

char *fname,*wname;
char defaultfname[] = "inputsample.txt";
char defaultwname[] = "index";
int (*nextsort)[3];
int sortstack = 0; 
int tempC = 260;
int codeA = 0; // = length of all Dictionaries
int codeB = 0; // = frontencoded codeA
int dcs[20]; //[i] = starting position of floor-i's first purse in the Dictionary
int dc2s[20]; //[i] = starting number of floor-i's first number in the Dictionary
int dbg[300]; //for debugings
int loops = 17; //the number of applying GCIS to the input file
int ans = 0;

int purselcp(unsigned int *T,int *P,int *P_2,int *I,int p){
    //Get Longest Common Prefix of the purse No.i and the purse No.i-1
    // if i = 0 : return 0.
    if(p<1){return 0;} 
    int a = 0;
    while(a<P_2[I[p-1]]){ 
        if(T[P[I[p-1]]+a] == T[P[I[p]]+a]){ 
            a++;
        }
        else{
            break;
        }
    }
    return a; 
}

void bbsort(unsigned int *T,int *P,int *P_2,int *I,int a,int b,int c){
    //to sort [c] purse(s) with [a]-th character from position [b] in I.
    //[c is really small] 
    sortstack--;
    if(DEBUGFLAG==1){printf("\nbbsort(%d,%d,%d)",a,b,c);}
    for(int i = 0;i<c;i++){
        int k = i;
        //int bc = T[P[I[b+i]]+a]; //a-th character in I[b+i]
        if(a+1 > P_2[I[b+i]]){continue;} //
        for(int j = i; j<c; j++){
            if(a+1 > P_2[I[b+j]]){k = j;break;}  //if the purse [i] doesn't have a-th character,exactly it's smaller;
            else{ //check next cheracter until one of each purse reaches the end
                int d = a;
                while(1){
                    //printf("\n[%d]",d);
                    if(T[P[I[b+k]]+d] > T[P[I[b+j]]+d]){k = j;break;}
                    if(T[P[I[b+k]]+d] < T[P[I[b+j]]+d]){break;}
                    d++;
                    if(d+1 > P_2[I[b+k]]){break;}
                    if(d+1 > P_2[I[b+j]]){k=j;break;}
                }
            }
        }
        if(k!=i){
            //printf("swap!");
            int tem = I[b+i];
            I[b+i] = I[b+k];
            I[b+k] = tem;
        }
    }
}

void csort(unsigned int *T,int *P,int *P_2,int *I,int a,int b,int c){ 
    //to sort [c] purse(s) with [a]-th character from position [b] in I.
    if(DEBUGFLAG==1){printf("\ncsort(%d,%d,%d)",a,b,c);}
    //if(a!=0){exit(6);}
    sortstack--;
    long int *cbox;
    int *cptr;
    cbox = (long int*)calloc(tempC,8);//for bucket sort
    cptr = (int*)calloc(tempC,4);//for bucket sort
    for(int i = 0; i<c; i++){
        if(a+1 > P_2[I[b+i]]){cptr[0]++;}  //if the purse [i] doesn't have a-th character,cptr[0]++;
        else{
            cbox[T[P[I[b+i]]+a]]++;
        }
    }
    for(int i = 0;i<tempC;i++){
        if(i>0){
            cptr[i] = cptr[i-1] + cbox[i-1];
        }
        if(cbox[i]>1){
            sortstack++;
            nextsort[sortstack][0] = a+1;
            nextsort[sortstack][1] = b+cptr[i];
            nextsort[sortstack][2] = cbox[i];
        }
        
    }
    
    int *H;
    H = (int*)calloc(c,4);  //temporary array for storing the sorted array
    if (H==NULL) {exit (1);}
    cptr[0] = 0;
    for(int i = 0; i<c; i++){
        int j = 0;
        if(P_2[I[b+i]] > a){ //if i-th purse has a-th character
             j = T[P[I[b+i]]+a]; //get it and put the purse in the corresponding backet
             
        }
        H[cptr[j]] = I[b+i];
        cptr[j]++;
        
    }
    for(int i = 0; i < c; i++){
        I[b+i] = H[i];
    }
    free(H);
    free(cbox);
    free(cptr);
}

int purse(unsigned int *T,int *P,int *P_2,long int fsize){ //purse T and set P.
    std::cout << std::endl;
    int tn = -1; //temporary number
    int pn = 1;
    P[0] = 0;
    for (int i = 0 ; i < fsize-1 ; i++){
        if(T[i] > T[i+1]){ //T[i]がL 
            tn = i;
        }
        if(T[i] < T[i+1] && tn != -1){
            P[pn] = tn+1;
            P_2[pn-1] = P[pn] - P[pn-1];
            pn++;
            tn = -1;
        }
    }
    P[pn] = tn+1;
    P_2[pn-1] = fsize - P[pn-1]; //not to left last character
    
    int k = 0;
    for(int i = 0 ; i < fsize ; i++){
        if(P[k]==i){
            if(i+40 > fsize){printf("(%d,%d)",P[k],P_2[k]);}
            k++;
        }
        //if(i > fsize/2){exit(3);}
        if(i+40 > fsize){
            std::cout << std::hex << int(T[i]);
            std::cout << " ";
        }
    }
    return pn;
}

void pnsort(unsigned int *T,int *P,int *P_2, int *I,int pn){ //Sort all purses
    nextsort = (int(*)[3])malloc(sizeof(int) * 1000000 * 3);
    sortstack = 0; 
    csort(T,P,P_2,I,0,0,pn); //sort (one time)
    while(sortstack >= 0){
        if(nextsort[sortstack][2]<1000){
            bbsort(T,P,P_2,I,nextsort[sortstack][0],nextsort[sortstack][1],nextsort[sortstack][2]);
        }
        else{
            csort(T,P,P_2,I,nextsort[sortstack][0],nextsort[sortstack][1],nextsort[sortstack][2]);
        }
    }
    free(nextsort);
}

void makedic(unsigned int *T,int *P,int *P_2,int *K,unsigned int *NS,unsigned int *D,int *D_2,int *D_3,int f){ 
    //make dictionary
    printf("\nf = %d / dcs[%d]=%d dc2s[%d] = %d\n",f,f+1,dcs[f+1],f+1,dc2s[f+1]);
    int t = 0; //the number of the kind of different purse
    int starti = dc2s[f];
    int starts = dcs[f]; 
    int s = starts;
    if(DEBUGFLAG==1){printf("\n%d,%d,%d",dc2s[f+1],dcs[f+1],starts);}
    for(int i = starti;i<=dc2s[f+1];i++){ //t = dc2s[f+1]
        int a = 0;
        D_3[i] = 0;
        D_2[i] = s;
        if(DEBUGFLAG==1){printf("\n[%d]",i);}
        while(a<P_2[K[t]]){
            D[s] = T[P[K[t]]+a];
            if(DEBUGFLAG==1){printf("%x ",D[s]);}
            if(f>0){D_3[i]+= D_3[dc2s[f-1]+T[P[K[t]]+a]];}
            if(f==0){D_3[i]++;}
            s++;
            a++;
        }
        if(DEBUGFLAG==1){printf(" / %d:%d:%d /",i,D_2[i],D_3[i]);}
        t++;
    }
    dc2s[f+1]++;
    D_2[dc2s[f+1]] = s;
    printf("\ns = %d, t = %d, dc2s[f+1] = %d\n",s,t,dc2s[f+1]);
}

int grammarmake(unsigned int *T,int *P,int *P_2,int *I,unsigned int *NS,int *K,int pn){ //文法を表示し，文法の総数tを返す．
    int t = -1; 
    for(int i = 0; i < pn; i++){ //check all purse in lexiorder
        int k = purselcp(T,P,P_2,I,i);
        if(k != P_2[I[i]]){
            codeA += P_2[I[i]];
            codeB += P_2[I[i]] - k;
            t++;
            K[t] = I[i]; //K[t] = t-th smallest purse in current round
        }
        NS[I[i]] = t; //transform T and put it into NS
    }
    printf("\nthe kind of all purse = %d",t+1);
    printf("\nsum of length of rightside characters = %d",codeA);
    printf("\n(when Front Encoding = %d)\n",codeB);
    return t;
}

int write6(unsigned int *D,int *D_2,int *D_3,unsigned int *NS,int NSsize){
    FILE *fp;
    //unsigned char chr;
    //unsigned int a;
    fp = fopen(wname, "w"); // open file or return null
    fwrite(&loops, sizeof(int),1, fp); //0:loops
    fwrite(&dcs[1], sizeof(int), loops, fp); //1:dcs[1][2][3]...
    fwrite(&dc2s[1], sizeof(int), loops, fp); //2:dc2s
    fwrite(D, sizeof(unsigned int), dcs[loops], fp); //3:D
    fwrite(D_2, sizeof(unsigned int), dc2s[loops]+1, fp); //4:D_2
    fwrite(D_3, sizeof(unsigned int), dc2s[loops], fp); //5:D_3
    fwrite(&NSsize, sizeof(int),1, fp); //6:NSsize
    fwrite(NS, sizeof(unsigned int),NSsize, fp); //7:NS
    fclose(fp); // open file or return null
    return 0;
}

int shelp(){
    printf("--Simple Usage------------------\n");
    printf("\nRead <file> and make <indexfile>. \n");
    printf(": ./gcis_nep_build -i <file>\n");
    printf("\nYou can set the name of output by using option -o. \n");
    printf(": ./gcis_nep_build -i <file> -o <outputfile>\n");
    printf("\n");
    printf("\n");
    return 0;
}

int main(int argc, char *argv[])
{
    long long int sps = 0;
    long long int spsod = 0;
    FILE *fp;
    unsigned int *T,*D,*NS;
    int *P,*P_2,*I,*K,*D_2,*D_3;
    int i = 0;
    int pn = 0;
    int round = 0;
    
    long long int fsize = 0;
    for(int i = 0;i<argc;i++){
      if('-'==argv[i][0]){ //符号
        if('i'==argv[i][1]){ //input
            int k = strlen(argv[i+1]);
            fname = (char*)malloc(k);
            for(int k2=0;k2<k;k2++){fname[k2]=argv[i+1][k2];}
        }
        if('o'==argv[i][1]){ //input
            int k = strlen(argv[i+1]);
            wname = (char*)malloc(k);
            for(int k2=0;k2<k;k2++){wname[k2]=argv[i+1][k2];}
        }
        if('h'==argv[i][1]){shelp();return 0;} //help
        i++;continue;
      }
    }
    printf("\ninput :[%s]\n",fname);
    if(fname==NULL){shelp();return 0;}
    if(wname==NULL){
      int k = strlen(defaultwname); 
      wname = (char*)malloc(k);
      for(int k2=0;k2<k;k2++){wname[k2]=defaultwname[k2];}
    }
    fsize = std::filesystem::file_size(fname);
    char chr;
    clock_t timesb = clock();
    fp = fopen(fname, "r"); // open file or return null
    if(fp == NULL) {
        printf("[%s] such file doesn't exist!\n", fname);
        return 1;
    }
    printf("\nfile %s 's size = %lld byte Last = ",fname,fsize);
    T= (unsigned int*)malloc(fsize*4);
    for(i=0;i<fsize;i++) {
        chr = fgetc(fp);
        T[i] = chr;
        if(chr<0){T[i]=chr + 256;}
    }
    fclose(fp);
    dcs[0] = 0;dc2s[0] = 0;
    
    for(int rs = 0;rs<loops;rs++){
        P = (int*)malloc(fsize*4); //for consutructing GCIS-index
        P_2 = (int*)malloc(fsize*4); 
        I = (int*)malloc(fsize*4); 
        if(I==NULL){exit(1);}
        pn = purse(T,P,P_2,fsize);
        for(int i = 0; i < pn ; i++){I[i] = i;}//put default number in I[]
        pnsort(T,P,P_2,I,pn);
        if(rs==0){ 
            D = (unsigned int*)malloc(fsize*8); //can save space with realloc() 
            D_2 = (int*)malloc(pn*8); //can save space with realloc() 
            D_3 = (int*)malloc(pn*8); //can save space with realloc() 
        }
        K = (int*)malloc(pn*4); 
        NS = (unsigned int*)malloc(pn*4); 
        round++;
        dc2s[round] = grammarmake(T,P,P_2,I,NS,K,pn); 
        tempC = dc2s[round]+2;
        dc2s[round] += dc2s[round-1];
        dcs[round] = codeA;
        printf("\nC=%d\ndc2s[%d]=%d\nfsize=%lld\npn=%d",tempC,round,dc2s[round],fsize,pn);
        makedic(T,P,P_2,K,NS,D,D_2,D_3,round-1);
        sps = dc2s[round]*4*2+dcs[round]*4+pn*4;
        printf("space : %lld << %lld bytes + O(log m)",sps,spsod);
        free(I);free(T);free(P);free(P_2);free(K); 
            if(rs>OVERTRANSFORM && spsod < sps){loops=rs+1;break;} // 
            if(rs==TRANSTIMES){loops=rs+1;break;}
            if(rs<loops-1){
                fsize = pn;
                T = (unsigned int*)malloc(fsize*4);
                for(i=0;i<fsize;i++){T[i]=NS[i];} //NS to T (NS is new input string)
                free(NS);
                spsod = sps;
            }
    }
    if(DEBUGFLAG==1){for(int yj = 0 ;yj<pn;yj++){printf("<%d>",NS[yj]);}printf("\n");}
    write6(D,D_2,D_3,NS,pn);
    clock_t timee = clock();
    
    const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC ;
    printf("\n[make index :  time %lf[s]\n",time);
    printf("space : %d bytes (%d,%d,%d)",dc2s[loops]*4*2+dcs[loops]*4+pn*4,dc2s[loops],dcs[loops],pn);
    printf("loop = %d",loops);
    printf("\n[u/n] = %d",1+dcs[loops]/dc2s[loops]);
    printf(" / dcs[16]=%d / ",D_2[dc2s[loops]]);
    free(NS); 
    free(D); 
    free(D_2); 
    free(D_3); 
    return 0;
}
