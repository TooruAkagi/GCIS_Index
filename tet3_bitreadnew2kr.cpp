#include<iostream>
#include<stdlib.h>
#include<string> 
#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream
#include <math.h>
#include <filesystem>

#define MMODE 3
#define MLONG 1146
#define MREP 3
#define MSTART 8402247
#define MRANDRANGE 1
#define MRANDBASE 1000
#define CHECKFLAG 0 //これが1の場合は，結果が全部本当に正しいかをファイルをひらきながら全部チェックする
#define DEBUGFLAG 0 //1ならデバッグしながら検索する
//MMODE = 
// 0 : query.txtの中の文字列を照合
// 1 : qnameで指定されているファイルの中からランダムに MLONG 文字の文字列を選択し，
//     MREP 回実行する．
// 2 : 指定されているファイルの中のMSTARTからMLONG文字の文字列を選択肢，照合する

char qname[] = "../datalist/english.001.2";
int ancl[MREP][5];
double ancd[MREP];
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
int core = 0;
int hh = 0;
int lpurse = 65536; //定数
int spurse = 64; //定数
int spursesuu = lpurse / spurse; //定数
unsigned int *NS;
unsigned short int *d2topsub2,*k3sub2,*D_3m;
int *d2topsub,*k3sub,*D_3;
//int ksize = 200000000;int k3size = 4000000; //配列暫定サイズの保管
int NSsize,D3smax,D3mmax,k3submax,k3sub2max,d2topsubmax,d2topsub2max,d2topmax,d2undermax; //配列サイズの保管
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
    if(DEBUGFLAG==1){printf("kad %d ->",kad);}
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
    if(DEBUGFLAG==1){printf("%d\n",kad);}
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
    //printf("\n[%d]番目の1を探す(%d)",c,u2[0]);
    int i = 0;
    int j = 0;
    int l = 0;
    int hmax = t2-1;int hmin = 0;
    if(u2[0]<c){
        while(hmax>hmin+1){
            i = (hmax+hmin)/2;
            if(u2[i]>=c){hmax = i-1;}
            else{hmin = i;}
        }
        if(u2[hmax]>=c){i = hmin;hmax = hmin-1;}
        else{i=hmax;hmin = hmax;}
        l = u2[i];
        i++;
    }
    int e = l;l=0;
    int ofi = i*spursesuu;
    j=0; //2回目のloglog探索
    hmin = 0;hmax = spursesuu-1;
    if(t2-1 == i){hmax = t3-1;}
    while(hmax>hmin+1){
        j = (hmax+hmin)/2;
        if(u3[ofi+j]>=c-e){hmax = j-1;}
        else{hmin = j;}
    }
    if(u3[ofi+hmax]>=c-e){j = hmin;}else{j = hmax;}
    if(j==spursesuu){j=spursesuu-1;}
    l = u3[ofi+j];
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
        if(c==723466){printf("\n[%d-%d-%d]",u[p/8],(8-rankct[u[p/8]][7]),p);}
        if(e+(8-rankct[u[p/8]][7])<c){
            e+=8-rankct[u[p/8]][7];p+=7;
        }
        else{
            for(int ji=0;ji<8;ji++){
                if(c==723466){
                    printf("\n後%d:LAST[%d-%d-%d]",c-e,u[p/8],(1+ji-rankct[u[p/8]][ji]),p);
                }
                if(e+(1+ji-rankct[u[p/8]][ji])==c){return p+ji;}
            }
        }
    }
    return n+1;
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
        //printf("\n%d:",tgm);
        int D2start = getd2(tgm);
        int D2end = getd2(tgm+1);
        int tgm3 = gethead(tgm);
        psread(tgm,k,D2start,D2end,tgm3);
}

