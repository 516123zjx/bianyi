#pragma once
#include "symtab.h"
#include "Scan.h"
#include <iostream>


class Syntax                                   //递归子程序分析
{
private:
	TokenType Token;                        //单词Token
	Scan sExam;                             //扫描器对象
	SymTable st;
public:
	//Syntax(SemRecord* p1, int* p2, double* p3, int* p4);
	Syntax() {
		st.Init(sExam.keymap, sExam.Imap, sExam.Cmap, sExam.Fmap);
	}
	void Parse();

protected:
	void ERR_EXI();
	void PROGRAM();                          //主程序           
	void FUN_BODY();                         //函数体
	void VARIABLE();                         //变量说明
	void ID_SEQUENCE(string str);                      //标识符表
	void TYPE();                             //类型定义
	void EVA_SENTENCE();                     //赋值语句
	void EXPRESSION();                       //表达式
	void ITEM();                             //项
	void DIVISOR();                          //因子
	void IF_CONDITION();                     //if条件
	void ELSE_CONDITION();                   //else条件
	void WHILE_LOOP();                       //while循环
	void RELATION_SEQUENCE();                //关系式列表
	void STATEMENT();                        //陈述语句
//    int map(int code);                       
};


void Syntax::ERR_EXI() {
	cout << "error!" << endl;
	exit(0);
}

void Syntax::Parse()
{
	string filename;                             //文件名区

	printf("please input file name:");             //输入文件名
	cin >> filename;
	if (!sExam.openfile(filename))                 //打开文件
		return;
	Token = sExam.Read();                                    //read(w)    
	PROGRAM();
	//cout << "\n\n语法分析结果： ";
	printf("\n\nOK!\n\n");
	st.MidCode(0);
	//cout << "生成的四元式及基本块划分：\n";
	st.DividMid();
	//cout << "DAG优化后的四元式：\n";
	st.SetUp();
	st.DividMid();
	//st.OutPutMid();
	//cout << "活跃信息:\n";
	st.happily();
	st.OutPutfhb();
};

//程序
void Syntax::PROGRAM() {
	if (Token.code == 'K' && Token.value == 2) {
		st.insert(st.token_to_str(Token));
		Token = sExam.Read();
		if (Token.code == 'I') {
			st.insert(st.token_to_str(Token));
			st.JudgePush(Token);
			Token = sExam.Read();
			if (Token.code == 'P' && Token.value == 3) {
				Token = sExam.Read();
				if (Token.code == 'P' && Token.value == 4) {
					Token = sExam.Read();
					st.MidCode(1);
					if (Token.code == 'P' && Token.value == 15) {
						Token = sExam.Read();
						FUN_BODY();
						if (Token.code == 'P' && Token.value == 16) {
							
							return;
						}
						else ERR_EXI();
					}
					else ERR_EXI();
				}
				else ERR_EXI();
			}
			else ERR_EXI();
		}
		else ERR_EXI();
	}
	else ERR_EXI();
}

//函数体
void Syntax::FUN_BODY() {
	if (Token.code == 'K' && Token.value == 12) {
		IF_CONDITION();
		FUN_BODY();
	}
	else if (Token.code == 'K' && Token.value == 5) {
		WHILE_LOOP();
		FUN_BODY();
	}
	else if (Token.code == 'K') {
		VARIABLE();
		FUN_BODY();
	}
	else if (Token.code == 'I') {
		EVA_SENTENCE();
		FUN_BODY();
	}
	return;
}

//变量声明
void Syntax::VARIABLE()
{
	string str = st.token_to_str(Token);
	TYPE();
	ID_SEQUENCE(str);
	if (Token.code == 'P' && Token.value == 13) {
		Token = sExam.Read();
	}
	else
		ERR_EXI();
};

void Syntax::ID_SEQUENCE(string str)
{
	if (Token.code == 'P' && Token.value == 12)                                 //标识符
	{
		Token = sExam.Read();
		ID_SEQUENCE(str);
	}
	else if (Token.code == 'P' && Token.value == 11) {

		Token = sExam.Read();
		EXPRESSION();
		st.MidCode(6);
		ID_SEQUENCE(str);
	}
	else if (Token.code == 'I') {     //读入一个标识符就把他写进栈中，如果下一个不是等号而是分号或者逗号就pop
		// st.insert(str);
		st.insert(st.token_to_str(Token));
		st.JudgePush(Token);  //如果下一个不是等号就pop
		Token = sExam.Read();
		if (Token.code == 'P' && (Token.value == 13 || Token.value == 12))st.Smean.pop();//如果标识符后边接的是分号或者逗号
		ID_SEQUENCE(str);
	}
};

void Syntax::TYPE()///////////////////////////////////////////////////////
{
	if (Token.code == 'K') {
		switch (Token.value)
		{
		case 1:st.insert(st.token_to_str(Token)); Token = sExam.Read(); break;                                         //integer
		case 4: st.insert(st.token_to_str(Token)); Token = sExam.Read(); break; //real
		case 15:st.insert(st.token_to_str(Token)); Token = sExam.Read(); break; //////////////////////////////////////
		case 16: st.insert(st.token_to_str(Token)); Token = sExam.Read(); break;                 //bool////////////////////////////////
		default:		ERR_EXI();
		}
	}
};


