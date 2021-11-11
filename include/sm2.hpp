extern "C"{
#include"miracl.h"
}
#define DEBUG true

//随机数生成器赋值
#define SEED 0x1BD8C95A

namespace SM2{    
    //椭圆曲线参数赋值
    char pn[65] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";
    char an[65] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC";
    char bn[65] = "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93";
    char nn[65] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123";
    char xGn[65] = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7";
    char yGn[65] = "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";

    //sm3补足函数，将比特流转换为字符串
    //stream,streamlength -> string
    //转换后的字符串存于相同字符串，请确保字符串长度为原长度的两倍+1
    //长度为32字节的流需要65字节长的字符串
    void streamToString(unsigned char* stream,int streamlength, unsigned char* string);

    //计算用户其他信息函数，被gen_Message()调用，用以符合标准第二章
    //ID,xA,yA -> ZA
    //ZA需要65字节长的字符串
    void calOtherInfo(char *ID,char *xA,char *yA,unsigned char* ZA);

    //计算_M = ZA||M
    void gen_Message(char* ID,char* xAn,char* yAn,char* message,char* _message);

    //计算e=H256(_M)
    void calE(char* _message,char* en);

    //生成随机数k in [1,n-1]，请预先初始化MIRACL系统以及k参数
    void genRandom(big k);

    //计算签名公钥
    void calP1(big k,big x1);

    //计算r=(e+x1)mod n
    void calr(big e,big x1,big r);

    //计算s==((1+dA)^-1·(k-r·dA))mod n
    void cals(big k,big dA,big r,big s);

}
//SM2椭圆曲线算法签名生成函数
//输入：ID 用户标识符
//  xAn yAn 用户公钥坐标
//  dAn 用户私钥
//  message 签名信息
//  messagelen 信息长度
//  r s 签名密钥对
void genSignment(char* ID,char* xAn,char* yAn,char* dAn,unsigned char* message,int messagelen,char* rn,char* sn);