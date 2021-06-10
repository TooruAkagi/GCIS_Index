#include<iostream>
#include<stdlib.h>
#include<string> 
#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream
#include <math.h>
#include <filesystem>
#include <vector>
#include <cstring> //depends on your enviroment

#define MMODE 3 //default mode
#define MLONG 100 //default length
#define MREP 10 //default number of query
#define MSTART 113 //for mode 2
#define CHECKFLAG 0 //if 1, check answers
#define DEBUGFLAG 0 //if 1, show the process
//MMODE = 
// 0 : open [queryfile], regard it as a pattern
// 1 : open [queryfile], select some ramdom strings of length [MLONG] in the file, and regard it as patterns
// 2 : open [queryfile], select a fixed string of length [MLONG] in the file, and regard it as a pattern
// 3 : mode 1, with srand()
// 4 : open [queryfile], regard it as a seqence of [MREP] patterns



char *inname,*qname;
char defaultinname[] = "codeindex";
int runmode = -1; //mode
int plength = -1; //patternlength
int querysuu = -1; //the number of query
int qstart = -1;
int checkpz[5];
int vain=0;
int cheatlen[11] = {512,256,128,64,32,16,8,4,2,1,0};
int transreco[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int kad = 0;
int kad2 = 0;
int dcs[18]; //[i] = starting position of floor-i's first purse in the Dictionary
int dc2s[18]; //[i] = starting number of floor-i's first number in the Dictionar
int ccs[18]; //[i] = starting position of floor-i's first purse in the Dictionary
int cc2s[18]; //[i] = starting number of floor-i's first number in the Dictionary
int exceptr[18];
int loops =5; //the number of applying GCIS to the input file
int ans = 0;
int coreans = 0;
int hh = 0;
int lpurse = 65536; //定数
int spurse = 64; //定数
int spursesuu = lpurse / spurse; //定数
int *NS;
unsigned short int *d2topsub2,*k3sub2,*D_3m;
int *d2topsub,*k3sub,*D_3;
//int ksize = 200000000;int k3size = 4000000; //配列暫定サイズの保管
int D3smax,D3mmax,k3submax,k3sub2max,d2topsubmax,d2topsub2max,d2topmax,d2undermax; //配列サイズの保管
int Sizelist[11] = {0,0,0,0,0,0,0,200000000,4000000,0,0};
unsigned char *D_3s;
bool *k,*k3,*d2under,*d2top;
unsigned char *k3x,*d2topx;
int imcokep = 0;
int imcokep2 = 0;
int jswt = 0;
int gate = 0;
int transk = 0;
int checkans[1000];
int mote=0;
int rankct[256][8];
int occart[10];
int occart2[30][10];
int coredata[10];
clock_t timessub;
int ocofs = 0; //the difference of Core occurance and pattern occurance


int anscheck(int msize,int *m){ //msizeに持っているデータと比較し，答えが合っているかを確認する．
    if(ans==0){printf("[特殊]答え0個");}
    int ckt = 0;
    int huseiresult = 0;
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

int loga(unsigned int u){ //指定の数字uがn桁の二進数で表せる時，nを返す．
    if(u<1){/*printf("(%d)",u);*/return 0;}
    unsigned int e = 2;
    for(int i=1;i<32;i++){
        if(e>u){return i;}
        e *=2;
    }
    return 0;
}

int bread(bool *u,int s,int l){ //bool列のsからl文字を読む．
    if(s<0){return 0;}
    int e=0;
    for(int i=s;i<s+l;i++){
        e*=2;
        e += u[i];
        vain++;
    }
    return e;
}

void logb(unsigned int u,int n,bool *ar){ //指定の数字uの下n桁を二進数で表して保存．
    unsigned int e = u;
    bool c[n];
    for(int i=n-1;i>=0;i--){
        c[i] = e%2;
        e /=2;
    }
    for(int i=0;i<n;i++){ar[kad]=c[i];kad++;}
    return;
}

void logg(unsigned int u, bool *ar){ //指定の数字uのガンマ符号を二進数で表す，配列kにその値を保存．
    if(u<1){printf("0以下の数(%d)はlogg表現不可\n",u);return;}
    int l = loga(u);
    for(int i=1;i<l;i++){ar[kad]=0;kad++;}
    logb(u,l,ar);
    return;
}

int cmlen(int *m,int p,int l,bool *ar){ //配列arの先頭から分割mを保存する．
    kad = 0; //必ず位置0から保存する．
    int u = 0;
    for(int i=0;i<l;i++){
        if(i!=0){
            int tm = m[p+i]-m[p+i-1];
            if(tm<0){
                if(u==0){u=i;}
                tm=-tm;
            }
            logg(tm+1,ar);
        }
    }
    if(u<=0){u=1;}
    return u;
}

int gread(int p,bool *ar){
    //配列kの位置pからガンマ符号化でどのようなもじが存在するのかを読み解く
    int i = 0;
    while(1){
        if(ar[p+i]==1){break;}
        i++;
    }
    int u = i;
    int a = 0;
    while(u>=0){
        a*=2;
        a+=ar[p+i];
        i++;u--;
    }
    return a;
}

int sranknew1(int c,unsigned char *u,int *u2,unsigned short int *u3,int t2,int t3){ //0から始まってk番目までの1の個数を返す．．
    if(c<=0){return 0;}
    gate++;
    int r = c/lpurse;
    int nk = (c - r*lpurse)/spurse;
    int s =0;int e = 0;
    if(r>0){s = lpurse*r;e = u2[r-1];}
    if(nk>0){s+=nk*spurse;e+=u3[r*spursesuu+nk];}
    for(int i=s;i<c;i++){
        //printf("\n<%d-%d-%d-[%d]>",i,c,e,u[i/8]);
        //for(int y=0;y<8;y++){printf("%d",rankct[u[i/8]][y]);}
        if(c-i>8){e+=rankct[u[i/8]][7];i+=7;}
        else{
            e+=rankct[u[i/8]][c-i-1];return e;
        }
    }
    return e;
}

int sranknew0(int c,unsigned char *u,int *u2,unsigned short int *u3,int t2,int t3){ 
    //0から始まってk番目までの1の個数を返す
    return c-sranknew1(c,u,u2,u3,t2,t3);
}

int selectnew1(unsigned char *u,int *u2,unsigned short int *u3,int c,int n,int t2,int t3){ //k番目の1を探す．
    //printf("\n[%d]番目の1を探す(%d)[%d-%d]",c,n,t2,t3);
    //for(int o = 0;o<t2;o++){printf("[%d:%d]",o,u2[o]);}
    int i = 0;
    int j = 0;
    int l = 0;
    int hmax = t2-1;int hmin = 0;
    if(u2[0]<c){
        while(hmax>hmin+1){;
            i = (hmax+hmin)/2;
            //printf("\n[%d ~~ %d] >> %d",hmax,hmin,i);
            if(u2[i]>=c){hmax = i-1;}
            else{hmin = i;}
        }
        //printf("\n通過[%d ~~ %d] >> %d",hmax,hmin,i);
        if(u2[hmax]>=c){i = hmin;hmax = hmin-1;}
        else{i=hmax;hmin = hmax;}
        //printf("\nl = %d(u2[%d])",u2[i],i);
        l = u2[i];
        i++;
    }
    int e = l;l=0;
    int ofi = i*spursesuu;
    j=0; //2回目のloglog探索
    hmin = 0;hmax = spursesuu-1;
    if(t2-1 == i){hmax = (t3-1)%spursesuu;}
    while(hmax>hmin+1){
        j = (hmax+hmin)/2;
        //printf("\n[%d ~ %d] >> %d",hmax,hmin,j);
        if(u3[ofi+j]>=c-e){hmax = j-1;}
        else{hmin = j;}
    }
    //printf("\n通過[%d ~ %d] >> %d",hmax,hmin,j);
    if(u3[ofi+hmax]>=c-e){j = hmin;}else{j = hmax;}
    if(j==spursesuu){j=spursesuu-1;}
    l = u3[ofi+j];
    //printf("\nl = %d(+%d)(u3[%d > %d-%d])",l,u3[ofi+j],ofi+j,ofi,j);
    e += l;
    for(int p=i*lpurse+j*spurse;p<n;p++){
        if(e+rankct[u[p/8]][7]<c){
            e+=rankct[u[p/8]][7];p+=7;
        }
        else{
            for(int ji=0;ji<8;ji++){
                if(e+rankct[u[p/8]][ji]==c){return p+ji;}
            }
        }
    }
    printf("到達");
    return n+1;
}

int selectnew0(unsigned char *u,int *u2,unsigned short int *u3,int c,int n,int t2,int t3){ //k番目の0を探す．
    int i = 0;
    int j = 0;
    int l = 0;
    int hmax = t2-1;int hmin = 0;
    if(lpurse-u2[0]<c){
        while(hmax>hmin+1){
            i = (hmax+hmin)/2;
            if((i+1)*lpurse-u2[i]>=c){hmax = i-1;}
            else{hmin = i;}
        }
        if((hmax+1)*lpurse-u2[hmax]>=c){i = hmin;hmax = hmin-1;}
        else{i=hmax;hmin = hmax;}
        l = (i+1)*lpurse-u2[i];
        i++;
    }
    int e = l;l=0;
    int ofi = i*spursesuu;
    //if(gate>0){printf("(select_0 - %d,%d)",e,t2);}
    j=0; //2回目のloglog探索
    hmin = 0;hmax = spursesuu-1;
    if(t2-1 == i){hmax = t3-1;}
    while(hmax>hmin+1){
        j = (hmax+hmin)/2;
        if(j*spurse-u3[ofi+j]>=c-e){hmax = j-1;}
        else{hmin = j;}
    }
    if(hmax*spurse-u3[ofi+hmax]>=c-e){j = hmin;}else{j = hmax;}
    if(j==spursesuu){j=spursesuu-1;}
    l = j*spurse-u3[ofi+j];
    e += l;
    //if(gate>0){printf("(select_0 - %d/ %d,%d,%d,%d)",c,e,l,n,t3);}
    for(int p=i*lpurse+j*spurse;p<n;p++){
        //if(c==723466){printf("\n[%d-%d-%d]",u[p/8],(8-rankct[u[p/8]][7]),p);}
        if(e+(8-rankct[u[p/8]][7])<c){
            e+=8-rankct[u[p/8]][7];p+=7;
        }
        else{
            for(int ji=0;ji<8;ji++){
                //if(c==723466){
                //    printf("\n後%d:LAST[%d-%d-%d]",c-e,u[p/8],(1+ji-rankct[u[p/8]][ji]),p);
                //}
                if(e+(1+ji-rankct[u[p/8]][ji])==c){return p+ji;}
            }
        }
    }
    return n+1;
}

int psreadx(std::vector<int>& x,int p,bool *ar,int D2start,int D2end,int basekey){
    //tx[p]からtx[p+1]までの差分を読んで配列xに再生．再生した文字数を返却
    //if(DEBUGFLAG==1){printf("[%d]%d ",D2start,basekey);}
    int a;
    x.push_back(basekey); //先頭はbasekey
    int turnsl = 0;
    int st = D2start;
    for(int i = 0;i<1000;i++){
        a = gread(st,ar);
        st += 2*loga(a)-1;
        if(i>0){
            if(i>=turnsl){basekey-=a-1;}
            else{basekey += a-1;}
            x.push_back(basekey);
            //if(DEBUGFLAG==1){printf("%d ",basekey);}
        }
        else{
            turnsl = a;
            if(a==1){turnsl=65535;}
        }
        if(st>=D2end){break;}
    }
    return x.size();
}

int psread(int p,bool *ar,int D2start,int D2end,int basekey){
    //tx[p]からtx[p+1]までの差分を読んで分割を再生．
    if(DEBUGFLAG==1){printf("[%d]%d ",D2start,basekey);}
    int a;
    int turnsl = 0;
    int st = D2start;
    for(int i = 0;i<1000;i++){
        a = gread(st,ar);
        st += 2*loga(a)-1;
        if(i>0){
            if(i>=turnsl){basekey-=a-1;}
            else{basekey += a-1;}
            if(DEBUGFLAG==1){printf("%d ",basekey);}
        }
        else{
            turnsl = a;
            if(a==1){turnsl=65535;}
        }
        if(st>=D2end){break;}
    }
    if(DEBUGFLAG==1){
        printf("(");
        for(int y8 = D2start;y8<D2end;y8++){printf("%d",ar[y8]);}
        printf(")");
    }
    return 0;
}

int getd2(int tgm){
    int tgm2 = selectnew0(d2topx,d2topsub,d2topsub2,tgm+1,Sizelist[9],Sizelist[5],Sizelist[6]); //tgm2 = その位置までの1の数を見る．
    int tgm3 = sranknew1(tgm2,d2topx,d2topsub,d2topsub2,Sizelist[5],Sizelist[6]);
    int tgm4 = bread(d2under,tgm*6,6);
    return tgm3*64+tgm4;
}

int gethead(int tgm){
    int tgm2 = selectnew0(k3x,k3sub,k3sub2,tgm+1,Sizelist[8],Sizelist[3],Sizelist[4]); //tgm2 = その位置までの1の数を見る．
    //printf("\n<%d>",tgm2);
    int tgm3 = sranknew1(tgm2,k3x,k3sub,k3sub2,Sizelist[3],Sizelist[4]); //その位置までの1の数 = 開始文字.
    //printf("\n<%d>",tgm3);
    for(int o = loops-1;o>0;o--){
            if(tgm2>=cc2s[o]){tgm3-=ccs[o];break;}
    }
    //printf("\n<%d>",tgm3);
    return tgm3;
}

void uhdisp(int tgm){
        printf("\n%d:",tgm);
        int D2start = getd2(tgm);
        int D2end = getd2(tgm+1);
        int tgm3 = gethead(tgm);
        psread(tgm,k,D2start,D2end,tgm3);
}

int xdisp(std::vector<int>& x,int tgm){ //tgm番の分割をxに蘇生
        //printf("\n%d:",tgm);
        int D2start = getd2(tgm);
        int D2end = getd2(tgm+1);
        int tgm3 = gethead(tgm);
        return psreadx(x,tgm,k,D2start,D2end,tgm3);
}

int rpsck(int *m,int *Ls,int *B1,int *e2,int round){
    //不一致があれば-1を，round x まで合っていればxを返す．
    if(DEBUGFLAG==1){printf("\nrpsck / [round %d]",round);}
    if(round==-1){return 0;}
    int starti = Ls[round];
    int mlen = B1[round];
    int i = 0;int n2;
    int l = e2[0];
    std::vector<int> x;
    xdisp(x,l);
    if(x.size()<mlen){
        if(DEBUGFLAG==1){printf("\n長さにより左が確実に不一致");}
        return -1;}
    int rofs=0;if(round>1){rofs=dc2s[round-1];}
    for(i = 0;i<mlen;i++){
        if(x[x.size()-1-i]+rofs!=m[starti+mlen-1-i]){
            //printf("\n[round %d で 左不一致 %d != %d]",round,D[D_2[l+1]-1-i]+rofs,m[starti+mlen-1-i]);
            return -1;
        }
        if(DEBUGFLAG==1){printf("\nrhit%d(%d)",x[x.size()-1-i]+rofs,rofs);}
    }
    //printf("[正解]");
    if(round==0){return 0;}
    //printf("<状況チェック%d-%d>",mlen,D_2[l+1]-D_2[l]);
    if(mlen ==x.size()){
        //printf("ゲート1");
        if(e2[1]<0){/*printf("<round %d>まで一致",round-1);*/return round-1;}
        n2 = e2[1];
        std::vector<int> x2;
        xdisp(x2,n2);
        e2[0] = rofs+x2[x2.size()-1];
        e2[1] = rofs+x2[x2.size()-2];
    } //e2[1]の左側2つ
    if(mlen+1==x.size()){
        //printf("ゲート2");
        if(e2[1]<0){e2[1]=-1;}
        else{
            n2 = e2[1];
            std::vector<int> x2;
            xdisp(x2,n2);
            e2[1] = rofs+x2[x2.size()-1];} //e2があるなら定義，なければ-1
        e2[0] = rofs+x[x.size()-1-mlen];
    } //e2[0]の左端とe2[1]の右側2つ
    if(mlen+1<x.size()){
        //printf("ゲート3");
        e2[0] = rofs+x[x.size()-1-mlen];
        e2[1] = rofs+x[x.size()-2-mlen];
    } //e2[0]のさらに左側2つ
    //printf("next e2[%d,%d(+%d)] =>",e2[0],e2[1],rofs);
    return rpsck(m,Ls,B1,e2,round-1);
}

int psck(int *m,int *Rs,int *B2,int *e,int round){
    if(DEBUGFLAG==1){printf("\n[psck / round %d]",round);}
    if(round==-1){return 0;}
    int starti = Rs[round];
    int mlen = B2[round];
    int rofs=0;if(round>1){rofs=dc2s[round-1];}
    int cnt = 0;int a1[3] = {-1,-1,-1};int cnt2 = 0;
    //if(gate==16){printf("?_[%d][%d][%d]",a1[0],a1[1],a1[2]);}
    for(int i=0;i<3;i++){
        int l = e[i];if(l<0){break;}
        std::vector<int> x;
        xdisp(x,l);
        for(int j = 0;j<x.size();j++){
            if(cnt<mlen){
                if(x[j]+rofs!=m[starti+cnt]){
                     if(DEBUGFLAG==1){printf("round %d で右不一致(%d !=%d)",round,x[j]+rofs,m[starti+cnt]);}
                    return -1;
                }
                cnt++;
            }
            else{
                if(round==0){/*printf("[完全正解+]");*/return 0;}
                a1[cnt2] = rofs+x[j];cnt2++;
                if(cnt2==3){
                    e[0] = a1[0];e[1] = a1[1];e[2] = a1[2];
                    //printf("[正解]");
                    //printf("next e2[%d,%d,%d] =>",e[0],e[1],e[2]);
                    return psck(m,Rs,B2,e,round-1);
                }
            }
        }
    }
    if(cnt<mlen){/*printf("不足・判別不可");*/return 1;}
    //if(gate==16){printf("?_[%d][%d][%d] -> [%d][%d][%d]",a1[0],a1[1],a1[2],e[0],e[1],e[2]);exit(4);}
    e[0] = a1[0];e[1] = a1[1];e[2] = a1[2];
    return psck(m,Rs,B2,e,round-1);
}

int pscmpr(bool *ar,bool *ar2,int start,int start2,int l,int bl,int plen,int y,int a){ //分割2つの長さをlまで比べる．
    int od = 0;
    int key = 0;
    int len = 0;
    int rt = l;
    if(a<l){rt = a;} //rtはlとaのうち小さいほう
    int readcount = 0;
    int x;int i;
    for(i = 0;i<rt;i++){
        if(readcount==y && y!=0 && od==1){return 1;}
        if(readcount==bl && bl!=0 && od==2){return 0;}
        //if(gate>0){printf("\n%d ? %d",ar[start+i],ar2[start2+i]);}
        if(ar[start+i]!=ar2[start2+i]){
            if(key==0){
                if(ar[start+i]==0){x = 1;}
                else{x = 0;}
            }
            if(key==1){
                if(ar[start+i]==0){x=0;}
                else{x=1;}
            }
            if(od==3){x = (x+1)%2;}
            if(od==1){return 0;}
            if(od==2){return 1;}
            return x;
        }
        if((ar[start+i] == 1) && key == 0){len+=1;key=1;}
        if(key==0){len++;}
        else{
            len--;
            if(len==0){
                readcount++;
                //if(gate>0){printf("\n%d++",readcount);}
                key=0;
                if(readcount==y){od+=2;}
                if(readcount==bl){od+=1;}
                if(readcount>=plen){break;} //分割を全て読んでしまった
                if(od==1){
                    return 0;}
                if(od==2){
                    if(i+1==a){return 0;}
                    return 1;
                }
            }
        }
    }
    if(a<l){return 0;}
    if(a>l){return 1;}
    if(a==l){return 2;}
    return 2;
}

int pscheck(int *m,int start,int plen, int round){ //完全一致を探す
    bool *bitlet;
    bitlet = (bool*)malloc(10000);
    //round 0 .. 0からccs[round]
    //round1 ccs[round](118)を足す
    //round2 dc2s[1](57466)を引いた値(0〜dc2s[1]〜[2]にccs[2](56461)を足す)
    //round3 dc2s[2]をひいてccs[3]を足す
    int rofs = ccs[round];
    if(round>=2){rofs-=dc2s[round-1];}
    int y = cmlen(m,start,plen,bitlet);
    int a,b,c,d;
    int posup = selectnew1(k3x,k3sub,k3sub2,m[start]+rofs,Sizelist[8],Sizelist[3],Sizelist[4]);
    int posbt = selectnew1(k3x,k3sub,k3sub2,m[start]+1+rofs,Sizelist[8],Sizelist[3],Sizelist[4]);
    int ry = posup;
    posup = sranknew0(posup,k3x,k3sub,k3sub2,Sizelist[3],Sizelist[4])-1;
    posbt = sranknew0(posbt,k3x,k3sub,k3sub2,Sizelist[3],Sizelist[4]);
    //if(posup>=661175){printf("[%d]のエラー:%d[%d]\n",m[start],ry,rofs);}
    //printf("%d > %d >> (%d 〜 %d)%d - %d[%d]\n",m[start]+rofs,ry,posup,posbt,plen,start,round);
    if(posup < dc2s[round]-1){posup = dc2s[round]-1;}
    if(posbt > dc2s[round+1]){posbt = dc2s[round+1];}
    int pos = 0;
    //printf("%d:(%d 〜 %d)%d - %d\n",rofs,posup,posbt,plen,start);
    //for(int y6=0;y6<plen;y6++){printf("[%d]",m[start+y6]);}
    //posup = 既に絶対違うと考えられるところ(round0だと-1)
    //posbt = 既に絶対違うと考えられるところ(round0だと分割数そのもの)
    while(1){
        pos = posup + (posbt-posup)/2;
        b = selectnew0(d2topx,d2topsub,d2topsub2,pos+1,Sizelist[9],Sizelist[5],Sizelist[6]);
        b = sranknew1(b,d2topx,d2topsub,d2topsub2,Sizelist[5],Sizelist[6]); //その位置までの1の数 = 開始文字.
        c = bread(d2under,pos*6,6);
        d = b*64+c;
        c = gread(d,k);d+=loga(c)*2-1;
            //if(round==9){gate++;}
            a = selectnew0(d2topx,d2topsub,d2topsub2,pos+2,Sizelist[9],Sizelist[5],Sizelist[6]);
            a = sranknew1(a,d2topx,d2topsub,d2topsub2,Sizelist[5],Sizelist[6]);
            a*=64;a+=bread(d2under,(pos+1)*6,6);a-=d;
        b = pscmpr(k,bitlet,d,0,kad,c-1,plen-1,y-1,a);
        //if(round==9){printf("\n[pscmpr(%d,%d,%d,%d / %d,%d,%d)\n]",a,c-1,y-1,b,posup,pos,posbt);}
        if(b==2){
            a = selectnew0(d2topx,d2topsub,d2topsub2,pos+2,Sizelist[9],Sizelist[5],Sizelist[6]);
            a = sranknew1(a,d2topx,d2topsub,d2topsub2,Sizelist[5],Sizelist[6]);
            a*=64;a+=bread(d2under,(pos+1)*6,6);
            if(a>d+kad){b=1;}
            if(a<=d+kad){b=pos;break;}
        }
        if(b==0){posup=pos;}
        if(b==1){posbt=pos;}
        if(posbt-posup<2){
            printf("\n分割にないため存在しない");pos=-1;
            break;
        }
    }
    if(pos==-1){
        //for(int yj=0;yj<plen;yj++){printf(" %d",m[start+yj]);}
        //for(int yj=0;yj<=loops;yj++){printf("\ndc2s[%d]=%d,%d",yj,dc2s[yj],ccs[yj]);}
        //for(int yj=1;yj<=loops;yj++){uhdisp(dc2s[yj]-1);}
        exit(6);
        }
   //printf(" -> %d\n",pos);
    return pos;
}

int ptrans(int *B1,int *B2,int *B3,int *Ls,int *Rs,int startm,int *m,int mrlen,int round){
    int *Rp;int flag = 0;Rp = (int*)malloc(mrlen*4+4);
    int tn = -1;int Rpptr = 0;int len = 0;
    coredata[6] = -1;coredata[8]=0; //Cコアは,最初はないし，長さも0;
    if(DEBUGFLAG==1){printf("\n[変換%d]",round);}
    for(int i = startm+mrlen-1;i>startm;i--){ //check from right(for setting L&S type)
        Rp[Rpptr] = m[i];len++;
        if(m[i-1] < m[i]){tn=0;}
        if(m[i-1] > m[i]){
            if(tn==0){
                if(B2[round] == 0){B2[round] = len;len = 0;flag++;}
                else{
                    Rpptr-=(len-1);
                    Rp[Rpptr] = pscheck(m,i,len,round); //i = 初期値 len = 長さ,round
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
    for(int i = 0;i<=Rpptr;i++){ //reverse Rp >> m
        m[startm+i] = Rp[Rpptr-i];
    }
    if(DEBUGFLAG==1){printf("\n[変換%d]end",round);}
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

int D3access(int c){ //c番目のD_3の値を返す
    if(c<Sizelist[1]){return D_3s[c];}
    if(c<Sizelist[2]){return D_3m[c-Sizelist[1]];}
    return D_3[c-Sizelist[2]];
}

void mkkmpc(int *m,int startm,int mrlen,int *kmpc){
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
                    //printf("[%d==%d]",i4,i2-i3+i4);
                }
                else{break;}
            }
            if(i4==i3){kmpc[i2]=i4;}
        }
    }
    
    return;
}

int rightopennd3(int *m,int round,int count,int tround,int mode){ //左の[4-6]をcount回開ける
    //printf("\n[%d-右%dオープン(%d)]",mode,count,round);
    //for(int i = 0; i<10; i++){printf("[%d]",occart2[tround][i]);}
    if(count<1){return 0;}
    //printf("\n");
    int a[3] = {-1,-1,-1};int tg = 0;
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    if(occart2[tround][7-mode] > -1){
        for(int k = 0; k<mode; k++){
            std::vector<int> x;
            xdisp(x,occart2[tround][7-mode+k]);
            //printf("\noccart2[%d] = %d",7-mode+k,occart2[tround][7-mode]);
            for(int i = 0;i<x.size();i++){
                a[tg] = rofs+x[i];tg++;
                //printf(" got(%d [+%d]) ",rofs+D[i],rofs);
                if(tg==mode){
                    for(int j = 0;j<mode;j++){
                        //printf("\noccart2[%d] = %d",7-mode+j,a[j]);
                        occart2[tround][7-mode+j] = a[j];
                    }
                    return rightopennd3(m,round-1,count-1,tround,mode);
                }
            }
        }
    }
    //printf("完全には埋まらず"); //右が3つ出てこない場合でも，展開は続行する(展開の過程で3つうまることもある)
    for(int j = 0;j<tg;j++){
        //printf("\noccart2[%d] = %d",7-mode+j,a[j]);
        occart2[tround][7-mode+j] = a[j];
    }
    return rightopennd3(m,round-1,count-1,tround,mode);
}

int coreCcheck(int *m,int round,int count,int tround){ //一つレベルの高い分割を2つ抑える．[-B][C*][**]と3つが得られる．
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
            std::vector<int> x;
            xdisp(x,occart2[tround][4+u]);
            for(int i = 0;i<x.size();i++){
                if(Clef>0 && count==1){
                    if(rofs+x[i]==coredata[7]){
                        Clef--;
                        if(DEBUGFLAG==1){printf("\n[hit %d(%d) / あと%d]",rofs+x[i],x[i],Clef);}
                        continue;}else{return -1;}
                }
                //if(fround==0){return 0;} //Clefがなくなって，froundが0ならもう右はあっているのでは
                a[tg] = rofs+x[i];tg++;
                 if(DEBUGFLAG==1){printf(" got(%d [+%d]) ",rofs+x[i],rofs);}
                if(tg==3){
                    for(int j = 0;j<3;j++){
                         if(DEBUGFLAG==1){printf("\noccart2[%d] = %d",4+j,a[j]);}
                        occart2[tround][4+j] = a[j];
                    }
                    if(Clef==0){occart2[tround][7] += 2;return rightopennd3(m,round-1,count-1,tround,3);} //Cコア把握
                    return coreCcheck(m,round-1,count-1,tround);
                }
            }
        }
    }
    if(count==1){//Cの存在を確かめた上で右分割が足りない場合
        if(tg==0 || Clef>0){printf("おかしい!(%d,%d)",tg,Clef);exit(7);}
        if(tg>=1){occart2[tround][4] = a[0];}
        if(tg>=2){occart2[tround][5] = a[1];}
        occart2[tround][7] += 2;
        return rightopennd3(m,round-1,count-1,tround,3);
    }
    else{ //countがもっと高い
        if(tg>=1){occart2[tround][4] = a[0];}
        if(tg>=2){occart2[tround][5] = a[1];}
        return coreCcheck(m,round-1,count-1,tround);
    }
}

void leftopen(int *m,int round,int count,int tround){ //左の[0]をcount回開ける
    if(DEBUGFLAG==1){
         printf("\n[1-左%dオープン(%d)]",count,round);
         if(occart2[tround][0] > -1){uhdisp(occart2[tround][0]);}
    }
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    if(occart2[tround][0] > -1){
        std::vector<int> x;
        xdisp(x,occart2[tround][0]);
       occart2[tround][0] = rofs+x[x.size()-1];
       if(DEBUGFLAG==1){printf(" >> %d ",occart2[tround][0]);}
       if(count>1){leftopen(m,round-1,count-1,tround);}
    }
    return;
}

int leftopennd(int *m,int round,int count,int tround){ //左の[0][1]をcount回開ける
     if(DEBUGFLAG==1){
         printf("\n[2-左%dオープン(%d)]",count,round);
         if(occart2[tround][1] > -1){uhdisp(occart2[tround][1]);}
         if(occart2[tround][0] > -1){uhdisp(occart2[tround][0]);;}
    }
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    if(occart2[tround][1] > -1){ //左辺文字Aが指定されていれば
        std::vector<int> x;
        xdisp(x,occart2[tround][1]);
         if(DEBUGFLAG==1){printf("open [%d,%d--%d] >>",occart2[tround][1],dc2s[round],rofs);}
        int a1 = rofs+x[x.size() - 1];
        if(DEBUGFLAG==1){printf("\na1 = %d(%d)",a1,count);}
        if(count>1){occart2[tround][1] = a1;return leftopennd(m,round-1,count-1,tround);}
        int a2;
        //if(D_2[occart2[tround][1]]<2){printf("a2を消す");a2 = -1;}
        a2 = rofs+x[x.size() - 2];
        occart2[tround][1] = a1;
        occart2[tround][0] = a2;
        if(DEBUGFLAG==1){printf("\n更新 = %d & %d",a1,a2);}
    }
    return 0;
}

int coreAcheck(int *m,int round,int count,int tround,int startm){ //左の[0][1]をcount回開ける
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
                std::vector<int> x;
                xdisp(x,l);
                if(l<0){
                    if(DEBUGFLAG==1){printf("-1を回避(j==%d)",j);}
                    break;}
                 if(DEBUGFLAG==1){printf("open[%d(+%d)]",l,rofs);}
                for(int i = x.size()-1;i>=0;i--){
                    if(count==1 && Alen>0){ //count1ならAコアの判定をする
                         if(DEBUGFLAG==1){printf("\n[%d-%d]",rofs+x[i],m[startm-1+Alen]);}
                        if(x[i]+rofs == m[startm-1+Alen]){
                            Alen--;continue;
                        }
                        else{return -1;}
                    }
                    else{ //count1以外か，コアを判定し終わったら
                        e[etg] = rofs+x[i];etg++;
                        if(etg==2){ //コアの横の文字が2つ集まったら
                            occart2[tround][1] = e[0]; //1こめ
                            occart2[tround][0] = e[1]; //1こめ
                            if(count>1){return coreAcheck(m,round-1,count-1,tround,startm);}
                            return 0; //Aを見終わったならそのままreturnしていいはず
                        }
                    }
                }
            }
    //coreAcheckは最低1文字ある状態で呼ぶ．
    if(count>1){
        if(etg==1){occart2[tround][1] = e[0];}
        return coreAcheck(m,round-1,count-1,tround,startm);} //無理やりチェック
    if(count != 1){printf("おかしい(count = %d)",count);exit(1);}
    if(Alen>0){return -1;}
    //printf("未定義[etg = %d]",etg);
    if(etg==0){printf("Aでぴったり切れている");occart2[tround][1] = -1;occart2[tround][0] = -1;return 0;}
    if(etg==2){printf("追加 > %d.%d",e[0],e[1]);occart2[tround][1] = e[0];occart2[tround][0] = e[1];return 0;}
    if(etg==1){printf("追加 > %d",e[0]);occart2[tround][1] = e[0];return 0;}
    return 0;
}


