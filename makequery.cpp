#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string> 
#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream
#include <math.h>
#include <time.h>
#include <filesystem>

#define MLONG 2500
#define MREP 10

char qname[] = "inputsample"; //input
char wname[] = "query_automake.txt"; //output
char ptlist[MREP][MLONG];


int qrmload(int c,int jo){
    char chr;FILE *fp;
    long long int qsize = std::filesystem::file_size(qname);
    fp = fopen(qname, "r"); // open file or return null
    if(fp == NULL) {
        printf("%s doesn't exist!\n", qname);
        exit(2);return -1;}
    int rlen = rand()%(qsize-c);
    fseek(fp,rlen,SEEK_SET);
    for(int i=0;i<c;i++) {
        chr = fgetc(fp);
        ptlist[jo][i] = chr;
        if(chr<0){ptlist[jo][i]=chr + 256;}}
    return 0;
}

int main()
{
    srand((unsigned int)time(NULL));
    for(int i = 0;i<MREP;i++){
        int t = qrmload(MLONG,i);
        if(t!=0){i--;} //continue
    }
    FILE *fp;
    fp = fopen(wname, "w");
    for(int jo = 0;jo<MREP;jo++){for(int i = 0;i<MLONG;i++){fprintf(fp,"%c",ptlist[jo][i]);}}
    fclose(fp);
    printf("\npattern has been saved in ",wname);
    return 0;
}
