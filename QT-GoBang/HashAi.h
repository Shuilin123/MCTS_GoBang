#include<vector>
typedef unsigned long long U64;
const int hash_exact = 0;
const int CBWIDTH = 15;
const int hash_alpha = 1;
const int hash_beta = 2;
const int unknown = -1000000;
const  long  HashSize = 1<<22;   // ��ϣ��ߴ�
const int depth1=12;
//��ϣ�ṹ 
struct HashItem{
	unsigned long long checkcode;//64λhashУ��ֵ
	int value;//��ֵ���ʷ�����÷�
	int depth;//�õ��÷ֵ��������
	int hashflag;//��¼��ֵ���ӵĵ÷����� 
};
	extern int LL;
	unsigned long long Rand64(void);
	extern unsigned long hashcode[3][15][15];//����32λhash�� 
	extern U64 checkcode[3][15][15];//����64λ���У���� 
	extern HashItem HashTable[HashSize];//����hash���С 
	void InitCheckCode(void);
	unsigned long HashCode(std::vector<std::vector<int>> board); 
	void SetHash(unsigned long hashcode,U64 checkcode,int value,int depth,int hashflag) ;//��¼��ϣ��
	int GetHash(unsigned long hashcode1,U64 checode1,int alpha1,int beta1,int depth1);//��ѯ��ϣ�� 