int findinNS(int *m,int startm,int mrlen,int round,int *kmpc,int *B1,int *B2,int *B3,int *Ls,int *Rs){
    if(DEBUGFLAG==1){
        printf("\nNSから探す[%d]\nkmpc=",mrlen);
        for(int i2 = 0;i2<mrlen;i2++){printf("%d ",kmpc[i2]);}
        printf("\n");
        for(int i2 = 0;i2<mrlen;i2++){printf("%d ",m[startm+i2]);}
    }
    int tg,kpofs;
    tg = 0;kpofs=0;
        for(int y8=0;y8<Sizelist[0];y8++){
            for(int y9=0;y9<mrlen;y9++){
                if(NS[y9+y8]+dc2s[loops-1]==m[startm+tg]){
                    tg++;
                    if(DEBUGFLAG==1){printf("\n%d:%d(%d - %d)[+%d]",kpofs,NS[y9+y8]+dc2s[loops-1],y9+y8,tg,D3access(NS[y9+y8]+dc2s[loops-1]));}
                    if(tg==mrlen){
                        if(DEBUGFLAG==1){printf("パターン発見[%d]",kpofs-ocofs);}
                        int e2[2] = {NS[y8-1]+dc2s[loops-1],NS[y8-2]+dc2s[loops-1]};
                        int tk = rpsck(m,Ls,B1,e2,loops-1);
                        if(DEBUGFLAG==1){if(tk==-1){printf("\nこれはコアではない2");}}
                        if(tk==0){
                            int e[3] = {NS[y8+mrlen]+dc2s[loops-1],NS[y8+mrlen+1]+dc2s[loops-1],NS[y8+mrlen+2]+dc2s[loops-1]};
                            tk = psck(m,Rs,B2,e,loops-1);
                            if(DEBUGFLAG==1){if(tk==-1){printf("\nこれはコアではない");}}
                            if(tk==0){ //左右も完璧に合ってた
                                if(DEBUGFLAG==1){printf("答え");}
                                //printf(" %d",kpofs-ocofs);
                                if(ans<1000 && CHECKFLAG==1){checkans[ans]=kpofs-ocofs;}
                                ans++;
                            }
                        }
                        kpofs+=D3access(NS[y8]+dc2s[loops-1]);
                        tg=0;break;
                    }
                }
                else{ //不一致
                    if(tg!=0){
                        //printf("\n%d[%d > %d]",y8,tg,kmpc[tg]);
                        for(int u=0;u<tg-kmpc[tg]+1;u++){
                            //printf("(+%d)",D3access(NS[u+y8]+dc2s[loops-1]));
                            kpofs+=D3access(NS[u+y8]+dc2s[loops-1]);
                        }
                        y8+=tg - kmpc[tg];
                        tg = kmpc[tg];
                    }
                    else{kpofs+=D3access(NS[y9+y8]+dc2s[loops-1]);}
                    break;
                }
            }
        }
    return 0;
}

