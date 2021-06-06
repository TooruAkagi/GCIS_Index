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
#define MLONG 17
#define MREP 20
#define MSTART 10162
#define CHECKFLAG 1 //if 1, check answers
#define DEBUGFLAG 1 //if 1, show the process
//MMODE = 
// 0 : open query.txt
// 1 : qnameで指定されているファイルの中からランダムに MLONG 文字の文字列を選択し，
//     MREP 回実行する．
// 2 : 指定されているファイルの中のMSTARTからMLONG文字の文字列を選択肢，照合する

char *inname,*qname;
char defaultinname[] = "index";
char defaultqname[] = "inputsample.txt";
int runmode = -1; //mode
int plength = -1; //patternlength
int querysuu = -1; //the number of query
int checkans[1000];
int checkpz[5];
int occart[10]; //-1,-1,X,ofc,-1,-1,-1
int ancl[MREP][5];
int occart2[30][10]; //高さ30に対して(左0,左1,分割番号,ofs,右1,右2,右3,測定内容)
// 0 = *, 1 = B,2 = AB,3 = BC,4 = ABC
int coredata[10]; //Aの判定,Aの開始位置,Aの長さ,Bの判定[1],Bの開始位置,Bの長さ,Cの判定,Cの開始位置,Cの長さ,
double ancd[MREP];
int dcs[18]; //[i] = starting position of floor-i's first purse in the Dictionary
int dc2s[18]; //[i] = starting number of floor-i's first number in the Dictionary
int dbg = 0; //for debuging
int loops =5; //the number of applying GCIS to the input file
int coreans = 0; //コア数を記録
int ans = 0;
int hh = 0;
int smode = 0;
int gate = 0;
int imcokep = 0;
int imcokep2 = 0;
int jswt = 0;
int ocofs = 0;
int *m;
int *NS;
int NSsize;
int transk = 0;
int fround = 0;
int huseiresult = 0;
clock_t timessub;

int core = 0;

void alldr(unsigned int *D,int *D_2,int *D_3,int i,int round){
    //for(int i=0;i<60;i++){
    int rofs = 0;if(round>1){rofs=dc2s[round-1];}
        printf("\n%d > %d[%d(%d)]:",round,i,D_2[i],D_3[i]);
        for(int j=D_2[i];j<D_2[i+1];j++){printf("%d(%d) ",D[j]+rofs,D[j]);printf("(+%d)",D_3[D[j]+rofs]);
            //if(round>0){alldr(D,D_2,D_3,D[j]+rofs,round-1);}
        }
    //}
}

int anscheck(int msize){ //msizeに持っているデータと比較し，答えが合っているかを確認する．
    if(ans==0){printf("[特殊]答え0個");}
    int ckt = 0;
    unsigned char chr;FILE *fp;fp = fopen(qname, "r");
    if(fp == NULL) {
        printf("%s such file doesn't exist!\n", qname);
        return -1;
    }
    for(int j = 0;j<ans;j++){
        if(j>999){break;}
        int c = checkans[j];
        fseek(fp,c,SEEK_SET);   
        if(DEBUGFLAG==1){printf("\nCK%d:rlen[%d]から\n[",j+1,c);}
        for(int i=0;i<msize;i++) { //c文字
            chr = fgetc(fp);
            if(DEBUGFLAG==1){printf("%c",chr);}
            if(m[i]!=chr){
                printf("[%d != %d] in %d",m[i],chr,c);exit(1);
                ckt++;
            }
        }
        if(DEBUGFLAG==1){printf("]\n");}
        if(ckt!=0){
            printf("[不正結果出力]");
            huseiresult++;
        }
    }
    if(huseiresult>0){printf("\n不正数:%d",huseiresult);exit(1);}
    printf("\n結果：出力された位置は全て正しい\n(過剰な出力はない)\n");
    return 0;
}

int pscheck(unsigned int *D,int *D_2,int start,int plen, int round){ 
    //配列mのstartからplen文字の完全一致を探す
    //if(DEBUGFLAG==1){
    //    printf("\npscheck[");
    //    for(int j = 0;j<plen;j++){printf("%d ",m[start+j]);}
    //    printf("]");
    //}
    int rofs = 0;if(round>1){rofs=dc2s[round-1];}
    int pos = 0;int posup = dc2s[round]-1;int posbt = dc2s[round+1]; //round内のみで計算する
    while(1){
        pos = posup + (posbt-posup)/2;
        //printf("\nテスト %d %d %d",pos,posbt,posup);
        int k = 0; // : the length of same prefix. if k == plen == the length of the purse, it's exactly the answer.
        int r = 0; // : for debuging
        for(int i = 0;i<plen;i++){
            if(i == D_2[pos+1]-D_2[pos]){k=-1;break;} //分割が短すぎる = この分割より後に出てくるはず
            if(m[start+i]-rofs==D[D_2[pos]+i]){k=0;}
            if(m[start+i]-rofs>D[D_2[pos]+i]){k=-1;break;} //pos should be larger than now
            if(m[start+i]-rofs<D[D_2[pos]+i]){k=1;break;} //pos should be smaller than now
        }
        if(k==-1){posup=pos;}
        if(k==1){posbt=pos;}
        if(k==0){
                if(D_2[pos+1]-D_2[pos]==plen){break;} //発見
                else{posbt=pos;} //パターンが短すぎる = もっと前に出てくるはず
        }
        //if(DEBUGFLAG==1){printf("\n >>  %d %d %d",pos,posbt,posup);}
        if(posbt-posup<2){pos=-1;break;} //もう探すところがない
    }
    //printf("位置%d ",pos);
    return pos;
}

int rpsck(unsigned int *D,int *D_2,int *Ls,int *B1,int *e2,int round){
    //不一致があれば-1を，round x まで合っていればxを返す．
    if(DEBUGFLAG==1){printf("\nrpsck / [round %d]",round);}
    if(round==-1){return 0;}
    int starti = Ls[round];
    int mlen = B1[round];
    int i = 0;int n2;
    int l = e2[0];
    if(D_2[l+1]-D_2[l]<mlen){
        if(DEBUGFLAG==1){printf("\n長さにより左が確実に不一致");}
        return -1;}
    if(DEBUGFLAG==1){
        printf("open -> [%d] :",l);
        printf("Debug[%d文字,%d / %d/%d]",D_2[l+1]-D_2[l],mlen,starti+mlen-1-i,D_2[l+1]-1-i);
    }
    int rofs=0;if(round>1){rofs=dc2s[round-1];}
    for(i = 0;i<mlen;i++){
        if(D[D_2[l+1]-1-i]+rofs!=m[starti+mlen-1-i]){
            //printf("\n[round %d で 左不一致 %d != %d]",round,D[D_2[l+1]-1-i]+rofs,m[starti+mlen-1-i]);
            return -1;
        }
        if(DEBUGFLAG==1){printf("\nrhit%d(%d)",D[D_2[l+1]-1-i]+rofs,rofs);}
    }
    //printf("[正解]");
    if(round==0){return 0;}
    //printf("<状況チェック%d-%d>",mlen,D_2[l+1]-D_2[l]);
    if(mlen ==D_2[l+1]-D_2[l]){
        //printf("ゲート1");
        if(e2[1]<0){/*printf("<round %d>まで一致",round-1);*/return round-1;}
        n2 = e2[1];
        e2[0] = rofs+D[D_2[n2+1]-1];
        e2[1] = rofs+D[D_2[n2+1]-2];
    } //e2[1]の左側2つ
    if(mlen+1==D_2[l+1]-D_2[l]){
        //printf("ゲート2");
        if(e2[1]<0){e2[1]=-1;}
        else{
            n2 = e2[1];
            e2[1] = rofs+D[D_2[n2+1]-1];} //e2があるなら定義，なければ-1
        e2[0] = rofs+D[D_2[l+1]-1-mlen];
    } //e2[0]の左端とe2[1]の右側2つ
    if(mlen+1<D_2[l+1]-D_2[l]){
        //printf("ゲート3");
        e2[0] = rofs+D[D_2[l+1]-1-mlen];
        e2[1] = rofs+D[D_2[l+1]-2-mlen];
    } //e2[0]のさらに左側2つ
    //printf("next e2[%d,%d(+%d)] =>",e2[0],e2[1],rofs);
    return rpsck(D,D_2,Ls,B1,e2,round-1);
}

