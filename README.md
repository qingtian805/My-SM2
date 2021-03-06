# My-SM2

**TO BUILD A SM2 BASED ON C/C++** is my 2021 Summer Pratical Training topic.  
This repo stores its code.  

## Introduction of files

### Prime Folder ./

**CMakeList.txt** CMake list, of my project. If you don`t know how it works, please refer to CMake.  
**notes.md** A studing note of basic ECC knowledge.   
**README.md** Common ReadMe file.  
**enctest.cpp** test file for encryption.  
**gektest.cpp** test file for key generate and key managing.  

### Source ./src

**sm2culculators.cpp** All functions calculates number in sm2.  
**sm2config.cpp** General config function. It makes My-SM2 changes its parameters.  
**sm2encrypt.cpp** Main functions of SM2 encryption.  
**sm2init.cpp** MIRACL initalizing funcions. Helps simplifying MIRACL init journels.  
**sm2KDF.cpp** KDF assisting function.  
**sm2keymgr.cpp** Key pair generating function and pubkey verify function.  
**sm2old.cpp** The main file of my project, contines the final version of My-SM2 when the summer pratical training over.  
**sm2signment.cpp** Main functions of SM2 signment.  
**sm2StreamString.cpp** Contains two functions converts stream and string.   
**sm3.c** sm3 file by qq_36298219 at [CSDN](https://blog.csdn.net/qq_36298219/article/details/85926747).  

### Include file ./include/

**mir\*.\*** [MIRACL](https://github.com/miracl/MIRACL) lib files. Using for educational propose. In replace of [uint256_t](https://github.com/calccrypto/uint256_t) which was used to deal with bignum calculations and been deleted now.  
**sm2.h** The master include file. Includes all main functions.  
**sm2calculators.h** Header of sm2calculators.cpp.  
**sm2config.h** Header of sm2config.cpp.  
**sm2encrypt.h** Header of sm2encrypt.cpp.  
**sm2init.h** Header of sm2init.cpp.  
**sm2KDF.h** Header of sm2KDF.cpp.  
**sm2keymgr.h** Header of sm2keymgr.cpp.  
**sm2parameter.h** Header contains parameters used in SM2 system. Now replaced with sm2config module.  
**sm2sigment.h** Header of sm2sigment.cpp.  
**sm2StreamString.h** Header of sm2StreamString.cpp.  
**sm2type.h** Header contains self defined types of my SM2 system.  
**sm3.h** SM3 project by qq_36298219 and you can find them on [here](https://blog.csdn.net/qq_36298219/article/details/85926747).  

### Libraries ./lib/

**libmiracl.a** Complied [MIRACL](https://github.com/miracl/MIRACL) lib file.   

## Feature
The function of My-SM2 now is very limited and most of it functions is not tested.  
You will be requested for plenty parameters to run the program, and finally, it will returns a pair of keys for signment when "Sing up" function is choosen or result for verifying when user chooses "Verify function".  

## Useage

### General setting
sm2config.h includes all the configue functions in My-SM2. You needn`t to include it manually as My-SM2 designed to use a single include file for everything.  

### Crypto Usage
Include the sm2encrypt.h should do it.  

### Signature Usage
Not tested yet. If it`s finished, include the sm2sigment.h should do it.  

### Full Usage
Include the sm2.h should do it.

## New
For my new learning term of 2021-2020-2, this repo will be reused for designing a user identificating system.  
Some new features may be added. I`m considering use the system as a simple demo of My-SM2.  

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