void findintop(int tg,int pofs){ //NS列からtgを探す
    if(DEBUGFLAG==1){printf("\n[%d]を探す(%d)",tg,dc2s[loops-1]);}
    int len = 0;
    for(int y1 = 0;y1<Sizelist[0];y1++){
        if(DEBUGFLAG==1){printf("[%d]答え",NS[y1]);}
        if(NS[y1]==tg-dc2s[loops-1]){
            printf(" %d",len+pofs);
            if(ans<1000 && CHECKFLAG==1){checkans[ans]=len+pofs;}
            ans++;
        }
        len+= D3access(NS[y1]+dc2s[loops-1]);
    }
    //printf("\nend of searching.(%d)[%d]",len,tg);
    return;
}


void findinround1(int tg,int round,int pofs){
    if(round==loops){return findintop(tg,pofs);}
    //あるラウンドから文字[tg]を探す
    //printf("%d(+%d)",tg,pofs); //引き継いだズレ値を記録
    int e2[2];
    int rofs = 0;
    if(round>1){rofs=dc2s[round-1];}
    for(int y8=dc2s[round];y8<dc2s[round+1];y8++){
        int ofs = 0;
        //printf("\n[%d(%d)]",y8,D_2[y8+1]-D_2[y8]);
        std::vector<int> x;
        xdisp(x,y8);
        for(int y9=0;y9<x.size();y9++){
            //printf(" %d",rofs+D[y9]);
            if(rofs+x[y9]==tg){
                //if(round>15){printf("\n発見(%d,%d[+%d])",tg,ofs+pofs,ofs);}
                findinround1(y8,round+1,pofs+ofs);
            }
            if(round>0){ofs+=D3access(rofs+x[y9]);}else{ofs++;}
        }
    }
    return;
}

