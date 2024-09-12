#include <unordered_map>
#include <queue>
#include <utility>

using namespace std;


class SemSys{
protected:
	//SemRecord* SR;
	queue<int> CONS;   //常数表
	queue<string> RCONS;   //常实数表
	queue<char> CT;   //字符表
	queue<string> ST;   //字符串表
	unordered_map<string, int> KEY;   //关键字表
	unordered_map<string, int> PW;   //界符表
};