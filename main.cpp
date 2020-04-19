#include "head.h"

extern int fileinput(char *filename,string str[]);   // 引入读文件函数
extern int fileoutput(char *filename,string str[],float result[],bool isError[],int length); // 引入写文件函数
extern void CreateArray(EXPnode *arraya[],int length);          // 引入创建数组的函数
extern void strTolist(string str,LinkEXP &E);          // 引入字符串存链表的函数
extern void midToback(LinkEXP &L,LinkQueue &Q);        // 引入中缀表达式转后缀表达式的函数
extern void computed(LinkQueue &Q,float &result,bool &isError);    // 引入后缀表达式计算函数
extern bool isOprAll(char ch);                         // 引入操作数和运算符判断函数
extern float strTofl(string str);                    // 引入字符串转浮点数的函数

extern void ListOutput(LinkEXP &L);     // 引入功能一测试函数
extern void ListOutput2(LinkQueue &Q);        // 引入功能二测试函数


int main()           // 主函数
{
    // 输入
    int expNum;  // 存放读入的表达式个数
    string orignstr[MAXEXP];  // 存放读入的表达式
    expNum = fileinput("input.txt",orignstr);  //读取 input.txt 文件，将表达式存放在数组中，再返回表达式个数

    // 功能一：拆分表达式
    EXPnode *arrayList[expNum];     // 创建存放各个链表的数组
    CreateArray(arrayList,expNum);     // 初始化存放各个链表的数组
    for(int i=0;i<expNum;i++)     // 将每一个表达式分离的结果分别放在数组的每一个链表里
    {
        strTolist(orignstr[i],arrayList[i]);
    }

    // 功能二：中缀表达式转后缀表达式
    LinkQueue arrayList2[expNum];    // 创建存放各个队列的数组
    for(int i=0;i<expNum;i++)
    {
        LinkQueue Q;                 // 初始化存放后缀表达式的队列
        InitQueue(Q);
        midToback(arrayList[i],Q);    // 将传进去的中缀表达式转为后缀表达式
        arrayList2[i] = Q;           // 将转化得到的后缀表达式赋值给数组元素
    }

    // 测试功能一和功能二
    /*for(int i=0;i<expNum;i++)
    {
        cout<<"这是第"<<i+1<<"行的中缀表达式："<<endl;
        ListOutput(arrayList[i]);
        cout<<"这是第"<<i+1<<"行中缀表达式对应的后缀表达式："<<endl;
        ListOutput2(arrayList2[i]);
    }*/

    // 功能三：计算后缀表达式
    bool isError[expNum];
    float result[expNum];
    for(int i=0;i<expNum;i++)
    {
        if(arrayList2[i].front -> next -> error == true){
            isError[i] = true;
        }
        else {
            isError[i] = false;
            computed(arrayList2[i],result[i],isError[i]);
        }
    }

    // 输出最终结果
    cout<<"*** 表达式及其运算结果 ***："<<endl<<endl;
    fileoutput("output.txt",orignstr,result,isError,expNum);    // 输出结果到目标文件和终端上
    cout<<"请到 output.txt 文件中查看表达式运算结果"<<endl;

    system("pause");     // 暂停
    return 0;
}
