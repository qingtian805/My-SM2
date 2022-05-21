#include <iostream>
#include "sm2encrypt.h"
#include <string.h>

//标准第五章测试参数
char message[] = "encryption standard";
/*
char xBn[] = "09F9DF311E5421A150DD7D161E4BC5C672179FAD1833FC076BB08FF356F35020";
char yBn[] = "CCEA490CE26775A52DC6EA718CC1AA600AED05FBF35E084A6632F6072DA9AD13";
char dBn[] = "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8";
*/

//标准第二\四章测试参数
char pn[] = "8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3";
char an[] = "787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498";
char bn[] = "63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A";
char nn[] = "8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7";
char Gxn[] = "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D";
char Gyn[] = "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2";
char xBn[] = "0AE4C7798AA0F119471BEE11825BE46202BB79E2A5844495E97C04FF4DF2548A";
char yBn[] = "7C0240F88F1CD4E16352A73C17B7F16F07353E53A176D684A9FE0C6BB798E857";
char dBn[] = "128B2FA8BD433C6C068C8D803DFF79792A519A55171B1B650C23661D15897263";
/*
char xBn[] = "435B39CCA8F3B508C1488AFC67BE491A0F7BA07E581A0E4849A5CF70628A7E0A";
char yBn[] = "75DDBA78F15FEECB4C7895E2C1CDF5FE01DEBB2CDBADF45399CCF77BBA076A42";
char dBn[] = "1649AB77A00637BD5E2EFE283FBF353534AA7F7CB89463F208DDBC2920BB0DA0";
*/

int main(void)
{
    //SM2系统初始化与设置
    sm2cfg *sm2p = sm2init();
    if ( sm2p == NULL ){
        std::cout << "init SM2 failed!" << std::endl;
    }

    sm2config(sm2p->P, pn);
    sm2config(sm2p->A, an);
    sm2config(sm2p->B, bn);
    sm2config(sm2p->Gx, Gxn);
    sm2config(sm2p->Gy, Gyn);
    sm2config(sm2p->N, nn);

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

    sm2exit(sm2p);
    return 0;
}