int psck(unsigned int *D,int *D_2,int *Rs,int *B2,int *e,int round){
    if(DEBUGFLAG==1){printf("\n[psck / round %d]",round);}
    if(round==-1){return 0;}
    int starti = Rs[round];
    int mlen = B2[round];
    int rofs=0;if(round>1){rofs=dc2s[round-1];}
    int cnt = 0;int a1[3] = {-1,-1,-1};int cnt2 = 0;
    //if(gate==16){printf("?_[%d][%d][%d]",a1[0],a1[1],a1[2]);}
    for(int i=0;i<3;i++){
        int l = e[i];if(l<0){break;}
         if(DEBUGFLAG==1){printf("\nopen -> [%d] :",l);}
        for(int j = D_2[l];j<D_2[l+1];j++){
            if(cnt<mlen){
                if(D[j]+rofs!=m[starti+cnt]){
                     if(DEBUGFLAG==1){printf("round %d で右不一致(%d !=%d)",round,D[j]+rofs,m[starti+cnt]);}
                    return -1;
                }
                if(DEBUGFLAG==1){printf("hit[%d(%d)]",D[j]+rofs,rofs);}
                cnt++;
            }
            else{
                if(round==0){/*printf("[完全正解+]");*/return 0;}
                a1[cnt2] = rofs+D[j];cnt2++;
                if(cnt2==3){
                    e[0] = a1[0];e[1] = a1[1];e[2] = a1[2];
                    //printf("[正解]");
                    //printf("next e2[%d,%d,%d] =>",e[0],e[1],e[2]);
                    return psck(D,D_2,Rs,B2,e,round-1);
                }
            }
        }
    }
    if(cnt<mlen){/*printf("不足・判別不可");*/return 1;}
    //if(gate==16){printf("?_[%d][%d][%d] -> [%d][%d][%d]",a1[0],a1[1],a1[2],e[0],e[1],e[2]);exit(4);}
    e[0] = a1[0];e[1] = a1[1];e[2] = a1[2];
    return psck(D,D_2,Rs,B2,e,round-1);
}

int ptrans(unsigned int *D,int *D_2,int *B1,int *B2,int *B3,int *Ls,int *Rs,int startm,int mrlen,int round){
    int *Rp; //generate transformed pattern and put it into Rp with reversed order.
    int flag = 0; 
    Rp = (int*)malloc(mrlen*4+4);
    int tn = -1;int Rpptr = 0;int len = 0;
    coredata[6] = -1;coredata[8]=0; //Cコアは,最初はないし，長さも0;
    if(DEBUGFLAG==1){printf("\n[変換%d]",round);}
    for(int i = startm+mrlen-1;i>startm;i--){ //check from right(for setting L&S type)
        Rp[Rpptr] = m[i];
        //printf("\n<[%d]%d>",B2[round],m[i]);
        len++;
        if(m[i-1] < m[i]){ //S*
            tn=0;
        }
        if(m[i-1] > m[i]){
            if(tn==0){ //m[i-1] & m[i] == L & S
                if(B2[round] == 0){B2[round] = len;len = 0;flag++;}
                else{
                    Rpptr-=(len-1);
                    Rp[Rpptr] = pscheck(D,D_2,i,len,round); 
                    //check whether corresponding purse exists in round[round] or not
                    if(Rp[Rpptr]<0){return -99;}
                    B3[round]++;
                    len = 0;
                }
            }
            if(tn==-1){
                coredata[6] = 1;
                coredata[7] = m[startm+mrlen-1]; //Cコアの番号は
                coredata[8] = len; //Cコア長さは1
            } //Cコアの長さ
            tn=1;
        }
        Rpptr++;
    }
    len++;
    Rp[Rpptr] = m[startm];
    for(int i = 0;i<=Rpptr;i++){
        m[startm+i] = Rp[Rpptr-i];
        //printf("%d ",m[startm+i]);
    }
    //printf("\nB2 = %d,B3 = %d",B2[round],B3[round]);
    if(B2[round]==0){ //確実に切れる場所[Aコア]がない
        coredata[0] = -1; //Aコアはない
        coredata[2] = 0;
        coredata[3] = 1; //Bコアはある
        coredata[4] = 0; //Bコア開始位置は0
        coredata[5] = mrlen-coredata[8]; //Bコア長さはmrlen - Cコア(あれば)
        if(DEBUGFLAG==1){printf("(in round%d) input string can't be pursed anymore．",round);}
        return -2;
    }
    if(B3[round]==0){ //Aコアがある
        coredata[0] = 1; //Aコアはある
        coredata[1] = 0; //Aコア開始位置は0
        coredata[2] = mrlen-B2[round]; //Aコアの長さは mrlen(Core) - B2[round]
        coredata[3] = 1; //Bコアはある
        coredata[4] = mrlen-B2[round]; //Bコア開始位置はmrlen-B2[eound]
        coredata[5] = B2[round]-coredata[8]; //Bコア長さはmrlen - Cコア(あれば)
        if(DEBUGFLAG==1){printf("(in round%d) input string desn't have core",round);}
        return -1;
    }
    
    Ls[round] = startm;B1[round]=len; //B1:leftside of core / Ls:starting posision of it
    Rs[round] = startm + B1[round] + B3[round]; //Rs corresponds to Ls, B3 to B1.
    free(Rp);
    return B3[round];
}

void mkkmpc(int startm,int mrlen,int *kmpc){
    //2文字一致の時 [0]と[1]が一緒なら，次は1文字進めて1文字一致してる前提で進める
    //3文字一致の時 [0]と[2]が一緒なら，次は2文字進めて1文字一致してる前提で進める
    //            [0-1]と[1-2]が一緒なら，次は1文字進めて2文字一致してる前提で進める．
    for(int i2 = 2;i2<mrlen;i2++){ //i2文字一緒の時
        kmpc[i2] = 0;
        for(int i3 = 1; i3 <i2;i3++){
            int i4 = 0;
            while(i4<i3){
                if(m[startm+i4]==m[startm+(i2-i3)+i4]){
                    i4++;
                    //printf("[m[%d]==m[%d]]",i4,i2-i3+i4);
                }
                else{break;}
            }
            if(i4==i3){kmpc[i2]=i4;}
        }
    }
    
    return;
}

void findintop(int *D_3,int tg,int pofs){ //NS列からtgを探す
    if(DEBUGFLAG==1){printf("\n[%d]を探す(%d)",tg,dc2s[fround-1]);}
    int len = 0;
    for(int y1 = 0;y1<NSsize;y1++){
        if(DEBUGFLAG==1){printf("[%d]答え",NS[y1]);}
        if(NS[y1]==tg-dc2s[fround-1]){
            printf(" %d",len+pofs);
            if(ans<1000 && CHECKFLAG==1){checkans[ans]=len+pofs;}
            ans++;
        }
        len+= D_3[NS[y1]+dc2s[fround-1]];
    }
    //printf("\nend of searching.(%d)[%d]",len,tg);
    return;
}

int pschecks(unsigned int *D,int *D_2,int start,int plen,int round){
    //mのstartmからmlenを，分割の最初から始まるものだけ探す
    //配列mのstartからplen文字一致する最も番号の小さいものを探す
    //printf("\n[");for(int y8=0;y8<plen;y8++){printf(" %d",m[start+y8]);}printf("]を探す");
    int rofs = 0;if(round>1){rofs=dc2s[round-1];}
    int pos = 0;int posup = dc2s[round]-1;int posbt = dc2s[round+1]; //round内のみで計算する
    while(1){
        pos = posup + (posbt-posup)/2;int k = 0;
        for(int i = 0;i<plen;i++){
            if(i == D_2[pos+1]-D_2[pos]){k=-1;break;} //分割が短すぎる = この分割より後に出てくるはず
            if(m[start+i]-rofs==D[D_2[pos]+i]){k=0;}
            if(m[start+i]-rofs>D[D_2[pos]+i]){k=-1;break;} //pos should be larger than now
            if(m[start+i]-rofs<D[D_2[pos]+i]){k=1;break;} //pos should be smaller than now
        }
        if(k==-1){posup=pos;}
        if(k==1){posbt=pos;}
        if(k==0){ //パターンと完全に一致するprefixを持っている
                posbt=pos;
        }
        if(posbt-posup<2){pos=-1;break;} //もう探すところがない
    }
    //printf("\n[最小:%d]",posbt);
    return posbt;
}

int pscheckb(unsigned int *D,int *D_2,int start,int plen,int round){
    //mのstartmからmlenを，分割の最初から始まるものだけ探す
    //配列mのstartからplen文字一致する最も番号の大きいものを探す
    //printf("\n[");for(int y8=0;y8<plen;y8++){printf(" %d",m[start+y8]);}printf("]を探す");
    int rofs = 0;if(round>1){rofs=dc2s[round-1];}
    int pos = 0;int posup = dc2s[round]-1;int posbt = dc2s[round+1]; //round内のみで計算する
    while(1){
        pos = posup + (posbt-posup)/2;int k = 0;
        for(int i = 0;i<plen;i++){
            if(i == D_2[pos+1]-D_2[pos]){k=-1;break;} //分割が短すぎる = この分割より後に出てくるはず
            if(m[start+i]-rofs==D[D_2[pos]+i]){k=0;}
            if(m[start+i]-rofs>D[D_2[pos]+i]){k=-1;break;} //pos should be larger than now
            if(m[start+i]-rofs<D[D_2[pos]+i]){k=1;break;} //pos should be smaller than now
        }
        if(k==-1){posup=pos;}
        if(k==1){posbt=pos;}
        if(k==0){ //パターンと完全に一致するprefixを持っている
                posup=pos;
        }
        if(posbt-posup<2){pos=-1;break;} //もう探すところがない
    }
    //printf("\n[最大:%d]",posup);
    return posup;
}