int rpsck(int *m,int starti,int mlen,int round,int *e2){
    int t = 1;
    //int eset = 0; //リバースでは使わない?
    int i = 0;
    int c = 0;
    int rep = 1; //mの該当範囲が1つの文字だけで表現されるとき，1
    int rofs=0;
    int rlist[1000];
    int rlistptr = 0;
    if(round>1){rofs=dc2s[round-1];}
    //見る範囲が1つの文字で表されない時，一度c文字一致した後もう一度c文字一致することは絶対にない
    //1つの文字だけで表現されるとき，c+1文字一致しているならc文字一致していることになるし
    //この時e2の2文字は左にシフトし，新たに追加される1文字はc+1文字目と同じである.
    for(int yj = 0;yj < mlen-1;yj++){if(m[starti+yj]!=m[starti+yj+1]){rep=0;}}
    //printf("\n<rofs[%d] = %d>(%d)",round,rofs,mlen);
    while(t>=0){
        int l = e2[t]+dc2s[round];  
        //printf("\n[%d+%d=%d]を見る",e2[t],dc2s[round],l);
        int su = getd2(l);
        int eu = getd2(l+1);
        //printf("\nsu,eu =(%d,%d)",su,eu);
        //uhdisp(l,d2under,d2top,d2topsub,d2topsub2,k,k3,k3sub,k3sub2,36000000);
        //for(int h = 0;h<mlen;h++){printf("(%d)",m[starti+h]-rofs);}
        int basekey = gethead(l)+rofs;
        rlist[rlistptr] = basekey; rlistptr++;
        //for(int h = 0;h<rlistptr;h++){printf("[-%d-]",rlist[h]);}
        //printf("\nnew key : %d(%d)",basekey,basekey-rofs);
        int a = gread(su,k);
        su += 2*loga(a)-1;
        if(l!=exceptr[round]){
            if(a==1){a=99999999;}
        }
        while(1){
            if(su>=eu){break;}
            int yp = gread(su,k)-1;a--; //ypは差分・これをbasekeyから引いて値を求める．aが1以外なら足すのではなく引く
            if(a>0){basekey+=yp;}else{basekey-=yp;}
            rlist[rlistptr] = basekey; rlistptr++;
            //printf("\n");
            //for(int h = 0;h<rlistptr;h++){printf("[-%d-]",rlist[h]);}
            //printf("\nnew key : %d(%d)",basekey,basekey-rofs);
            su += 2*loga(yp+1)-1;
        }
        t--;
    }
    c = 0;
    for(int i = 0;i<mlen;i++){
        if(rlist[rlistptr-1-i] != m[starti + mlen - 1 -i]){
            //printf("\n[%d]%d != %d",c,rlist[rlistptr-1-i],m[starti + mlen - 1 -i]);
            //printf("\n%d文字一致",c);
            return 0;
        }
        else{
            c++;
            //printf("\n[%d]%d == %d",c,rlist[rlistptr-1-i],m[starti + mlen - 1 -i]);
        }
    }
    //printf("\nc == %d",c);
    if(c==mlen){
            //printf("round%dは完全一致",round);
            for(i=0;i<2;i++){e2[i]=rlist[rlistptr-mlen-1-i]-rofs;}
            //for(int rh=0;rh<2;rh++){printf("\n<e2[%d] = %d(-%d)>",rh,e2[rh],rofs);}
            return 1;
    }
    printf("\n異常終了[289]");exit(1);
    return 0;
}

int psck(int *m,int starti,int mlen,int round,int *e){
    int t = 0;
    int eset = 0;
    int earray[3];
    int i = 0;
    int c = 0; //合ってる文字数
    int rofs=0;
    if(round>1){rofs=dc2s[round-1];}
    while(t<3){
        if(e[t]==-1){return 0;}
        int l = e[t]+dc2s[round];
        //printf("\n[%d+%d=%d]を見る",e[t],dc2s[round],l);
        int su = getd2(l);
        int eu = getd2(l+1);
        //printf("\nsu,eu =(%d,%d)",su,eu);
        uhdisp(l);
        int basekey = gethead(l)+rofs;
        //printf("[got %d]",basekey);
        if(c<mlen){
            if(basekey==m[starti+c]){c++;}
            else{
                //printf("ここで失敗[%d != %d]",basekey,m[starti+c]);
                return 0;}
        }
        else{
            //printf("\ne[%d]=%d",eset,basekey);
            earray[eset] = basekey;eset++;
        }
        int a = gread(su,k);
        //printf("/%d/",2*loga(a)-1);
        su += 2*loga(a)-1;
        if(a==1){a=99999999;}
        while(eset<3){  
            if(su>=eu){break;}
            int yp = gread(su,k)-1;a--;
            if(a>0){basekey+=yp;}else{basekey-=yp;}
            su += 2*loga(yp+1)-1;
            if(c<mlen){
                //printf("[got %d = %d]",basekey,m[starti+c]);
                if(basekey==m[starti+c]){c++;}
                else{c=0;return 0;}
            }
            else{
                //printf("\ne[%d]=%d を追加",eset,basekey);
                earray[eset] = basekey;eset++;
            }
        }
        if(eset==3){
            //printf("完璧です");
            for(i=0;i<3;i++){e[i]=earray[i]-rofs;}
            return 1;
        }
        //printf("\n%d文字一致 : %d文字e[]追加",c,eset);
        t++;
    }
    return 0;
}

