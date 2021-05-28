#include<iostream>
#include<stdlib.h>
#include<string> 
#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream
#include <math.h>
#include <filesystem>

int vain=0;
int cheatlen[11] = {512,256,128,64,32,16,8,4,2,1,0};
int kad = 0;
int kad2 = 0;
int dcs[18]; //[i] = starting position of floor-i's first purse in the Dictionary
int dc2s[18]; //[i] = starting number of floor-i's first number in the Dictionar
int ccs[18]; //[i] = starting position of floor-i's first purse in the Dictionary
int cc2s[18]; //[i] = starting number of floor-i's first number in the Dictionary
int exceptr[18];
int loops =6; //the number of applying GCIS to the input file
int ans = 0;
int hh = 0;
int lpurse = 65536; //定数
int spurse = 64; //定数
int spursesuu = lpurse / spurse; //定数
unsigned int *NS;
unsigned short int *d2topsub2,*k3sub2,*D_3m;
int *d2topsub,*k3sub,*D_3;
//int NSsize,D3smax,D3mmax,k3submax,k3sub2max,d2topsubmax,d2topsub2max,d2topmax,d2undermax; //配列サイズの保管
int Sizelist[11] = {0,0,0,0,0,0,0,1980000000,80000000,0,0};
unsigned char *D_3s;
bool *k,*k3,*d2under,*d2top;

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

