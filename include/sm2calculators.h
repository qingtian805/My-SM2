#ifndef __SM2_CALC_H__
#define __SM2_CALC_H__

extern "C"{
#include "miracl.h"
}
#include "sm2type.h"

namespace SM2
{
    //计算_M = ZA||M
    //输入：ZA 用户其他信息，sm3哈希值，32长度字节串
    //  message 签名信息，字节串
    //  mlen 签名信息长度(字节)
    //输出：_message  上划线M的对应值
    void gen_Message(char* ZA,byte* message,int mlen,byte* _message);

    //计算e = H256(_M)，并转化整数
    //输入：_message 上划线M的对应值
    //  _mlen 上划线M的长度(字节)
    //输出：e 转换为整数后的e
    void calE(byte* _message,int _mlen,big e);

    //生成随机数k in [1,n-1]
    //编译器定义：SEED 随机数生成种子
    //从系统获取：time 时间，当作种子
    //输出：k 大小位于[1,n-1]的整数随机数
    void genRandom(big k);

    //生成随机数k in [1,n-2]
    //编译器定义：SEED 随机数生成种子
    //从系统获取：time 时间，作为种子
    //输出：k 大小位于[1,n-2]的整数随机数
    void genRandom2(big k);

    //计算r = (e + x1)mod n
    //计算R = (e`+ x1`)mod n
    //计算t = (r`+ s`)mod n
    void calrt(big er,big x1s,big rt);

    //计算s==((1 + dA)^ -1·(k - r·dA))mod n
    void cals(big k,big dA,big r,big s);

    //计算签名公钥 P1(x1,y1) = [k]G
    //输出：x1 取出的坐标x值
    //     y1 取出的坐标y值
    void calP1(big k,big x1,big y1);

    //计算椭圆曲线点P1`(x1`,y1`)=[s`]G+[t]PA
    void cal_P1(big s,big t,big xA,big yA,big x1,big y1);

    //计算椭圆曲线点P2(x2,y2) = [k]PB(xB,yB)
    void calP2(big k,big xB,big yB,big x2,big y2);
    
    //返回：判断输入给出数值是否处于[1,n-1]，如果不在则返回真
    bool notIn_1n(big __x);

    //返回：判断输入是否等于n
    bool is_n(big __x);

    //返回：判断给出数值是否为0，为0返回真
    bool is_zero(big __x);

    //返回：判断输入是否相等，相等返回真
    bool is_equal(big __x,big __y);

    //返回：判断字节串是否为全0,是则返回真
    bool is_allzero(byte* Bs,int lenB);
}

#endif