int roundck(int *m,int round,int *Rs,int *Ls,int *B1,int *B2,int *e,int *e2){
    //printf("\n<round %d>",round);
    int g = psck(m,Rs[round],B2[round],round,e);
    if(g==0){
        if(DEBUGFLAG==1){printf("\npsckでミスマッチ");}
        return 0;} //mismatch
    g = rpsck(m,Ls[round],B1[round],round,e2);
    if(g==0){
        if(DEBUGFLAG==1){printf("\nrpsckでミスマッチ");}
        return 0;}//mismatch
    return 1;
}

int look(int *posi,int *nem,int *rev,int *leg,int *rc,int round,int t){
    //printf("\nlook(%d,%d,%d,%d)",nem[round],rev[round],round,t);
    //if(rand()%10000000==0){exit(3);}
    if(loops<=round){hh=0;return -1;}
    int i = nem[round]+dc2s[round];
    if(t==0){hh--;return i;}
    //printf("\n[i]%d =%d + %d /%d",i,nem[round],dc2s[round],round);
    if(posi[round]<0 && nem[round]>=0){
        posi[round] = getd2(i);
        rev[round] = getd2(i+1);
        rc[round] = gethead(i);
        leg[round] = gread(posi[round],k);posi[round]+=loga(leg[round])*2-1;
        if(i!=exceptr[round]){
            if(leg[round]==1){leg[round]=99999999;}
        }
        if(t==1){
            rc[round]+=dc2s[round-1];
            return rc[round];
        }
        nem[round-1] = rc[round];
        hh++;
        return look(posi,nem,rev,leg,rc,round-1,t-1);        
    }
    if(posi[round]<rev[round]){
        int y = gread(posi[round],k)-1;
        if(leg[round]<=1){rc[round]-=y;}else{rc[round] += y;}
        posi[round]+=loga(y+1)*2-1;
        leg[round]--;
        if(t==1){
            return rc[round];
        }
        nem[round-1] = rc[round];
        hh++;
        return look(posi,nem,rev,leg,rc,round-1,t-1);        
    }
    else{
        nem[round]=-1;posi[round]=-1;hh--;
        return look(posi,nem,rev,leg,rc,round+1,t+1);
    }
}

int ckcore(int *e3,int *m,int starti,int mlen,int *kmpc,int msta,int upc){
    //u3[msta]から判定を行う
    //compare (1)from [starti], length[mlen] in m / (2) e3,from position 2.
    //e3[0][1] == for checking leftside, e3[mlen+2][mlen+3][mlen+4] == for rightside
    for(int i = imcokep2;i<mlen;i++){
        //printf("/%d==%d",m[starti+i],e3[i+2]);
        if(m[starti+i]!=e3[(msta+i+2)%upc]){ //i文字目で不一致
            if(i>1){ //2文字以上一致してないと意味ない
                if(kmpc[i]!=0){ //borderがある
                    imcokep = i-kmpc[i]; //i-border文字飛ばせる
                    imcokep2 = kmpc[i]; //現在の一致文字数
                }
                else{ //開始文字はこの先にある
                    imcokep = i; //i文字飛ばせる
                    imcokep2 = 0; //現在の一致文字数
                }
            }
            else{imcokep=0;imcokep2=0;}
            //if(i>18){
            //    printf("\n%d文字目で不一致がおきたので%d文字既に一致してるとして%d文字飛ばせる",i,imcokep2,imcokep);
            //}
            return 0;
        }
    }
    if(kmpc[mlen-1]!=0){ //borderがある
                    imcokep = mlen-1-kmpc[mlen-1]; //i-border文字飛ばせる
                    imcokep2 = kmpc[mlen-1]; //現在の一致文字数
                }
                else{ //開始文字はこの先にある
                    imcokep = mlen-1; //i文字飛ばせる
                    imcokep2 = 0; //現在の一致文字数
    }
    //imcokep = kmpc[mlen-1];imcokep2= (mlen-1)-kmpc[mlen-1]; //
    //printf("Core is found. ");
    //printf("\n%d文字目まで一致したので%d文字既に一致してるとして%d文字飛ばせる",mlen,imcokep2,imcokep);
    //exit(5);
    //if(rand()%100000==0){exit(9);}
    return 1;
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
    int rofs = ccs[round];
    if(DEBUGFLAG==1){
        printf("\n完全一致を検索 : rofs = [%d](%d) / ",ccs[round],round);}
    if(round>=2){rofs-=dc2s[round-1];}
    if(DEBUGFLAG==1){
        printf("\n検索する分割: ");
        for(int y7=0;y7<plen;y7++){printf("%d ",m[start+y7]);}
        printf("\n");
    }
    int y = cmlen(m,start,plen,bitlet);
    if(DEBUGFLAG==1){
        printf("\nbit列: ");
        for(int y7=0;y7<20;y7++){printf("%d",bitlet[y7]);}}
    int a,b,c,d;
    int posup = selectnew1(k3x,k3sub,k3sub2,m[start]+rofs,Sizelist[8],Sizelist[3],Sizelist[4]);
    int posbt = selectnew1(k3x,k3sub,k3sub2,m[start]+1+rofs,Sizelist[8],Sizelist[3],Sizelist[4]);
    posup = sranknew0(posup,k3x,k3sub,k3sub2,Sizelist[3],Sizelist[4])-1;
    posbt = sranknew0(posbt,k3x,k3sub,k3sub2,Sizelist[3],Sizelist[4]);
    if(posup < dc2s[round]-1){posup = dc2s[round]-1;}
    if(posbt > dc2s[round+1]){posbt = dc2s[round+1];}
    int pos = 0;
    //printf("(%d 〜 %d)%d - %d\n",posup,posbt,plen,start);
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
    if(pos==-1){exit(6);}
    if(DEBUGFLAG==1){printf(" got <%d>",pos);}
    return pos;
}

