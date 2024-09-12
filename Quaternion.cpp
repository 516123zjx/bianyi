#include"symtab.h"

int numcge(string s, int i)
{
	string num;
	for (; s[i] <= '9' && s[i] >= '0'; i++)
	{
		num += s[i];
	}
	int n;
	n = atoi(num.c_str());
	return n - 1;
}

string SymTable::token_to_str(TokenType& token) {
	string str = "(_,";
	str[1] = token.code;
	str += to_string(token.value);
	return str;
}



void SymTable::ADDQNode(string op, string ob1, string ob2, string re) {
	string Id1, Id2, Id3;
	for (int i = 1; i < ob1.length(); i++) {
		Id1 += ob1[i];      //标识符的id
	}
	istringstream is(Id1);
	int Int_Id1;
	is >> Int_Id1;
	for (int i = 1; i < ob2.length(); i++) {
		Id2 += ob2[i];      //标识符的id
	}
	istringstream is1(Id2);
	int Int_Id2;
	is1 >> Int_Id2;
	for (int i = 1; i < re.length(); i++) {
		Id3 += re[i];      //标识符的id
	}
	istringstream is2(Id3);
	int Int_Id3;
	is2 >> Int_Id3;
	if (ob1[0] == 'I') {
		ob1 = Imap[Int_Id1];
	}
	else if (ob1[0] == 'C') {
		ob1 = Cmap[Int_Id1];
	}
	else if (ob1[0] == 'F') {
		ob1 = Fmap[Int_Id1];
	}
	if (ob2[0] == 'I') {
		ob2 = Imap[Int_Id2];
	}
	else if (ob2[0] == 'C') {
		ob2 = Cmap[Int_Id2];
	}
	else if (ob2[0] == 'F') {
		ob2 = Fmap[Int_Id2];
	}
	if (re[0] == 'I') {
		re = Imap[Int_Id3];
	}
	else if (re[0] == 'C') {
		re = Cmap[Int_Id3];
	}
	else if (re[0] == 'F') {
		re = Fmap[Int_Id3];
	}
	PTQnode W = new Qnode();  //新建四元式节点
	PTQnode P = this->Qhead;
	while (P->QNext)
	{
		P = P->QNext;
	}
	P->QNext = W;
	W->QNext = NULL;
	W->Obj1 = ob1;
	W->Obj2 = ob2;
	W->result = re;
	W->Operater = op;
}

string  SymTable::ADDInvNode(string type) {
	PTINVnode W = new INVNode();  //新建中间变量
	PTINVnode P = this->INVHead;
	while (P->INVNext)
	{
		P = P->INVNext;
	}
	P->INVNext = W;
	W->INVNext = NULL;
	W->IName = "t" + to_string(++this->NumInvNode);
	W->IType = type;
	///////////////////////////////////////////////////////ywf
	string ste, stp;
	stp = "(T," + W->IName;
	char a = nodes->Type->type;
	if (a == 'i')ste = "(K,1";
	else if (a == 'f')ste = "(K,4";
	else if (a == 'b')ste = "(K,15";
	else if (a == 'c')ste = "(K,16";
	insert(ste);
	insert(stp);
	return W->IName;
}
string SymTable::TopPop() {
	string a;
	if (!this->Smean.empty()) {
		a = this->Smean.top();
		this->Smean.pop();
	}
	return a;
}
void SymTable::JudgePush(TokenType t1) {        //根据语法分析部分压入翻译文法所要的token
	if (t1.code == 'I' || t1.code == 'C' || t1.code == 'F')this->Smean.push(t1.code + to_string(t1.value));          //压入常数和标识符
	//cout <<"insert"; cout << t1.code + to_string(t1.value) << endl;
   // this->OutPutMid();
}
string SymTable::JudgeIType(string n1, string n2) {  //判断中间变量类型
	if (n1[0] == 'F' || n2[0] == 'F')return "F";
	else if (n1[0] == 'B' && n2[0] == 'B')return"bool";   //针对关系式所写
	else return "INT";
}