int rightopennd3(unsigned int *D,int *D_2,int round,int count,int tround,int mode){ //左の[4-6]をcount回開ける
    //printf("\n[%d-右%dオープン(%d)]",mode,count,round);
    //for(int i = 0; i<10; i++){printf("[%d]",occart2[tround][i]);}
    if(count<1){return 0;}
    //printf("\n");
    int a[3] = {-1,-1,-1};int tg = 0;
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    if(occart2[tround][7-mode] > -1){
        for(int k = 0; k<mode; k++){
            //printf("\noccart2[%d] = %d",7-mode+k,occart2[tround][7-mode]);
            for(int i = D_2[occart2[tround][7-mode+k]];i<D_2[1+occart2[tround][7-mode+k]];i++){
                a[tg] = rofs+D[i];tg++;
                //printf(" got(%d [+%d]) ",rofs+D[i],rofs);
                if(tg==mode){
                    for(int j = 0;j<mode;j++){
                        //printf("\noccart2[%d] = %d",7-mode+j,a[j]);
                        occart2[tround][7-mode+j] = a[j];
                    }
                    return rightopennd3(D,D_2,round-1,count-1,tround,mode);
                }
            }
        }
    }
    //printf("完全には埋まらず"); //右が3つ出てこない場合でも，展開は続行する(展開の過程で3つうまることもある)
    for(int j = 0;j<tg;j++){
        //printf("\noccart2[%d] = %d",7-mode+j,a[j]);
        occart2[tround][7-mode+j] = a[j];
    }
    return rightopennd3(D,D_2,round-1,count-1,tround,mode);
}

int coreCcheck(unsigned int *D,int *D_2,int round,int count,int tround){ //一つレベルの高い分割を2つ抑える．[-B][C*][**]と3つが得られる．
    //コアCが出現するかを確認する．出現するなら，余った部分をrightopennd3に回す．
    //coredata[7]がcoredata[8]回出現するのを見届ける
    if(DEBUGFLAG==1){
        printf("\n[Cコア%dを%d回チェック(%d-%d / %d)]",coredata[7],coredata[8],round,tround,count);
        for(int i = 0; i<10; i++){printf("[%d]",occart2[tround][i]);}
    }
    if(count<1){return 0;}
    //printf("\n");
    int a[3] = {-1,-1,-1};int tg = 0;
    int rofs=0;int Clef = coredata[8];
    if(round>1){rofs=dc2s[round-1];}
    for(int u = 0;u<3;u++){
        if(occart2[tround][4+u] > -1){
            if(DEBUGFLAG==1){
                 printf("\n展開:occart2[%d] = %d",4+u,occart2[tround][4+u]);
            }
            for(int i = D_2[occart2[tround][4+u]];i<D_2[1+occart2[tround][4+u]];i++){
                if(Clef>0 && count==1){
                    if(rofs+D[i]==coredata[7]){
                        Clef--;
                        if(DEBUGFLAG==1){printf("\n[hit %d(%d) / あと%d]",rofs+D[i],D[i],Clef);}
                        continue;}else{return -1;}
                }
                //if(fround==0){return 0;} //Clefがなくなって，froundが0ならもう右はあっているのでは
                a[tg] = rofs+D[i];tg++;
                 if(DEBUGFLAG==1){printf(" got(%d [+%d]) ",rofs+D[i],rofs);}
                if(tg==3){
                    for(int j = 0;j<3;j++){
                         if(DEBUGFLAG==1){printf("\noccart2[%d] = %d",4+j,a[j]);}
                        occart2[tround][4+j] = a[j];
                    }
                    if(Clef==0){occart2[tround][7] += 2;return rightopennd3(D,D_2,round-1,count-1,tround,3);} //Cコア把握
                    return coreCcheck(D,D_2,round-1,count-1,tround);
                }
            }
        }
    }
    if(count==1){//Cの存在を確かめた上で右分割が足りない場合
        if(tg==0 || Clef>0){printf("おかしい!(%d,%d)",tg,Clef);exit(7);}
        if(tg>=1){occart2[tround][4] = a[0];}
        if(tg>=2){occart2[tround][5] = a[1];}
        occart2[tround][7] += 2;
        return rightopennd3(D,D_2,round-1,count-1,tround,3);
    }
    else{ //countがもっと高い
        if(tg>=1){occart2[tround][4] = a[0];}
        if(tg>=2){occart2[tround][5] = a[1];}
        return coreCcheck(D,D_2,round-1,count-1,tround);
    }
}

void leftopen(unsigned int *D,int *D_2,int *D_3,int round,int count,int tround){ //左の[0]をcount回開ける
    if(DEBUGFLAG==1){
         printf("\n[1-左%dオープン(%d)]",count,round);
         if(occart2[tround][0] > -1){alldr(D,D_2,D_3,occart2[tround][0],tround-1);}
    }
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    if(occart2[tround][0] > -1){
       occart2[tround][0] = rofs+D[D_2[1+occart2[tround][0]]-1];
       if(DEBUGFLAG==1){printf(" >> %d ",occart2[tround][0]);}
       if(count>1){leftopen(D,D_2,D_3,round-1,count-1,tround);}
    }
    return;
}

int leftopennd(unsigned int *D,int *D_2,int *D_3,int round,int count,int tround){ //左の[0][1]をcount回開ける
     if(DEBUGFLAG==1){
         printf("\n[2-左%dオープン(%d)]",count,round);
         if(occart2[tround][1] > -1){alldr(D,D_2,D_3,occart2[tround][1],tround-1);}
         if(occart2[tround][0] > -1){alldr(D,D_2,D_3,occart2[tround][0],tround-1);}
    }
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    if(occart2[tround][1] > -1){ //左辺文字Aが指定されていれば
         if(DEBUGFLAG==1){printf("open [%d,%d--%d] >>",occart2[tround][1],dc2s[round],rofs);}
        int a1 = rofs+D[D_2[1+occart2[tround][1]] - 1];
        if(DEBUGFLAG==1){printf("\na1 = %d(%d)",a1,count);}
        if(count>1){occart2[tround][1] = a1;return leftopennd(D,D_2,D_3,round-1,count-1,tround);}
        int a2;
        if(D_2[occart2[tround][1]]<2){printf("a2を消す");a2 = -1;}
        a2 = rofs+D[D_2[1+occart2[tround][1]] - 2];
        occart2[tround][1] = a1;
        occart2[tround][0] = a2;
        if(DEBUGFLAG==1){printf("\n更新 = %d & %d",a1,a2);}
    }
    return 0;
}


int coreAcheck(unsigned int *D,int *D_2,int round,int count,int tround,int startm){ //左の[0][1]をcount回開ける
    if(DEBUGFLAG==1){
        printf("\n[2-左%dオープン(%d)]count==1ならAコアを%d文字調べる\n",count,round,coredata[2]);
        for(int i = 0; i<10; i++){printf("[%d]",occart2[tround][i]);}
    }
    int rofs=0;int e[2] = {-1,-1};int etg = 0;
    if(round>1){rofs=dc2s[round-1];}
    int l = occart2[tround][1];
    int Alen = coredata[2];
    //printf("Aチェック(%d / %d)",occart2[tround][0],occart2[tround][1]);
    //count>=2 の時，必ず2つ準びできるので，for以降に行くことはない.count==1の時，Aを判定して終わる場合は
    //-1が一部残ってforを抜けるはずなので,
            for(int j = 0;j<2;j++){
                l = occart2[tround][1-j];
                if(l<0){
                    if(DEBUGFLAG==1){printf("-1を回避(j==%d)",j);}
                    break;}
                 if(DEBUGFLAG==1){printf("open[%d(+%d)]",l,rofs);}
                for(int i = D_2[l+1]-1;i>=D_2[l];i--){
                    if(count==1 && Alen>0){ //count1ならAコアの判定をする
                         if(DEBUGFLAG==1){printf("\n[%d-%d]",rofs+D[i],m[startm-1+Alen]);}
                        if(D[i]+rofs == m[startm-1+Alen]){
                            Alen--;continue;
                        }
                        else{return -1;}
                    }
                    else{ //count1以外か，コアを判定し終わったら
                        e[etg] = rofs+D[i];etg++;
                        if(etg==2){ //コアの横の文字が2つ集まったら
                            occart2[tround][1] = e[0]; //1こめ
                            occart2[tround][0] = e[1]; //1こめ
                            if(count>1){return coreAcheck(D,D_2,round-1,count-1,tround,startm);}
                            return 0; //Aを見終わったならそのままreturnしていいはず
                        }
                    }
                }
            }
    //coreAcheckは最低1文字ある状態で呼ぶ．
    if(count>1){
        if(etg==1){occart2[tround][1] = e[0];}
        return coreAcheck(D,D_2,round-1,count-1,tround,startm);} //無理やりチェック
    if(count != 1){printf("おかしい(count = %d)",count);exit(1);}
    if(Alen>0){return -1;}
    //printf("未定義[etg = %d]",etg);
    if(etg==0){printf("Aでぴったり切れている");occart2[tround][1] = -1;occart2[tround][0] = -1;return 0;}
    if(etg==2){printf("追加 > %d.%d",e[0],e[1]);occart2[tround][1] = e[0];occart2[tround][0] = e[1];return 0;}
    if(etg==1){printf("追加 > %d",e[0]);occart2[tround][1] = e[0];return 0;}
    return 0;
}

