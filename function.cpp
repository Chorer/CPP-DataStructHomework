#include "head.h"

// 栈相关方法的实现
Status InitStack(SqStack &S) {
	S.base = new SElemType[MAXSIZE];
	if (!S.base)
		exit(OVERFLOW);
	S.top = S.base;
	S.stacksize = MAXSIZE;
	return OK;
}

Status InitStackFl(SqStackFl &S) {
	S.base = new float[MAXSIZE];
	if (!S.base)
		exit(OVERFLOW);
	S.top = S.base;
	S.stacksize = MAXSIZE;
	return OK;
}

Status Push(SqStack &S, SElemType e) {
	if (S.top - S.base == S.stacksize)
		return ERROR;
	*(S.top++) = e;
	return OK;
}

Status PushFl(SqStackFl &S, float e) {
	if (S.top - S.base == S.stacksize)
		return ERROR;
	*(S.top++) = e;
	return OK;
}

Status Pop(SqStack &S, SElemType &e) {
	if (S.base == S.top)
		return ERROR;
	e = *(--S.top);
	return OK;
}

Status PopFl(SqStackFl &S, float &e) {
	if (S.base == S.top)
		return ERROR;
	e = *(--S.top);
	return OK;
}

// 队列相关方法的实现
Status InitQueue(LinkQueue &Q) {         // 初始化队列
	Q.front = Q.rear = new QNode;
	Q.front->next = NULL;
	return OK;
}

Status EnQueuefl(LinkQueue &Q, float e) {    // 操作数入队
	QueuePtr p;
	p = new QNode;
	p->operand = e;     // 设置浮点数域
	p->judge = false;    // 设置判断域，false 表示存储操作数
	p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	return OK;
}

Status EnQueuech(LinkQueue &Q, char e) {    // 运算符入队
	QueuePtr p;
	p = new QNode;
	p->operation = e;   // 设置符号域
	p->judge = true;   // 设置判断域,true 表示存储运算符
	p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	return OK;
}


Status DeQueuefl(LinkQueue &Q, float &e) {    // 操作数出队
	QueuePtr p;
	if (Q.front == Q.rear)
		return ERROR;
	p = Q.front->next;
	e = p->operand;       // 返回出队的操作数给 e
	Q.front->next = p->next;
	if (Q.rear == p)
		Q.rear = Q.front;
	delete p;
	return OK;
}

Status DeQueuech(LinkQueue &Q, char &e) {    // 运算符出队
	QueuePtr p;
	if (Q.front == Q.rear)
		return ERROR;
	p = Q.front->next;
	e = p->operation;         // 返回出队的运算符给 e
	Q.front->next = p->next;
	if (Q.rear == p)
		Q.rear = Q.front;
	delete p;
	return OK;
}



// 主要功能一：将字符串按照操作数和运算符进行分离
void strTolist(string str,LinkEXP &E)
{
    str = str+"#";              // # 作为字符串结束的标记
    LinkEXP r = E;
    string Stroper;               // Stroper 存放每次遍历到的操作数字符串
    float  Floper;            // Floper 存放操作数字符串对应的浮点数
    int i = 0;
    while(i<str.length())
    {
        if(!isOprAll(str[i]))     // 如果是操作数
        {
            Stroper = Stroper + str[i];       // 不是运算符就进行拼接
        }
        else                         // 如果是运算符
        {
            if(Stroper.length()!=0)   // 保存上次扫描到的操作数
            {
                Floper = strTofl(Stroper);    // 操作数字符串转换成浮点数
                r->next = new EXPnode;      // 生成结点追加到链表后面
                r = r->next;
                r->next = NULL;
                r->operand = Floper;         // 设置结点数据域
                r->judge = false;         // 设置结点数据域，表明是操作数
            }
            Stroper = "";   // 重置存放操作数字符串的变量
            if(str[i]!='#')         // # 只作为结束标记，方便末尾操作数可以成功存入，# 本身不存入结点
            {
                r->next = new EXPnode;      // 生成结点追加到链表后面
                r = r->next;
                r->next = NULL;
                r->operation = str[i];      // 设置结点数据域
                r->judge = true;            // 设置结点数据域，表明是运算符
            }
        }
        i++;
    }
}


// 主要功能二：将中缀表达式转换为后缀表达式
void midToback(LinkEXP &L,LinkQueue &Q)
{
    SqStack S;
    InitStack(S);    // 初始化存放运算符的栈
    SElemType el;     // 初始化存放出栈运算符的变量
    EXPnode* p;
	p = L->next;
	while(p!=NULL)     // 开始遍历链表
    {
        if(p->judge == false)    // 如果是操作数，直接进队列
        {
            EnQueuefl(Q, p->operand);
        } else if(p->judge == true && p->operation == ')')   // 如果是运算符中的右括号
        {
            if(S.top == S.base)     // 如果是右括号且栈空，则表达式不合法。存放错误信息到队列中，并直接返回
            {
                EnQueuech(Q, p->operation); Q.front->next->error = true; return;
            }
            while((*(S.top - 1) != '(') && (S.top != S.base))   // 如果不是空栈，则不断出栈直到遇到左括号
            {
                Pop(S, el);    // 栈顶运算符弹出
                EnQueuech(Q,el);  // 弹出的运算符入队
            }
            if(*(S.top - 1) == '(')    // 有左括号
            {
                Pop(S, el);
            } else if(S.top == S.base)   // 无左括号
            {
                Q.front->next->error = true; return;
            }
        } else if(isOpr(p->operation) || p->operation == '(')   // 如果是其它运算符
        {
            if(S.top == S.base)
            {
                Push(S,p->operation);  // 如果栈空，直接进
            } else                      // 如果栈非空，不能急着进，要先判断优先级是否大于栈顶元素的优先级
            {
                // 左括号当且仅当遇到右括号时才出栈
                while( S.top != S.base && *(S.top-1) != '(' && isTopHigher(*(S.top - 1), p->operation))
                {
                    Pop(S, el);    // 栈顶运算符弹出
                    EnQueuech(Q,el);  // 弹出的运算符入队
                }
                Push(S,p->operation);                // 符合条件，才可以进栈
            }

        }
        p = p->next;
    }
    while(S.top != S.base)     // 如果栈不空，则让剩余的所有元素出栈，进队列
    {
        /*
        如果栈中剩余元素还有左括号，则表达式必缺少匹配的右括号（因为如果不缺，则之前就应该已经出栈了，不会留到现在）
        */
        if(*(S.top - 1) == '(') Q.front->next->error = true;
        Pop(S, el);
        EnQueuech(Q,el);
    }
}