int ptrans(int *B1,int *B2,int *B3,int *Ls,int *Rs,int startm,int *m,int mrlen,int round){
    int *Rp;int flag = 0;Rp = (int*)malloc(mrlen*4+4);
    int tn = -1;int Rpptr = 0;int len = 0;
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
            tn=1;
        }
        Rpptr++;
    }
    len++;
    Rp[Rpptr] = m[startm];
    for(int i = 0;i<=Rpptr;i++){ //reverse Rp >> m
        m[startm+i] = Rp[Rpptr-i];
    }

    if(B2[round]==0){printf("(in round%d) input string can't be pursed anymore．",round);return -2;}
    if(B3[round]==0){printf("(in round%d) input string desn't have core",round);return -1;}
    
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

int patternmatching1(int *m,int absm,int fround){
    int ocofs = 0; //the difference of Core occurance and pattern occurance
    int startm = 0;
    int mrlen = absm; 
    int round = 0;
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
        startm += B1[round]; 
        mrlen = B3[round]; //mrlen stores the length of core (written in the function;)
        //printf("]\n startm = %d,mrlen = %d",startm,mrlen);
        round++;
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

    int len = 0;
    int *e,*e2,*e3,*posi,*nem,*rev,*leg,*rc;
    int rofs=0;
    posi = (int*)calloc(fround+1,4); //今の開始位置は何bitなのか
    nem = (int*)calloc(fround+1,4); //各層で，今なんの文字なのか
    rev = (int*)calloc(fround+1,4); //あと何bitなのか
    leg = (int*)calloc(fround+1,4); //あと何文字加算のムキなのか
    rc = (int*)calloc(fround+1,4);
    for(int i = 0;i<fround+1;i++){posi[i]=-1;nem[i]=-1;}
    e = (int*)malloc(3*4);
    e2 = (int*)malloc(2*4);
    int upc = mrlen+6;
    int *kmpc = (int*)calloc(mrlen,4);
    mkkmpc(m,startm,mrlen,kmpc);
    e3 = (int*)malloc((upc)*4);
    for(int i=0;i<upc;i++){e3[i]=-1;}
    if(round>1){rofs=dc2s[fround-1];}
    int i = 0;
    i=0;int mk = 0; //mk文字
    int cflag = 4;
    //int fi= 0;
    int msta = 0;
    int mend = 2; //e3[msta -- (mend-1)]までもじが入っている
    //exit(8);
    while(1){ 
        if(i<Sizelist[0]){
            nem[fround-1] = NS[i];
            e3[mend] = look(posi,nem,rev,leg,rc,fround-1-hh,fround-round-hh);
            if(e3[mend]==-1){i++;continue;} //挿入できなかったらiを進めてもう一度
            else{
                mend++;
                //printf("<%d>",mend);
                if(mend>=upc){
                    //printf("reset[%d,%d]",mend,upc);
                    mend=0;}
            } //入れたらmendをひとつ進める
        }
        else{
            e3[mend] = -1;cflag--; //cflag回は-1を入れて誤魔化す
            mend++;if(mend>=upc){mend=0;}
            if(cflag<1){break;}
        }
        if(e3[(mend+1)%upc]>=0){
            if(round>0){
                len+= D3access(e3[(mend+1)%upc]);
                if(DEBUGFLAG==1){
                    if(len>=10400000 && len<10405000){
                        printf("\n(%d,%d):通過[",msta,mend);
                        for(int jko=0;jko<upc;jko++){
                            printf("%d ",e3[jko]);
                        }
                        printf("]%d : [%d](+%d)",e3[(mend+1)%upc],len,D3access(e3[(mend+1)%upc]));
                    }
                }
            }
            else{len++;}
        }
        if(msta==mend){//(upc+mend-msta)%upcがmrlen+3を超えていれば
            //-1 -1 [mrlen] * * * の形であり，早速検証ができる．
            //手前の-1は直前に調べた物がまだ残っており，最初は-1から始められる
            mk = ckcore(e3,m,startm,mrlen,kmpc,msta,upc); //msta+2をスタートとしCore照合を行う
            if(mk==1){
                core++;
                e[0]=e3[(msta+mrlen+2)%upc]-dc2s[round-1];e[1]=e3[(msta+mrlen+3)%upc]-dc2s[round-1];e[2]=e3[(msta+mrlen+4)%upc]-dc2s[round-1];
                e2[0]=e3[(msta+1)%upc]-dc2s[round-1];e2[1]=e3[msta]-dc2s[round-1];
                if(DEBUGFLAG==1){
                    printf("\nCore:occurs in position %d(-%d) (round%d) ",len,ocofs,round);
                    printf("\n[%d][%d] - Core - [%d,%d,%d]\n",e2[1]+dc2s[round-1],e2[0]+dc2s[round-1],e[0]+dc2s[round-1],e[1]+dc2s[round-1],e[2]+dc2s[round-1]);
                }
                if(round==0){
                    if(ans<1000 && CHECKFLAG==1){checkans[ans]=len-ocofs;}
                    ans++;printf(" %d ",len-ocofs);
                }
                int tr = round-1;
                int g = 0;
                while(tr>=0){
                    g = roundck(m,tr,Rs,Ls,B1,B2,e,e2);
                    if(g==0){/*printf("\nCore but [%d] is not an occurance!",len-ocofs);*/break;}
                    if(tr==0){
                        if(ans<1000 && CHECKFLAG==1){checkans[ans]=len-ocofs;}
                        printf(" %d",len-ocofs);
                        ans++;break;}
                    tr--;
                }
            }
            msta+=imcokep;
            if(imcokep==0){msta++;} //特に指定がなく0文字飛ばす場合ても1文字は飛ばす
            if(msta>=upc){msta-=upc;} //飛ばしていい文字数だけmstaを進める
        }
    }
    printf("\nend of searching.(%d)[%d,%d]",len,i,Sizelist[0]);
    printf(" / ans :%d 個 ",ans);
    return 0;
}


