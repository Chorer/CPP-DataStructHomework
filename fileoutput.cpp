#include "head.h"

int fileoutput(char *filename,string str[],float result[],bool isError[],int length)
{
    ofstream outfile(filename,ios::out);   // 定义输出文件流对象 outfile
    if(!outfile)    // 如果文件不存在
    {
        cout<<"文件打开出错！"<<endl;
        exit(1);
    }
    outfile<<"***表达式及其运算结果***\n";
    outfile<<endl;
    for(int i = 0;i<length;i++)     // 遍历数组进行输出
    {
        if(isError[i])
        {
            outfile<<str[i]<<" = "<<"该表达式错误"<<endl;
            cout<<str[i]<<" = "<<"该表达式错误"<<endl;
        }
        else{
            outfile<<str[i]<<" = "<<result[i]<<endl;
            cout<<str[i]<<" = "<<result[i]<<endl;
        }
    }
    outfile.close();
    cout<<endl<<endl;
    return 0;
}
