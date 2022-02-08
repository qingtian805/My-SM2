#ifndef __SM2_KEYMGR_H__
#define __SM2_KEYMGR_H__

extern "C"{
#include"miracl.h"
}

//生成SM2算法密钥对
//输出：R[65] 生成的私钥 pRivate
//     Ux[65] 生成的公钥 pUblic 的x值
//     Uy[65] 生成的公钥 pUblic 的y值
bool gen_keypair(char* d, char* Px, char* Py);

//公钥验证算法
//输入：Px 公钥的x值
//     Py 公钥的y值
//返回：公钥是否通过验证
bool verify_pubkey(char* Px, char* Py);

namespace SM2
{
    //生成SM2算法密钥对
    //输出：R 生成的私钥 pRivate
    //     Ux 生成的公钥 pUblic 的x值
    //     Uy 生成的公钥 pUblic 的y值
    void gen_keypair(big d, big Px,big Py);

    //公钥验证算法
    //输入：Px 公钥的x值
    //     Py 公钥的y值
    //返回：公钥是否通过验证
    bool verify_pubkey(big Px, big Py);
}

#endif