void SymTable::MidCode(int k) {    //根据递归下降的路径判断规约类型，读取token缓冲区的内容，生成四元式
	string x1, x2, t;//t为中间变量名字
	switch (k)
	{                //要注意函数定义时的大括号也要考虑
	case 0:x1 = this->TopPop(); this->ADDQNode("}", x1, "_", "_"); break;    //主函数的结束
	case 1:x1 = this->Smean.top(); this->ADDQNode("void", x1, "_", "_"); break;   //主函数的定义  void
	case 2:x1 = this->TopPop(); x2 = this->TopPop(); t = this->ADDInvNode(this->JudgeIType(x1, x2)); this->ADDQNode("+", x2, x1, t); ErrorJudge2(x2); ErrorJudge2(x1); this->Smean.push(t); break; //运算的中间代码生成
	case 3:x1 = this->TopPop(); x2 = this->TopPop(); t = this->ADDInvNode(this->JudgeIType(x1, x2)); this->ADDQNode("-", x2, x1, t); ErrorJudge2(x2); ErrorJudge2(x1); this->Smean.push(t); break;//运算的中间代码生成
	case 4:x1 = this->TopPop(); x2 = this->TopPop(); t = this->ADDInvNode(this->JudgeIType(x1, x2)); this->ADDQNode("*", x2, x1, t); ErrorJudge2(x2); ErrorJudge2(x1); this->Smean.push(t); break;//运算的中间代码生成
	case 5:x1 = this->TopPop(); x2 = this->TopPop(); t = this->ADDInvNode(this->JudgeIType(x1, x2)); this->ADDQNode("/", x2, x1, t); ErrorJudge2(x2); ErrorJudge2(x1); this->Smean.push(t); break;//运算的中间代码生成
	case 6:x1 = this->TopPop(); x2 = this->TopPop();  this->ADDQNode("=", x1, "_", x2); ErrorJudge2(x2); ErrorJudge2(x1); break;  //赋值语句
	case 7:x1 = this->TopPop(); this->ADDQNode("if", x1, "_", "_"); break;//if--K
	case 8:this->ADDQNode("else", "_", "_", "_"); break;  //else
	case 9:this->ADDQNode("while", "_", "_", "_"); break; //while循环头
	case 10:if (!this->Smean.empty())x1 = this->TopPop(); this->ADDQNode("do", x1, "_", "_"); break;//do函数	
	case 11:this->ADDQNode("we", "_", "_", "_"); break;//循环尾
	case 12:this->ADDQNode("ie", "_", "_", "_"); break;  //条件结束
	case 13:x1 = this->TopPop(); x2 = this->TopPop(); t = this->ADDInvNode(this->JudgeIType("B", "B")); this->ADDQNode(">", x2, x1, t); ErrorJudge2(x2); ErrorJudge2(x1); this->Smean.push(t); break;   //关系式>
	case 14:x1 = this->TopPop(); x2 = this->TopPop(); t = this->ADDInvNode(this->JudgeIType("B", "B")); this->ADDQNode("<", x2, x1, t); ErrorJudge2(x2); ErrorJudge2(x1); this->Smean.push(t); break;  //关系式<
	case 15:x1 = this->TopPop(); x2 = this->TopPop(); t = this->ADDInvNode(this->JudgeIType("B", "B")); this->ADDQNode("<=", x2, x1, t); ErrorJudge2(x2); ErrorJudge2(x1);  this->Smean.push(t); break;   //关系式<=
	case 16:x1 = this->TopPop(); x2 = this->TopPop(); t = this->ADDInvNode(this->JudgeIType("B", "B")); this->ADDQNode(">=", x2, x1, t); ErrorJudge2(x2); ErrorJudge2(x1); this->Smean.push(t); break;    //关系式>=
	case 17:x1 = this->TopPop(); x2 = this->TopPop(); t = this->ADDInvNode(this->JudgeIType("B", "B")); this->ADDQNode("==", x2, x1, t); ErrorJudge2(x2); ErrorJudge2(x1); this->Smean.push(t); break;  //关系式==
	default:
		break;
	}
	return;
}
void SymTable::ErrorJudge1(TokenType t1) {  //重定义检查，在标识符后面使用
	if (this->SYNHead->SNext == NULL)return;
	PTSYNnode P = this->SYNHead->SNext;
	while (P)
	{
		if (t1.code == 'I') {
			if (P->TName == Imap[t1.value]) {  //说明重定义了
				cout << Imap[t1.value] << " " << "have been defined" << endl;
			}
		}
		P = P->SNext;
	}
}
void SymTable::ErrorJudge2(string str) {  //未定义检查
	if (str[0] != 'I')return;
	string Id;
	for (int i = 1; i < str.length(); i++) {
		Id += str[i];      //标识符的id
	}
	istringstream is(Id);
	int Int_Id;
	is >> Int_Id;
	//cout<<"1111" << Int_Id;
	PTSYNnode P = this->SYNHead->SNext;
	while (P)
	{
		if (P->TName == Imap[Int_Id]) {
			break;
		}
		P = P->SNext;
	}
	if (P == NULL) {  //说明没有定义
		cout << endl;
		cout << Imap[Int_Id] << " " << "is no defined" << endl;
	}
}