int findupsintop(int *m,int startm,int mrlen,int *B1,int *B2,int *B3,int *Ls,int *Rs,int count){ //一番上の階層まで来てもまだ見つからない例があるなら全力で対処する
    if(DEBUGFLAG==1){
        printf("\nfround(%d)まで到達[%d] / 探すもの[%d]\n",loops,count,occart2[loops-1][2]);
        uhdisp(occart2[loops-1][2]);
        for(int i = 0; i<10; i++){printf("[%d]",occart2[loops-1][i]);}
    }
    int len = 0;
    int tg = occart2[loops-1][2];
    int ofs = occart2[loops-1][3];
    for(int y1 = 0;y1<Sizelist[0];y1++){
        if(len < 82134000 && len>82124000){
            if(DEBUGFLAG==1){
                printf("\n[%d]",y1);
                printf("%d(+%d [%d])",len,D3access(NS[y1]+dc2s[loops-1]),NS[y1]+dc2s[loops-1]);
             }
        }
        if(NS[y1]+dc2s[loops-1]==tg){
            if(DEBUGFLAG==1){printf("\nコア(単独)を発見%d(+%d)",len+ofs,ofs);}
            coreans++;
            for(int i = 0; i<10; i++){occart2[loops][i] = occart2[loops-1][i];} //コアを見つけたら下階層からゲット
            if(DEBUGFLAG==1){
                printf("\n最初 → ");
                for(int i = 0; i<10; i++){printf("[%d]",occart2[loops][i]);}
            }
            if(y1>0 && occart2[loops][0]<0){ //左が埋まっていなくて，かつ左があるなら
                if(occart2[loops][1] == -1){ //2つとも埋まっていないなら
                    occart2[loops][1] = NS[y1-1]+dc2s[loops-1];
                    if(y1>1){occart2[loops][0] = NS[y1-2]+dc2s[loops-1];}
                    if(occart2[loops][7]%2==0){
                        if(coreAcheck(m,loops-1,count,loops,startm)==-1){
                            if(DEBUGFLAG==1){printf("Aコアミスマッチ");}
                            len+= D3access(NS[y1]+dc2s[loops-1]);
                            continue;
                        }
                        else{occart2[loops][7]++;}//1文字は入れたんだから0が帰ってきていればAはもう探し終わっている
                    }
                    else{leftopennd(m,loops-1,count,loops);}
                }
                else{ //1つ埋まっていないなら
                    occart2[loops][0] = NS[y1-1]+dc2s[loops-1];
                    leftopen(m,loops-1,count,loops);
                }
            }
            if(y1<Sizelist[0] && occart2[loops][6]<0){ //右が埋まっていなくて，かつ右があるなら
                if(occart2[loops][5]==-1){ //2文字埋まってないなら
                        if(occart2[loops][4]==-1){ //3文字埋まってないなら
                            occart2[loops][4] = NS[y1+1]+dc2s[loops-1];
                            if(y1+2<Sizelist[0]){occart2[loops][5] = NS[y1+2]+dc2s[loops-1];}
                            if(y1+3<Sizelist[0]){occart2[loops][6] = NS[y1+3]+dc2s[loops-1];}
                            if(occart2[loops][7]/2<1){
                                if(coreCcheck(m,loops-1,count,loops)==-1){
                                    if(DEBUGFLAG==1){printf("\nCコアミスマッチ[top内]");}
                                    len+= D3access(NS[y1]+dc2s[loops-1]);
                                    continue;
                                }
                            } //Cコアがあるなら調べる
                            else{rightopennd3(m,loops-1,count,loops,3);} //3文字count回解体
                        }
                        else{
                            occart2[loops][5] = NS[y1+1]+dc2s[loops-1];
                            if(y1+2<Sizelist[0]){occart2[loops][6] = NS[y1+2]+dc2s[loops-1];}
                            rightopennd3(m,loops-1,count,loops,2);
                        }
                    }
                    else{ //1つだけ
                        occart2[loops][6] = NS[y1+1]+dc2s[loops-1];
                        rightopennd3(m,loops-1,count,loops,1);
                    }
            }
            if(DEBUGFLAG==1){
                printf("\nこうなる → ");
                for(int i = 0; i<10; i++){printf("[%d]",occart2[loops][i]);}
            }
            //occart2[round][6]> -1 && occart2[round][0] > -1 ?
            //printf("\n[早速チェック]");
            int e2[2] = {occart2[loops][1],occart2[loops][0]};
            int tk = rpsck(m,Ls,B1,e2,loops-count-1);
            //if(tk==-1){printf("\nこれはコアではない");}
            if(tk==0){
                int e[3] = {occart2[loops][4],occart2[loops][5],occart2[loops][6]};
                tk = psck(m,Rs,B2,e,loops-count-1);
                    //if(tk==-1){printf("\nこれはコアではない");}
                if(tk==0){ //左右も完璧に合ってた
                    if(DEBUGFLAG==1){printf("答え");}
                    printf(" %d",len+occart2[loops][3]);
                    if(len+occart2[loops][3]==94818246){printf("ここで強制終了\n");exit(0);}
                    if(ans<1000 && CHECKFLAG==1){checkans[ans]=len+occart2[loops][3];}
                    ans++;
                }
            }
            //exit(5);
        }
        len+= D3access(NS[y1]+dc2s[loops-1]);
        //if(len<100){printf("\n[.%d < %d + (%d)%d]",len,D_3[NS[y1]+dc2s[fround-1]],y1,NS[y1]+dc2s[fround-1]);
        //    alldr(D,D_2,D_3,NS[y1]+dc2s[fround-1],fround-1);
        //}
    }
    if(DEBUGFLAG==1){printf("\n[findupsintop 終了]");}
    return 0;
}

