#include<sm2config.h>
#include<string.h>
#include<stdlib.h>

struct sm2cfg *__SM2_GLOBAL_CONF__;

int sm2init(sm2cfg *sm2p = NULL)
{
    sm2p = ( sm2cfg* ) malloc ( sizeof(sm2cfg) );
    //内存不足，分配失败
    if ( sm2p == NULL ){
        return -1;
    }
    //将默认参数输入设置
    sm2p->POINT_TYPE = 4;
    strcpy( sm2p->P, __SM2_P__ );
    strcpy( sm2p->A, __SM2_A__ );
    strcpy( sm2p->B, __SM2_B__ );
    strcpy( sm2p->N, __SM2_N__ );
    strcpy( sm2p->Gx, __SM2_Gx__ );
    strcpy( sm2p->Gy, __SM2_Gy__ );

    //在系统中存储设置的指针
    __SM2_GLOBAL_CONF__ = sm2p;

    return 0;
}