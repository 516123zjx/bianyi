#pragma once

#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<vector>
#include<sstream> 
#include<stack>
#include<fstream>
#include<utility>
#include<cctype>
#include<map>
#include "Token.h"

using namespace std;
//当读入TOKEN时，判断是否是标识符，如果是标识符，它又是什么种类呢，根据其种类来确定其他信息对应的链表结构

typedef struct TYPnode* PTYnode; //类型表结点的定义
typedef struct Rnode* PTRnode; //结构体表结点的定义
typedef struct LENnode* PTLnode; //长度表结点的定义
typedef struct Cnode* PTCnode; //常量表结点的定义
typedef struct Vnode* PTVnode; //变量表结点的定义
typedef struct Pnode* PTPnode; //参数表结点的定义
typedef struct Fnode* PTFnode; //函数表结点的定义
typedef struct AInode* PTAInode; //数组表结点的定义
typedef struct SYNnode* PTSYNnode; //符号表结点的定义
typedef struct INVnode* PTINVnode; //中间变量表结点的定义
typedef struct Qnode* PTQnode; //四元式结点的定义

struct DAGNode {
	int _id;
	string _operator;
	vector<string> _mark;
	int lchild;
	int rchild;
	DAGNode() {
		_id = 0;
		_operator = "";
		lchild = -1;
		rchild = -1;
	}
};

typedef struct LENnode {
	int Len;//长度大小
	PTLnode LNext;//指向下一个邻接点
}LENNode;

typedef struct Cnode {
	double Len;    //常量大小
	PTCnode CNext;//指向下一个邻接点
}CNode;

typedef struct Vnode {
	int Vdeep;    //所在层数
	int Vstart;    //存储的开始单元
	PTVnode VNext;//指向下一个邻接点
}VNode;

typedef struct Pnode {
	string Pname;  //变量名
	PTYnode Ptype;//变量类型
	string PCat;//变量种类
	PTVnode Pvar;   //变量的分配
	PTPnode PNext;//指向下一个邻接点
}PNode;

typedef struct Fnode {
	int deep;//函数层数
	int F_off;//该过函自身数据区起始单元相对该过函值区区头位置
	int FN; //参数个数
	PTSYNnode entry;                                 ///////////////////////////ywf///那个entry
	PTPnode	Paral;//指向参数表
	PTFnode FNext;//指向下一个邻接点
}FNode;
//以上为五种其他信息表

typedef struct Rnode {
	string Id;//结构域名
	int Off;//区距
	PTYnode IdType;//id的成分类型
	PTRnode RNext;//指向下一个邻接点    
}RNode;

typedef struct AInode {
	int SubScr;  //下标   下标对应小的//c语言自动设成0
	int SuperScr;  //上标
	PTYnode AType;//数组对象的类型，指向类型表
	int UnitLen;   //一个数组单元的长度
	PTAInode ANext;//指向下一个数组表邻接点    
}AINode;

typedef struct TYPnode {
	char type;//类型
	PTAInode PTAinfl;      //指向数组表头指针 
	PTRnode PTRinfl;//指向结构表头指针  ;设置一个类型表的头节点，里面不存type，只存一个数组表头指针和一个结构表头指针，这两个头指针用于找到两个数据结构
	PTYnode TNext;//指向下一个邻接点    
}TYPNode;

typedef struct SYNnode {
	string TName;//标识符源码
	PTYnode Type; //该token标识符类型,指向类型表，类型表前几个已经定义好了，对应着基本类型i,b,......
	string TCat;      //该token种类  
	union {
		PTLnode Lform;//指向其他信息表
		PTCnode Cform;
		PTVnode Vform;
		PTFnode Fform;
	};
	char AInformation;                //该标识符的活跃信息
	PTSYNnode SNext;//指向下一个邻接点
}SYNNode;

typedef struct INVnode {
	string IName;//中间变量名 
	string IType;//中间变量名 
	PTINVnode INVNext;//指向下一个邻接点   
	char AInformation;                //该标识符的活跃信息
}INVNode;