int findups(int *m,int startm,int mrlen,int round,int *B1,int *B2,int *B3,int *Ls,int *Rs,int count){
    if(DEBUGFLAG==1){
        printf("\n[%d]を探す(round %d / %d)",occart2[round-1][2],round,loops);
        for(int i = 0; i<10; i++){printf("[%d]",occart2[round-1][i]);}
        uhdisp(occart2[round-1][2]);
    }
    if(round==loops){return findupsintop(m,startm,mrlen,B1,B2,B3,Ls,Rs,count);}
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    for(int y8=dc2s[round];y8<dc2s[round+1];y8++){
        std::vector<int> x;
        xdisp(x,y8);
        int ofs = 0;
        for(int y9=0;y9<x.size();y9++){
            //printf(" %d",rofs+D[y9]);
            if(rofs+x[y9]==occart2[round-1][2]){ //該当する1文字を見つけたら
                gate++;
                if(DEBUGFLAG==1){
                     printf("\n[%dの%d文字目に発見(+%d)]",y8,y9,ofs);
                }
                coreans++;
                //左がないなら左を探す(このroundで1つ見つかれば良い)
                for(int i = 0; i<10; i++){occart2[round][i] = occart2[round-1][i];} //コアを見つけたら下階層からゲット
                if(y9==0 || occart2[round-1][0]>-1){ //先頭にないか，左[0]が埋まっているなら左は追加できない
                }
                else{ 
                    if(occart2[round][1] == -1){ //2つとも埋まっていないなら
                        occart2[round][1] = rofs+x[y9-1];
                        if(1<y9){occart2[round][0] = rofs+x[y9-2];}
                        //printf("\n[%d]記入数 %d %d",y9-D_2[y8],occart2[round][0],occart2[round][1]);
                        if(occart2[round][7]%2==0){
                            if(coreAcheck(m,round-1,count,round,startm)==-1){
                                 if(DEBUGFLAG==1){printf("Aミスマッチ");}
                                continue;
                            }
                            else{occart2[round][7]++;} //1文字少なくとも入れたはずだし0が帰ってきていれば必ずAコアは調べ終わっている
                        }
                        else{leftopennd(m,round-1,count,round);}
                    }
                    else{ //1つ埋まっていないなら
                        occart2[round][0] = rofs+x[y9-1];
                        leftopen(m,round-1,count,round);
                    }
                }
                //printf("\n演算[%d = %d][%d]",y9+1,D_2[y8],occart2[round-1][6]);
                if(y9+1<x.size() && occart2[round-1][6]==-1){ //右があり，右が埋まっていないなら
                    if(occart2[round][5]==-1){ //2文字埋まってないなら
                        if(occart2[round][4]==-1){ //3文字埋まってないなら
                            occart2[round][4] = rofs+x[y9+1];
                            if(y9+2<x.size()){occart2[round][5] = rofs+x[y9+2];}
                            if(y9+3<x.size()){occart2[round][6] = rofs+x[y9+3];}
                            if(occart2[round][7]/2<1){
                                if(coreCcheck(m,round-1,count,round)==-1){
                                     if(DEBUGFLAG==1){printf("Cコアミスマッチ");}
                                    continue;}
                            } //Cのチェックもしていないなら
                            else{rightopennd3(m,round-1,count,round,3);}
                        }
                        else{
                            occart2[round][5] = rofs+x[y9+1];
                            if(y9+2<x.size()){occart2[round][6] = rofs+x[y9+2];}
                            rightopennd3(m,round-1,count,round,2);
                        }
                    }
                    else{ //1つだけ
                        occart2[round][6] = rofs+x[y9+1];
                        rightopennd3(m,round-1,count,round,1);
                    }
                    //exit(7);
                }
                //printf("\n結果:");
                //for(int i = 0; i<10; i++){printf("[%d]",occart2[round][i]);}
                if(occart2[round][6]> -1 && occart2[round][0] > -1){
                    //printf("\n[早速チェック]");
                    int e2[2] = {occart2[round][1],occart2[round][0]};
                    int tk = rpsck(m,Ls,B1,e2,round-count-1);
                    //if(tk==-1){printf("\nこれはコアではない");}
                    if(tk==0){
                        int e[3] = {occart2[round][4],occart2[round][5],occart2[round][6]};
                        tk = psck(m,Rs,B2,e,round-count-1);
                        //if(tk==-1){printf("\nこれはコアではない");}
                        if(tk==0){ //左右も完璧に合ってた
                            //printf("\n正しい出現を%dの+%dに発見!\n",y8,occart2[round][3]+ofs);
                            findinround1(y8,round+1,occart2[round][3]+ofs);
                            //exit(1);
                        }
                    }
                }
                else{ //もっと上の階層に再帰するしかない
                    int e[3] = {occart2[round][4],occart2[round][5],occart2[round][6]};
                    int tk = psck(m,Rs,B2,e,round-count-1);
                    if(tk!=-1){
                        occart2[round][2] = y8;occart2[round][3] += ofs;
                        findups(m,startm,mrlen,round+1,B1,B2,B3,Ls,Rs,count+1);
                    }
                    //else{printf("\n[排除できる例(%d,%d)]",y8,y9-D_2[y8]);}
                }
            }
            if(round>0){ofs+=D3access(rofs+x[y9]);}else{ofs++;}
        }
    }
   if(DEBUGFLAG==1){printf("\nfindups--[%d]探し 終了--\n",occart2[round-1][2]);}
    return 1;
}