// 主要功能三：计算后缀表达式
void computed(LinkQueue &Q,float &result,bool &isError)
{
    QueuePtr q;
	q = Q.front->next;
    SqStackFl s;      // 存放操作数的栈
    InitStackFl(s);
    float left,right;   // 左右操作数
	while(q!=NULL)
    {
        if(q->judge == false)  // 如果是操作数
        {
            PushFl(s,q->operand);
        }
        else {                      // 如果是运算符
            PopFl(s,right);    // 取出栈顶两个操作数
            PopFl(s,left);
            switch(q->operation)
            {
            case '+':
                result = left+right;
                break;
            case '-':
                result = left-right;
                break;
            case '*':
                result = left*right;
                break;
            case '/':
                if(right == 0){           // 如果除数为 0
                    isError = true;
                    return;
                }
                else {
                    result = left/right;
                }
                break;
            case '%':
                if(left != (int)left || right != (int)right)      //如果是小数
                {
                    isError = true;
                    return;
                }
                result = (float)((int)left%(int)right);
                break;
            case '^':
                if(right != (int)right || (left==0 && right==0) || (left==0 && right<0)) // 错误情况判断
                {
                    isError = true;
                    return;
                }
                else {
                   result = pow(left,right);
                }
                break;
            }
            PushFl(s,result);
        }
        q = q->next;
    }
    PopFl(s,result);
}


// 其它功能：构造存放各个链表的数组
void CreateArray(EXPnode *arraya[],int length)
{
  for(int i=0;i<length;i++)
  {
      EXPnode *L = new EXPnode;
      L->next = NULL;
      arraya[i] = L;
  }
}

// 其他功能：将字符串操作数转换为浮点型操作数
float strTofl(string str)
{
    return atof(str.c_str());
}


// 其他功能：判断是否是运算符（不包括括号的判断）
bool isOpr(char ch)
{
    if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '%')
        return true;          //是 +-*/^% 则返回true
    return false;             //否则返回false
}

// 其他功能：判断是否是运算符的方法（包括括号的判断）
bool isOprAll(char ch)
{
    if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '%' || ch == '(' || ch == ')' || ch == '#')
        return true;          //是 +-*/^% ( )则返回true
    return false;             //否则返回false
}

bool isOprAll(string ch)
{
    if(ch == "+" || ch == "-" || ch == "*" || ch == "/" || ch == "^" || ch == "%" || ch == "(" || ch == ")")
        return true;          //是 +-*/^% ( )则返回true
    return false;             //否则返回false
}


// 其他功能：判断运算符优先级
bool isTopHigher(char top, char in)   // 当栈顶元素优先级高于即将进栈元素优先级时，返回true并出栈，但注意左括号是例外
{
    if(top == '^' && (in != '('))         //因为^这个优先级最大，只要栈顶为^，一定返回true就是出栈
        return true;
    if(top == '(' && (in == '*' || in == '/' || in == '+' || in == '-' || in == '%' || in == '^'))
        return true;
    if(top == '*' && (in == '*' || in == '/' || in == '+' || in == '-' || in == '%'))
        return true;
    if(top == '/' && (in == '*' || in == '/' || in == '+' || in == '-' || in == '%'))
        return true;
    if(top == '%' && (in == '*' || in == '/' || in == '+' || in == '-' || in == '%'))
        return true;
    if(top == '+' && (in == '+' || in == '-'))
        return true;
    if(top == '-' && (in == '+' || in == '-'))
        return true;
    return false;
}

// 其它功能：功能一测试函数
void ListOutput(LinkEXP &L) //功能一检测：输出拆分后的中缀表达式
{
	EXPnode* p;
	p = L->next;
	while(p!=NULL)
    {
        // 注意这里应该用 judge 来判断，不然无法识别0
        if(p->judge == false) cout<<"操作数："<<" "<<p->operand<<endl;
        if(p->judge == true) cout<<"运算符："<<" "<<p->operation<<endl;
        p=p->next;
    }
    cout<<endl;
}

// 其它功能：功能二测试函数
void ListOutput2(LinkQueue &Q) ////功能二检测：输出后缀表达式
{
    QueuePtr q;
	q = Q.front->next;
	while(q!=NULL)
    {
        if(q->error == true)
        {
            /*
            表达式不合法，直接跳过当前轮（不合法的表达式的特征是，队列第一个结点error域为true）
            */
            cout<<"表达式不合法，这里不会输出后缀表达式"<<endl;
            break;
        }
        if(q->judge == false) cout<<"操作数："<<" "<<q->operand<<endl;
        if(q->judge == true) cout<<"运算符："<<" "<<q->operation<<endl;
        q = q->next;
    }
    cout<<endl;
}



