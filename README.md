# My-SM2

**TO BUILD A SM2 BASED ON C/C++** is my 2021 Summer Pratical Training topic.  
This repo stores its code.  

## Introduction of files

### Prime Folder ./

**certify_server.cpp** A very simple usage of my-sm2, SM3 and SM4.This file is its server program source.  
**certify_client.cpp** A very simple usage of my-sm2, SM3 and SM4.This file is its client program source.  
**CMakeList.txt** CMake list, of my project. If you don`t know how it works, please refer to CMake.  
**notes.md** A studing note of basic ECC knowledge.  
**OTPclient.cpp** OTP client main file, using TOTP.cpp.  
**README.md** Common ReadMe file.  
**enctest.cpp** test file for encryption.  
**gektest.cpp** test file for key generate and key managing.  

### Source ./src

**sm2culculators.cpp** All functions calculates number in sm2.  
**sm2encrypt.cpp** Main functions of SM2 encryption.  
**sm2init.cpp** Initalizing funcions.  
**sm2KDF.cpp** KDF assisting function.  
**sm2keymgr.cpp** Key pair generating function and pubkey verify function.  
**sm2old.cpp** The main file of my project, contines the final version of My-SM2 when the summer pratical training over.  
**sm2signment.cpp** Main functions of SM2 signment.  
**sm2StreamString.cpp** Contains two functions converts stream and string.  
**TOTP.cpp** C++ version of TOTP using SM3.  
**sm3.c** sm3 file by qq_36298219 at [CSDN](https://blog.csdn.net/qq_36298219/article/details/85926747).  
**sm4.cpp** SM4 source file by [tonyonce2017](https://github.com/tonyonce2017/SM4).  

### Include file ./include/

**mir\*.\*** [MIRACL](https://github.com/miracl/MIRACL) lib files. Using for educational propose. In replace of [uint256_t](https://github.com/calccrypto/uint256_t) which was used to deal with bignum calculations and been deleted now.  
**sm2.h** The master include file. Includes all main functions.  
**sm2calculators.h** Header of sm2calculators.cpp.  
**sm2encrypt.h** Header of sm2encrypt.cpp.  
**sm2init.h** Header of  
**sm2KDF.h** Header of sm2KDF.cpp.  
**sm2keymgr.h** Header of sm2keymgr.cpp.  
**sm2parameter.h** Header contains parameters used in SM2 system.  
**sm2sigment.h** Header of sm2sigment.cpp.  
**sm2StreamString.h** Header of sm2StreamString.cpp.  
**sm2type.h** Header contains self defined types of my SM2 system.  
**sm3.h** SM3 project by qq_36298219 and you can find them on [here](https://blog.csdn.net/qq_36298219/article/details/85926747).  
**sm4.h** SM4 header by [tonyonce2017](https://github.com/tonyonce2017/SM4).  

### Libraries ./lib/

**libmiracl.a** Complied MIRACL lib file.  

### SM4-main ./SM4-main/

**README.md** README file of SM4 project by [tonyonce2017](https://github.com/tonyonce2017/SM4).

## Feature
As for limited time and my poor code ability, the function of My-SM2 is very limited which means it can only verify keys.  
You will be requested for plenty parameters to run the program, and finally, it will returns a pair of keys for signment when "Sing up" function is choosen or result for verifying when user chooses "Verify function".  

## New
For my new learning term of 2021-2020-2, this repo will be reused for designing a user identificating system.  
Some new features may be added.  

### Progress

1. A TOTP for future use. FINISHED  
2. Split functions and use my newly learned techniques to build a new version EASIER DEBUGING and MORE FLEXABLE for reuse.
   1. Split functions in signment. FINISHED  
   2. Divide SM2 into several files. FINISHED
   3. Develop SM2 into a lib like format. FINISHED
   4. Finish encrypt function. FINISHED  
   5. Searching for some details I didn`t noticed and complete them.  
   6. Point compression function.  
   7. Develop key exchange function.  