typedef struct Qnode {   //四元式节点
	string Operater;
	string Obj1;
	//char Information1; //运算数1的活跃信息
	string Obj2;
	//char Information2; //运算数2的活跃信息
	string result;
	//char Information3;  //结果单元的活跃信息
	char Position;     //该四元式节点在基本块里的位置信息 begin end midprocess
	PTQnode QNext;//指向下一个邻接点    
}QNode;

class SymTable
{
public:
	friend class Syntax;      //友元，让语法分析部分可以调用语义分析中的成员
	SymTable();
	~SymTable();
	string token_to_str(TokenType&);
	void Init(string*, string*, string*, string*);
	void MidCode(int k);
	void ADDQNode(string op, string ob1, string ob2, string re);//增加四元式节点
	string ADDInvNode(string type);//增加中间变量节点
	string JudgeIType(string n1, string n2);//根据运算数字到符号表里寻找中间变量的类型
	string TopPop();//取得语意栈顶符号并且弹出栈顶
	void JudgePush(TokenType t1);
	void ErrorJudge1(TokenType t1);  //重定义
	void ErrorJudge2(string str);  //未定义
	/////////////ywf
	void happily();
	void insert(string token);
	PTQnode fanz(PTQnode head);
	void exc(string str, int i);
	//int numcge(string s, int i);
	PTSYNnode screat();
	void pcreat();
	void lcreat();
	void fcreat();
	void ccreat();
	void vcreat();
	void OutPutfhb();
	void OutPutMid();
	void DividMid();//划分基本块
private:
	string* keymap;
	string* Imap;        //标识符表I
	string* Cmap;        //常整数表C
	string* Fmap;
	int off_set = 5;
	PTSYNnode SYNHead;//符号表				
	PTLnode	LHead;  //长度表
	PTFnode FHead;	//函数表
	PTCnode	CHead;//常量表
	PTVnode	VHead;//变量表
	PTINVnode INVHead;//中间变量表
	int NumInvNode;//中间变量个数
	PTQnode Qhead;  //四元式头指针
	stack<string>Smean;//语义的堆栈

		//////////////////////////链表进度指针
	//int off_set = 5;
	PTYnode THead;   //类型表头
	PTSYNnode nodes;//符号表
	PTPnode nodep;	//参数表
	PTLnode	nodel;  //长度表
	PTFnode nodef;	//函数表
	PTCnode	nodec;//常量表
	PTVnode	nodev;//变量表

	vector<DAGNode> m_dag;
	map<string, int> m_map;
	map<string, int> m_ope;
protected:
	DAGNode& Find_cur(int id) {
		for (auto it = m_dag.begin(); it != m_dag.end(); it++) {
			if (it->_id == id) {
				return (*it);
			}
		}
	}

	void Erase(string str) { //清除已有节点
		if (!m_map.count(str))return;
		int tmp = m_map.find(str)->second;
		if (Find_cur(tmp)._mark[0] != str) {
			for (auto it = Find_cur(tmp)._mark.begin(); it != Find_cur(tmp)._mark.end(); it++) {
				if ((*it) == str) {
					Find_cur(tmp)._mark.erase(it);
					break;
				}
			}
		}
	}