void findinround1(unsigned int *D,int *D_2,int *D_3,int tg,int round,int pofs){
    if(round==fround){return findintop(D_3,tg,pofs);}
    //あるラウンドから文字[tg]を探す
    //printf("%d(+%d)",tg,pofs); //引き継いだズレ値を記録
    int e2[2];
    int rofs = 0;
    if(round>1){rofs=dc2s[round-1];}
    for(int y8=dc2s[round];y8<dc2s[round+1];y8++){
        int ofs = 0;
        //printf("\n[%d(%d)]",y8,D_2[y8+1]-D_2[y8]);
        for(int y9=D_2[y8];y9<D_2[y8+1];y9++){
            //printf(" %d",rofs+D[y9]);
            if(rofs+D[y9]==tg){
                //if(round>15){printf("\n発見(%d,%d[+%d])",tg,ofs+pofs,ofs);}
                findinround1(D,D_2,D_3,y8,round+1,pofs+ofs);
            }
            if(round>0){ofs+=D_3[rofs+D[y9]];}else{ofs++;}
        }
    }
    return;
}

int findinNS(unsigned int *D,int *D_2,int *D_3,int startm,int mrlen,int round,int *kmpc,int *B1,int *B2,int *B3,int *Ls,int *Rs){
    if(DEBUGFLAG==1){
        printf("\nNSから直接探す[%d](%d)\nkmpc=",mrlen,ocofs);
        for(int i2 = 0;i2<mrlen;i2++){printf("%d ",kmpc[i2]);}
        printf("\n");
        for(int i2 = 0;i2<mrlen;i2++){printf("%d ",m[startm+i2]);}
    }
    int tg,kpofs;
    tg = 0;kpofs=0;
        for(int y8=0;y8<NSsize;y8++){
            //if(y8>=300 && y8<500){printf("\n<%d - %d>",y8,kpofs);}
            for(int y9=0;y9<mrlen;y9++){
                if(NS[y9+y8]+dc2s[fround-1]==m[startm+tg]){
                    tg++;
                    if(DEBUGFLAG==1){printf("\n%d:%d(%d - %d)[+%d]",kpofs,NS[y9+y8]+dc2s[fround-1],y9+y8,tg,D_3[NS[y9+y8]+dc2s[fround-1]]);}
                    if(tg==mrlen){
                        if(DEBUGFLAG==1){printf("[%d]パターン発見[%d]",y8,kpofs-ocofs);}
                        int e2[2] = {NS[y8-1]+dc2s[fround-1],NS[y8-2]+dc2s[fround-1]};
                        int tk = rpsck(D,D_2,Ls,B1,e2,fround-1);
                        if(DEBUGFLAG==1){if(tk==-1){printf("\nこれはコアではない2");}}
                        if(tk==0){
                            int e[3] = {NS[y8+mrlen]+dc2s[fround-1],NS[y8+mrlen+1]+dc2s[fround-1],NS[y8+mrlen+2]+dc2s[fround-1]};
                            tk = psck(D,D_2,Rs,B2,e,fround-1);
                            if(DEBUGFLAG==1){if(tk==-1){printf("\nこれはコアではない");}}
                            if(tk==0){ //左右も完璧に合ってた
                                if(DEBUGFLAG==1){printf("答え");}
                                printf(" %d",kpofs-ocofs);
                                if(ans<1000 && CHECKFLAG==1){checkans[ans]=kpofs-ocofs;}
                                ans++;
                            }
                        }
                        kpofs+=D_3[NS[y8]+dc2s[fround-1]];
                        tg=0;break; //y9 break
                    }
                }
                else{ //不一致
                    if(tg!=0){
                        if(DEBUGFLAG==1){printf("\n%d[%d > %d]",y8,tg,kmpc[tg]);}
                        for(int u=0;u<tg-kmpc[tg]+1;u++){
                            if(DEBUGFLAG==1){printf("(+%d)",D_3[NS[u+y8]+dc2s[fround-1]]);}
                            kpofs+=D_3[NS[u+y8]+dc2s[fround-1]];
                        }
                        y8+=tg - kmpc[tg];
                        tg = kmpc[tg];
                    }
                    else{kpofs+=D_3[NS[y9+y8]+dc2s[fround-1]];}
                    break;
                }
            }
        }
    return 0;
}

int findupsintop(unsigned int *D,int *D_2,int *D_3,int startm,int mrlen,int *B1,int *B2,int *B3,int *Ls,int *Rs,int count){ //一番上の階層まで来てもまだ見つからない例があるなら全力で対処する
    if(DEBUGFLAG==1){
        printf("\nfround(%d)まで到達[%d] / 探すもの[%d]\n",fround,count,occart2[fround-1][2]);
        alldr(D,D_2,D_3,occart2[fround-1][2],fround-1);
        for(int i = 0; i<10; i++){printf("[%d]",occart2[fround-1][i]);}
    }
    int len = 0;
    int tg = occart2[fround-1][2];
    int ofs = occart2[fround-1][3];
    for(int y1 = 0;y1<NSsize;y1++){
        if(len < 82134000 && len>82124000){
            if(DEBUGFLAG==1){
                printf("\n[%d]",y1);
                printf("%d(+%d [%d])",len,D_3[NS[y1]+dc2s[fround-1]],NS[y1]+dc2s[fround-1]);
             }
        }
        if(NS[y1]+dc2s[fround-1]==tg){
            if(DEBUGFLAG==1){printf("\nコア(単独)を発見%d(+%d)",len+ofs,ofs);}
            //coreans++;
            for(int i = 0; i<10; i++){occart2[fround][i] = occart2[fround-1][i];} //コアを見つけたら下階層からゲット
            if(DEBUGFLAG==1){
                printf("\n最初 → ");
                for(int i = 0; i<10; i++){printf("[%d]",occart2[fround][i]);}
            }
            if(y1>0 && occart2[fround][0]<0){ //左が埋まっていなくて，かつ左があるなら
                if(occart2[fround][1] == -1){ //2つとも埋まっていないなら
                    occart2[fround][1] = NS[y1-1]+dc2s[fround-1];
                    if(y1>1){occart2[fround][0] = NS[y1-2]+dc2s[fround-1];}
                    if(occart2[fround][7]%2==0){
                        if(coreAcheck(D,D_2,fround-1,count,fround,startm)==-1){
                            if(DEBUGFLAG==1){printf("Aコアミスマッチ");}
                            len+= D_3[NS[y1]+dc2s[fround-1]];
                            continue;
                        }
                        else{occart2[fround][7]++;}//1文字は入れたんだから0が帰ってきていればAはもう探し終わっている
                    }
                    else{leftopennd(D,D_2,D_3,fround-1,count,fround);}
                }
                else{ //1つ埋まっていないなら
                    occart2[fround][0] = NS[y1-1]+dc2s[fround-1];
                    leftopen(D,D_2,D_3,fround-1,count,fround);
                }
            }
            if(y1<NSsize && occart2[fround][6]<0){ //右が埋まっていなくて，かつ右があるなら
                if(occart2[fround][5]==-1){ //2文字埋まってないなら
                        if(occart2[fround][4]==-1){ //3文字埋まってないなら
                            occart2[fround][4] = NS[y1+1]+dc2s[fround-1];
                            if(y1+2<NSsize){occart2[fround][5] = NS[y1+2]+dc2s[fround-1];}
                            if(y1+3<NSsize){occart2[fround][6] = NS[y1+3]+dc2s[fround-1];}
                            if(occart2[fround][7]/2<1){
                                if(coreCcheck(D,D_2,fround-1,count,fround)==-1){
                                    if(DEBUGFLAG==1){printf("\nCコアミスマッチ[top内]");}
                                    len+= D_3[NS[y1]+dc2s[fround-1]];
                                    continue;
                                }
                            } //Cコアがあるなら調べる
                            else{rightopennd3(D,D_2,fround-1,count,fround,3);} //3文字count回解体
                        }
                        else{
                            occart2[fround][5] = NS[y1+1]+dc2s[fround-1];
                            if(y1+2<NSsize){occart2[fround][6] = NS[y1+2]+dc2s[fround-1];}
                            rightopennd3(D,D_2,fround-1,count,fround,2);
                        }
                    }
                    else{ //1つだけ
                        occart2[fround][6] = NS[y1+1]+dc2s[fround-1];
                        rightopennd3(D,D_2,fround-1,count,fround,1);
                    }
            }
            if(DEBUGFLAG==1){
                printf("\nこうなる → ");
                for(int i = 0; i<10; i++){printf("[%d]",occart2[fround][i]);}
            }
            //occart2[round][6]> -1 && occart2[round][0] > -1 ?
            //printf("\n[早速チェック]");
            int e2[2] = {occart2[fround][1],occart2[fround][0]};
            int tk = rpsck(D,D_2,Ls,B1,e2,fround-count-1);
            //if(tk==-1){printf("\nこれはコアではない");}
            if(tk==0){
                int e[3] = {occart2[fround][4],occart2[fround][5],occart2[fround][6]};
                tk = psck(D,D_2,Rs,B2,e,fround-count-1);
                    //if(tk==-1){printf("\nこれはコアではない");}
                if(tk==0){ //左右も完璧に合ってた
                    if(DEBUGFLAG==1){printf("答え");}
                    printf(" %d",len+occart2[fround][3]);
                    if(len+occart2[fround][3]==94818246){printf("ここで強制終了\n");exit(0);}
                    if(ans<1000 && CHECKFLAG==1){checkans[ans]=len+occart2[fround][3];}
                    ans++;
                }
            }
            //exit(5);
        }
        len+= D_3[NS[y1]+dc2s[fround-1]];
        //if(len<100){printf("\n[.%d < %d + (%d)%d]",len,D_3[NS[y1]+dc2s[fround-1]],y1,NS[y1]+dc2s[fround-1]);
        //    alldr(D,D_2,D_3,NS[y1]+dc2s[fround-1],fround-1);
        //}
    }
    return 0;
}