int findAB(int *m,int startm,int mrlen,int round,int *kmpc,int *B1,int *B2,int *B3,int *Ls,int *Rs){
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
    for(int y8=dc2s[round];y8<dc2s[round+1];y8++){ //変数y8
        std::vector<int> x;
        xdisp(x,y8);
        //printf("\n[%d(%d)]",y8,D_2[y8+1]-D_2[y8]);
        ofs=0;tg = 0;
        for(int y9=0;y9<x.size();y9++){
            //printf(" %d",rofs+D[y9]);
            if(rofs+x[y9]==m[startm+coredata[4]+tg]){ //coredata[4]だけずらす
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
            if(x.size()-tg>0){occart2[round][4] = rofs+x[tg];}else{occart2[round][4] = -1;}
            if(x.size()-tg>1){occart2[round][5] = rofs+x[tg+1];}else{occart2[round][5] = -1;}
            if(x.size()-tg>2){occart2[round][6] = rofs+x[tg+2];}else{occart2[round][6] = -1;}
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
            findups(m,startm,mrlen,round+1,B1,B2,B3,Ls,Rs,1); //その不完全なコアの出現を探す
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
           findups(m,startm,mrlen,round+1,B1,B2,B3,Ls,Rs,1); //その不完全なコアの出現を探す
        }
    }
    //printf("\nend of searching.(%d)[%d]",len,i);
    return 0;
}

int findB(int *m,int startm,int mrlen,int round,int *kmpc,int *B1,int *B2,int *B3,int *Ls,int *Rs){ //列からtgを探す
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
        std::vector<int> x;
        xdisp(x,y8);
        for(y9=0;y9<x.size();y9++){
            if(rofs+x[y9]==m[startm+tg]){
                tg++;if(tg==1){kpofs = ofs;} //1文字一致を確認したらオフセットくらいは覚えておく
                if(tg==coredata[5]){
                     if(DEBUGFLAG==1){printf("(Bコア発見[%d文字目(%d)] / %dの(+%d))",y9,tg,y8,ofs);}
                }
                if(tg==mrlen){ //CコアがなければB = Cの発見と同じ.コアはどうせ1つしかないのでそのままbreak
                    //printf("\n(コア発見[%d文字目(%d)] / %d(+%d))",y9-D_2[y8],tg,y8,ofs);
                    bsp[0] = y9; //コアの最後尾の位置
                    bsp[1] = y9+1 - mrlen; //手前に残ってる文字数
                    bsp[2] = x.size()-y9-1; //この先に残ってる文字数
                    break;
                }
            }else{ //不一致
                if(tg!=0){
                    if(y8==279){printf("[kmpc%d->%d]",tg,kmpc[tg]);}
                    if(round==0){kpofs+= (tg - kmpc[tg]);}
                    else{kpofs+=D3access(m[startm])*(tg - kmpc[tg]);}
                    tg = kmpc[tg];y9--;continue;}
                tg=0;
            }
            //printf(" %d",rofs+D[y9]);
            if(round>0){ofs+=D3access(rofs+x[y9]);}else{ofs++;}
        }
        //if(fis>0){printf("(左に右出現[%d文字])",fis);} //先頭から出現
        if(tg==mrlen){ //完全一致して抜けてきたやつ(A・Cコアが確実にない)
                coreans++;
                if(bsp[1]>0){occart2[round][1] = rofs+x[y9-mrlen];}else{occart2[round][1] = -1;}
                if(bsp[1]>1){occart2[round][0] = rofs+x[y9-mrlen-1];}else{occart2[round][0] = -1;}
                occart2[round][2] = y8;
                occart2[round][3] = kpofs-ocofs; //コアとのずれを最初に払っておく
                if(bsp[2]>0){occart2[round][4] = rofs+x[y9+1];}else{occart2[round][4] = -1;}
                if(bsp[2]>1){occart2[round][5] = rofs+x[y9+2];}else{occart2[round][5] = -1;}
                if(bsp[2]>2){occart2[round][6] = rofs+x[y9+3];}else{occart2[round][6] = -1;}
                occart2[round][7] = 3; //ABC全部発見 
                if(DEBUGFLAG==1){
                    printf("\nチェック：");
                    for(int i = 0;i<10;i++){printf("[%d]",occart2[round][i]);}
                }
                findups(m,startm,mrlen,round+1,B1,B2,B3,Ls,Rs,1); //その不完全なコアの出現を探す
        }
        else{
            if(tg==coredata[5]){ //最後にBコアだけ出現
                //printf("\n(右にBコア出現[%d文字] +%d)\n",tg,ofs);
                coreans++;
                if(x.size()>tg+1){occart2[round][0] = rofs+x[x.size()-2-tg];}else{occart2[round][0] = -1;}
                if(x.size()>tg){occart2[round][1] = rofs+x[x.size()-1-tg];}else{occart2[round][1] = -1;}
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
                findups(m,startm,mrlen,round+1,B1,B2,B3,Ls,Rs,1); //その不完全なコアの出現を探す
            }
        }
    }
    //printf("\nend of searching.(%d)[%d]",len,i);
    return 0;
}

