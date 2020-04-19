#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <math.h>
using namespace std;


#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define MAXSIZE 100
#define MAXEXP 20

typedef int Status;
typedef char SElemType;

// 顺序栈结构定义
typedef struct {
    SElemType *base;
    SElemType *top;
    int stacksize;
} SqStack;

typedef struct {
	float *base;
	float *top;
	int stacksize;
} SqStackFl;


// 链队列结构定义
typedef struct QNode {
	float operand;            // 存放操作数
    char operation;           // 存放运算符
    bool error;               // 判断表达式是否不合法
    bool judge;               //  判断存放的是操作数还是运算符
    struct QNode *next;       //   指向下一结点
} QNode, *QueuePtr;
typedef struct {
	QueuePtr front; //队头指针
	QueuePtr rear; //队尾指针
} LinkQueue;

// 链表结构定义
typedef struct EXPnode {
    float operand;            // 存放操作数
    char operation;           // 存放运算符
    bool judge;               //  判断存放的是操作数还是运算符
    struct EXPnode *next;     //   指向下一结点
} EXPnode,*LinkEXP;


// 栈相关方法的声明
Status InitStack(SqStack &S);     // 栈的初始化
Status Push(SqStack &S, SElemType e);    // 入栈
Status Pop(SqStack &S, SElemType &e);      // 出栈

Status InitStackFl(SqStackFl &S);     // 栈的初始化
Status PushFl(SqStackFl &S, float e);    // 入栈
Status PopFl(SqStackFl &S, float &e);      // 出栈


// 队列相关方法的声明
Status InitQueue(LinkQueue &Q);     // 队列的初始化
Status EnQueuefl(LinkQueue &Q, float e);    // 操作数入队
Status EnQueuech(LinkQueue &Q, char e);     // 运算符入队
Status DeQueuefl(LinkQueue &Q, float &e);    // 操作数出队
Status DeQueuech(LinkQueue &Q, char &e);    // 运算符出队



//其它方法的声明
float strTofl(string str);
bool isOpr(char ch);
bool isOprAll(char ch);
bool isLeftBrac(char ch);
bool isRightBrac(char ch);
bool isTopHigher(char top, char in);









