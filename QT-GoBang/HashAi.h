#include<vector>
typedef unsigned long long U64;
const int hash_exact = 0;
const int CBWIDTH = 15;
const int hash_alpha = 1;
const int hash_beta = 2;
const int unknown = -1000000;
const  long  HashSize = 1<<22;   // 哈希表尺寸
const int depth1=12;
//哈希结构 
struct HashItem{
	unsigned long long checkcode;//64位hash校验值
	int value;//棋局的历史搜索得分
	int depth;//得到得分的搜索深度
	int hashflag;//记录估值棋子的得分性质 
};
	extern int LL;
	unsigned long long Rand64(void);
	extern unsigned long hashcode[3][15][15];//定义32位hash码 
	extern U64 checkcode[3][15][15];//定义64位随机校验码 
	extern HashItem HashTable[HashSize];//定义hash表大小 
	void InitCheckCode(void);
	unsigned long HashCode(std::vector<std::vector<int>> board); 
	void SetHash(unsigned long hashcode,U64 checkcode,int value,int depth,int hashflag) ;//记录哈希表
	int GetHash(unsigned long hashcode1,U64 checode1,int alpha1,int beta1,int depth1);//查询哈希表 

