#pragma once
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include <string.h>
#include"token.h"
using namespace std;


void trim(string& s)
{
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}
}

//---------扫描器类------------------------------

class Scan
{
public:
	int pI;
	int pC;
	int pF;
	int pH;
	int pS;
	string keymap[17];
	string Pmap[17];
	string Imap[81];        //标识符表I
	string Cmap[81];        //常整数表C
	string Fmap[81];        //浮点数表C
	char Hmap[81];		//字符常量表
	string Smap[81];		//字符串常量表
	string filename;
	string line[200];
	int p_line;				//line[p_line]
	int p_sum;				//p_line<=p_sum
public:
	Scan();
	~Scan();
	void xray(string& filename);
	void printtoken();				//调用该函数打印Token序列前必须先调用一次xray，完成填写line和p_cnt的任务
	bool openfile(string filename);
	TokenType Read();					//调用该函数打印Token序列前必须先调用一次xray
	TokenType writetoken(string str);
private:
	int jkey(string& s);               //判断字符串s是否属于关键字表keymap
	int jPmap(string& s);              //判断字符串s是否属于界符表P
	int jNum(string& s);				//判断字符串s是否是数字
	int vc(string& s);                          //看是否与已有常整数重复
	int vf(string& s);                          //看是否与已有浮点数重复
	int vi(string& s);                          //看是否与已有标识符重复
	int vh(char& c);							//看是否与已有字符常量重复
	int vs(string& s);							//看是否与已有字符串常量重复
	bool jPmap(char& c);
};

Scan::Scan()
{
	keymap[0] = "int";	keymap[1] = "void";		keymap[2] = "break";
	keymap[3] = "float";	keymap[4] = "while";	keymap[5] = "do";
	keymap[6] = "struct";	keymap[7] = "const";	keymap[8] = "case";
	keymap[9] = "for";	keymap[10] = "return";	keymap[11] = "if";
	keymap[12] = "default";		keymap[13] = "else";	keymap[14] = "char";
	keymap[15] = "bool";	keymap[16] = "string";
	Pmap[0] = "-";	Pmap[1] = "/";	Pmap[2] = "(";
	Pmap[3] = ")";	Pmap[4] = "==";	Pmap[5] = "<=";
	Pmap[6] = "<";	Pmap[7] = "+";	Pmap[8] = "*";
	Pmap[9] = ">";	Pmap[10] = "=";	Pmap[11] = ",";
	Pmap[12] = ";";	Pmap[13] = "++";	Pmap[14] = "{";
	Pmap[15] = "}"; Pmap[16] = ">=";
	pI = 1;	pC = 1;	pF = 1; pH = 1; pS = 1;
	for (int i = 0; i < 200; i++)
		line[i] = "";
	p_line = 0;
}

bool Scan::openfile(string filename) {
	ifstream indexFile;
	indexFile.open(filename, ios::in);

	if (!indexFile.is_open())
	{
		cout << "读取文件失败" << endl;
		return false;
	}
	int i = 0;
	string str = "";
	while (getline(indexFile, str))
	{
		istringstream s(str);	//按空格分割
		string temp = "";
		while (s >> temp)
		{
			string op;
			string s1 = "";
			string s2 = "";
			int t_len = temp.length();
			for (int j = 0; j < t_len; j++)
			{
				string jtemp = "";
				jtemp = temp[j];
				jtemp = jtemp + temp[j + 1];
				if ((jPmap(temp[j]) && !jPmap(temp[j + 1]) && (j + 1 < t_len) && (!jNum(jtemp))) || (jPmap(temp[j]) && jPmap(temp[j + 1]) && (j + 1 < t_len) && (!jPmap(jtemp))))
				{
					s1 = temp.substr(0, j);
					s2 = temp.substr(j + 1);
					op = temp[j];
					temp = s1 + " ";
					temp = temp + op;
					temp = temp + " ";
					temp = temp + s2;
					t_len += 2;
					j++;

				}
				else if (jPmap(temp[j]) && jPmap(temp[j + 1]) && (j + 1 < t_len))
				{
					s1 = temp.substr(0, j);
					s2 = temp.substr(j + 2);
					op = temp[j];
					op = op + temp[j + 1];
					temp = s1 + " ";
					temp = temp + op;
					temp = temp + " ";
					temp = temp + s2;
					t_len += 2;
					j += 2;
				}
				else if (jPmap(temp[j]) && j + 1 == t_len)
				{
					s1 = temp.substr(0, j);
					op = temp[j];
					temp = s1 + " ";
					temp = temp + op;
					t_len += 1;
					j++;
				}
			}
			istringstream sour(temp);	//按空格分割
			string dest = "";
			while (sour >> dest)
			{
				trim(dest);
				line[i] = dest;
				i++;
			}
		}

	}
	indexFile.close();
	p_sum = i;
	return true;
}