void Syntax::EVA_SENTENCE()
{
	if (Token.code == 'I')                                 //标识符
	{
		st.JudgePush(Token);
		Token = sExam.Read();
		if (Token.code == 'P' && Token.value == 11)                           //:=
		{
			Token = sExam.Read();
			EXPRESSION();
			st.MidCode(6);
			if (Token.code == 'P' && Token.value == 13) {
				Token = sExam.Read();
			}
			else
				ERR_EXI();
		}
		else
			ERR_EXI();
	}
	else
		ERR_EXI();
};

//void Syntax::COM_SENTENCE()
//{
//	if (Token.code == 8)                                //begin
//	{
//		Token = sExam.Read();
//		SEN_SEQUENCE();
//		if (Token.code == 9)                            //end
//			Token = sExam.Read();
//		else
//			ERR_EXI();
//	}
//	else
//		ERR_EXI();
//};

void Syntax::EXPRESSION() {
	ITEM();
	while (Token.code == 'P' && (Token.value == 8 || Token.value == 1)) {
		int flag = Token.value;
		Token = sExam.Read();
		ITEM();
		if (flag == 8)st.MidCode(2);                        /////////////////+
		else st.MidCode(3);//   -
	}
}

void Syntax::ITEM() {
	DIVISOR();
	while (Token.code == 'P' && (Token.value == 2 || Token.value == 9)) {
		int flag = Token.value;
		Token = sExam.Read();
		DIVISOR();
		if (flag == 9)st.MidCode(4);                        /////////////////*
		else st.MidCode(5);//   
	}
}

void Syntax::DIVISOR() {
	if (Token.code == 'I' || Token.code == 'C' || Token.code == 'F') {
		st.JudgePush(Token);
		Token = sExam.Read();
		return;
	}
	else if (Token.code == 'P' && Token.value == 3) {
		Token = sExam.Read();
		EXPRESSION();
		if (Token.code == 'P' && Token.value == 4) {
			Token = sExam.Read();
			return;
		}
		else
			ERR_EXI();
	}
	else
		ERR_EXI();
}

void Syntax::IF_CONDITION() {
	if (Token.code == 'K' && Token.value == 12) {
		Token = sExam.Read();
		if (Token.code == 'P' && Token.value == 3) {
			Token = sExam.Read();
			EXPRESSION();
			RELATION_SEQUENCE();
			if (Token.code == 'P' && Token.value == 4) {
				st.MidCode(7);
				Token = sExam.Read();
				STATEMENT();
				ELSE_CONDITION();
				st.MidCode(12);
			}
			else
				ERR_EXI();
		}
		else
			ERR_EXI();
	}
	else
		ERR_EXI();
}

void Syntax::ELSE_CONDITION() {
	if (Token.code == 'K' && Token.value == 14) {
		st.MidCode(8);
		Token = sExam.Read();
		STATEMENT();
	}
}

void Syntax::WHILE_LOOP() {
	if (Token.code == 'K' && Token.value == 5) {
		st.MidCode(9);
		Token = sExam.Read();
		if (Token.code == 'P' && Token.value == 3) {
			Token = sExam.Read();
			EXPRESSION();
			RELATION_SEQUENCE();
			if (Token.code == 'P' && Token.value == 4) {
				st.MidCode(10);
				Token = sExam.Read();
				STATEMENT();
				st.MidCode(11);
			}
			else
				ERR_EXI();
		}
		else
			ERR_EXI();
	}
	else
		ERR_EXI();
}

void Syntax::STATEMENT() {
	if (Token.code == 'K' && Token.value == 12) {
		Token = sExam.Read();
		IF_CONDITION();
	}
	else if (Token.code == 'I') {
		EVA_SENTENCE();
	}
	else if (Token.code == 'K' && Token.value == 5) {
		Token = sExam.Read();
		WHILE_LOOP();
	}
	else if (Token.code == 'P' && Token.value == 15) {
		Token = sExam.Read();
		FUN_BODY();
		if (Token.code == 'P' && Token.value == 16) {
			Token = sExam.Read();
			return;
		}
		else
			ERR_EXI();
	}
	else
		ERR_EXI();
}

void Syntax::RELATION_SEQUENCE() {
	if (Token.code == 'P') {
		switch (Token.value) {
		case(5):Token = sExam.Read(); EXPRESSION(); st.MidCode(17); break;
		case(6):Token = sExam.Read(); EXPRESSION(); st.MidCode(15);  break;
		case(7):Token = sExam.Read(); EXPRESSION(); st.MidCode(14);  break;
		case(10):Token = sExam.Read(); EXPRESSION(); st.MidCode(13);  break;
		case(17):Token = sExam.Read(); EXPRESSION(); st.MidCode(16); break;
		default:return;
		}
	}
}




