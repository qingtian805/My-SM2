#ifndef __SM2_SS_H__   
#define __SM2_SS_H__

#include "sm2type.h"

//DEBUG
#define __SM2_DEBUG_SS__ false

namespace SM2
{
    //sm3补足函数，将比特流转换为字符串
    //输入：stream 比特流
    //  streamlen 比特流长度(字节)
    //输出：string 可打印字符串
    //请确保字符串存储区长度为原长度的两倍+1
    //例：长度为32字节的流需要65字节长的字符串
    void streamToString(byte* stream,int streamlen, char* string);

    //输入：string 字符串
    //  stringlen 字符串长度(字节)
    //输出：stream 比特流
    //请确保字符串存储区与比特流不在同一区域存储
    //且比特流区域为大于stringlen/2的最小整数
    void stringToStream(char* string,int sringlen,byte* stream);
}

#endif