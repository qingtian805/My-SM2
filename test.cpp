#include<iostream>
#include"sm3.c"
#include<string.h>

using std::cout;
using std::endl;

void streamToString(unsigned char* stream){
    int pm,pr;
    pr = 63;
    for(pm = 31; pm>=0; pm--)
    {
        stream[pr] = stream[pm] & 0b00001111;//低4位
        stream[pr] = stream[pr] + '0';
        if(stream[pr] > '9'){
            stream[pr] = stream[pr] + 7;
        }
        pr--;
        stream[pr] = stream[pm] >> 4;//高4位
        stream[pr] = stream[pr] + '0';
        if(stream[pr] > '9'){
            stream[pr] = stream[pr] + 7;
        }
        pr--;
    }
}

void toNegative(unsigned char* stream)
{
    int length, i;
    length = strlen((char*)stream);
    for(i = 0;i<length;i++){
        stream[i] = ~stream[i];
    }
    stream[length - 1] = stream[length - 1] + 1;
}

int main(){
    unsigned char b[10] = "abcdefghi";
    unsigned char res[65] = "";
    unsigned char origin[65] = "";
    sm3(b,11,res);
    strcpy((char*)origin,(char*)res);
    streamToString(origin);
    cout << origin <<endl;
    toNegative(res);
    streamToString(res);
    cout << res << endl;
    return 0;
}