Scan::~Scan()
{
}

void Scan::xray(string& filename)
{
	ifstream indexFile;
	indexFile.open(filename, ios::in);
	ofstream de;		//写入的目标文件
	de.open("./file/token.txt", ios::trunc);
	de.close();
	de.open("./file/token.txt", ios::app);
	if (!indexFile.is_open())
	{
		cout << "读取文件失败" << endl;
		return;
	}

	string temp = "";	//逐行读入到line中
	TokenType token;
	int cnt = 0;	//write Scan->line
	int i = 0;
	while (getline(indexFile, temp))
	{
		string op;
		string s1 = "";
		string s2 = "";
		int t_len = temp.length();
		for (int j = 0; j < t_len; j++)
		{
			string jtemp = "";
			jtemp = temp[j];
			jtemp = jtemp + temp[j + 1];
			if ((jPmap(temp[j]) && !jPmap(temp[j + 1]) && (j + 1 < t_len)) || (jPmap(temp[j]) && jPmap(temp[j + 1]) && (j + 1 < t_len) && (!jPmap(jtemp))))
			{
				s1 = temp.substr(0, j);
				s2 = temp.substr(j + 1);
				op = temp[j];
				temp = s1 + " ";
				temp = temp + op;
				temp = temp + " ";
				temp = temp + s2;
				t_len += 2;
				j++;

			}
			else if (jPmap(temp[j]) && jPmap(temp[j + 1]) && (j + 1 < t_len))
			{
				s1 = temp.substr(0, j);
				s2 = temp.substr(j + 2);
				op = temp[j];
				op = op + temp[j + 1];
				temp = s1 + " ";
				temp = temp + op;
				temp = temp + " ";
				temp = temp + s2;
				t_len += 2;
				j += 2;
			}
			else if (jPmap(temp[j]) && j + 1 == t_len)
			{
				s1 = temp.substr(0, j);
				op = temp[j];
				temp = s1 + " ";
				temp = temp + op;
				t_len += 1;
				j++;
			}
		}
		istringstream sour(temp);	//按空格分割
		string dest = "";
		while (sour >> dest)
		{
			trim(dest);
			line[i] = dest;
			token = writetoken(line[i]);
			de << "(" << token.code << "," << token.value << "),";
			i++;

		}

		de << endl;
	}
	p_sum = cnt;
	indexFile.close();
}

bool Scan::jPmap(char& c)
{
	for (int i = 0; i < 17; i++)
	{
		if (c == Pmap[i][0])
			return 1;
	}
	return 0;
}


TokenType Scan::writetoken(string str)
{
	int flag;
	TokenType token{};

	if (jkey(str))		//如果str是关键字
	{
		flag = jkey(str);
		token.code = 'K';
		token.value = flag;
	}
	else if (jPmap(str))
	{
		flag = jPmap(str);
		token.code = 'P';
		token.value = flag;
	}
	else if (jNum(str))
	{
		flag = jNum(str);
		if (flag == 1 || flag == 2)		//整数
		{
			if (vc(str) != -1)			//已有
			{
				token.code = 'C';
				token.value = vc(str);
			}
			else
			{
				token.code = 'C';
				token.value = pC;
				Cmap[pC] = str;
				pC++;
			}
		}
		else if (flag == 3 || flag == 4)	//浮点数
		{
			if (vf(str) != -1)			//已有
			{
				token.code = 'F';
				token.value = vf(str);
			}

			else
			{
				token.code = 'F';
				token.value = pF;
				Fmap[pF] = str;
				pF++;
			}
		}
	}
	else if (str[0] == '\'' && str[str.length() - 1] == '\'' && str.length() == 3)
	{
		if (vh(str[1]) != -1)			//已有
		{
			token.code = 'H';
			token.value = vh(str[1]);
		}

		else
		{
			token.code = 'H';
			token.value = pH;
			Hmap[pH] = str[1];
			pH++;
		}
	}
	else if (str[0] == '\"' && str[str.length() - 1] == '\"' && str.length() > 3)
	{
		int strlen = str.length();
		string temp = "";
		temp = str.substr(1, strlen - 2);
		if (vs(temp) != -1)			//已有
		{
			token.code = 'S';
			token.value = vs(temp);
		}

		else
		{
			token.code = 'S';
			token.value = pS;
			Smap[pS] = temp;
			pS++;
		}
	}
	else //标识符
	{
		if (vi(str) != -1)			//已有
		{
			token.code = 'I';
			token.value = vi(str);
		}

		else
		{
			token.code = 'I';
			token.value = pI;
			Imap[pI] = str;
			pI++;
		}
	}
	cout << "(" << token.code << ",";
	cout << token.value << ")" << ",";
	return token;
}

