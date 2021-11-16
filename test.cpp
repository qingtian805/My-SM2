#include<iostream>
#include"sm3.c"
#include<string.h>

using std::cout;
using std::endl;

void streamToString(unsigned char* stream,int streamlength, char* string){
    //sm3补足函数，将比特流转换为字符串
    //stream,streamlength -> string
    //转换后的字符串存于相同字符串，请确保字符串长度为原长度的两倍+1
    //长度为32字节的流需要65字节长的字符串
    int pm,pr;
    string[streamlength * 2] = '\0';
    pr = streamlength * 2 - 1;
    for(pm = streamlength - 1; pm>=0; pm--)
    {
        string[pr] = stream[pm] & 0b00001111;//低4位
        if(string[pr] > 9)
            string[pr] = string[pr] + 55;
        else
            string[pr] = string[pr] + '0';
        pr--;
        string[pr] = stream[pm] >> 4;//高4位
        if(string[pr] > 9)
            string[pr] = string[pr] + 55;
        else
            string[pr] = string[pr] + '0';
        pr--;
    }
}
void stringToStream(char* string,int stringlen,unsigned char* stream)
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
        if(string[pr] > '9')
        {
            stream[pm] = string[pr] - 55; 
        }
        else
        {
            stream[pm] = string[pr] - '0';
        }
        pr--;
        if(string[pr] > '9')
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