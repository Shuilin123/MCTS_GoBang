#include "HashAi.h"
#include<stdlib.h>
U64 checkcode[3][15][15];//3 表示3种状态
unsigned long hashcode[3][15][15];
HashItem HashTable[HashSize];
//unsigned long LL;
U64  Rand64() {
  	return rand() ^ ((U64) rand() << 15) ^ ((U64) rand() << 30) ^ ((U64) rand() << 45) ^ ((U64) rand() << 60);
}
void InitCheckCode(void)
{//64位校验值生成函数 
	for (int i = 0; i < CBWIDTH; i++) 
	{
      for (int j = 0; j < CBWIDTH; j++) 
	   {
         checkcode[0][i][j] = Rand64();
         checkcode[1][i][j] = Rand64();
         checkcode[2][i][j] = Rand64();
       }
  }
}
unsigned long  HashCode(std::vector<std::vector<int>> board){//hash值生成函数
		unsigned long hashkey=0;  
		for(int y=0;y<CBWIDTH;y++){
			for(int x=0;x<CBWIDTH;x++){
				hashkey^=hashcode[board[y][x]][y][x];
			}
		}
		return hashkey;
}
void SetHash(unsigned long hashcode1,U64 checkcode1,int value1,int depth1,int hashflag1)//保存hash表 
{
	HashItem *HT=&HashTable[hashcode1&(HashSize-1)];
	HT->checkcode=checkcode1;
	HT->value=value1;
	HT->depth=depth1;
	HT->hashflag=hashflag1; 
}
int GetHash(unsigned long hashcode1,U64 checkcode1,int alpha1,int beta1,int depth1)
{//查询hash表 
	HashItem *HT=&HashTable[hashcode1&(HashSize-1)];
	if(HT->checkcode==checkcode1){
		if(HT->depth>=depth1){
			return HT->value;
		if(HT->hashflag == hash_exact){
        	return HT->value;
        }else if(HT->hashflag == hash_alpha && HT->value <= alpha1){
        	return HT->value;
        }else if(HT->hashflag == hash_beta && HT->value >= beta1){
        	return HT->value;
      }
		}
		return unknown;
	}
	return unknown;		
}