int Scan::jkey(string& s)    //判断字符串s是否属于关键字表keymap
{
	int result = 0;
	for (int i = 0; i < 17; i++)
	{
		if (s == keymap[i])
		{
			result = i + 1;
			break;
		}
	}
	return result;
}

int Scan::jPmap(string& s)    //判断字符串s是否属于关键字表keymap
{
	int result = 0;
	for (int i = 0; i < 17; i++)
	{
		if (s == Pmap[i])
		{
			result = i + 1;
			break;
		}
	}
	return result;
}

int Scan::jNum(string& s)
{
	int len = s.length();
	int pflag = 0;
	int sflag = 0;
	if (s[0] == '-')sflag = 1;
	int i = 1;
	while (i < len && sflag == 1)
	{
		if (s[i] >= '0' && s[i] <= '9') i++;
		else if (s[i] == '.')
		{
			pflag++;
			i++;
		}
		else
		{
			sflag = 2;
			break;
		}
	}

	if (pflag == 1 && i == len && sflag == 1)return 4;	//负浮点数
	else if (sflag == 1 && i == len)return 2;			//负整数

	i = 0;
	while (i < len && sflag == 0)
	{
		if (s[i] >= '0' && s[i] <= '9') i++;
		else if (s[i] == '.')
		{
			pflag++;
			i++;
		}
		else
		{
			sflag = 2;
			break;
		}
	}
	if (pflag == 1 && i == len && sflag == 0)return 3;	//正浮点数
	else if (pflag == 0 && sflag == 0 && i == len)return 1;			//正整数

	return 0;
}

int Scan::vc(string& s)                           //看是否与已有常整数重复
{
	int flag = 1;
	for (flag = 1; flag < pC; flag++)
	{
		if (s == Cmap[flag])
			return flag;
	}
	return -1;
}

int Scan::vf(string& s)                           //看是否与已有浮点数重复
{
	int flag = 1;
	for (flag = 1; flag < pF; flag++)
	{
		if (s == Fmap[flag])
			return flag;
	}
	return -1;
}

int Scan::vi(string& s)                           //看是否与已有标识符重复
{
	int flag = 1;
	for (flag = 1; flag < pI; flag++)
	{
		if (s == Imap[flag])
			return flag;
	}
	return -1;
}

TokenType Scan::Read()
{
	TokenType token{};
	token = writetoken(line[p_line]);
	p_line++;
	return token;
}

void Scan::printtoken()
{
	ifstream indexFile;
	indexFile.open("./file/token.txt", ios::in);
	if (!indexFile.is_open())
	{
		cout << "读取文件失败" << endl;
		return;
	}
	string cline = "";
	cout << "\tToken序列" << endl;
	while (getline(indexFile, cline))
	{
		cout << cline << endl;
	}
}

int Scan::vh(char& c)                           //看是否与已有字符常量重复
{
	int flag = 1;
	for (flag = 1; flag < pH; flag++)
	{
		if (c == Hmap[flag])
			return flag;
	}
	return -1;
}

int Scan::vs(string& s)                           //看是否与已有字符串常量重复
{
	int flag = 1;
	for (flag = 1; flag < pS; flag++)
	{
		if (s == Smap[flag])
			return flag;
	}
	return -1;
}