int findups(unsigned int *D,int *D_2,int *D_3,int startm,int mrlen,int round,int *B1,int *B2,int *B3,int *Ls,int *Rs,int count){
    if(DEBUGFLAG==1){
        printf("\n[%d]を探す(round %d / %d)",occart2[round-1][2],round,fround);
        alldr(D,D_2,D_3,occart2[round-1][2],round-1);
        for(int i = 0; i<10; i++){printf("[%d]",occart2[round-1][i]);}
    }
    //1つroundが下の場所でBコアが発見できたものをXとする．
    //Xを探し，その左右のデータを追加するのがこの仕事．
    //count回分だけ下の文字を2-3個揃えるのが仕事．
    if(round==fround){return findupsintop(D,D_2,D_3,startm,mrlen,B1,B2,B3,Ls,Rs,count);}
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    for(int y8=dc2s[round];y8<dc2s[round+1];y8++){
        //printf("\n[%d(%d)]",y8,D_2[y8+1]-D_2[y8]);
        //int tg = 0;
        int ofs = 0;
        for(int y9=D_2[y8];y9<D_2[y8+1];y9++){
            //printf(" %d",rofs+D[y9]);
            if(rofs+D[y9]==occart2[round-1][2]){ //該当する1文字を見つけたら
                gate++;
                if(DEBUGFLAG==1){
                     printf("\n[%dの%d文字目に発見(+%d)]",y8,y9-D_2[y8],ofs);
                }
                //coreans++;
                //左がないなら左を探す(このroundで1つ見つかれば良い)
                for(int i = 0; i<10; i++){occart2[round][i] = occart2[round-1][i];} //コアを見つけたら下階層からゲット
                if(y9==D_2[y8] || occart2[round-1][0]>-1){ //先頭にないか，左[0]が埋まっているなら左は追加できない
                }
                else{ 
                    if(occart2[round][1] == -1){ //2つとも埋まっていないなら
                        occart2[round][1] = rofs+D[y9-1];
                        if(D_2[y8]+1<y9){occart2[round][0] = rofs+D[y9-2];}
                        //printf("\n[%d]記入数 %d %d",y9-D_2[y8],occart2[round][0],occart2[round][1]);
                        if(occart2[round][7]%2==0){
                            if(coreAcheck(D,D_2,round-1,count,round,startm)==-1){
                                 if(DEBUGFLAG==1){printf("Aミスマッチ");}
                                continue;
                            }
                            else{occart2[round][7]++;} //1文字少なくとも入れたはずだし0が帰ってきていれば必ずAコアは調べ終わっている
                        }
                        else{leftopennd(D,D_2,D_3,round-1,count,round);}
                    }
                    else{ //1つ埋まっていないなら
                        occart2[round][0] = rofs+D[y9-1];
                        leftopen(D,D_2,D_3,round-1,count,round);
                    }
                }
                //printf("\n演算[%d = %d][%d]",y9+1,D_2[y8],occart2[round-1][6]);
                if(y9+1<D_2[y8+1] && occart2[round-1][6]==-1){ //右があり，右が埋まっていないなら
                    if(occart2[round][5]==-1){ //2文字埋まってないなら
                        if(occart2[round][4]==-1){ //3文字埋まってないなら
                            occart2[round][4] = rofs+D[y9+1];
                            if(y9+2<D_2[y8+1]){occart2[round][5] = rofs+D[y9+2];}
                            if(y9+3<D_2[y8+1]){occart2[round][6] = rofs+D[y9+3];}
                            if(occart2[round][7]/2<1){
                                if(coreCcheck(D,D_2,round-1,count,round)==-1){
                                     if(DEBUGFLAG==1){printf("Cコアミスマッチ");}
                                    continue;}
                            } //Cのチェックもしていないなら
                            else{rightopennd3(D,D_2,round-1,count,round,3);}
                        }
                        else{
                            occart2[round][5] = rofs+D[y9+1];
                            if(y9+2<D_2[y8+1]){occart2[round][6] = rofs+D[y9+2];}
                            rightopennd3(D,D_2,round-1,count,round,2);
                        }
                    }
                    else{ //1つだけ
                        occart2[round][6] = rofs+D[y9+1];
                        rightopennd3(D,D_2,round-1,count,round,1);
                    }
                    //exit(7);
                }
                //printf("\n結果:");
                //for(int i = 0; i<10; i++){printf("[%d]",occart2[round][i]);}
                if(occart2[round][6]> -1 && occart2[round][0] > -1){
                    //printf("\n[早速チェック]");
                    int e2[2] = {occart2[round][1],occart2[round][0]};
                    int tk = rpsck(D,D_2,Ls,B1,e2,round-count-1);
                    //if(tk==-1){printf("\nこれはコアではない");}
                    if(tk==0){
                        int e[3] = {occart2[round][4],occart2[round][5],occart2[round][6]};
                        tk = psck(D,D_2,Rs,B2,e,round-count-1);
                        //if(tk==-1){printf("\nこれはコアではない");}
                        if(tk==0){ //左右も完璧に合ってた
                            //printf("\n正しい出現を%dの+%dに発見!\n",y8,occart2[round][3]+ofs);
                            findinround1(D,D_2,D_3,y8,round+1,occart2[round][3]+ofs);
                            //exit(1);
                        }
                    }
                }
                else{ //もっと上の階層に再帰するしかない
                    int e[3] = {occart2[round][4],occart2[round][5],occart2[round][6]};
                    int tk = psck(D,D_2,Rs,B2,e,round-count-1);
                    if(tk!=-1){
                        occart2[round][2] = y8;occart2[round][3] += ofs;
                        findups(D,D_2,D_3,startm,mrlen,round+1,B1,B2,B3,Ls,Rs,count+1);
                    }
                    //else{printf("\n[排除できる例(%d,%d)]",y8,y9-D_2[y8]);}
                }
            }
            if(round>0){ofs+=D_3[rofs+D[y9]];}else{ofs++;}
        }
    }
    //printf("\n--[%d]探し 終了--\n",occart2[round-1][2]);
    return 1;
}

int findAB(unsigned int *D,int *D_2,int *D_3,int startm,int mrlen,int round,int *kmpc,int *B1,int *B2,int *B3,int *Ls,int *Rs){
    //printf("\nAコアがある場合の[");
    //for(int i2 = 0;i2<coredata[5];i2++){printf("%d ",m[startm+coredata[4]+i2]);}
    //printf("]Bを探す(round %d)\nkmpc = ",round);
    //for(int i2 = 0;i2<mrlen;i2++){printf("%d ",kmpc[i2]);}
    int len = 0;
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    //int bs = 0; //コアが完全一致した時，どこまで戻るか
    //int fis = -1;
    int i = 0;
    int tg = 0;
    int ofs = 0;
    //int kpofs = 0;
    //int e2[2];
    for(int y8=dc2s[round];y8<dc2s[round+1];y8++){
        //printf("\n[%d(%d)]",y8,D_2[y8+1]-D_2[y8]);
        ofs=0;tg = 0;
        for(int y9=D_2[y8];y9<D_2[y8+1];y9++){
            //printf(" %d",rofs+D[y9]);
            if(rofs+D[y9]==m[startm+coredata[4]+tg]){ //coredata[4]だけずらす
            tg++;
            }else{tg=0;break;}//先頭からしか見ないので不一致が会ったら即forを脱出する
            if(coredata[6]==-1 && tg==coredata[5]){break;} //Cコアがないなら，B判定でbreak
            if(coredata[6]==1 && tg==coredata[5]+coredata[8]){break;} //Cコアがあり，5+8ならbreak
            
        }
        //printf(" / 一致数%d(出現)[%d+%d]",tg,coredata[5],coredata[8]);
        if(tg==coredata[5]+coredata[8]){ //Cコアがある場合は最後にBCコア，そうでない場合はBコアが出現
            //printf("\n(BC出現[%d / %d])",tg,D_2[y8+1]-D_2[y8]);
            coreans++;
            occart2[round][0] = -1;
            occart2[round][1] = -1;
            occart2[round][2] = y8;
            occart2[round][3] = -ocofs; //先頭から出現するのでずれなし
            if(D_2[y8+1]-D_2[y8]-tg>0){occart2[round][4] = rofs+D[D_2[y8]+tg];}else{occart2[round][4] = -1;}
            if(D_2[y8+1]-D_2[y8]-tg>1){occart2[round][5] = rofs+D[D_2[y8]+tg+1];}else{occart2[round][5] = -1;}
            if(D_2[y8+1]-D_2[y8]-tg>2){occart2[round][6] = rofs+D[D_2[y8]+tg+2];}else{occart2[round][6] = -1;}
            if(coredata[6]==1){
                occart2[round][7] = 2;
                //printf("\n(BC出現[%d / %d])",tg,D_2[y8+1]-D_2[y8]);
            } //Cコアがあるなら，BC発見にする
            else{
                //printf("\n(B出現[%d])[Cなし]",tg);
                occart2[round][7] = 2; //B発見(Aはまだ)
            }
            //for(int i = 0;i<10;i++){printf("[%d]",occart2[round][i]);}
            //exit(2);
            findups(D,D_2,D_3,startm,mrlen,round+1,B1,B2,B3,Ls,Rs,1); //その不完全なコアの出現を探す
        }
        else if(tg==coredata[5]){ //最後にBコアだけ出現
           // printf("\n(B出現[%d])",tg);
            coreans++;
            occart2[round][0] = -1;
            occart2[round][1] = -1;
            occart2[round][2] = y8;
            occart2[round][3] = -ocofs; //先頭から出現するのでずれなし + コアと出現位置のズレ + Aコアのずれを補正
            occart2[round][4] = -1;
            occart2[round][5] = -1;
            occart2[round][6] = -1;
            occart2[round][7] = 0; //Bのみ発見(Aはまだ)
            //for(int i = 0;i<10;i++){printf("[%d]",occart2[round][i]);}
           findups(D,D_2,D_3,startm,mrlen,round+1,B1,B2,B3,Ls,Rs,1); //その不完全なコアの出現を探す
        }
    }
    //printf("\nend of searching.(%d)[%d]",len,i);
    return 0;
}

