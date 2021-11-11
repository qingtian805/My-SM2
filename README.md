# My-SM2

**TO BUILD A SM2 BASED ON C/C++** is my 2021 Summer Pratical Training topic.  
This repo stores its code.  

## Introduction of files

### Prime Folder ./

**README.md** Common ReadMe file.  
**notes.md** A studing note of basic knowledge.  
**sm2core.c** The main file of my project, contines the final version of My-SM2.  
**sm2.cpp** Will be developed as an advanced version of my sm2. All functions will be splited. In order to reuse for multiple times and reduce code length.  
**test.cpp** Test file for streamToString() and toNegative() function.  
**TOTP.cpp** C++ version of TOTP using SM3.  

### ./include/

**sm2.hpp** In assist of sm2.cpp, making My-SM2 an more advanced version.  
**sm3.c sm3.h** SM3 project by another coder and you can find them on [here](https://blog.csdn.net/qq_36298219/article/details/85926747).  
**mir\*.\*** [MIRACL](https://github.com/miracl/MIRACL) lib files. Using for educational propose. In replace of [uint256_t](https://github.com/calccrypto/uint256_t) which was used to deal with bignum calculations.  

### ./lib/

**miracl.a** Complied MIRACL lib file.

## Feature
As for limited time and my poor code ability, the function of My-SM2 is very limited which means it can only verify keys.  
You will be requested for plenty parameters to run the program, and finally, it will returns a pair of keys for signment when "Sing up" function is choosen or result for verifying when user chooses "Verify function".  

## New
For my new learning term of 2021-2020-2, this repo will be reused for designing a user identificating system.  
Some new features may be added.  

### Developing

A TOTP for future use. FINISHED
Split functions and use my newly learned techniques to build a new version EASIER DEBUGING and MORE FLEXABLE for reuse.  