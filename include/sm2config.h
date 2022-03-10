#ifndef __SM2_CONF_H__
#define __SM2_CONF_H__

//随机数生成器赋值
#define __SM2_SEED__ 0x1BD8C95A

//椭圆曲线参数赋值
//使用16进制字符串赋值，使用时按需转换
#define __SM2_P__  "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF"
#define __SM2_A__  "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC"
#define __SM2_B__  "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93"
#define __SM2_N__  "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123"
#define __SM2_Gx__ "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7"
#define __SM2_Gy__ "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0"

//SM2配置结构
struct sm2cfg{
    int POINT_TYPE;
    char P[65];
    char A[65];
    char B[65];
    char N[65];
    char Gx[65];
    char Gy[65];
};

extern struct sm2cfg *__SM2_GLOBAL_CONF__;

//系统内设置指针
//struct sm2cfg *__SM2_GLOBAL_CONF__;

//用户设置接口
typedef sm2cfg sm2cfg;

/*SM2系统设置初始化函数
 *输入：sm2p 一个sm2cfg结构指针。
 *sm2cfg结构包含椭圆曲线参数以及点压缩方式定义
 *返回：sm2cfg* 非NULL 初始化成功 NULL 初始化失败
 */
sm2cfg *sm2init(void);

/*SM2系统退出函数
 *输入：sm2p 一个由sm2init初始化的SM2系统设置地址
 */
void sm2exit(sm2cfg *sm2p);

#endif