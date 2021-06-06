#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string> 
#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream
#include <math.h>
#include <time.h>
#include <filesystem>

#define MLONG 1000
#define MREP 100

char *fname,*wname;
char defaultfname[] = "inputsample";
char defaultwname[] = "query_automake.txt";
char ptlist[MREP][MLONG];

int plength = -1; //patternlength
int querysuu = -1; //the number of query


int qrmload(int c,int jo){
    char chr;FILE *fp;
    long long int qsize = std::filesystem::file_size(fname);
    fp = fopen(fname, "r"); // open file or return null
    if(fp == NULL) {
        printf("%s doesn't exist!\n", fname);
        exit(2);return -1;}
    int rlen = rand()%(qsize-c);
    fseek(fp,rlen,SEEK_SET);
    for(int i=0;i<c;i++) {
        chr = fgetc(fp);
        ptlist[jo][i] = chr;
        if(chr<0){ptlist[jo][i]=chr + 256;}}
    return 0;
}

int shelp(){
    printf("--Simple Usage------------------\n");
    printf("\nRead <inputfile> and make <queryfile>. \n");
    printf(": ./genpattern -i <inputfile>\n");
    printf("\nYou can set the name of output by using option -o. \n");
    printf(": ./genpattern -i <inputfile> -o <outputfile>\n");
    printf("\nYou can set the number of query by using option -r. \n");
    printf("10 is used by default.(defined 'MREP' in this code.)\n");
    printf(": ./genpattern -i <inputfile> -o <outputfile> -r <times>\n");
    printf("\nYou can set the length of pattern by using option -l. \n");
    printf("100 is used by default.(defined 'MLONG' in this code.)\n");
    printf(": ./genpattern -i <inputfile> -o <outputfile> -l <length>\n");
    printf("\n");
    printf("\n");
    return 0;
}


int main(int argc, char *argv[])
{
     for(int i = 0;i<argc;i++){
    //printf("[%d : %s]",i,argv[i]);
    if('-'==argv[i][0]){ //符号
        if('i'==argv[i][1]){ //input
            int k = strlen(argv[i+1]);
            printf("<<%d,%d,%d>>",k,sizeof(argv[i+1]),sizeof(argv[i+1][0]));
            fname = (char*)malloc(k);
            for(int k2=0;k2<k;k2++){fname[k2]=argv[i+1][k2];}
        }
        if('o'==argv[i][1]){ //output
            int k = strlen(argv[i+1]);
            wname = (char*)malloc(k);
            for(int k2=0;k2<k;k2++){wname[k2]=argv[i+1][k2];}
        }
        if('r'==argv[i][1]){ //mode
            querysuu=0;
            for(int k = 0;k<strlen(argv[i+1]);k++){
                if(10 > argv[i+1][k]-'0' && argv[i+1][k]-'0'>=0){
                querysuu *=10;
                querysuu += argv[i+1][k] - '0';
                }
                else{break;}
            }
        }
        if('l'==argv[i][1]){ //mode
            plength=0;
            for(int k = 0;k<strlen(argv[i+1]);k++){
                if(10 > argv[i+1][k]-'0' && argv[i+1][k]-'0'>=0){
                plength *=10;
                plength += argv[i+1][k] - '0';
                }
                else{break;}
            }
        }
        if('h'==argv[i][1]){shelp();return 0;} //help
        i++;
        continue;
    }
  }
  if(fname==NULL){shelp();return 0;}
  if(wname==NULL){
    int k = strlen(defaultwname);
    wname = (char*)malloc(k);
    for(int k2=0;k2<k;k2++){wname[k2]=defaultwname[k2];}
  }
  if(plength==-1){plength=MLONG;}
  if(querysuu==-1){querysuu=MREP;}

    srand((unsigned int)time(NULL));
    for(int i = 0;i<MREP;i++){
        int t = qrmload(MLONG,i);
        if(t!=0){i--;} //continue
    }
    FILE *fp;
    fp = fopen(wname, "w");
    for(int jo = 0;jo<querysuu;jo++){for(int i = 0;i<plength;i++){fprintf(fp,"%c",ptlist[jo][i]);}}
    fclose(fp);
    printf("\npattern has been saved in %s\n",wname);
    return 0;
}