int queryload(int *m){
    char qname[] = "query.txt";
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

int qrmload(int *m,int c,int jo){ //mの中に，english.001.2 の中からc文字をランダムに選択して保存する
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
    if(MMODE==2){rlen = MSTART;}
    ancl[jo][4] = rlen;
    fseek(fp,rlen,SEEK_SET);
    printf("\nrlen[%d]から[%d]文字(%lld)\n[",rlen,c,qsize);
    if(CHECKFLAG == 1){checkpz[0]=rlen;checkpz[1]=c;} //記憶
    for(i=0;i<c;i++) { //c文字
        chr = fgetc(fp);
        m[i] = chr;
        if(chr<0){m[i]=chr + 256;}
        if(m[i]==10){jswt=1;}
        //printf("\n%x(%d)",m[i],i);
        if(DEBUGFLAG==1){printf("%c",m[i]);}
    }
    printf("]");
    return c;
}


int bitload(bool *ar,unsigned char a2,int p,int mx){
    int a = a2;
    //位置pから8個分bit配列arを埋めるが，mxを超えたら-1を返して帰還する．
    //if(mx-100<p){
    //    printf("\nbitload(%d,%d,%d,%d)",a,p,mx,Sizelist[9]);
    //}
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
        if(DEBUGFLAG==1){printf("%c",chr);}
        if(chr<0){m[i]=chr + 256;}
    }
    printf("]");
    return checkpz[1];
}