int patternmatching1(int *m,int absm,int fround){
    int startm = 0;
    int mrlen = absm; 
    int round = 0;
    ocofs = 0;
    int *B1,*B2,*B3;
    B1 = (int*)calloc(fround+1,4);
    B2 = (int*)calloc(fround+1,4);
    B3 = (int*)calloc(fround+1,4);
    int Ls[fround+1],Rs[fround+1]; 
    for(int rs=0;rs<fround;rs++){
        int uy = ptrans(B1,B2,B3,Ls,Rs,startm,m,mrlen,round);
        if(uy<0){
            if(uy<-30){return -5;} //if pursing is not found in dictionary, end patternmatching
            break;
        }
        if(round==0){ocofs+=B1[round];}
        if(round>0){for(int i = 0;i<B1[round];i++){ocofs+=D3access(m[startm+i]);/*printf("\n[=%d]",m[startm+i]);*/}}
        startm += B1[round];mrlen = B3[round];round++;}
    if(round!=fround){    
        if(coredata[0]>0){ //Aコアがあるのでその分だけocofsをずらす
            for(int i = 0;i<coredata[2];i++){
                ocofs+=D3access(m[startm+i]);
            }
        }
    }
    transk = round;
    if(DEBUGFLAG==1){printf("rs==%d/%d\n",round,fround);}
    transreco[round]++;
    if(DEBUGFLAG==1){
    for(int i = 0;i<round;i++){
        for(int i2 = 0;i2<B1[i];i2++){
            printf(" %d",m[Ls[i]+i2]);
        }
        printf(" / ");
    }
    printf("(Core)");
    for(int i2 = 0;i2<mrlen;i2++){
            printf(" %d",m[startm+i2]);
    }
    printf(" / ");
    for(int i = round-1;i>=0;i--){
        for(int i2 = 0;i2<B2[i];i2++){
            printf(" %d",m[Rs[i]+i2]);
        }
        printf(" / ");
    }
    printf("\nlength of Core == %d\n",mrlen);
    }
    //fround[一番上]から指定の一文字を探して見る
    timessub = clock();
    int *kmpc = (int*)calloc(mrlen,4);
    mkkmpc(m,startm,mrlen,kmpc);
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
    if(transk==loops){
        findinNS(m,startm,mrlen,round,kmpc,B1,B2,B3,Ls,Rs);
        printf(" / ans :%d 個 (core : %d個)",ans,coreans);
        return 0;
    }
    if(coredata[0]==-1){
        if(DEBUGFLAG==1){printf("Bルートで検索");}
        findB(m,startm,mrlen,round,kmpc,B1,B2,B3,Ls,Rs);
    }
    else{
        if(DEBUGFLAG==1){printf("Aルートで検索");}
        findAB(m,startm,mrlen,round,kmpc,B1,B2,B3,Ls,Rs);
    }
    printf(" / ans :%d 個 (core : %d個)",ans,coreans);
    return 0;
}

int qrmloadset(int *m,int jo){ //c番目を読む
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


int queryload(int *m){
    char chr;
    FILE *fp;
    long long int qsize = std::filesystem::file_size(qname);
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
    }
    return (int)qsize;
}

int qrmload(int *m,int c,int jo,int *anst){ //mの中に，english.001.2 の中からc文字をランダムに選択して保存する
    char chr;
    FILE *fp;
    long long int qsize = std::filesystem::file_size(qname);
    fp = fopen(qname, "r"); // open file or return null
    if(fp == NULL) {
        printf("%s such file doesn't exist!\n", qname);
        return -1;
    }
    int i = 0;
    jswt = 0;
    int rlen = rand()%(qsize-c);
    if(runmode==2){rlen = qstart;}
    anst[jo] = rlen;
    fseek(fp,rlen,SEEK_SET);
    printf("\nrlen[%d]から[%d]文字(%lld)\n[",rlen,c,qsize);
    if(CHECKFLAG == 1){checkpz[0]=rlen;checkpz[1]=c;} //記憶
    for(i=0;i<c;i++) { //c文字
        chr = fgetc(fp);
        m[i] = chr;
        if(chr<0){m[i]=chr + 256;}
        if(m[i]==10){jswt=1;}
        //printf("\n%x(%d)",m[i],i);
        //if(DEBUGFLAG==1){printf("%c",m[i]);}
    }
    printf("]");
    return c;
}


int bitload(bool *ar,unsigned char a2,int p,int mx){
    int a = a2;
    //if(mote==1){printf("<%d>",a);}
    ar[p+0] = a/128;a-=ar[p+0]*128;
    //if(mote==1){printf("<%d>",a);}
    if(p+1<mx){ar[p+1] = a/64;a-=ar[p+1]*64;}else{return -1;}
    //if(mote==1){printf("<%d>",a);}
    if(p+2<mx){ar[p+2] = a/32;a-=ar[p+2]*32;}else{return -1;}
    //if(mote==1){printf("<%d>",a);}
    if(p+3<mx){ar[p+3] = a/16;a-=ar[p+3]*16;}else{return -1;}
    //if(mote==1){printf("<%d>",a);}
    //if(mx-5<=p && mx< 120000 && mx > 90000){printf(" / %d < %d(%d) / ",p+4,mx,ar[p+4]);exit(5);}
    if(p+4<mx){ar[p+4] = a/8;a-=ar[p+4]*8;}else{return -1;}
    //if(mote==1){printf("<%d>",a);}
    //if(mx-5<=p && mx< 120000 && mx > 90000){printf(" / %d < %d(%d) / ",p+4,mx,ar[p+4]);exit(5);} 
    if(p+5<mx){ar[p+5] = a/4;a-=ar[p+5]*4;}else{return -1;}
    //if(mote==1){printf("<%d>",a);}
    if(p+6<mx){ar[p+6] = a/2;a-=ar[p+6]*2;}else{return -1;}
    //if(mote==1){printf("<%d>",a);}
    if(p+7<mx){ar[p+7] = a/1;a-=ar[p+7]*1;}else{return -1;}
    //if(mote==1){printf("<%d>",a);}
    if(p+8>=mx){return -1;}
    return 0;
}

int qrmreload(int *m,int c){ //mの中に，english.001.2 の中からc文字をもう一度選択して保存する
    char chr;FILE *fp;
    long long int qsize = std::filesystem::file_size(qname);
    fp = fopen(qname, "r"); // open file or return null
    printf("\n[%lld]rlen[%d]から[%d]文字\n[",qsize,checkpz[0],checkpz[1]);
    fseek(fp,checkpz[0],SEEK_SET); //指定したシークまで移動する
    for(int i=0;i<checkpz[1];i++) { //c文字
        chr = fgetc(fp);
        m[i] = chr;
        //if(DEBUGFLAG==1){printf("%c",chr);}
        if(chr<0){m[i]=chr + 256;}
    }
    printf("]");
    return checkpz[1];
}

int shelp(){
    printf("--Simple Usage------------------\n");
    printf("\nRead 'codeindex' and search using <queryfile>. \n");
    printf(": ./gcis_uni_locate -q <queryfile>\n");
    printf("\nYou can set the indexfile by using option -i. \n");
    printf(": ./gcis_uni_locate -i <indexfile> -q <queryfile>\n");
    printf("\nYou can set the location mode 0 ~ 4 by using option -m. \n");
    printf("mode 3 is used by default.(defined 'MMODE' in this code.)\n");
    printf(": ./gcis_uni_locate -i <indexfile> -q <queryfile> -m <mode>\n");
    printf("\nYou can set the number of query by using option -r. \n");
    printf("10 is used by default.(defined 'MREP' in this code.)\n");
    printf(": ./gcis_uni_locate -i <indexfile> -q <queryfile> -m <mode> -r <times>\n");
    printf("\nYou can set the length of pattern by using option -l. \n");
    printf("10 is used by default.(defined 'MLONG' in this code.)\n");
    printf(": ./gcis_uni_locate -i <indexfile> -q <queryfile> -m <mode> -l <length>\n");
    printf("\n");
    printf("\n");
    return 0;
}