int findB(unsigned int *D,int *D_2,int *D_3,int startm,int mrlen,int round,int *kmpc,int *B1,int *B2,int *B3,int *Ls,int *Rs){ //列からtgを探す
    //Bコアを探す
    if(DEBUGFLAG==1){
        printf("\nAコアがない場合の[");
        for(int i2 = 0;i2<coredata[5];i2++){printf("%d ",m[startm+coredata[4]+i2]);}
        printf("](%d 文字)Bを探す(round %d)\nkmpc = ",mrlen,round);
        for(int i2 = 0;i2<mrlen;i2++){printf("%d ",kmpc[i2]);}
    }
    int len = 0;
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    int bsp[3] = {0,0,0}; //コアが完全一致した時，どこまで戻るか
    int i = 0;
    int tg = 0;
    int ofs = 0;
    int kpofs = 0;
    int y9;
    for(int y8=dc2s[round];y8<dc2s[round+1];y8++){
        //if(DEBUGFLAG==1){printf("\n[%d(%d)]",y8,D_2[y8+1]-D_2[y8]);}
        ofs=0;tg = 0;
        for(y9=D_2[y8];y9<D_2[y8+1];y9++){
            if(rofs+D[y9]==m[startm+tg]){
                tg++;if(tg==1){kpofs = ofs;} //1文字一致を確認したらオフセットくらいは覚えておく
                if(tg==coredata[5]){
                     if(DEBUGFLAG==1){printf("(Bコア発見[%d文字目(%d)] / %dの(+%d))",y9-D_2[y8],tg,y8,ofs);}
                }
                if(tg==mrlen){ //CコアがなければB = Cの発見と同じ.コアはどうせ1つしかないのでそのままbreak
                    //printf("\n(コア発見[%d文字目(%d)] / %d(+%d))",y9-D_2[y8],tg,y8,ofs);
                    bsp[0] = y9; //コアの最後尾の位置
                    bsp[1] = y9-D_2[y8]+1 - mrlen; //手前に残ってる文字数
                    bsp[2] = D_2[y8+1]-y9-1; //この先に残ってる文字数
                    break;
                }
            }else{ //不一致
                if(tg!=0){
                    if(y8==279){printf("[kmpc%d->%d]",tg,kmpc[tg]);}
                    if(round==0){kpofs+= (tg - kmpc[tg]);}
                    else{kpofs+=D_3[m[startm]]*(tg - kmpc[tg]);}
                    tg = kmpc[tg];y9--;continue;}
                tg=0;
            }
            //printf(" %d",rofs+D[y9]);
            if(round>0){ofs+=D_3[rofs+D[y9]];}else{ofs++;}
        }
        //if(fis>0){printf("(左に右出現[%d文字])",fis);} //先頭から出現
        if(tg==mrlen){ //完全一致して抜けてきたやつ(A・Cコアが確実にない)
                coreans++;
                if(bsp[1]>0){occart2[round][1] = rofs+D[y9-mrlen];}else{occart2[round][1] = -1;}
                if(bsp[1]>1){occart2[round][0] = rofs+D[y9-mrlen-1];}else{occart2[round][0] = -1;}
                occart2[round][2] = y8;
                occart2[round][3] = kpofs-ocofs; //コアとのずれを最初に払っておく
                if(bsp[2]>0){occart2[round][4] = rofs+D[y9+1];}else{occart2[round][4] = -1;}
                if(bsp[2]>1){occart2[round][5] = rofs+D[y9+2];}else{occart2[round][5] = -1;}
                if(bsp[2]>2){occart2[round][6] = rofs+D[y9+3];}else{occart2[round][6] = -1;}
                occart2[round][7] = 3; //ABC全部発見 
                //transk==0なら，findinroundに行っても良いはず
                if(DEBUGFLAG==1){
                    printf("\nチェック：");
                    for(int i = 0;i<10;i++){printf("[%d]",occart2[round][i]);}
                }
                findups(D,D_2,D_3,startm,mrlen,round+1,B1,B2,B3,Ls,Rs,1); //その不完全なコアの出現を探す
        }
        else{
            if(tg==coredata[5]){ //最後にBコアだけ出現
                //printf("\n(右にBコア出現[%d文字] +%d)\n",tg,ofs);
                coreans++;
                if(D_2[y8+1]-D_2[y8]>tg+1){occart2[round][0] = rofs+D[D_2[y8+1]-2-tg];}else{occart2[round][0] = -1;}
                if(D_2[y8+1]-D_2[y8]>tg){occart2[round][1] = rofs+D[D_2[y8+1]-1-tg];}else{occart2[round][1] = -1;}
                occart2[round][2] = y8;
                occart2[round][3] = kpofs-ocofs; //コアとのずれを最初に払っておく
                occart2[round][4] = -1;
                occart2[round][5] = -1;
                occart2[round][6] = -1;
                occart2[round][7] = 1; //Bのみ発見
                if(DEBUGFLAG==1){
                    printf("Bコアのみが最後に出現するタイプ\n");
                    for(int i = 0;i<10;i++){printf("[%d]",occart2[round][i]);}
                }
                findups(D,D_2,D_3,startm,mrlen,round+1,B1,B2,B3,Ls,Rs,1); //その不完全なコアの出現を探す
            }
        }
    }
    //printf("\nend of searching.(%d)[%d]",len,i);
    return 0;
}

int findC(unsigned int *D,int *D_2,int *D_3,int startm,int mrlen,int round,int *kmpc,int *B1,int *B2,int *B3,int *Ls,int *Rs){ //コアがある1種類の文字だけで構成されている時
    //printf("\nコアは1種類の文字%dが%d個続いている",coredata[7],mrlen);
    //int len = 0;
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    int bsp[3] = {0,0,0}; //コアが完全一致した時，どこまで戻るか
    int i = 0;
    int tg = 0;
    int ofs = 0;
    int kpofs = 0;
    //int e2[2];
    int y9;
    for(int y8=dc2s[round];y8<dc2s[round+1];y8++){
        //printf("\n[%d(%d)]",y8,D_2[y8+1]-D_2[y8]);
        ofs=0;tg = 0;
        for(y9=D_2[y8];y9<D_2[y8+1];y9++){
            if(rofs+D[y9]==coredata[7]){
                tg++;if(tg==1){kpofs = ofs;} //1文字一致を確認したらオフセットくらいは覚えておく
                if(tg==mrlen){ //CコアがなければB = Cの発見と同じ.コアはどうせ1つしかないのでそのままbreak
                    //printf("\n(コア発見[%d文字目(%d)] / %d(+%d))",y9-D_2[y8],tg,y8,ofs);
                    bsp[0] = y9; //コアの最後尾の位置
                    bsp[1] = y9-D_2[y8]+1 - mrlen; //手前に残ってる文字数
                    bsp[2] = D_2[y8+1]-y9-1; //この先に残ってる文字数
                    if(bsp[1]>0){occart2[round][0] = rofs+D[y9-mrlen];}else{occart2[round][0] = -1;}
                    if(bsp[1]>1){occart2[round][1] = rofs+D[y9-mrlen-1];}else{occart2[round][1] = -1;}
                    occart2[round][2] = y8;
                    occart2[round][3] = kpofs-ocofs; //コアとのずれを最初に払っておく
                    if(bsp[2]>0){occart2[round][4] = rofs+D[y9+1];}else{occart2[round][4] = -1;}
                    if(bsp[2]>1){occart2[round][5] = rofs+D[y9+2];}else{occart2[round][5] = -1;}
                    if(bsp[2]>2){occart2[round][6] = rofs+D[y9+3];}else{occart2[round][6] = -1;}
                    occart2[round][7] = 3; //ABC全部発見 
                    //printf("\nチェック：");
                    //for(int i = 0;i<10;i++){printf("[%d]",occart2[round][i]);}
                    findups(D,D_2,D_3,startm,mrlen,round+1,B1,B2,B3,Ls,Rs,1); //その不完全なコアの出現を探す
                    break;
                }
            }else{ //不一致
                tg=0;
            }
            //printf(" %d",rofs+D[y9]);
            if(round>0){ofs+=D_3[rofs+D[y9]];}else{ofs++;}
        }
        //if(fis>0){printf("(左に右出現[%d文字])",fis);} //先頭から出現
    }
    //printf("\nend of searching.(%d)[%d]",len,i);
    return 0;
}

