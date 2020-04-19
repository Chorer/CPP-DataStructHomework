#include "head.h"

int fileinput(char *filename,string str[])
{
    ifstream infile(filename,ios::in);   // 定义输入文件流对象 infile
    if(!infile)    // 如果文件不存在
    {
        cout<<"文件打开出错！"<<endl;
        exit(1);
    }
    int i = 0;
    while(!infile.eof())     // 逐行获取表达式，存入数组中
    {
        getline(infile,str[i]);
        ++i;
    }
    infile.close();
    return i;     // 返回表达式个数
}