//////////符号表的添加
void SymTable::insert(string token)//////////////符号表头指针部分不装数据
{
	int flag = 0;
	if (token[1] == 'K')//关键字转类型表
	{
		string name = keymap[numcge(token, 3)];
		//if (nodes->SNext->TCat == "const")//常数判断
		//{
		//	if (name == "int")
		//	{
		//		nodes->SNext->Type = (PTYnode)malloc(sizeof(struct TYPnode));
		//		nodes->SNext->Type = THead->TNext;
		//	}
		//	else if (name == "float")
		//	{
		//		nodes->SNext->Type = (PTYnode)malloc(sizeof(struct TYPnode));
		//		nodes->SNext->Type = THead->TNext->TNext->TNext->TNext;
		//	}
		//}
		////else if (nodes->Fform != NULL)//参数判断
		////{
		////	if (name == "int")//整形
		////	{
		////		nodep->Ptype= THead;
		////		nodep->PCat = name;
		////		nodep->Pvar = nodev;


		////	}
		////	else if (name == "float")//实型
		////	{

		////	}
		////	else if (name == "bool")//布尔型
		////	{

		////	}
		////	else if (name == "char")//字符型
		////	{

		////	}
		////}
		//else
		{

			if (name == "int")//整形
			{
				PTSYNnode news = screat();
				//PTSYNnode news = (PTSYNnode)malloc(sizeof(struct SYNnode));
				news->Type = (PTYnode)malloc(sizeof(struct TYPnode));
				news->Type = THead->TNext;
				//node = node->SNext;
			}
			else if (name == "float")//实型
			{
				PTSYNnode news = screat();
				news->Type = (PTYnode)malloc(sizeof(struct TYPnode));
				news->Type = THead->TNext->TNext->TNext->TNext;
				//node = node->SNext;
			}
			else if (name == "bool")//布尔型
			{
				PTSYNnode news = screat();
				news->Type = (PTYnode)malloc(sizeof(struct TYPnode));
				news->Type = THead->TNext->TNext;
				//node = node->SNext;
			}
			else if (name == "char")//字符型
			{
				PTSYNnode news = screat();
				news->Type = (PTYnode)malloc(sizeof(struct TYPnode));
				news->Type = THead->TNext->TNext->TNext;
				//node = node->SNext;
			}
			else if (name == "void")//函数
			{
				PTSYNnode news = screat();
				news->TCat = "P";
				news->Fform = (PTFnode)malloc(sizeof(struct Fnode));
				news->Fform = nodef;
				news->Type = NULL;
			}
			else if (name == "const")//常量
			{
				PTSYNnode news = screat();
				news->TCat = "C";
				news->Cform = (PTCnode)malloc(sizeof(struct Cnode));
				news->Cform = nodec;
			}
		}
	}
	else if (token[1] == 'I')//标识符
	{
		string name = Imap[numcge(token, 3) + 1];
		if (nodes->SNext == NULL)//连续定义标识符
		{
			string ste;
			char a = nodes->Type->type;
			if (a == 'i')ste = "(K,1";
			else if (a == 'f')ste = "(K,4";
			else if (a == 'b')ste = "(K,15";
			else if (a == 'c')ste = "(K,16";
			insert(ste);
		}
		if (nodes->SNext->Type != NULL)
		{
			//if (nodes->SNext->TCat == "const")//常数表+
			//{
			//	nodes->SNext->TName = name;
			//}
			//else					 //////////变量表
			{
				nodes->SNext->TName = name;
				nodes->SNext->TCat = "V";
				nodes->SNext->Vform = (PTVnode)malloc(sizeof(struct Vnode));
				nodes->SNext->Vform = nodev;
				nodes->SNext->AInformation = 'y';
				int n = 0;
				char p = nodes->SNext->Type->type;
				if (p == 'b' || p == 'c')
				{
					n = 1;
				}
				else if (p == 'i' || p == 'r')
				{
					n = 4;
				}
				nodev->Vdeep = 0;
				nodev->Vstart = off_set;
				off_set += n;
				vcreat();
				flag++;
			}
		}
		else
		{
			if (nodes->SNext->TCat == "P")//函数表
			{
				nodes->SNext->Fform = nodef;
				nodes->SNext->TName = name;
				nodef->deep = 0;
				nodef->F_off = 3;
				nodef->FN = 0;
				nodef->entry = nodes->SNext;
				nodef->Paral = nodep;
				fcreat();
				flag++;
			}
		}
	}
	else if (token[1] == 'T')//临时变量
	{
		string name = token.substr(3, token.length());
		nodes->SNext->TName = name;
		nodes->SNext->TCat = "V";
		nodes->SNext->Vform = (PTVnode)malloc(sizeof(struct Vnode));
		nodes->SNext->Vform = nodev;
		nodes->SNext->AInformation = 'n';
		int n = 0;
		char p = nodes->SNext->Type->type;
		if (p == 'b' || p == 'c')
		{
			n = 1;
		}
		else if (p == 'i' || p == 'r')
		{
			n = 4;
		}
		nodev->Vdeep = 0;
		nodev->Vstart = off_set;
		off_set += n;
		vcreat();
		flag++;
	}
	//else if (token[1] == 'C')//常数
	//{
	//	string name = Cmap[numcge(token, 3)];
	//	if (nodes->SNext->TCat == "c")//常数表
	//	{
	//		nodes->SNext->Cform->Len = numcge(name, 0);
	//		ccreat();
	//		flag++;
	//	}
	//	else					  //长度表
	//	{
	//		//nodes->Lform->Len = numcge(name, 0);
	//		//lcreat();
	//	}
	//}
	//if (!nodes->SNext->Type)cout << "yeah" << endl;
	if (flag == 1)
	{
		nodes = nodes->SNext;
		nodes->SNext = NULL;
	}
}
PTSYNnode SymTable::screat()//符号表//新建链表初始化
{
	//PTSYNnode news = (PTSYNnode)malloc(sizeof(struct SYNnode));
	PTSYNnode news = new SYNNode();
	news->SNext = NULL;
	news->Type = NULL;
	news->Cform = NULL;
	news->Fform = NULL;
	news->Lform = NULL;
	news->Vform = NULL;
	nodes->SNext = news;
	//node = node->SNext;
	return news;
}
void SymTable::pcreat()//参数表
{
	PTSYNnode news = (PTSYNnode)malloc(sizeof(struct SYNnode));
	news->SNext = NULL;
	nodes->SNext = news;
}
void SymTable::lcreat()//长度表
{
	PTLnode news = (PTLnode)malloc(sizeof(struct LENnode));
	news->LNext = NULL;
	nodel->LNext = news;
	nodel = nodel->LNext;
}
void SymTable::fcreat()//函数表
{

	PTFnode news = (PTFnode)malloc(sizeof(struct Fnode));
	news->FNext = NULL;
	nodef->FNext = news;
	nodef = nodef->FNext;
}
void SymTable::ccreat()//常量表
{
	PTCnode	news = (PTCnode)malloc(sizeof(struct Cnode));
	news->CNext = NULL;
	nodec->CNext = news;
	nodec = nodec->CNext;
}
void SymTable::vcreat()//变量表
{
	PTVnode	news = (PTVnode)malloc(sizeof(struct Vnode));
	news->VNext = NULL;
	nodev->VNext = news;
	nodev = nodev->VNext;
}

