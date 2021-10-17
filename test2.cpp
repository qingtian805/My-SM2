#include<iostream>
#include<stdlib.h>
#include<memory.h>
#include<time.h>
#include<string.h>
extern "C"{
#include"miracl.h"
}
#include"sm3.c"

using namespace std;

//随机数生成器赋值
#define SEED 0x1BD8C95A
//椭圆曲线参数赋值
char pn[65] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";
char an[65] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC";
char bn[65] = "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93";
char nn[65] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123";
char xn[65] = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7";
char yn[65] = "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";

//sm3补足函数，将比特流转换为字符串
void streamToString(unsigned char* stream){
    int pm,pr;
    stream[64] = '\0';
    pr = 63;
    for(pm = 31; pm>=0; pm--)
    {
        stream[pr] = stream[pm] & 0b00001111;//低4位
        stream[pr] = stream[pr] + '0';
        if(stream[pr] > '9'){
            stream[pr] = stream[pr] + 7;
        }
        pr--;
        stream[pr] = stream[pm] >> 4;//高4位
        stream[pr] = stream[pr] + '0';
        if(stream[pr] > '9'){
            stream[pr] = stream[pr] + 7;
        }
        pr--;
    }
}
//计算用户其他信息Za，ZA应传入有65空间的字符串
void calOtherInfo(char *ID,int IDlen,char *xA,char *yA,unsigned char* ZA)
{
    char info[IDlen + 387] = "";//64 * 6 + 2 + 1 = 387
    strncat(info,ID,2);//ENTLA取ID首两字符
    strcat(info,ID);
    strcat(info,an);
    strcat(info,bn);
    strcat(info,xn);
    strcat(info,yn);
    strcat(info,xA);
    strcat(info,yA);
    sm3((unsigned char*)info,IDlen+387,ZA);
    streamToString(ZA);
}
//取负数
void toNegative(unsigned char* stream)
{
    int length, i;
    length = strlen((char*)stream);
    for(i = 0;i<length;i++){
        stream[i] = ~stream[i];
    }
    stream[length - 1] = stream[length - 1] + 1;
}