int patternmatching2(unsigned int *D,int *D_2,int *D_3,int absm){
    //alldr(D,D_2,D_3,1188,0);
    //alldr(D,D_2,D_3,1427,0);
    ans = 0;coreans = 0;ocofs = 0; //the difference of Core occurance and pattern occurance
    int startm = 0;int mrlen = absm; int round = 0;int *B1,*B2,*B3;
    B1 = (int*)calloc(fround+1,4);
    B2 = (int*)calloc(fround+1,4);
    B3 = (int*)calloc(fround+1,4);
    int Ls[fround+1],Rs[fround+1]; //array of length O(log m) * 5
    
    for(int rs=0;rs<fround;rs++){
        if(DEBUGFLAG==1){printf("\nocofs = %d",ocofs);}
        int uy = ptrans(D,D_2,B1,B2,B3,Ls,Rs,startm,mrlen,round);
        if(uy<0){
            if(uy<-30){return -5;} //if pursing is not found in dictionary, end patternmatching
            break;}
        if(round==0){ocofs+=B1[round];}
        if(round>0){
            for(int i = 0;i<B1[round];i++){ocofs+=D_3[m[startm+i]];}}
        startm += B1[round];mrlen = B3[round];round++;}
    if(round!=fround){    
        if(coredata[0]>0){ //Aコアがあるのでその分だけocofsをずらす
            for(int i = 0;i<coredata[2];i++){
                ocofs+=D_3[m[startm+i]];
                if(DEBUGFLAG==1){
                    printf("\n[A]ocofs+=%d(%d)",D_3[m[startm+i]],ocofs);
                }
            }
        }
    }
    int leftof = 0; //leftofはコアの出現位置とパターンの出現位置のずれを修正する
    transk = round;
    printf("パターン変形回数:%d\n",transk);
    if(DEBUGFLAG==1){
        printf("rs==%d/%d\n",round,fround);
        for(int i = 0;i<round;i++){
            for(int i2 = 0;i2<B1[i];i2++){printf(" %d",m[Ls[i]+i2]);}
            int y7 = pscheck(D,D_2,Ls[i],B1[i],i);
            if(y7>=0){printf("([%d]",y7);for(int i2 = D_2[y7];i2<D_2[y7+1];i2++){printf(" %d",D[i2]);}printf("にある)");}
            printf(" / ");
        }
        printf("(Core)");
        for(int i2 = 0;i2<mrlen;i2++){
            printf(" %d",m[startm+i2]);
        }
        printf(" / ");
        for(int i = round-1;i>=0;i--){
            for(int i2 = 0;i2<B2[i];i2++){printf(" %d",m[Rs[i]+i2]);}
            int y7 = pscheck(D,D_2,Rs[i],B2[i],i);
            if(y7>=0){printf("([%d]",y7);for(int i2 = D_2[y7];i2<D_2[y7+1];i2++){printf(" %d",D[i2]);}printf("にある)");}
            printf(" / ");
        }
    }
// --------------------------------------
    //fround[一番上]から指定の一文字を探して見る
    timessub = clock();
    int *kmpc = (int*)calloc(mrlen,4);
    mkkmpc(startm,mrlen,kmpc);
    //int possibleP = 1;
    if(DEBUGFLAG==1){
        printf("\nocofs = %d",ocofs);
        if(coredata[0]==-1){printf("\nAコア = なし");}
        else{printf("\nAコア = 位置%dから長さ%d",coredata[1],coredata[2]);}
        if(coredata[3]==-1){printf("\nBコア = なし");}
        else{printf("\nBコア = 位置%dから長さ%d",coredata[4],coredata[5]);}
        if(coredata[6]==-1){printf("\nCコア = なし");}
        else{printf("\nCコア = 文字%dが%d個",coredata[7],coredata[8]);}
    }
    if(CHECKFLAG==2){return 0;}
    if(transk==fround){
        findinNS(D,D_2,D_3,startm,mrlen,round,kmpc,B1,B2,B3,Ls,Rs);
        return 0;
    }
    if(coredata[0]==-1){
        if(DEBUGFLAG==1){printf("Bルートで検索");}
        findB(D,D_2,D_3,startm,mrlen,round,kmpc,B1,B2,B3,Ls,Rs);
    }
    else{
        if(DEBUGFLAG==1){printf("Aルートで検索");}
        findAB(D,D_2,D_3,startm,mrlen,round,kmpc,B1,B2,B3,Ls,Rs);
    }
    //int bgp = pscheckb(D,D_2,startm+possibleP,mrlen-possibleP,round);
    //int smp = pschecks(D,D_2,startm+possibleP,mrlen-possibleP,round);
    //findinroundで発見した後分割に，smpより大きくbgpより小さい分割が続いているものを
    //一つ上のroundで検索する
    return 0;
}

int queryload(){
    //load[qname] file and put it in array [m]
    char qname[] = "query.txt";
    char chr;
    FILE *fp;
    long long int qsize = 400;
    qsize = std::filesystem::file_size(qname);
    fp = fopen(qname, "r"); // open file or return null
    if(fp == NULL) {
        printf("%s such file doesn't exist!\n", qname);
        return -1;
    }
    int i = 0;
    for(i=0;i<qsize;i++) {
        chr = fgetc(fp);
        m[i] = chr;
        if(chr<0){m[i]=chr + 256;}
        //printf("\n%d(%d)",m[i],i);
    }
    return (int)qsize;
}

int qrmreload(int c){ //mの中に，english.001.2 の中からc文字をもう一度選択して保存する
    char chr;FILE *fp;
    long long int qsize = std::filesystem::file_size(qname);
    fp = fopen(qname, "r"); // open file or return null
    printf("\n[%lld]rlen[%d]から[%d]文字\n[",qsize,checkpz[0],checkpz[1]);
    fseek(fp,checkpz[0],SEEK_SET); //指定したシークまで移動する
    for(int i=0;i<checkpz[1];i++) { //c文字
        chr = fgetc(fp);
        m[i] = chr;
        if(DEBUGFLAG==1){printf("%c",chr);}
        if(chr<0){m[i]=chr + 256;}
    }
    printf("]");
    return checkpz[1];
}

int qrmloadset(int jo){ //c番目を読む
    FILE *fp;
    long long int qsize = std::filesystem::file_size(qname); //qsizeには，ファイルのサイズが書かれているはず
    fp = fopen(qname,"r"); // open file or return null
    if(fp == NULL) {
        printf("%s such file doesn't exist!\n", qname);
        return -1;
    }
    int c = qsize/querysuu;
    fseek(fp,jo*c,SEEK_SET); //指定したシークまで移動する
    for(int i=0;i<c;i++) { //c文字
        char chr = fgetc(fp);
        m[i] = chr;
        if(chr<0){m[i]=chr + 256;}
        if(m[i]==10){jswt=1;}
        if(DEBUGFLAG==1){printf("%c",chr);}
    }
    printf("]");
    return c;
}

int qrmload(int c,int jo){ //mの中に，english.001.2 の中からc文字をランダムに選択して保存する
    char chr;FILE *fp;
    long long int qsize = std::filesystem::file_size(qname); //qsizeには，ファイルのサイズが書かれているはず
    fp = fopen(qname, "r"); // open file or return null
    if(fp == NULL) {
        printf("%s such file doesn't exist!\n", qname);
        return -1;
    }
    int i = 0;
    jswt = 0;
    int rlen = rand()%(qsize-c);
    if(runmode==2){rlen = MSTART;}
    ancl[jo][4] = rlen;
    fseek(fp,rlen,SEEK_SET); //指定したシークまで移動する
    printf("\nquery%d : [%lld]rlen[%d]から[%d]文字(%lld)\n[",jo,qsize,rlen,c,qsize);
    if(CHECKFLAG == 1){checkpz[0]=rlen;checkpz[1]=c;}
    for(i=0;i<c;i++) { //c文字
        chr = fgetc(fp);
        m[i] = chr;
        if(chr<0){m[i]=chr + 256;}
        if(m[i]==10){jswt=1;}
        if(DEBUGFLAG==1){printf("%c",chr);}
    }
    printf("]");
    return c;
}

