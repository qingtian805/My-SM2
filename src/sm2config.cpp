#include<sm2config.h>
#include<string.h>
#include<stdlib.h>

struct sm2cfg{
    int POINT_TYPE;
    char P[65];
    char A[65];
    char B[65];
    char N[65];
    char Gx[65];
    char Gy[65];
};

int sm2init(sm2cfg *custom = NULL)
{
    custom = ( sm2cfg* ) malloc ( sizeof(sm2cfg) );
    //内存不足，分配失败
    if ( custom == NULL ){
        return -1;
    }
    //将默认参数输入设置
    custom->POINT_TYPE = 4;
    strcpy( custom->P, __SM2_P__ );
    strcpy( custom->A, __SM2_A__ );
    strcpy( custom->B, __SM2_B__ );
    strcpy( custom->N, __SM2_N__ );
    strcpy( custom->Gx, __SM2_Gx__ );
    strcpy( custom->Gy, __SM2_Gy__ );

    //在系统中存储设置的指针
    __SM2_GLOBAL_CONF__ = custom;

    return 0;
}