int main(){
    //签名步骤
    //计算数字和其字符形式以n区分
    unsigned char message[65];//信息
    char _message[130];//ZA+message
    char dAn[65];//用户私钥
    char xAn[65], yAn[65];//用户公钥
    char ID[65];//用户ID
    char rn[65], sn[65];//签名密钥对
    char en[65] = "F0B43E94BA45ACCAACE692ED534382EB17E6AB5A19CE7B31F4486FDFC0D28640";//计算过程变量e，杂凑值状态
    char ZA[65];//用户其他信息计算结果
    big a, b, p, n, x, y;//椭圆曲线参数
    epoint *g,*p1;
    big x1, y1, k;//签名参数
    big r, e, tmp;//5
    big dA, und, s;//6
    int rlen, slen;
    char deb[65];

    miracl *mip;//MIRACL系统地址

    cout << "Initing MIRACL..." <<endl;

    mip = mirsys(64,16);//64位16进制数||256 bits
    if(mip == NULL)
    {
        cout << "NOT ENOUGH MEMORY!" << endl;
        system("pause");
        return 0;
    }
    mip->IOBASE = 16;
    //mip->ERCON = TRUE;
    cout << "MIRACL Inited." << endl;

    cout << "Entering sign progress.\n";
    cout << "Enter your ID" << endl;//输入ID
    cin >> ID;
    cout << "Enter your public key:\n";//输入公钥
    cout << "x:" <<endl;
    cin >> xAn;
    cout << "y:" <<endl;
    cin >> yAn;
    cout << "Enter your private key:" << endl;//输入私钥
    cin >> dAn;
    cout << "Input message:" << endl;//输入信息
    cin >> message;
    cout << "Progress started..." << endl;
      
    cout << "Entering Step 1..." << endl;

    //step 1
    //calOtherInfo(ID, strlen(ID), xAn, yAn, (unsigned char*)ZA);
    //strcpy(_message,ZA);
    //strcat(_message,(char*)message);

    cout << "Step 1 finished.\nEntering Step 2..." << endl;

    //step 2
    //sm3((unsigned char*)_message,strlen(_message),(unsigned char*)en);
    //streamToString((unsigned char*)en);

    cout << "Step 2 finished." << endl;

    //step 3-6 and loop
    int j1, j2;
    //外层loop判断 s?
    do
    {
        do//内层loop判断 r||r+k?
        {
            //step 3-4
            cout << "Entering Step 3..." << endl;

            //init number
            x1 = mirvar(0);
            y1 = mirvar(0);
            k = mirvar(0);

            char kn[65] = "59276E27D506861A16680F3AD9C02DCCEF3CC1FA3CDBE4CE6D54B80DEAC1BC21";

            p = mirvar(0);
	        a = mirvar(0);
            b = mirvar(0);
            n = mirvar(0);
            x = mirvar(0);
            y = mirvar(0);

            cinstr(p,pn);      //将大数字符串转换成大数,这里是16进制的字符串转换大数
	        cinstr(a,an);
            cinstr(b,bn);
	        cinstr(n,nn);
	        cinstr(x,xn);
            cinstr(y,yn);
            cinstr(k,kn);

            //初始化椭圆曲线
	        ecurve_init(a,b,p,MR_PROJECTIVE);   
            g = epoint_init();
	        p1 = epoint_init();
            epoint_set(x,y,0,g);    //g=(x,y)为基点G

	        //产生签名密钥对
            //irand(time(NULL)+SEED);   //初始化种子
            //bigrand(n,k);    //生成随机数k,0<=k<n

            cout << "Step 3 finished.\nEntering step 4..." << endl;

            ecurve_mult(k,g,p1);   //p1 = k*g
            epoint_get(p1,x1,y1);    //取pB上的点（x，y）x和y即为公钥

            big_to_bytes(0,x1,deb,FALSE);//tmp = -dA
            streamToString((unsigned char*)deb);
            cout << deb << endl;
            big_to_bytes(0,y1,deb,FALSE);//tmp = -dA
            streamToString((unsigned char*)deb);
            cout << deb << endl;

            cout << "Step 4 finished.\nEntering step 5..." << endl;

            //step 5
            //init number
            tmp = mirvar(0);
            r = mirvar(0);
            e = mirvar(0);

            cinstr(e,en);

            add(x1,e,tmp);//tmp = x1 + e
            divide(tmp,n,n);//tmp = tmp (mod n)
            copy(tmp,r);//r = tmp

            rlen = big_to_bytes(0,r,rn,FALSE);
            streamToString((unsigned char*)rn);

            //compare
            zero(tmp);//tmp = 0
            j1 = mr_compare(r,tmp);//r = 0?
            subtract(n,k,tmp);//tmp = n - k
            j2 = mr_compare(r,tmp);//r + k = n?
        }while(j1 == 0 || j2 == 0);//r = 0 || r + k = n

        cout << "Step 5 finished.\nEntering step 6..." << endl;

        //step 6
        und = mirvar(0);
        s = mirvar(0);
        dA = mirvar(0);

        cinstr(dA,dAn);

        char negative[65];

        incr(dA,1,und);//und = 1 + dA
        xgcd(und,n,und,und,und);//und = 1/und (mod n)

        //big_to_bytes(0,dA,negative,FALSE);//tmp = -dA
        //toNegative((unsigned char*)negative);
        //streamToString((unsigned char*)negative);
        //cinstr(tmp,negative);

        //negify(dA,tmp);//tmp = -dA

        //big_to_bytes(0,tmp,deb,FALSE);
        //streamToString((unsigned char*)deb);
        //cout << deb << endl;

        mad(r,dA,dA,n,n,tmp);//tmp = (r * dA) mod n
        if(mr_compare(k,tmp) >= 0)//tmp = k-tmp
        {
            subtract(k,tmp,tmp);
        }
        else
        {
            subtract(n,tmp,tmp);
            add(k,tmp,tmp);
        }
        mad(tmp,und,und,n,n,tmp);//tmp = (tmp * und) mod n
        //divide(tmp,n,n);//tmp = tmp (mod n)
        copy(tmp,s);//s = tmp

        slen = big_to_bytes(0,s,sn,FALSE);
        streamToString((unsigned char*)sn);

        zero(tmp);//tmp = 0
        j1 = mr_compare(s,tmp);//s = 0?
    }while(j1 == 0);//s = 0?

    cout << "Progress finished.\nPrinting results..." << endl;

    cout << rlen << "|" << slen << endl;

    int j;
    cout << "签名r:\n" << rn << endl;
    cout << "签名s:\n" << sn << endl;

	mirexit();//关闭MIRACL系统
    return 0;
}