void SymTable::Init(string* key, string* I, string* C, string* F) {
	keymap = key;
	Imap = I;
	Cmap = C;
	Fmap = F;
}


SymTable::SymTable()
{
	this->SYNHead = (PTSYNnode)malloc(sizeof(struct SYNnode));//符号表		
	if (SYNHead != NULL) {
		SYNHead->Type = (PTYnode)malloc(sizeof(struct TYPnode));  //让符号链表的头指针指向类型表头指针
		if (SYNHead->Type != NULL) {
			SYNHead->Type->PTAinfl = (PTAInode)malloc(sizeof(struct AInode));//类型头指针指向数组头指针
			SYNHead->Type->PTRinfl = (PTRnode)malloc(sizeof(struct Rnode));//类型头指针指向结构头指针
		}
		//初始化前几个类型表节点
		PTYnode W1 = (PTYnode)malloc(sizeof(struct TYPnode));
		PTYnode W2 = (PTYnode)malloc(sizeof(struct TYPnode));
		PTYnode W3 = (PTYnode)malloc(sizeof(struct TYPnode));
		PTYnode W4 = (PTYnode)malloc(sizeof(struct TYPnode));
		W1->PTAinfl = NULL;
		W1->PTRinfl = NULL;
		W1->type = 'i';
		W1->TNext = W2;
		SYNHead->Type->TNext = W1;
		W2->PTAinfl = NULL;
		W2->PTRinfl = NULL;
		W2->type = 'b';
		W2->TNext = W3;
		W3->PTAinfl = NULL;
		W3->PTRinfl = NULL;
		W3->type = 'c';
		W3->TNext = W4;
		W4->TNext = NULL;
		W4->PTAinfl = NULL;
		W4->PTRinfl = NULL;
		W4->type = 'r';
	}
	this->LHead = (PTLnode)malloc(sizeof(struct LENnode)); //长度表
	LHead->LNext = NULL;
	this->FHead = (PTFnode)malloc(sizeof(struct Fnode));	//函数表
	FHead->Paral = (PTPnode)malloc(sizeof(struct Pnode));  //参数表
	FHead->Paral->PNext = NULL;
	FHead->FNext = NULL;
	this->CHead = (PTCnode)malloc(sizeof(struct Cnode));//常量表
	CHead->CNext = NULL;
	this->VHead = (PTVnode)malloc(sizeof(struct Vnode));//变量表
	VHead->VNext = NULL;
	this->INVHead = (PTINVnode)malloc(sizeof(struct INVnode));//中间变量表
	INVHead->INVNext = NULL;
	int NumInvNode = 0;//中间变量个数
	this->Qhead = (PTQnode)malloc(sizeof(struct  Qnode));  //四元式头指针
	Qhead->QNext = NULL;
	///////////////////链表进度指针
	THead = SYNHead->Type;
	nodes = SYNHead;
	nodep = FHead->Paral;
	nodel = LHead;
	nodef = FHead;
	nodec = CHead;
	nodev = VHead;
	m_ope.insert(make_pair("-", 0));
	m_ope.insert(make_pair("==", 4));
	m_ope.insert(make_pair("<=", 5));
	m_ope.insert(make_pair("<", 6));
	m_ope.insert(make_pair("+", 7));
	m_ope.insert(make_pair("*", 8));
	m_ope.insert(make_pair(">", 9));
	m_ope.insert(make_pair(">=", 16));
	m_ope.insert(make_pair("/", 1));
	m_ope.insert(make_pair("=", 10));
}
SymTable::~SymTable()
{
}
void SymTable::OutPutMid() {
	PTQnode head = this->Qhead->QNext;
	cout << endl;
	while (head) {
		head->Operater + head->Obj1 + head->Obj2 + head->result;
		cout << "(" << head->Operater << "," << head->Obj1 << "," << head->Obj2 << "," << head->result << ")" << endl;
		head = head->QNext;
	}
}
void SymTable::OutPutfhb() {
	PTSYNnode p = SYNHead->SNext;
	cout << endl;
	while (p) {
		cout << p->TName << " ";
		if (p->Type)
		{
			cout << p->Type->type << " ";
		}
		else
		{
			cout << "_ ";
		}
		cout << p->TCat << " ";
		if (p->Vform)
		{
			cout << p->Vform->Vdeep << " " << p->Vform->Vstart << " " << p->AInformation << endl;
		}
		else if (p->Fform)
		{
			cout << p->Fform->deep << " " << p->Fform->F_off << " " << p->Fform->FN << endl;
		}
		p = p->SNext;
	}
	PTINVnode head = INVHead->INVNext;
	while (head) {
		cout << head->IName << " " << head->AInformation << endl;
		head = head->INVNext;
	}
	cout << endl;
}