int main(){
    FILE *fp;
    char wname[] = "codeindex"; //index file name
    //unsigned int *D;
    //int *D_2;
    //int i = 0;int pn = 0;int round = 0;
    dcs[0] = 0;dc2s[0] = 0;ccs[0] = 0;cc2s[0] = 0; 
    unsigned char chr;
    unsigned int a;
    fp = fopen(wname, "r");
    int flag = 0; //0:loops,1:dcs,2:dc2s,3:D,4:D_2,5:D3,6:NS
    int w = 0;
    while(flag<11){ //18種類の配列を，その様式ごとにindexファイルから拾う
        fscanf(fp,"%c",&chr);a = chr;
        fscanf(fp,"%c",&chr);a += chr*256;
        fscanf(fp,"%c",&chr);a += chr*65536;
        fscanf(fp,"%c",&chr);a += chr*65536*256;
        if(flag==11 && w==0){
            //printf("\n[%d]:%u [%d,%d]",flag,a,dc2s[loops]-Sizelist[2],D_3[w]);
            for(int yj=0;yj<loops;yj++){printf("(%d)",cc2s[yj]);}
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
            Sizelist[w]=a; //printf("\nSizelist[%d] = %d",w,a);
            if(w==10){
                flag++;w=1; //各種サイズを設定
                NS = (unsigned int*)malloc(4*Sizelist[0]);
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
        //fseek(fp,16,SEEK_CUR);

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
    for(int i=0;i<256;i++){
        int ju = 128;
        int je = 0;
        for(int j=0;j<8;j++){
            if(i%(ju*2)>=ju){je++;}
            rankct[i][j] = je;
            ju = ju/2;
        }
    } //rankctの定義
    //exit(1);
    //printf("\nNS = %d,%d,%d ...",NS[0],NS[1],NS[2]);
    ////同じ配列が得られているかのチェック
    //for(int y8 = 0;y8<30;y8++){uhdisp(y8);}
    int *m;
    int msize = 10000;
    m = (int*)malloc(4*msize);
    msize = queryload(m);
    double avt = 0;
    int jogaipar = 0;
    if(MMODE==2){
        msize = qrmload(m,MLONG,0);
        clock_t timesb = clock();
        patternmatching1(m,msize,loops);
        clock_t timee = clock();
        const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC * 1000.0;
        printf("\n[length of the pattern : %d] / ans:%d / core:%d : time %lf[ms]\n",msize,ans,core,time);
        printf("\nvain = %d",vain);
        if(CHECKFLAG==1){
            msize = qrmload(m,MLONG,0);
            anscheck(msize,m);
        }
    }
    if(MMODE==1 || MMODE==3){
        if(MMODE==3){srand((unsigned int)time(NULL));}
        for(int jo = 0;jo < MREP;jo++){
            int ralen = MRANDBASE+rand()%MRANDRANGE;
            if(MMODE==1){ralen = MLONG;}
            msize = qrmload(m,ralen,jo);
            clock_t timesb = clock();
            patternmatching1(m,msize,loops);
            clock_t timee = clock();
            const double time = static_cast<double>(timee - timesb) / CLOCKS_PER_SEC * 1000.0;
            printf("\n[length of the pattern : %d] / (%d) : time %lf[ms]\n",msize,ans,time);
            printf("\nvain = %d",vain);
            ancl[jo][0] = ans;
            ancl[jo][1] = core;core=0;
            ancl[jo][2] = ralen;
            ancl[jo][3] = transk;
            ancd[jo] = time;
            avt+=time;
            if(CHECKFLAG==1){
                msize = qrmreload(m,ralen);
                anscheck(msize,m);
            }
            ans=0;
        }
        printf("avarage time : %lf[ms]",avt/MREP);
        printf("除外数 %d / %d",jogaipar,jogaipar+MREP);
        for(int jo=0;jo<MREP;jo++){
            printf("\n(P-length : %d from position %d[%d trans] / ans:%d / core:%d / time %lf[ms])",ancl[jo][2],ancl[jo][4],ancl[jo][3],ancl[jo][0],ancl[jo][1],ancd[jo]);
            //printf("\n%d\n%d\n%d\n%d\n%d\n%lf",ancl[jo][2],ancl[jo][4],ancl[jo][3],ancl[jo][0],ancl[jo][1],ancd[jo]);
        }
    }
    if(MMODE==0){
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