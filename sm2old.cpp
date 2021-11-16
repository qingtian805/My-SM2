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
char xGn[65] = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7";
char yGn[65] = "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";

void streamToString(unsigned char* stream,int streamlength, unsigned char* string){
    //sm3补足函数，将比特流转换为字符串
    //stream,streamlength -> string
    //转换后的字符串存于相同字符串，请确保字符串长度为原长度的两倍+1
    //长度为32字节的流需要65字节长的字符串
    int pm,pr;
    stream[streamlength * 2] = '\0';
    pr = streamlength * 2 - 1;
    for(pm = streamlength - 1; pm>=0; pm--)
    {
        string[pr] = stream[pm] & 0b00001111;//低4位
        if(string[pr] > 9)
            stream[pr] = string[pr] + 55;//55='A'-10
        else
            string[pr] = string[pr] + '0';
        pr--;
        string[pr] = stream[pm] >> 4;//高4位
        if(string[pr] > 9)
            stream[pr] = string[pr] + 55;//55='A'-10
        else
            string[pr] = string[pr] + '0';
        pr--;
    }
}
//计算用户其他信息Za，ZA应传入有65空间的字符串
void calOtherInfo(char *ID,int IDlen,char *xA,char *yA,unsigned char* ZA)
{
    char ENTLA[3];
    char info[IDlen + 387] = "";//64 * 6 + 2 + 1 = 387
    int entlena = IDlen * 8;
    ENTLA[0] = entlena & 0x0F;
    ENTLA[1] = (entlena & 0xF0) >> 4;
    ENTLA[2] = '\0';
    strcat(info,ENTLA);
    strcat(info,ID);
    strcat(info,an);
    strcat(info,bn);
    strcat(info,xGn);
    strcat(info,yGn);
    strcat(info,xA);
    strcat(info,yA);
    sm3((unsigned char*)info,IDlen+387,ZA);
    streamToString(ZA,32,ZA);
}

