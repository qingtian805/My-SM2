#include "sm2StreamString.h"

#ifdef __SM2_DEBUG_SS__
#include<iostream>
#include"sm3.c"
using namespace std;
#endif

using namespace SM2;

void SM2::streamToString(byte* stream,int streamlen, char* string)
{
    int pm,pr;
    string[streamlen * 2] = '\0';
    pr = streamlen * 2 - 1;
    
    for(pm = streamlen - 1; pm>=0; pm--)
    {
        string[pr] = stream[pm] & 0b00001111;//低4位
        if(string[pr] > 9)
        {
            string[pr] = string[pr] + 55;//55='A'-10
        }
        else
        {
            string[pr] = string[pr] + '0';
        }
        pr--;
        string[pr] = stream[pm] >> 4;//高4位
        if(string[pr] > 9)
        {
            string[pr] = string[pr] + 55;//55='A'-10
        }
        else
        {
            string[pr] = string[pr] + '0';
        }
        pr--;
    }
}

void SM2::stringToStream(char* string,int stringlen,unsigned char* stream)
{
    int pm,pr;
    if((stringlen % 2)!=0)
    {
        pm = stringlen/2;
    }
    else
    {
        pm = stringlen/2 - 1;
    }
    pr = stringlen - 1;

    while(pm >= 0){
        if(string[pr] > '9')//低4位
        {
            stream[pm] = string[pr] - 55;//55 = 'A'-10
        }
        else
        {
            stream[pm] = string[pr] - '0';
        }
        pr--;
        if(string[pr] > '9')//高4位
        {
            stream[pm] = stream[pm] | (string[pr] - 55) << 4;
        }
        else
        {
            stream[pm] = stream[pm] | (string[pr] - '0') << 4;
        }
        pr--;
        pm--;
    }
}

#if DEBUG
using namespace std;
int main(){
    unsigned char b[10] = "abcdefghi";
    unsigned char res[65] = "";
    char origin[65] = "";
    unsigned char test[65] = "";
    sm3(b,9,res);
    streamToString(res,32,origin);
    cout << origin <<endl;
    stringToStream(origin,64,test);
    cout << origin << endl;
    
    return 0;
}
#endif