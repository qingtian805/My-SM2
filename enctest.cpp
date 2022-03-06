#include <iostream>
#include "sm2encrypt.h"

//标准第五章测试参数
char message[] = "encryption standard";
char xBn[] = "09F9DF311E5421A150DD7D161E4BC5C672179FAD1833FC076BB08FF356F35020";
char yBn[] = "CCEA490CE26775A52DC6EA718CC1AA600AED05FBF35E084A6632F6072DA9AD13";
char dBn[] = "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8";


int main(void)
{
    char emessage[sizeof(message)+97];
    char dmessage[sizeof(message)];
    if ( encryptMessage(message, sizeof(message), xBn, yBn, emessage)
        == -1){
            std::cout << "ENC FAILURE" << std::endl;
            return 1;
        }
    if (decryptMessage(emessage, sizeof(emessage), dBn, dmessage)
        == -1){
            std::cout << "DEC FAILURE" << std::endl;
            return 1;
        }
    std::cout << message << "\n" << dmessage << std::endl;
    return 0;
}