int main()
{
    int choice;

    cout << "SM2 System, options:\n";
    cout << "1: Sign up\n";
    cout << "2: Justify\n";
    cout << "Choose an option:" << endl;
    cin >> choice;

    if(choice == 1)
    {
        //签名步骤
        //计算数字和其字符形式以n区分
        unsigned char message[65];//信息
        char _message[130];//ZA+message
        char dAn[65];//用户私钥
        char xAn[65], yAn[65];//用户公钥
        char ID[65];//用户ID
        char rn[65], sn[65];//签名密钥对
        char en[65];//计算过程变量e，杂凑值状态
        char ZA[65];//用户其他信息计算结果
        big a, b, p, n, x, y;//椭圆曲线参数
        epoint *g,*p1;
        big x1, y1, k;//签名参数
        big r, e, tmp;//5
        big dA, und, s;//6

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
        calOtherInfo(ID, strlen(ID), xAn, yAn, (unsigned char*)ZA);
        strcpy(_message,ZA);
        strcat(_message,(char*)message);

        cout << "Step 1 finished.\nEntering Step 2..." << endl;

        //step 2
        sm3((unsigned char*)_message,strlen(_message),(unsigned char*)en);
        streamToString((unsigned char*)en,32,(unsigned char*)en);

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
	            cinstr(x,xGn);
                cinstr(y,yGn);

                //初始化椭圆曲线
	            ecurve_init(a,b,p,MR_PROJECTIVE);   
                g = epoint_init();
	            p1 = epoint_init();
                epoint_set(x,y,0,g);    //g=(x,y)为基点G

	            //产生签名密钥对
                irand(time(NULL)+SEED);   //初始化种子
                bigrand(n,k);    //生成随机数k,0<=k<n

                cout << "Step 3 finished.\nEntering step 4..." << endl;

                ecurve_mult(k,g,p1);   //p1 = k*g
                epoint_get(p1,x1,y1);    //取pB上的点（x，y）x和y即为公钥

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

            incr(dA,1,und);//und = 1 + dA
            xgcd(und,n,und,und,und);///und = 1/und (mod n)

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
            copy(tmp,s);//s = tmp

            zero(tmp);//tmp = 0
            j1 = mr_compare(s,tmp);//s = 0?
        }while(j1 == 0);//s = 0?

        cout << "Progress finished.\nPrinting results..." << endl;

        big_to_bytes(0,r,rn,FALSE);
        streamToString((unsigned char*)rn,32,(unsigned char*)rn);
        big_to_bytes(0,s,sn,FALSE);
        streamToString((unsigned char*)sn,32,(unsigned char*)sn);

        int j;
    	cout << "签名r:\n" << rn << endl;
        cout << "签名s:\n" << sn << endl;

        mirkill(a);
        mirkill(b);
        mirkill(p);
        mirkill(n);
        mirkill(x);
        mirkill(y);
        mirkill(x1);
        mirkill(y1);
        mirkill(k);
        mirkill(r);
        mirkill(e);
        mirkill(tmp);
        mirkill(dA);
        mirkill(und);
        mirkill(s);
        epoint_free(g);
        epoint_free(p1);
	    mirexit();//关闭MIRACL系统
    }
    else if(choice == 2)
    {
        //验证步骤
        //计算数字和其字符形式以n区分
        unsigned char message[65];//信息
        char _message[130];//ZA + message
        char ID[65];//用户标识
        char xAn[65], yAn[65];//用户公钥
        char rn[65], sn[65];//签名参数
        char ZA[65];//用户其他信息
        char en[65];//e,杂凑值状态
        char Rn[65];//签名验证参数
        big a, b, p, n, x, y;//椭圆曲线参数
        epoint *g, *p1, *pA;
        big r, s;//签名参数
        big t, tmp;//step 5 参数
        big x1, y1;//p1坐标
        big xA, yA;//pA坐标
        big R, e;//step 7 参数


        miracl *mip;

        cout << "Initing MIRACL..." <<endl;

        mip = mirsys(64,16);//64位16进制数||256 bits
        if(mip == NULL)
        {
            cout << "NOT ENOUGH MEMORY!" << endl;
            system("pause");
            return 0;
        }
        mip->IOBASE = 16;

        cout << "MIRACL Inited." << endl;

        cout << "Entering verifying progress.\n";
        cout << "Enter sender`s ID" << endl;//输入ID
        cin >> ID;
        cout << "Enter sender`s public key:\n";//输入公钥
        cout << "x:" <<endl;
        cin >> xAn;
        cout << "y:" <<endl;
        cin >> yAn;
        cout << "Enter message sign key:" << endl;//输入私钥
        cout << "s" << endl;
        cin >> sn;
        cout << "r" << endl;
        cin >> rn;
        cout << "Input message:" << endl;//输入信息
        cin >> message;
        cout << "Progress started..." << endl;

        cout << "Entering Step 1..." << endl;

        //Step 1
        int j1;
        r = mirvar(0);
        n = mirvar(0);
        cinstr(r, rn);
        cinstr(n, nn);
        j1 = mr_compare(r, n);
        if(j1 >= 0)
        {
            cout << "Verify Faliure" << endl;
            return 0;
        }

        cout << "Step 1 finished.\nEntering step 2..." << endl;

        //step 2
        s = mirvar(0);
        cinstr(s, sn);
        j1 = mr_compare(s, n);
        if(j1 >= 0)
        {
            cout << "Verify Faliure" << endl;
            return 0;
        }
        
        cout << "Step 2 finished.\nEntering step 3..." << endl;

        //step 3
        calOtherInfo(ID, strlen(ID), xAn, yAn, (unsigned char*)ZA);
        strcpy(_message, ZA);
        strcat(_message, (char*)message);

        cout << "Step 2 finished.\nEntering step 3..." << endl;

        //step 4
        sm3((unsigned char*)_message,strlen(_message),(unsigned char*)en);
        streamToString((unsigned char*)en,32,(unsigned char*)en);

        cout << "Step 4 finished.\nEntering step 5..." << endl;

        //step 5
        t = mirvar(0);
        tmp = mirvar(0);

        add(r, s, tmp);//tmp = r + s
        divide(tmp,n,n);//tmp = tmp(mod n)
        copy(tmp,t);//t = tmp
        zero(tmp);//tmp = 0

        j1 = mr_compare(t,tmp);
        if(j1 == 0)
        {
            cout << "Verify Faliure" << endl;
            return 0;
        }

        cout << "Step 5 finished.\nEntering step 6..." << endl;

        //step 6
        x1 = mirvar(0);
        y1 = mirvar(0);

        p = mirvar(0);
	    a = mirvar(0);
        b = mirvar(0);
        //n已经初始化
        x = mirvar(0);
        y = mirvar(0);

        xA = mirvar(0);
        yA = mirvar(0);

        cinstr(p,pn);
        cinstr(a,an);
        cinstr(b,bn);
        cinstr(x,xGn);
        cinstr(y,yGn);

        cinstr(xA,xAn);
        cinstr(yA,yAn);
 
        ecurve_init(a,b,p,MR_PROJECTIVE);
        g = epoint_init();
        pA = epoint_init();
        p1 = epoint_init();
        epoint_set(x,y,0,g);
        epoint_set(xA,yA,0,pA);
        ecurve_mult2(s,g,t,pA,p1);//p1 = s*g + t*pA
        epoint_get(p1,x1,y1);//取出p1的坐标值

        cout << "Step 6 finished.\nEntering step 7..." << endl;

        //step 7
        e = mirvar(0);
        R = mirvar(0);
        cinstr(e,en);
        add(e,x1,tmp);//tmp = e + x1
        divide(tmp,n,n);//tmp = tmp(mod n)
        copy(tmp,R);//R = tmp
        j1 = mr_compare(r,R);

        if(j1 != 0)
        {
            cout << "Verify Faliure" << endl;
            return 0;
        }

        cout << "Step 7 finished.\nAll progress finished correctly.\n\n";
        cout << "Verify successed." << endl;

        mirkill(a);
        mirkill(b);
        mirkill(p);
        mirkill(n);
        mirkill(x);
        mirkill(y);
        mirkill(s);
        mirkill(r);
        mirkill(t);
        mirkill(tmp);
        mirkill(x1);
        mirkill(y1);
        mirkill(xA);
        mirkill(yA);
        mirkill(R);
        mirkill(e);
        epoint_free(g);
        epoint_free(p1);
        epoint_free(pA);
        mirexit();
    }

    system("pause");
    return 0;
}
//12345678901234567890
//09F9DF311E5421A150DD7D161E4BC5C672179FAD1833FC076BB08FF356F35020
//CCEA490CE26775A52DC6EA718CC1AA600AED05FBF35E084A6632F6072DA9AD13
//3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8
//