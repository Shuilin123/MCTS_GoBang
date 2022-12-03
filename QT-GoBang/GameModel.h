#ifndef GAMEMODEL_H
#define GAMEMODEL_H

// ---- 五子棋游戏模型类 ---- //
#include <vector>
#include<cmath>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include<QThread>
#include<QTime>
#include"Ai.h"
// 游戏类型，双人还是AI（目前固定让AI下黑子）
typedef struct{
    std::vector<std::vector<int>> m_gameMapVec;//棋盘
    int win_count;//赢的数量
    int count;//运行数量
    float winning_probability;
    int node_isWin;//是否被探索过
}Node;
//树的节点
typedef struct tree_node{
    //父节点
    struct tree_node *parent;
    //节点集合
    std::vector<struct tree_node*> m_Child;
    //节点值
    Node node;
}MCTS_TREE_NODE;
//树
enum GameType
{
    PERSON,
    BOT
    ,MCTSX
};

// 游戏状态
enum GameStatus
{
    PLAYING,
    WIN,
    DEAD
};

// 棋盘尺寸
const int kBoardSizeNum = 15;

class GameModel: public QThread
{
public:
    GameModel();
    ~GameModel();
    std::vector<std::vector<int>> gameMapVec; // 存储当前游戏棋盘和棋子的情况,空白为0，白子1，黑子-1
    std::vector<std::vector<int>> scoreMapVec; // 存储各个点位的评分情况，作为AI下棋依据
    bool playerFlag; // 标示下棋方
    bool MCTS_Flag; // 标示MCTS下棋方
    GameType gameType; // 游戏模式
    GameStatus gameStatus; // 游戏状态
    bool writeChessHigh_P(int row,int col);
    int startGame(GameType type); // 开始游戏
    void getPostion(int &clickRow, int &clickCol);
    void calculateScore(); // 计算评分
    void actionByPerson(int row, int col); // 人执行下棋
    void actionByAI(bool &isSerachFinish,int &clickRow, int &clickCol); // 机器执行下棋
    void actionByAI1(int &clickRow, int &clickCol); // 机器执行下棋
    void updateGameMap(int row, int col); // 每次落子后更新游戏棋盘
    void updateGameMap(std::vector<std::vector<int>> &map,int row, int col,bool isExpand); // 每次落子后更新游戏棋盘
    bool isWin(int row, int col); // 判断游戏是否胜利
    bool isWin(std::vector<std::vector<int>> map,int row, int col);
    bool is_Win(Node node);
    bool isDeadGame(); // 判断是否和棋
    bool getThreadFlags();
    void setThreadFlags(bool flags);
    int randm(int length);
    int game_Simulation();
    int game_Simulation(std::vector<std::vector<int>> map);
    int run_game_Simulation();
    int run_game_Simulation(std::vector<std::vector<int>> &map);
    void showGrapih(std::vector<std::vector<int>> map);
    void showGrapih();
    //MCTS
    void Simulation();
    void ShowTree(MCTS_TREE_NODE rootx);
    void ShowPath(MCTS_TREE_NODE rootx);
    void ShowChild(MCTS_TREE_NODE rootx);
    int Simulation(Node node);
    int getSelect(MCTS_TREE_NODE *tree_nodex);
    void Select(MCTS_TREE_NODE *tree_nodex,int &Select_node_index,int T);
    void Expand(MCTS_TREE_NODE *mcts_tree,Node node/*,std::vector<MCTS_TREE_NODE> &NodeSet*/);
    Node MCTS(Node &node,unsigned int depth);
    void feedback(MCTS_TREE_NODE *mcts_treex);
    int do_not(int x, int y, int playerID);
    std::vector<Node> m_NodeSet;//节点集合
   MCTS_TREE_NODE *root;
   bool Destroy_MCTS_TREE(MCTS_TREE_NODE *root);
   int flags;
   int roundCount;
   int depth_index;
   int Quikly_play;//快速走子次数
   float MCTS_runime;
   private:
   int x,y;
   bool isThreadOver;
   bool isSearchOver;
   virtual void run();
};

#endif // GAMEMODEL_H