int shelp(){
    printf("--Simple Usage------------------\n");
    printf("\nRead 'codeindex' and search using <queryfile>. \n");
    printf(": ./idx_nep -q <queryfile>\n");
    printf("\nYou can set the indexfile by using option -i. \n");
    printf(": ./idx_nep -i <indexfile> -q <queryfile>\n");
    printf("\nYou can set the location mode 0 ~ 4 by using option -m. \n");
    printf("mode 3 is used by default.(defined 'MMODE' in this code.)\n");
    printf(": ./idx_nep -i <indexfile> -q <queryfile> -m <mode>\n");
    printf("\nYou can set the number of query by using option -r. \n");
    printf("10 is used by default.(defined 'MREP' in this code.)\n");
    printf(": ./idx_nep -i <indexfile> -q <queryfile> -m <mode> -r <times>\n");
    printf("\nYou can set the length of pattern by using option -l. \n");
    printf("10 is used by default.(defined 'MLONG' in this code.)\n");
    printf(": ./idx_nep -i <indexfile> -q <queryfile> -m <mode> -l <length>\n");
    printf("\n");
    printf("\n");
    return 0;
}


int main(int argc, char *argv[])
{
    for(int i = 0;i<argc;i++){
      if('-'==argv[i][0]){ //符号
        if('i'==argv[i][1]){ //input
            int k = strlen(argv[i+1]);
            inname = (char*)malloc(k);
            for(int k2=0;k2<k;k2++){inname[k2]=argv[i+1][k2];}
        }
        if('q'==argv[i][1]){ //input
            int k = strlen(argv[i+1]);
            qname = (char*)malloc(k);
            for(int k2=0;k2<k;k2++){qname[k2]=argv[i+1][k2];}
        }
        if('m'==argv[i][1]){ //mode
            runmode=0;
            for(int k = 0;k<strlen(argv[i+1]);k++){
                if(10 > argv[i+1][k]-'0' && argv[i+1][k]-'0'>=0){
                runmode *=10;
                runmode += argv[i+1][k] - '0';
                }
                else{break;}
            }
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
        i++;continue;
      }
    }
    if(qname==NULL){shelp();return 0;} //
    if(inname==NULL){
      int k = strlen(defaultinname);
      inname = (char*)malloc(k);
      for(int k2=0;k2<k;k2++){inname[k2]=defaultinname[k2];}
    }
    if(runmode==-1){runmode=MMODE;}
    if(plength==-1){plength=MLONG;}
    if(querysuu==-1){querysuu=MREP;}

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
    fp = fopen(inname, "r");
    int flag = 0; //0:loops,1:dcs,2:dc2s,3:D,4:D_2,5:D_3,6:NS
    int w = 1;int tm1;
    while(flag<8){
        fscanf(fp,"%c",&chr);a = chr;
        fscanf(fp,"%c",&chr);a += chr*256;
        fscanf(fp,"%c",&chr);a += chr*65536;
        fscanf(fp,"%c",&chr);a += chr*65536*256;
        //if(w<30){printf("\n[%d]:%u",flag,a);}
        if(flag==7){NS[w]=a;w++;if(w==tm1){flag++;printf("\n-finishing load index-\n");}}
        if(flag==6){tm1=a;NS = (int*)malloc(4*tm1);flag++;}
        if(flag==5){D_3[w]=a;w++;if(w==dc2s[loops]){flag++;w=0;}}
        if(flag==4){D_2[w]=a;w++;if(w==dc2s[loops]+1){flag++;w=0;}}
        if(flag==3){D[w]=a;w++;if(w==dcs[loops]){flag++;w=0;}}
        if(flag==2){dc2s[w]=a;if(w==loops){flag++;w=0;D_2 = (int*)malloc(4*dc2s[loops]+4);D_3 = (int*)malloc(4*dc2s[loops]+4);}else{w++;}}
        if(flag==1){dcs[w]=a;if(w==loops){flag++;w=1;D = (unsigned int*)malloc(4*dcs[loops]);}else{w++;}}
        if(flag==0){loops=a;flag++;}
        //fseek(fp,16,SEEK_CUR);

    }
    fclose(fp);
    pn = tm1;
    NSsize = pn;
    round=loops;
    fround = round;
    double avt = 0;
    //int jogaipar = 0;
    if(runmode==2){
        msize = qrmload(plength,0);
        clock_t timesb = clock();
        patternmatching2(D,D_2,D_3,msize); 
        clock_t timee = clock();
        const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC * 1000.0;
        printf("\n[length of the pattern : %d] / (ans:%d / core:%d ) : time %lf[ms]\n",msize,ans,coreans,time);
        if(CHECKFLAG==1){
            msize = qrmload(plength,0);
            anscheck(msize);
        }
    }
    if(runmode==1 || runmode == 3){
        float avetransk = 0.00;
        double pertime = 0.00;
        if(runmode==3){srand((unsigned int)time(NULL));}
        for(int jo = 0;jo < MREP;jo++){
            int ralen = plength;
            if(runmode==1){ralen = plength;}
            msize = qrmload(ralen,jo);
            //if(jswt==1){printf("\n除外");jogaipar++;jo--;continue;}
            if(DEBUGFLAG==1){
                printf("\n検索文字列:[");
                for(int rek = 0;rek<ralen;rek++){printf("%c",m[rek]);}
                printf("]\n");
            }
            clock_t timesb = clock();
            patternmatching2(D,D_2,D_3,msize);
            clock_t timee = clock();
            const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC * 1000.0;
            const double time2 = static_cast<double>(timessub - timesb) / CLOCKS_PER_SEC * 1000.0;
            printf("\n[length of the pattern : %d] / (ans:%d / core:%d ) : time %lf[ms](%.3lf %)\n",msize,ans,coreans,time,(time2*100)/time);
            ancl[jo][0] = ans;
            ancl[jo][1] = coreans;
            ancl[jo][2] = ralen;
            ancl[jo][3] = transk;
            ancd[jo] = time;
            pertime += (time2*100)/time;
            avt += time;avetransk+=transk;
            if(CHECKFLAG==1){
                msize = qrmreload(ralen);
                anscheck(msize);
            }
        }
        for(int jo=0;jo<MREP;jo++){
            printf("\n(P-length : %d from position %d[%d trans] / ans:%d / core:%d / time %lf[ms])",ancl[jo][2],ancl[jo][4],ancl[jo][3],ancl[jo][0],ancl[jo][1],ancd[jo]);
            //printf("\n%d\n%d\n%d\n%d\n%d\n%lf",ancl[jo][2],ancl[jo][4],ancl[jo][3],ancl[jo][0],ancl[jo][1],ancd[jo]);
        }
        printf("\navarage time : %lf[ms]\n avarage transform : %f times\n",avt/MREP,avetransk/MREP);
        printf("[transformation time : %.3f %]\n",pertime/MREP);
    }
    if(runmode==4){
        for(int jo = 0;jo < querysuu;jo++){
            msize = qrmloadset(jo);
            //if(jswt==1){printf("\n除外");jogaipar++;jo--;continue;}
            if(DEBUGFLAG==1){
                printf("\n検索文字列:[");
                for(int rek = 0;rek<msize;rek++){printf("%c",m[rek]);}
                printf("]\n");
            }
            clock_t timesb = clock();
            patternmatching2(D,D_2,D_3,msize);
            clock_t timee = clock();
            const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC * 1000.0;
            printf("\n[length of the pattern : %d] / (ans:%d / core:%d ) : time %lf[ms]\n",msize,ans,coreans,time);
            ancl[jo][0] = ans;
            ancl[jo][1] = coreans;
            ancl[jo][2] = msize;
            ancl[jo][3] = transk;
            ancl[jo][4] = -1;
            ancd[jo] = time;
            avt += time;
        }
        printf("avarage time : %lf[ms]",avt/querysuu);
        for(int jo=0;jo<querysuu;jo++){
            printf("\n(P-length : %d from position %d[%d trans] / ans:%d / core:%d / time %lf[ms])",ancl[jo][2],ancl[jo][4],ancl[jo][3],ancl[jo][0],ancl[jo][1],ancd[jo]);
            //printf("\n%d\n%d\n%d\n%d\n%d\n%lf",ancl[jo][2],ancl[jo][4],ancl[jo][3],ancl[jo][0],ancl[jo][1],ancd[jo]);
        }
    }
    
    if(runmode==0){
        msize = queryload();
        clock_t timesb = clock();
        patternmatching2(D,D_2,D_3,msize);    
        clock_t timee = clock();
        const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC * 1000.0;
        printf("\n[length of the pattern : %d] / (ans:%d / core:%d ) : time %lf[ms]\n",msize,ans,coreans,time);
        
    }
    //exit(1);
    //printf("space : %d bytes (%d,%d,%d)",dc2s[loops]*4*2+dcs[loops]*4+pn*4,dc2s[loops],dcs[loops],pn);
    //printf("loop = %d",loops);
    
    free(NS); 
    free(D); 
    free(D_2); 
    free(D_3); 
    return 0;
}