int main(int argc, char *argv[]){
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
        if('s'==argv[i][1]){ //mode
            qstart=0;
            for(int k = 0;k<strlen(argv[i+1]);k++){
                if(10 > argv[i+1][k]-'0' && argv[i+1][k]-'0'>=0){
                qstart *=10;
                qstart += argv[i+1][k] - '0';
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
    if(qstart==-1){qstart=MSTART;}
    FILE *fp;
    dcs[0] = 0;dc2s[0] = 0;ccs[0] = 0;cc2s[0] = 0; 
    unsigned char chr;
    unsigned int a;
    fp = fopen(inname, "r");
    int flag = 0; //0:loops,1:dcs,2:dc2s,3:D,4:D_2,5:D3,6:NS
    int w = 0;
    while(flag<11){ //18種類の配列を，その様式ごとにindexファイルから拾う
        fscanf(fp,"%c",&chr);a = chr;
        fscanf(fp,"%c",&chr);a += chr*256;
        fscanf(fp,"%c",&chr);a += chr*65536;
        fscanf(fp,"%c",&chr);a += chr*65536*256;
        if(flag==11 && w==0){
            //printf("\n[%d]:%u [%d,%d]",flag,a,dc2s[loops]-Sizelist[2],D_3[w]);
            //for(int yj=0;yj<loops;yj++){printf("(%d)",cc2s[yj]);}
            exit(1);
        }
        if(flag==10){NS[w]=a;w++;if(w==Sizelist[0]){flag++;w=0;}}
        if(flag==9){k3sub[w]=a;w++;if(w==Sizelist[3]){flag++;w=0;}}
        if(flag==8){d2topsub[w]=a;w++;if(w==Sizelist[5]){flag++;w=0;}}
        if(flag==7){D_3[w]=a;w++;if(w==dc2s[loops]-Sizelist[2]){flag++;w=0;}}
        if(flag==6){exceptr[w]=a;
            //printf("\nexceptr[%d] = %d",w,exceptr[w]);
            if(w==loops){
                flag++;w=0;
                if(dc2s[loops] == Sizelist[2]){flag++;} //D_3(4byte)がなかったら飛ばす
            }else{w++;}
        }
        if(flag==5){cc2s[w]=a;if(w==loops){flag++;w=0;}else{w++;}}
        if(flag==4){ccs[w]=a;if(w==loops){flag++;w=1;}else{w++;}}
        if(flag==3){dc2s[w]=a;
            //printf("\ndc2s[%d] = %d",w,a);
            if(w==loops){
                D_3 = (int*)malloc(4*(dc2s[loops] - Sizelist[2])); //D_3の配列サイズだけは後から決まる
                flag++;w=1;
            }
            else{w++;}
        }
        if(flag==2){dcs[w]=a;if(w==loops){flag++;w=1;}else{w++;}}
        if(flag==1){
            Sizelist[w]=a;
            if(w==10){
                flag++;w=1; //各種サイズを設定
                NS = (int*)malloc(4*Sizelist[0]);
                D_3s = (unsigned char*)malloc(Sizelist[1]);
                D_3m = (unsigned short int*)malloc(2*(Sizelist[2]-Sizelist[1]));
                d2topsub = (int*)malloc(4*Sizelist[5]);
                d2topsub2 = (unsigned short int*)malloc(2*Sizelist[6]);
                k3sub = (int*)malloc(4*Sizelist[3]);
                k3sub2 = (unsigned short int*)malloc(2*Sizelist[4]);
                k = (bool*)malloc(Sizelist[7]);
                k3 = (bool*)malloc(Sizelist[8]);
                d2under = (bool*)malloc(Sizelist[10]);
                d2top = (bool*)malloc(Sizelist[9]);
            }else{w++;}}
        if(flag==0){loops=a;flag++;}
    }
    //printf("\n-finishing 4byte part-\n");
    if(Sizelist[2]-Sizelist[1]==0){flag++;w=0;} //D_3mはないので，flagを一つ進める
    while(flag<14){ //2byteデータを読み込む
        fscanf(fp,"%c",&chr);a = chr;
        fscanf(fp,"%c",&chr);a += chr*256;
        //if(flag==13 && w == 1060){printf("//%d(%d)//",chr,Sizelist[4]);exit(3);}
        if(flag==13){k3sub2[w]=a;if(w+1==Sizelist[4]){flag++;w=0;}else{w++;}}
        if(flag==12){d2topsub2[w]=a;if(w+1==Sizelist[6]){flag++;w=0;}else{w++;}}
        if(flag==11){D_3m[w]=a;if(w+1==Sizelist[2]-Sizelist[1]){flag++;w=0;}else{w++;}}
    }
    d2topx = (unsigned char*)malloc(Sizelist[9]/8 + 1);
    k3x = (unsigned char*)malloc(Sizelist[8]/8 + 1);
    //printf("\n-finishing 2byte part-\n");
    while(flag<19){ //1byte式のデータ・bit列を読み込む
        fscanf(fp,"%c",&chr);a = chr;
        if(flag==16 && w>=0 && w<300){mote=1;}else{mote=0;}
        if(flag==18){
            d2topx[w/8]=a;
            if(bitload(d2top,a,w,Sizelist[9])==-1){flag++;w=0;}
            else{w+=8;}}
        if(flag==17){if(bitload(d2under,a,w,Sizelist[10])==-1){flag++;w=0;}else{w+=8;}}
        if(flag==16){
            k3x[w/8]=a;
            if(bitload(k3,a,w,Sizelist[8])==-1){flag++;w=0;}
            else{w+=8;}
        }
        if(flag==15){if(bitload(k,a,w,Sizelist[7])==-1){flag++;w=0;}else{w+=8;}}
        if(flag==14){D_3s[w]=a;if(w+1==Sizelist[1]){flag++;w=0;}else{w++;}}
    }
    fclose(fp);
    cc2s[loops]=Sizelist[8];ccs[loops]=Sizelist[8]-dc2s[loops];
    for(int i=0;i<256;i++){
        int ju = 128;
        int je = 0;
        for(int j=0;j<8;j++){
            if(i%(ju*2)>=ju){je++;}
            rankct[i][j] = je;
            ju = ju/2;
        }
    } //rankctの定義
    int popo = 5;
    int ujk = ccs[popo]; //ccs[popo+1]-ccs[popo]+2 ccs[popo+1]-ccs[popo]+2
    
    int ancl[querysuu][5];
    int anst[querysuu];
    double ancd[querysuu];
    int *m;
    int msize = 10000;
    m = (int*)malloc(4*msize);
    msize = queryload(m);
    double avt = 0;
    int jogaipar = 0;
    if(runmode==2){
        msize = qrmload(m,plength,0,anst);
        clock_t timesb = clock();
        patternmatching1(m,msize,loops);
        clock_t timee = clock();
        const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC * 1000.0;
        printf("\n[length of the pattern : %d] / ans:%d / core:%d : time %lf[ms]\n",msize,ans,coreans,time);
        printf("\nvain = %d",vain);
        if(CHECKFLAG==1){
            msize = qrmload(m,plength,0,anst);
            anscheck(msize,m);
        }
    }
    if(runmode==1 || runmode==3){
        double pertime = 0.00;
        if(runmode==3){srand((unsigned int)time(NULL));}
        for(int jo = 0;jo < querysuu;jo++){
            int ralen = plength;
            if(runmode==1){ralen = plength;}
            msize = qrmload(m,ralen,jo,anst);
            clock_t timesb = clock();
            patternmatching1(m,msize,loops);
            clock_t timee = clock();
            const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC * 1000.0;
            const double time2 = static_cast<double>(timessub - timesb) / CLOCKS_PER_SEC * 1000.0;
            printf("\n[length of the pattern : %d] / (%d) : time %lf[ms]\n",msize,ans,time);
            printf("\nvain = %d",vain);
            ancl[jo][0] = ans;
            ancl[jo][1] = coreans;coreans=0;
            ancl[jo][2] = ralen;
            ancl[jo][3] = transk;
            ancd[jo] = time;
            avt+=time;
            pertime += (time2*100)/time;
            if(CHECKFLAG==1){
                msize = qrmreload(m,ralen);
                anscheck(msize,m);
            }
            ans=0;
        }
        printf("avarage time : %lf[ms]",avt/querysuu);
        for(int jo=0;jo<querysuu;jo++){
            printf("\n(%d : P-length : %d from position %d[%d trans] / ans:%d / core:%d / time %lf[ms])",jo,ancl[jo][2],anst[jo],ancl[jo][3],ancl[jo][0],ancl[jo][1],ancd[jo]);
            }
        printf("\n[transformation time : %.3f %]\n",pertime/querysuu);
    }
    if(runmode==4){
        for(int jo = 0;jo < querysuu;jo++){
            msize = qrmloadset(m,jo);
            //if(jswt==1){printf("\n除外");jogaipar++;jo--;continue;}
            if(DEBUGFLAG==1){
                printf("\n検索文字列:[");
                for(int rek = 0;rek<msize;rek++){printf("%c",m[rek]);}
                printf("]\n");
            }
            clock_t timesb = clock();
            patternmatching1(m,msize,loops);
            clock_t timee = clock();
            const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC * 1000.0;
            printf("\n[length of the pattern : %d] / (ans:%d / core:%d ) : time %lf[ms]\n",msize,ans,coreans,time);
            ancl[jo][0] = ans;ans=0;
            ancl[jo][1] = coreans;coreans=0;
            ancl[jo][2] = msize;
            ancl[jo][3] = transk;
            ancd[jo] = time;
            avt += time;
        }
        printf("avarage time : %lf[ms]",avt/querysuu);
        for(int jo=0;jo<querysuu;jo++){
            printf("\n(P-length : %d from position %d[%d trans] / ans:%d / core:%d / time %lf[ms])",ancl[jo][2],anst[jo],ancl[jo][3],ancl[jo][0],ancl[jo][1],ancd[jo]);
            }
    }
    if(runmode==0){
        msize = queryload(m);
        clock_t timesb = clock();
        patternmatching1(m,msize,loops);
        clock_t timee = clock();
        const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC * 1000.0;
        printf("\n[length of the pattern : %d] / (%d) : time %lf[ms]\n",msize,ans,time);
        printf("\nvain = %d",vain);
    }
    return 0;
}