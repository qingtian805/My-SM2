#include<iostream>
#include"sm3.c"
#include<string.h>

using std::cout;
using std::endl;

void streamToString(unsigned char* stream,int streamlength, unsigned char* string){
    //sm3补足函数，将比特流转换为字符串
    //stream,streamlength -> string
    //转换后的字符串存于相同字符串，请确保字符串长度为原长度的两倍+1
    //长度为32字节的流需要65字节长的字符串
    int pm,pr;
    stream[streamlength * 2] = '\0';
    pr = streamlength * 2 - 1;
    for(pm = streamlength - 1; pm>=0; pm--)
    {
        string[pr] = stream[pm] & 0b00001111;//低4位
        if(string[pr] > 9)
            stream[pr] = string[pr] + 55;
        else
            string[pr] = string[pr] + '0';
        pr--;
        string[pr] = stream[pm] >> 4;//高4位
        if(string[pr] > 9)
            stream[pr] = string[pr] + 55;
        else
            string[pr] = string[pr] + '0';
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
    streamToString(origin,32,origin);
    cout << origin <<endl;
    //toNegative(res);
    //streamToString(res);
    //cout << res << endl;
    return 0;
}