void SymTable::happily() //////调整活跃信息
{
	PTQnode head = this->Qhead->QNext;
	PTQnode temp = new Qnode();
	temp->QNext = NULL;
	PTQnode Head = temp;
	while (head) {                 ///////////////////////复制链表
		PTQnode W = new Qnode();  //新建四元式节点
		W->Obj1 = head->Obj1;
		W->Obj2 = head->Obj2;
		W->Operater = head->Operater;
		W->result = head->result;
		W->QNext = NULL;
		temp->QNext = W;
		temp = temp->QNext;
		head = head->QNext;
	}
	PTQnode H = fanz(Head);
	while (H)
	{
		if (H->result != "_")exc(H->result, 2);
		if (H->Obj1 != "_")exc(H->Obj1, 4);
		if (H->Obj2 != "_")exc(H->Obj2, 4);
		H = H->QNext;
	}
	PTQnode L = fanz(H);
	PTQnode d = this->Qhead->QNext;
}
void SymTable::exc(string str, int i)////////改活跃信息值
{
	PTINVnode head = INVHead->INVNext;
	PTSYNnode daeh = SYNHead->SNext;
	string p = str;
	string f;
	if (p[0] == 't') {
		while (head) {
			if (head->IName == p) {
				break;
			}
			head = head->INVNext;
		}
		while (daeh) {
			if (daeh->TName == p) {
				break;
			}
			daeh = daeh->SNext;
		}
		if (daeh == NULL || head == NULL)return;
		head->AInformation = daeh->AInformation;
		if (i == 2)daeh->AInformation = 'n';
		else if (i == 4)daeh->AInformation = 'y';
	}
	else if (p[0] <= '9' && p[0] >= '0')return;
	else if (p[0] == '-')return;
	else
	{
		//f = Imap[numcge(p.substr(1, p.length()), 0) + 1];
		while (daeh) {
			if (daeh->TName == p)
			{
				if (daeh->TCat == "P")return;
				break;
			}
			daeh = daeh->SNext;
		}
		if (daeh == NULL)return;
		if (daeh->TCat == "P")return;
		if (i == 2)daeh->AInformation = 'n';
		else if (i == 4)daeh->AInformation = 'y';
	}
}
PTQnode SymTable::fanz(PTQnode head) //翻转
{
	// head看作是前一结点，head.getNext()是当前结点，reHead是反转后新链表的头结点
	if (head == NULL || head->QNext == NULL) {
		return head;// 若为空链或者当前结点在尾结点，则直接还回
	}
	PTQnode reHead = fanz(head->QNext);// 先反转后续节点head.getNext()
	head->QNext->QNext = head;// 将当前结点的指针域指向前一结点
	head->QNext = NULL;// 前一结点的指针域令为null;
	return reHead;// 反转后新链表的头结点
}
void SymTable::DividMid() {  //生成基本块
	PTQnode head = this->Qhead->QNext->QNext; //指向程序开始语句的下一句,void后面的那一句
	if (head->QNext == NULL) {  //即函数体为空，只有两个四元式结构
		cout << "无函数体" << endl;
		return;
	}
	//开始语句的设置
	head->Position = 'b';         //void的下一句,程序的第一句，设为开始语句
	if (head->Operater == "while" || head->Operater == "if") {  //如果第一句是while 或者if ，则下一条语句为begin
		head = head->QNext;
		head->Position = 'b';
	}
	head = head->QNext;  //读下一条语句
	while (head->Operater != "}") {  //设置开始标志，当没读到程序结束标志，则判断他是不是转向语句转到的语句，或者是不是紧跟在转向语句后的语句
		if (head->Operater == "do") {
			head = head->QNext;
			head->Position = 'b';
			continue;
		}
		else if (head->Operater == "else" || head->Operater == "if" || head->Operater == "we") { //紧跟在转向语句后的语句
			head = head->QNext;
			head->Position = 'b';
			continue;
		}
		else if (head->Operater == "ie") {
			head->Position = 'b';
			head = head->QNext;
			continue;
		}
		else {
			if (head->Position != 'b')head->Position = 'm';   //暂设为中间过程
			head = head->QNext;
			continue;
		}
	}
	//结束标志的设置
	head = this->Qhead->QNext->QNext;  //这个肯定是入口之一
	while (head->Operater != "}") {
		if (head->Position != 'm' && head->Position != 'b') {
			head->Position = 'e';
		}
		if (head->Position == 'm' && head->QNext->Operater == "}")head->Position = 'e';//将末尾位置设为结束标志
		head = head->QNext;
	}
	PTQnode head2 = this->Qhead->QNext;  //输出带有基本块划分信息的四元式
	cout << endl;
	while (head2) {
		head2->Operater + head->Obj1 + head2->Obj2 + head2->result;
		cout << "(" << head2->Operater << "," << head2->Obj1 << "," << head2->Obj2 << "," << head2->result << ")" << head2->Position << endl;
		head2 = head2->QNext;
	}
}