	void Add_Edge(Qnode node) { //添加边及结点
		if (node.Operater == "=") {
			if (m_map.count(node.Obj1)) {
				Erase(node.result);
				DAGNode tmp = Find_cur(m_map.find(node.Obj1)->second);
				tmp._mark.push_back(node.result);
				if (tmp._mark[0][0] == 't' && node.result[0] != 't') {
					auto it = tmp._mark.end();
					it--;
					swap(Find_cur(m_map.find(node.Obj1)->second)._mark[0], *(it));
				}
				m_map.insert(make_pair(node.result, tmp._id));
			}
			else {
				DAGNode tmp;
				tmp._id = m_dag.size();
				tmp._mark.push_back(node.Obj1);
				tmp._mark.push_back(node.result);
				//cout << node.result << endl;
				//cout << node.Obj1 << "   " << node.result << endl;
				Erase(node.result);
				m_dag.push_back(tmp);
				m_map.insert(make_pair(node.Obj1, tmp._id));
				m_map.insert(make_pair(node.result, tmp._id));
			}
		}
		else if (m_ope.count(node.Operater)) {
			if (!m_map.count(node.Obj1)) {
				DAGNode tmp;
				tmp._id = m_dag.size();
				//cout << tmp._id<<"   ";
				tmp._mark.push_back(node.Obj1);
				m_dag.push_back(tmp);
				m_map.insert(make_pair(node.Obj1, tmp._id));
			}
			if (!m_map.count(node.Obj2)) {
				DAGNode tmp;
				tmp._id = m_dag.size();
				//cout << tmp._id << endl;
				tmp._mark.push_back(node.Obj2);
				m_dag.push_back(tmp);
				m_map.insert(make_pair(node.Obj2, tmp._id));
			}
			if (isdigit(node.Obj1[0]) && isdigit(node.Obj2[0])) { 
				float obj1 = stof(node.Obj1);
				float obj2 = stof(node.Obj2);
				if (node.Operater == "+")
					obj1 += obj2;
				else if (node.Operater == "-")
					obj1 -= obj2;
				else if (node.Operater == "*")
					obj1 *= obj2;
				else if (node.Operater == "/")
					obj1 /= obj2;
				else if (node.Operater == "==")
					obj1 = (obj1 == obj2);
				else if (node.Operater == "<=")
					obj1 = (obj1 <= obj2);
				else if (node.Operater == ">=")
					obj1 = (obj1 >= obj2);
				else if (node.Operater == "<")
					obj1 = (obj1 < obj2);
				else if (node.Operater == ">")
					obj1 = (obj1 > obj2);
				string str = to_string(obj1);
				if (m_map.count(str)) {
					Erase(node.result);
					DAGNode tmp = Find_cur(m_map.find(str)->second);
					tmp._mark.push_back(node.result);
					if (tmp._mark[0][0] == 't' && node.result[0] != 't') {
						auto it = tmp._mark.end();
						it--;
						swap(Find_cur(m_map.find(node.Obj1)->second)._mark[0], *(it));
					}
					m_map.insert(make_pair(node.result, tmp._id));
				}
				else {
					DAGNode tmp;
					tmp._id = m_dag.size();
					tmp._mark.push_back(str);
					tmp._mark.push_back(node.result);
					Erase(node.result);
					m_dag.push_back(tmp);
					m_map.insert(make_pair(str, tmp._id));
					m_map.insert(make_pair(node.result, tmp._id));
				}
			}
			else {
				bool flag = false;
				for (auto it = m_dag.begin(); it != m_dag.end(); it++) {
					if (m_map.count(node.Obj1) && m_map.count(node.Obj2)) {
						if ((*it)._operator == node.Operater && (*it).lchild == m_map.find(node.Obj1)->second &&
							(*it).rchild == m_map.find(node.Obj2)->second && (*it).lchild != -1 && (*it).rchild != -1) {
							Erase(node.result);
							it->_mark.push_back(node.result);
							if (it->_mark[0][0] == 't' && node.result[0] != 't') {
								auto it2 = it->_mark.end();
								it2--;
								swap(Find_cur(m_map.find(node.Obj1)->second)._mark[0], *(it2));
							}
							m_map.insert(make_pair(node.result, it->_id));
							flag = true;
							break;
						}
					}
				}
				if (!flag) {
					//cout << 1 << endl;
					DAGNode tmp;
					tmp._id = m_dag.size();
					tmp._operator = node.Operater;
					tmp.lchild = m_map.find(node.Obj1)->second;
					tmp.rchild = m_map.find(node.Obj2)->second;
					//cout << tmp.lchild << "   " << tmp.rchild << endl;
					tmp._mark.push_back(node.result);
					Erase(node.result);
					m_dag.push_back(tmp);
					m_map.insert(make_pair(node.result, tmp._id));

				}
			}
		}
	}
public:
	void SetUp() { //生成DAG图并进行优化
		Qnode* pq = NULL;
		if (Qhead->QNext) {
			pq = Qhead->QNext;
		}
		Qnode* pq_replace_head = new Qnode();
		Qnode* pq_replace_it = pq_replace_head;
		while (pq != NULL) {
			m_dag.clear();
			m_map.clear();
			Qnode tmpq;
			while (pq && (pq->Position != 'b' && pq->Position != 'e' && pq->Position != 'm')) {
				Qnode* p = new Qnode();
				pq_replace_it->QNext = p;
				pq_replace_it = pq_replace_it->QNext;
				pq_replace_it->QNext = NULL;
				pq_replace_it->Obj1 = pq->Obj1;
				pq_replace_it->Obj2 = pq->Obj2;
				pq_replace_it->Operater = pq->Operater;
				pq_replace_it->result = pq->result;
				pq = pq->QNext;
			}
			if (!m_ope.count(pq->Operater)) {
				Qnode* p = new Qnode();
				pq_replace_it->QNext = p;
				pq_replace_it = pq_replace_it->QNext;
				pq_replace_it->QNext = NULL;
				pq_replace_it->Obj1 = pq->Obj1;
				pq_replace_it->Obj2 = pq->Obj2;
				pq_replace_it->Operater = pq->Operater;
				pq_replace_it->result = pq->result;
				pq = pq->QNext;
				continue;
			}
			//Qnode* pq_replace_per = Qhead;
			if (pq->Position == 'b') {
				//while (pq_replace_per->QNext != pq) {
				//	pq_replace_per = pq_replace_per->QNext;
				//}
				Add_Edge(*pq);
				pq = pq->QNext;
			}
			while (pq && pq->Position != 'b') {
				if (!m_ope.count(pq->Operater)) {
					tmpq.Obj1 = pq->Obj1;
					tmpq.Obj2 = pq->Obj2;
					tmpq.Operater = pq->Operater;
					tmpq.result = pq->result;
					pq = pq->QNext;
					continue;
				}
				Add_Edge(*pq);
				pq = pq->QNext;
			}
			for (auto it = m_dag.begin(); it != m_dag.end(); it++) {
				if (it->lchild == -1 && it->rchild == -1 && it->_mark.size() > 1) {
					auto it2 = it->_mark.begin();
					it2++;
					for (; it2 != it->_mark.end(); it2++) {
						if ((*it2)[0] != 't') {
							Qnode* p = new Qnode();
							pq_replace_it->QNext = p;
							pq_replace_it = pq_replace_it->QNext;
							pq_replace_it->QNext = NULL;
							pq_replace_it->Operater = "=";
							pq_replace_it->result = (*it2);
							pq_replace_it->Obj1 = it->_mark[0];
							pq_replace_it->Obj2 = "_";
						}
					}
				}
				else if (it->lchild != -1 && it->rchild != -1) {
					Qnode* p = new Qnode();
					pq_replace_it->QNext = p;
					pq_replace_it = pq_replace_it->QNext;
					pq_replace_it->QNext = NULL;
					pq_replace_it->Operater = it->_operator;
					pq_replace_it->Obj1 = Find_cur(it->lchild)._mark[0];
					//cout << it->_operator << "  " << Find_cur(it->rchild)._id << endl;
					pq_replace_it->Obj2 = Find_cur(it->rchild)._mark[0];
					//cout << pq_replace_it->Obj1 << "  " << pq_replace_it->Obj2 <<"  "<< pq_replace_it->result << endl;
					pq_replace_it->result = it->_mark[0];
					if (it->_mark.size() > 1) {
						auto it2 = it->_mark.begin();
						it2++;
						for (; it2 != it->_mark.end(); it2++) {
							if ((*it2)[0] != 't') {
								Qnode* p = new Qnode();
								pq_replace_it->QNext = p;
								pq_replace_it = pq_replace_it->QNext;
								pq_replace_it->QNext = NULL;
								pq_replace_it->Operater = "=";
								pq_replace_it->result = (*it2);
								pq_replace_it->Obj1 = it->_mark[0];
								pq_replace_it->Obj2 = "_";

							}
						}
					}
				}
			}
			if (tmpq.Operater != "") {
				Qnode* p = new Qnode();
				pq_replace_it->QNext = p;
				pq_replace_it = pq_replace_it->QNext;
				pq_replace_it->QNext = NULL;
				pq_replace_it->Operater = tmpq.Operater;
				pq_replace_it->result = tmpq.result;
				pq_replace_it->Obj1 = tmpq.Obj1;
				pq_replace_it->Obj2 = tmpq.Obj2;
			}
		}
		Qhead = pq_replace_head;
	}
};


