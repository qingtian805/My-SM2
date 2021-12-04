#ifndef __SM2_INIT_H__
#define __SM2_INIT_H__

extern "C"
{
#include "miracl.h"
}

namespace SM2
{
    //初始化miracl系统
    //输入/输出：mip 指向miracl系统的指针
    //    返回： 初始化结果，成功为true，失败为false
    bool init_miracl(miracl *mip);
    
    //初始化椭圆曲线
    //参数在此sm2.hpp中指定，无需输入输出返回
    void init_ecruve(void);
}

#endif