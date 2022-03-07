#include <iostream>
#include "sm2encrypt.h"
#include "sm2config.h"

//标准第五章测试参数
char message[] = "encryption standard";
char xBn[] = "09F9DF311E5421A150DD7D161E4BC5C672179FAD1833FC076BB08FF356F35020";
char yBn[] = "CCEA490CE26775A52DC6EA718CC1AA600AED05FBF35E084A6632F6072DA9AD13";
char dBn[] = "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8";


int main(void)
{
    //SM2系统初始化与设置
    sm2cfg *sm2p;
    if ( sm2init(sm2p) == -1 ){
        std::cout << "init SM2 failed!" << std::endl;
    }

    char emessage[sizeof(message)+97];
    char dmessage[sizeof(message)];
    //加密消息存储在emessage中
    if ( encryptMessage(message, sizeof(message), xBn, yBn, emessage)
        == -1){
            std::cout << "ENC FAILURE" << std::endl;
            return 1;
    }
    //解密消息存储在dmessage中
    if (decryptMessage(emessage, sizeof(emessage), dBn, dmessage)
        == -1){
            std::cout << "DEC FAILURE" << std::endl;
            return 1;
    }
    std::cout << message << "\n" << dmessage << std::endl;
    return 0;
}