int bwrite(bool *u,int s,int l,int a){ //bool列のsからl文字分に，数字aを追加．
    //数字が小さすぎるときは0で埋める．
    int c = s+l-loga(a);
    int e=a;
    for(int i=s+l-1;i>=s;i--){
        if(i<c){u[i]=0;continue;}
        if(e%2==0){u[i]=0;}else{u[i]=1;e--;}
        e/=2;
    }
    return 0;
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

void blogg(int p,unsigned int *D,int *D_2,bool *ar){
    for(int i = D_2[p]+1;i<D_2[p+1];i++){logg(D[i]+1,ar);}
}

void diff(unsigned int *T,int s,int n){ //配列Tのsからn個(先頭以外)を差分保存に変換する．sは0でない．
    for(int i=n-2;i>=0;i--){
        if(T[s+i+1]>=T[s+i]){T[s+i+1] = T[s+i+1] - T[s+i];}
        else{T[s+i+1] = T[s+i] - T[s+i+1];}
    }
    return;
}

int ppow(int n){ //2のn乗を返す
    if(n<1){return 1;}
    return 2*ppow(n-1); 
}

int cd1(int x,bool *ar){ //xの一進数符号化
    if(x==0){ar[kad2]=0;kad2++;return 0;}
    ar[kad2]=1;kad2++;
    return cd1(x-1,ar);
}

int fano(int *T,int s,int n,int p,bool *ar,bool *ar2){ //配列Tのsからn個をElias fanoコーディングする．
    kad = 0;
    kad2 = 0;
    int topofs = 0;
    int t = ppow(p);
    printf("\np=%d / t=%d",p,t);
    for(int i=s;i<n;i++){
        cd1(T[i]/t-topofs,ar2);
        topofs = T[i]/t;
        logb(T[i]%t,p,ar);
    }
    printf("fanoコーディング 下位ビット = %d[%d bytes] / 上位ビット = %d[%d bytes]",kad,kad/8,kad2,kad2/8);
    Sizelist[9] = kad2;
    Sizelist[10] = kad;
    return kad/8 + kad2/8;
}

void fano0(unsigned int *D,int *D_2,bool *ar){
    kad = 0;
    ccs[kad]=0;cc2s[kad]=0;kad++;
    int r = 0;
    for(int i=0;i<dc2s[loops];i++){
        int h = D[D_2[i]]; //hにはその文字の番号．
        if(i>0){h = D[D_2[i]]-D[D_2[i-1]];} //0なら
        if(h<0){h=D[D_2[i]];cc2s[kad]=i+r;ccs[kad]=r;kad++;}
        cd1(h,ar); //k3にはrankクエリを要求するコーディング．kad2が増加する．
        r+=h;
    }
}

int dmlen(int p,unsigned int *D,int *D_2){
    int u = 0;
    for(int i=D_2[p];i<D_2[p+1];i++){
        if(i!=D_2[p]){
            if(D[i]<D[i-1] && u==0){u=D_2[p+1]-i;break;}
        }
    }
    if(u==D_2[p+1]-D_2[p]-1){ //分割の先頭にある，例外処理
        for(int o = loops-1;o>0;o--){
            if(p>=dc2s[o]){
                exceptr[o] = p;
                break;
            }
        }
    }
    if(u>0){u = D_2[p+1]-D_2[p]-u;}
    else{u=1;}
    return u;
}

int cmlen(int *m,int p,int l,bool *ar){ //配列arの先頭より分割mを保存する．
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

int rank1(bool *u,int ar,int s){ //bool配列u上の，sから始まってar文字までの1の個数を返す()．
    if(s<0){printf("[異常値]rank1()で，sに負の値が渡されています！");exit(0);}
    if(ar<=0){return 0;}
    int e = 0;
    for(int i=s;i<s+ar;i++){
        if(u[i]==1){
            e++;
        }
    }
    return e;
}

int srank1(int c,bool *u,int *u2,unsigned short int *u3,int t2,int t3){ //0から始まってk番目までの1の個数を返す．．
    if(c<=0){return 0;}
    int r = c/lpurse;
    int nk = (c - r*lpurse)/spurse;
    int s =0;int e = 0;
    if(r>0){
        s = lpurse*r;
        e = u2[r-1];
    }
    if(nk>0){
        e+=u3[r*spursesuu+nk]; //t2*[k/t2]まで数えた．
        s+=nk*spurse;
    }
    for(int i=s;i<c;i++){if(u[i]==1){e++;}}
    return e;
}

int srank0(int c,bool *u,int *u2,unsigned short int *u3,int t2,int t3){ 
    //0から始まってk番目までの1の個数を返す
    return c-srank1(c,u,u2,u3,t2,t3);
}

int select1(bool *u,int *u2,unsigned short int *u3,int c,int n,int t2,int t3){ //k番目の1を探す．
    int i = 0;
    int j = 0;
    int l = 0;
    printf("\n(select1)c = %d",c);
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
    printf("\n[2]get %d(%d,%d)",u2[0],c,spursesuu);
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
    printf("\n[3]get %d(%d,%d %d %d %d)",e,l,j,u3[0],u3[1],u3[2]);
    for(int p=i*lpurse+j*spurse;p<n;p++){
        if(u[p]==1){
            e++;
            if(e==c){return p;}
        }
    }
    return n+1;
}

int select0(bool *u,int *u2,unsigned short int *u3,int c,int n,int t2,int t3){ //k番目の0を探す．
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
    for(int p=i*lpurse+j*spurse;p<n;p++){
        if(u[p]==0){
            e++;
            if(e==c){return p;}
        }
    }
    return n+1;
}

int getd2(int tgm){
    int tgm2 = select0(d2top,d2topsub,d2topsub2,tgm+1,Sizelist[9],Sizelist[5],Sizelist[6]); //tgm2 = その位置までの1の数を見る．
    int tgm3 = srank1(tgm2,d2top,d2topsub,d2topsub2,Sizelist[5],Sizelist[6]);
    int tgm4 = bread(d2under,tgm*6,6);
    return tgm3*64+tgm4;
}

int gethead(int tgm){
    int tgm2 = select0(k3,k3sub,k3sub2,tgm+1,Sizelist[8],Sizelist[3],Sizelist[4]); //tgm2 = その位置までの1の数を見る．
    //printf("\n<%d>",tgm2);
    int tgm3 = srank1(tgm2,k3,k3sub,k3sub2,Sizelist[3],Sizelist[4]); //その位置までの1の数 = 開始文字.
    //printf("\n<%d>",tgm3);
    for(int o = loops-1;o>0;o--){
            if(tgm2>=cc2s[o]){tgm3-=ccs[o];break;}
    }
    //printf("\n<%d>",tgm3);
    return tgm3;
}

int psread(int p,bool *ar,int D2start,int D2end,int basekey){
    //tx[p]からtx[p+1]までの差分を読んで分割を再生．
    printf("[%d]%d ",D2start,basekey);
    int a;
    int turnsl = 0;
    int st = D2start;
    for(int i = 0;i<1000;i++){
        a = gread(st,ar);
        st += 2*loga(a)-1;
        if(i>0){
            if(i>=turnsl){basekey-=a-1;}
            else{basekey += a-1;}
            printf("%d ",basekey);
        }
        else{
            turnsl = a;
            if(a==1){turnsl=65535;}
        }
        if(st>=D2end){break;}
    }
    return 0;
}

void uhdisp(int tgm){
    printf("\n%d:",tgm);
    int D2start = getd2(tgm);
    int D2end = getd2(tgm+1);
    int tgm3 = gethead(tgm);
    psread(tgm,k,D2start,D2end,tgm3);
}

int rsbuild(bool *ar,int *arsub,unsigned short int *arsub2,int t2,int newn){
    //int purses = spursesuu;
    //int pursesize = i2 / spursesuu;
    kad = 0;int tmpmax = 0;int tmpmax2=0;
    printf("\npursesize = %d : lpursesize = %d",spurse,lpurse);
    for(int i =0;i<=t2;i++){
        int ky = lpurse*i;
        int pastrank = 0;
        for(int j=0;j<spursesuu;j++){
            if(ky+j*spurse>=newn){
                int valu = rank1(ar,newn-ky-1,ky);
                arsub2[i*spursesuu+j] = valu;kad++;
                if(i*spursesuu+j>tmpmax2){tmpmax2 = i*spursesuu+j;}
                //printf("\narsub2[%d] = %d",i*purses+j,valu);
                //bwrite(arsub2,(i*spursesuu+j)*10,10,rank1(ar,newn-ky-1,ky)); //最後の数ビット
                arsub[t2] = arsub[t2-1] + valu + 1; //絶対超えない数字(本当か？)
                printf("\nu2[%d]=%d(%d)",t2,arsub[t2],valu+1);
                if(t2>tmpmax){tmpmax = t2;}
                break; //末尾はブレイク
            }
            //printf("[%d,%d,%d]",ky,j,spurse);
            //pastrank = pastrank + rank1(ar,spurse,ky+(j-1)*spurse);
            if(j==0){pastrank = rank1(ar,0,ky);}
            else{pastrank = pastrank + rank1(ar,spurse,ky+(j-1)*spurse);}
            arsub2[i*spursesuu+j] = pastrank;kad++;
            if(i*spursesuu+j>tmpmax2){tmpmax2 = i*spursesuu+j;}
            //printf("\narsub2[%d] = %d",i*purses+j,pastrank);
            //bwrite(arsub2,(i*spursesuu+j)*10,10,pastrank);
        }
        if(i<t2){
            if(i>0){pastrank +=arsub[i-1];}
            pastrank += rank1(ar,spurse,ky+(lpurse-spurse));
            arsub[i]=pastrank;
            printf("\nu2[%d]=%d",i,arsub[i]);
            if(i>tmpmax){tmpmax = i;}
        }
    }
    //arsub[t2] = 999999999;
    printf("\narsub[0] = %d",arsub[0]);
    printf("newn = %d : rankselectサイズ :%d,%d\n",newn,t2*4,kad*2);
    printf("入力最大位置[%d,%d]",tmpmax,tmpmax2);
    return t2*4 + kad*2;
}

int pscheck(int *m,int start,int plen, int round){ //完全一致を探す
    bool *bitlet;
    bitlet = (bool*)malloc(10000);
    int rofs = ccs[round];
    if(round>=2){rofs-=dc2s[round-1];}
    int y = cmlen(m,start,plen,bitlet);
    int a,b,c,d;
    printf("\n<check>:");for(int yj = 0;yj<plen;yj++){printf("[%d]",m[start+yj]);}
    int posup = select1(k3,k3sub,k3sub2,m[start]+rofs,Sizelist[8],Sizelist[3],Sizelist[4]);
    int posbt = select1(k3,k3sub,k3sub2,m[start]+1+rofs,Sizelist[8],Sizelist[3],Sizelist[4]);
    printf("\nposbt / posup = %d / %d (%d,%d)",posbt,posup,cc2s[round],cc2s[round+1]);
    posup = srank0(posup,k3,k3sub,k3sub2,Sizelist[3],Sizelist[4])-1;
    posbt = srank0(posbt,k3,k3sub,k3sub2,Sizelist[3],Sizelist[4]);
        printf("\nstart:%d/%d\n",posup,posbt);
        uhdisp(posup);
        uhdisp(posbt);
    return 0;
}

int ptrans(int *B1,int *B2,int *B3,int *Ls,int *Rs,int startm,int *m,int mrlen,int round){
    int *Rp;
    int flag = 0;
    Rp = (int*)malloc(mrlen*4+4);
    int tn = -1;int Rpptr = 0;int len = 0;
    for(int i = startm+mrlen-1;i>startm;i--){ //check from right(for setting L&S type)
        Rp[Rpptr] = m[i];
        len++;
        if(m[i-1] < m[i]){
            tn=0;
        }
        if(m[i-1] > m[i]){
            if(tn==0){ //m[i-1] & m[i] == L & S
                if(B2[round] == 0){B2[round] = len;len = 0;flag++;}
                else{
                    Rpptr-=(len-1);
                    Rp[Rpptr] = pscheck(m,i,len,round); 
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

void bfwrite(bool *ar,int mx,FILE *fp){ //ビット列arをmx個書き込む
    int ps = 0;
    unsigned char a;
    while(ps<mx){
        a = ar[ps]*128;
        if(ps+1<mx){a += ar[ps+1]*64;}
        if(ps+2<mx){a += ar[ps+2]*32;}
        if(ps+3<mx){a += ar[ps+3]*16;}
        if(ps+4<mx){a += ar[ps+4]*8;}
        if(ps+5<mx){a += ar[ps+5]*4;}
        if(ps+6<mx){a += ar[ps+6]*2;}
        if(ps+7<mx){a += ar[ps+7]*1;}
        fwrite(&a, sizeof(unsigned char),1,fp);
        ps+=8;
    }
}

int write74(){
    FILE *fp;
    //unsigned char chr;
    char wname[] = "codeindex";
    fp = fopen(wname, "w"); // open file or return null
    fwrite(&loops, sizeof(int),1, fp); //0:loops
    fwrite(Sizelist, sizeof(int),11, fp); //0:定数
    fwrite(&dcs[1], sizeof(int), loops, fp); //1:dcs[1][2][3]...
    fwrite(&dc2s[1], sizeof(int), loops, fp); //2:dc2s
    fwrite(&ccs[1], sizeof(int), loops, fp); //1:dcs[1][2][3]...
    fwrite(&cc2s[1], sizeof(int), loops, fp); //2:dc2s
    fwrite(&exceptr[0], sizeof(int), loops+1, fp); //2:例外

    fwrite(D_3, sizeof(int), dc2s[loops]-Sizelist[2], fp); //5:D3
    fwrite(d2topsub, sizeof(int), Sizelist[5], fp); //3:D
    fwrite(k3sub, sizeof(int), Sizelist[3], fp); //3:D
    fwrite(NS, sizeof(unsigned int),Sizelist[0], fp); //7:NS
    //ここから2byte
    
    fwrite(D_3m, sizeof(unsigned short int), Sizelist[2]-Sizelist[1], fp); //5:D3
    fwrite(d2topsub2, sizeof(unsigned short int), Sizelist[6], fp); //3:D
    fwrite(k3sub2, sizeof(unsigned short int), Sizelist[4], fp); //3:D
    //ここから1byte
    fwrite(D_3s, sizeof(unsigned char), Sizelist[1], fp); //5:D3
    bfwrite(k,Sizelist[7],fp); //3:D
    bfwrite(k3,Sizelist[8],fp); //4:D_2
    bfwrite(d2under,Sizelist[10],fp); //3:D
    bfwrite(d2top,Sizelist[9],fp); //4:D_2
    fclose(fp); // open file or return null
    return 0;
}

int main(){
    FILE *fp;
    char wname[] = "index"; //index file name
    unsigned int *D;
    int *D_2;
    int i = 0;int round = 0;dcs[0] = 0;dc2s[0] = 0; 
    unsigned char chr;
    unsigned int a;
    fp = fopen(wname, "r");
    int flag = 0; //0:loops,1:dcs,2:dc2s,3:D,4:D_2,5:D3,6:NS
    int w = 1;
    while(flag<8){
        fscanf(fp,"%c",&chr);a = chr;
        fscanf(fp,"%c",&chr);a += chr*256;
        fscanf(fp,"%c",&chr);a += chr*65536;
        fscanf(fp,"%c",&chr);a += chr*65536*256;
        if(flag == 9){printf("\n[%d]:%u",flag,a);exit(1);}
        if(flag==7){NS[w]=a;w++;if(w==Sizelist[0]){flag++;printf("\n-finishing load index-\n");}}
        if(flag==6){Sizelist[0]=a;NS = (unsigned int*)malloc(4*Sizelist[0]);flag++;}
        if(flag==5){D_3[w]=a;w++;if(w==dc2s[loops]){flag++;w=0;}}
        if(flag==4){D_2[w]=a;w++;if(w==dc2s[loops]+1){flag++;w=0;}}
        if(flag==3){D[w]=a;w++;if(w==dcs[loops]){flag++;w=0;}}
        if(flag==2){dc2s[w]=a;if(w==loops){flag++;w=0;D_2 = (int*)malloc(4*dc2s[loops]+4);D_3 = (int*)malloc(4*dc2s[loops]+4);}else{w++;}}
        if(flag==1){dcs[w]=a;if(w==loops){flag++;w=1;D = (unsigned int*)malloc(4*dcs[loops]);}else{w++;}}
        if(flag==0){loops=a;flag++;}
        //fseek(fp,16,SEEK_CUR);

    }
    fclose(fp);
    for(int o = loops-1;o>=0;o--){exceptr[o]=-1;}
    Sizelist[1] = dc2s[loops];Sizelist[2] = dc2s[loops];
    for(int yj = 0;yj<dc2s[loops];yj++){
        if(D_3[yj]>255){Sizelist[1] = yj;break;}
    }
    for(int yj = Sizelist[1];yj<dc2s[loops];yj++){
        if(D_3[yj]>65535){Sizelist[2] = yj;break;}
    }
    D_3s = (unsigned char*)malloc(Sizelist[1]);
    D_3m = (unsigned short int*)malloc((Sizelist[2]-Sizelist[1])*2);
    for(int yj = 0;yj<Sizelist[1];yj++){D_3s[yj] = D_3[yj];}
    for(int yj = 0;yj<Sizelist[2]-Sizelist[1];yj++){D_3m[yj] = D_3[yj+Sizelist[1]];}
    //D_3をサイズごとに分割して保存している
    int roid;
    int totalsps = 0;
    int maxl = dc2s[loops];
    int *tx;
    tx = (int*)malloc((maxl+1)*4); //D_2の置き換え配列
    k = (bool*)malloc(Sizelist[7]); //最初に十分な領域を割り当てる.
    k3 = (bool*)malloc(Sizelist[8]);
    printf("ここを通過[%d,%d,%d]",maxl,Sizelist[7],Sizelist[8]);
    for(i=0;i<maxl;i++){
        //if(i==(maxl-5)){printf("[%d]まではOK(%d)",i,kad);} //Sizelist[7]を大きくするとセグフォしなくなります
        tx[i] = kad;
        roid=dmlen(i,D,D_2);
        diff(D,D_2[i],D_2[i+1]-D_2[i]);
        logg(roid,k); //1--反転なし:2--roid番目から文字が反転
        blogg(i,D,D_2,k);
    } //ガンマ符号
    tx[maxl] = kad;
    printf("\n[D]配列サイズ : %d[%d bytes] / [D_2] = %d[%d bytes]\n",kad,kad/8,maxl,maxl*4);
    Sizelist[7] = kad;
    totalsps += kad/8; //符号化したD
    fano0(D,D_2,k3); //fの先頭の文字のコーディングをする．kadが消えるので，再生すること．
    printf("[D先頭]配列サイズ:%d[%d bytes]\n",kad2,kad2/8);
    Sizelist[8] = kad2;
    totalsps += kad2/8; //符号化したD[先頭]
    free(D);
    for(i=0;i<=maxl;i++){D_2[i]=tx[i];} //作ったtxをD_2に差し替えるs
    free(tx);
    for(int o = 0;o<loops;o++){printf("\n%d[%d]",o,exceptr[o]);}
    int newn = kad2;
    //int t2=newn/lpurse;int t3=newn/spurse;
    Sizelist[3] = newn/lpurse + 1;Sizelist[4] =newn/spurse + 2;
    k3sub = (int*)malloc(Sizelist[3]*4);
    k3sub2 = (unsigned short int*)malloc(Sizelist[4]*2);
    printf("\n予測rs記述量[%d / %d](サイズnewn = %d)\n", Sizelist[3],Sizelist[4],newn);
    totalsps += rsbuild(k3,k3sub,k3sub2,Sizelist[3]-1,newn);
    printf("tx[loops] = %d[NS] + %d[D] + %d[D_2] + [%d + %d(for rank.select query)] = ",Sizelist[0]*4,D_2[maxl]/8 + newn/8,dc2s[loops]*4,Sizelist[3]*4,Sizelist[4]*2);
    printf("\nD_2[maxl]/maxl = %d(大体64 = 6 桁として実行)",D_2[maxl]/maxl);
    totalsps += Sizelist[0]*4 + dc2s[loops]*4; //NF && D
    d2under = (bool*)malloc((maxl+1)*6);
    d2top = (bool*)malloc(maxl+1+D_2[maxl]/64);
    printf("\n[[%d]]",maxl+1+D_2[maxl]/64);
    totalsps += fano(D_2,0,maxl+1,6,d2under,d2top);
    //d2top用にrank.select配列を作ります
    newn = kad2;
    //t2=newn/lpurse;t3=newn/spurse;
    Sizelist[5] = newn/lpurse + 1;Sizelist[6] =newn/spurse + 1;
    d2topsub = (int*)malloc(Sizelist[5]*4);
    d2topsub2 = (unsigned short int*)malloc(Sizelist[6]*2);
    totalsps += rsbuild(d2top,d2topsub,d2topsub2,Sizelist[5]-1,newn);
    printf("\n総合サイズ = %d byte(%d,%d)",totalsps,Sizelist[7],Sizelist[8]);
    write74(); //インデックスをcodeindexと言うファイルに保管する
    //for(int yj= 0 ;yj < 40;yj++){printf(" %d",d2topsub[yj]);}
    printf("符号化データをファイルに書き出し完了");
    return 0;
}