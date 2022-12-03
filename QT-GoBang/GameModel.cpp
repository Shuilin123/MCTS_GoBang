#include <utility>
#include "GameModel.h"
#include <QMessageBox>
#include <QDebug>
//#define Quikly_play 5
#define delta_value 5
#define Quikly_play_num 12
#define  jingshou
const int serach_depth[Quikly_play_num]={10000,18000,24000,30000,36000,40000};
GameModel::GameModel()
{
  roundCount=0;
  Quikly_play=5;
  MCTS_runime=15;
  depth_index=0;
  isThreadOver=false;
  isSearchOver=true;
}
//摧毁树
bool GameModel::Destroy_MCTS_TREE(MCTS_TREE_NODE *root){
    if(root->m_Child.empty()){
        delete root;
        root=nullptr;
        return true;
    }
    for(int i=0;i<root->m_Child.size();i++){
        Destroy_MCTS_TREE(root->m_Child[i]);
    }
}
GameModel::~GameModel()
{
   Destroy_MCTS_TREE(root);
}
void GameModel::showGrapih(){
    for (int row = 0; row < kBoardSizeNum; row++){
        for (int col = 0; col < kBoardSizeNum; col++)
        {
            // 前提是这个坐标是空的
            printf("%d ",gameMapVec[row][col]);
            fflush(stdout);
            //qDebug()<<gameMapVec[row][col]<<" ";
        }
        printf("\n");
        fflush(stdout);
        //qDebug()<<"\n";
    }
    printf("\n");
    fflush(stdout);
    //qDebug()<<"\n";
}
void GameModel::showGrapih(std::vector<std::vector<int>> map){
    for (int row = 0; row < kBoardSizeNum; row++){
        for (int col = 0; col < kBoardSizeNum; col++)
        {
            // 前提是这个坐标是空的
            printf("%d ",map[row][col]);
            fflush(stdout);
            //qDebug()<<gameMapVec[row][col]<<" ";
        }
        printf("\n");
        fflush(stdout);
        //qDebug()<<"\n";
    }
    printf("\n");
    fflush(stdout);
    //qDebug()<<"\n";
}

int GameModel::startGame(GameType type)
{
    gameType = type;
    // 初始棋盘
    gameMapVec.clear();
    for (int i = 0; i < kBoardSizeNum; i++)
    {
        std::vector<int> lineBoard;
        for (int j = 0; j < kBoardSizeNum; j++)
            lineBoard.push_back(0);
        gameMapVec.push_back(lineBoard);
    }

    // 如果是AI模式，需要初始化评分数组
    if (gameType == BOT||gameType == MCTSX)
    {
        scoreMapVec.clear();
        for (int i = 0; i < kBoardSizeNum; i++)
        {
            std::vector<int> lineScores;
            for (int j = 0; j < kBoardSizeNum; j++)
                lineScores.push_back(0);
            scoreMapVec.push_back(lineScores);
        }
    }

    if(gameType == MCTSX){
        //QMessageBox::warning(NULL,  "warning",  "MCTS finish!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return 1;
    }
    // 己方下为true,对方下位false
   playerFlag = true;
   //playerFlag=false;
   return -1;
}

int GameModel::run_game_Simulation(){
    // 随机落子，如果有多个点的话
    //while(1)
    {
       int x=randm(15);
       int y=randm(15);
      if (gameMapVec[x][y] == 0){
          updateGameMap(x,y);
          if(isWin(x,y)){
            return 1;
          }
      }
   }
    return 0;
}
int GameModel::run_game_Simulation(std::vector<std::vector<int> > &map){

    int x=randm(15);
    int y=randm(15);
   if (map[x][y] == 0){
       updateGameMap(map,x,y,false);
       if(isWin(map,x,y)){
         //printf("is_WIN1=%d\n",map[x][y]);
         //fflush(stdout);
         return map[x][y];
       }
   }
   return 0;
}

int GameModel::randm(int length){
    return rand()%length;
}

int GameModel::game_Simulation(){
    while(1)
    {
        if(run_game_Simulation()){
            break;
        }
    }
    return 0;
}
int GameModel::game_Simulation(std::vector<std::vector<int>> map){
    int temp=0;
    while(1)
    {
        int temp1=run_game_Simulation(map);//temp1 =-1、0、1
        if(temp1!=0){
            temp=temp1;//-1、1
           break;
       }
    }

   return temp;
}

void GameModel::updateGameMap(int row, int col)
{
    if (playerFlag)
        gameMapVec[row][col] = 1;//白棋
    else
        gameMapVec[row][col] = -1;
    // 换手
    playerFlag = !playerFlag;
}
void GameModel::updateGameMap(std::vector<std::vector<int>> &map,int row, int col,bool isExpand){
    if (playerFlag)
        map[row][col] = 1;//白棋
    else
        map[row][col] = -1;//黑棋
    //不是展开阶段需要换手
    if(isExpand==false){
     playerFlag = !playerFlag;
    }
}

void GameModel::actionByPerson(int row, int col)
{
#ifdef jingshou
    int playerID=(playerFlag==false)? -1:1;
    if(do_not(row,col,playerID)!=1){
        if(roundCount>=Quikly_play){
            if(isSearchOver){
                updateGameMap(row, col);
            }
            else{
                printf("MCTS AI is runing,please do not chick the mouse.\n");
                fflush(stdout);
            }
        }else{
            updateGameMap(row, col);
            isSearchOver=true;
        }
    }else {
        printf("Error: this point is 'forbidden hand'.Please choose another point to continue playing chess, please refer to <<Chinese Gobang Competition Rules>>for details.\n");
        fflush(stdout);
    }
#else
    if(roundCount>=Quikly_play){
        if(isSearchOver){
            updateGameMap(row, col);
        }
        else{
            printf("MCTS AI is runing,please do not chick the mouse.\n");
            fflush(stdout);
        }
    }else{
        updateGameMap(row, col);
        isSearchOver=true;
    }
#endif

}

void GameModel::Simulation(){

    while(1)
    {
        if(run_game_Simulation()==1){
            break;
        }
    }
}
int GameModel::Simulation(Node node){
    /*if (playerFlag){
        printf("Simulation Start play is:bai\n");
        fflush(stdout);
    }
    else{
        printf("Simulation Start play is:hei\n");
        fflush(stdout);
    }*/
    int t=0;
    while(1){
        int temp=run_game_Simulation(node.m_gameMapVec);// -1/0/1
        if(temp!=0){// -1、1
            if(temp==-1){//黑棋获胜
             t=1;
            }
            break;
        }
    }
    return t;
}

bool GameModel::writeChessHigh_P(int row,int col){

    bool result =false;
    // 统计玩家或者电脑连成的子
    int personNum = 0; // 玩家连成子的个数
    int botNum = 0; // AI连成子的个数
    int emptyNum = 0; // 各方向空白位的个数
       // 遍历周围八个方向
       for (int y = -1; y <= 1; y++)
         for (int x = -1; x <= 1; x++){
            // 重置
            personNum = 0;
            botNum = 0;
            emptyNum = 0;

           // 原坐标不算
           if (!(y == 0 && x == 0)){
        // 每个方向延伸4个子
        // 对玩家白子评分（正反两个方向）
        for (int i = 1; i <= 4; i++)
        {
            if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                col + i * x > 0 && col + i * x < kBoardSizeNum &&
               gameMapVec[row + i * y][col + i * x] == 1) // 玩家的子
            {
                personNum++;
            }
            else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                     col + i * x > 0 && col + i * x < kBoardSizeNum &&
                    gameMapVec[row + i * y][col + i * x] == 0) // 空白位
            {
                emptyNum++;
                break;
            }
            else            // 出边界
                break;
        }

        for (int i = 1; i <= 4; i++)
        {
            if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                col - i * x > 0 && col - i * x < kBoardSizeNum &&
               gameMapVec[row - i * y][col - i * x] == 1) // 玩家的子
            {
                personNum++;
            }
            else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                     col - i * x > 0 && col - i * x < kBoardSizeNum &&
                    gameMapVec[row - i * y][col - i * x] == 0) // 空白位
            {
                emptyNum++;
                break;
            }
            else            // 出边界
                break;
        }
        if (personNum == 3)                 // 杀四
        {
            // 量变空位不一样，优先级不一样
            if (emptyNum == 2){
               result=true;
           }
       }
    }
  }
    return result;
}
int GameModel::getSelect(MCTS_TREE_NODE *tree_nodex){
    float MAX_P=-100000, MAX_P_Opponent=-100000;
    int index=0,index_my=0;
    /*int flags=0;
    int index_opponent=0;
    int MostPostion=0;
    */
    for(int i=0;i<tree_nodex->m_Child.size();i++){
       MCTS_TREE_NODE *temp=tree_nodex->m_Child[i];
       if(!(temp->node.winning_probability==1&&temp->node.count==1)){
           if(temp->node.winning_probability>MAX_P){
               MAX_P=temp->node.winning_probability;
               index_my=i;
           }
       }
    }
    index=index_my;
    /*根据UCB法求解应该选择的局面
    for(int i=0;i<tree_nodex->m_Child.size();i++){
       MCTS_TREE_NODE *temp=tree_nodex->m_Child[i];
       //我方胜率最高点
       if(temp->node.winning_probability>MAX_P&&flags==0){
           MAX_P=temp->node.winning_probability;
           index_my=i;
       }
       //我方败的概率高的点
       if((1.0-temp->node.winning_probability)>MAX_P_Opponent&&flags==0){
           MAX_P_Opponent=1.0-temp->node.winning_probability;
           index_opponent=i;
       }
       if(temp->node.node_isWin>MostPostion){
           MostPostion=temp->node.node_isWin;
           printf("my MostPostion=%d\n",MostPostion);
           fflush(stdout);
           index=i;
           flags=1;
       }
    }
    if(flags!=1){
        printf("my Max_index=%d,my winning_probability=%f\n",index_my,MAX_P);
        fflush(stdout);
        showGrapih(tree_nodex->m_Child[index_my]->node.m_gameMapVec);
        printf("opponent index=%d,opponent winning_probability=%f\n",index_opponent,MAX_P_Opponent);
        fflush(stdout);
        showGrapih(tree_nodex->m_Child[index_opponent]->node.m_gameMapVec);
        if(tree_nodex->m_Child[index_my]->node.winning_probability<tree_nodex->m_Child[index_opponent]->node.winning_probability){
            index=index_opponent;
        }else{
            index=index_my;
        }
      }
    printf("Select_Max_index=%d,Select_node.winning_probability=%f\n",index,tree_nodex->m_Child[index]->node.winning_probability);
    fflush(stdout);
    showGrapih(tree_nodex->m_Child[index]->node.m_gameMapVec);
   */
   printf("Select_Max_index=%d,Select_node.winning_probability=%f\n",index,tree_nodex->m_Child[index]->node.winning_probability);
   return index;
}

void GameModel::Select(MCTS_TREE_NODE *tree_nodex,int &Select_node_index,int T){
    float MAX_UCB=-100000;
    //根据UCB法求解应该选择的局面
    for(int i=0;i<tree_nodex->m_Child.size();i++){
       MCTS_TREE_NODE *temp=tree_nodex->m_Child[i];
       float UCB=0.0;
       if(temp->node.count==0){
           temp->node.winning_probability=0.0;
           UCB=100000;
       }else{
           float x=temp->node.win_count*1.0;
           float y=temp->node.count*1.0;
           temp->node.winning_probability=x/y;
           UCB=temp->node.winning_probability+sqrt((2.0)*log(T)/y);
       }
       /*printf("temp->node.winning_probability=%f,temp->node.win_count=%d,UCB=%f\n",temp->node.winning_probability,temp->node.win_count,UCB);
       fflush(stdout);*/
       if(MAX_UCB<UCB){
          //printf("temp->node.winning_probability=%f,temp->node.win_count=%d,UCB=%f\n",temp->node.winning_probability,temp->node.win_count,UCB);
          //fflush(stdout);
          MAX_UCB=UCB;
          Select_node_index=i;
          /*if(MAX_UCB==100000){
              break;
          }*/
       }
    }
   /*printf("Select_NODE_UCB:%f\n",MAX_UCB);
   fflush(stdout);*/

}

bool GameModel::is_Win(Node node){
    for (int row = 0; row < kBoardSizeNum; row++){
       for (int col = 0; col < kBoardSizeNum; col++){
           if (node.m_gameMapVec[row][col]!=0&&isWin(node.m_gameMapVec,row, col)){
            return true;
         }
      }
   }
    return false;
}
void GameModel::Expand(MCTS_TREE_NODE *tree_head,Node node/*,std::vector<MCTS_TREE_NODE> &NodeSet*/){
    bool tempxFlags=playerFlag;
    int direction[3]={-1,0,1};
    int flags=0;
    //产生当前局面下可能作法
    for (int row = 0; row < kBoardSizeNum; row++) {
        for (int col = 0; col < kBoardSizeNum; col++) {//枚举当点状态所有情况
            if (node.m_gameMapVec[row][col] == 0) {
                for(int k=0;k<3;k++){
                    int x=row+direction[k];
                    for(int m=0;m<3;m++){
                        int y=col+direction[m];
                        if((x>=0&&x<15)&&(y>=0&&y<15)&&node.m_gameMapVec[x][y]!=0){
                            flags=1;
                        }
                    }
                }
                if(flags==1){
                    flags=0;
                    //产生新节点
                    Node new_nodex;
                    //初始化新节点
                    new_nodex.count = 0;
                    new_nodex.win_count = 0;
                    new_nodex.winning_probability = 0.0;
                    new_nodex.node_isWin=0;
                    //将旧局面更新到新节点中
                    new_nodex.m_gameMapVec = node.m_gameMapVec;
                    playerFlag=tempxFlags;
                    //将旧局面更新通过新棋手下棋
                    updateGameMap(new_nodex.m_gameMapVec, row, col,true);
                    if(isWin(new_nodex.m_gameMapVec,row, col)){//当前选点已经赢了这层后续节点不需要加入了
                        new_nodex.node_isWin=3;//这一点一定要下
                        //break;
                    }
                    //产生树的节点
                    MCTS_TREE_NODE *new_node = new MCTS_TREE_NODE();
                    //初始化节点
                    new_node->parent = tree_head;
                    new_node->m_Child.clear();
                    new_node->node = new_nodex;//给树的节点赋值
                    tree_head->m_Child.push_back(new_node);//将该节点作为tree_head的子节点
                }
            }
        }
    }
}
void GameModel::feedback(MCTS_TREE_NODE *mcts_treex){

    MCTS_TREE_NODE *current=mcts_treex;
    while(current->parent!=nullptr){
       current=current->parent;
       current->node.count++;
       if(mcts_treex->node.win_count!=0){//赢了我才加
          current->node.win_count++;
       }
       float x=current->node.win_count*1.0;
       float y=current->node.count*1.0;
       current->node.winning_probability=x/y;
    }
}
void GameModel::ShowTree(MCTS_TREE_NODE rootx){

    printf("ShowTree Start\n");
    fflush(stdout);
    int t=0;
    while(t < rootx.m_Child.size()) {
        MCTS_TREE_NODE *new_node2 = rootx.m_Child[t];
        if (new_node2 != nullptr) {
            printf("index=%d!\n",t);
            fflush(stdout);
            showGrapih(new_node2->node.m_gameMapVec);
            printf("current_node_count:%d current_node_count_WIN:%d p=%f\n",new_node2->node.count,new_node2->node.win_count,new_node2->node.winning_probability);
            fflush(stdout);
            t++;
            //如果节点不为空递归
            /*if(!new_node2->m_Child.empty()){
                ShowTree(*new_node2);
            }*/
        }
        else {
            printf("child is NULL!\n");
            fflush(stdout);
        }
     }
    printf("ShowTree END\n");
    fflush(stdout);
}
void GameModel::ShowPath(MCTS_TREE_NODE rootx){
    int t=0;;
    MCTS_TREE_NODE *current=&rootx;
    while(current->parent!=nullptr){
        printf("dao_shu_di=%d ceng\n",t++);
        fflush(stdout);
        showGrapih(current->node.m_gameMapVec);
        printf("parents_child_count:%d parents_child_count_WIN_count:%d p=%f\n",current->node.count,current->node.win_count,current->node.winning_probability);
        fflush(stdout);
       current=current->parent;
    }
    printf("dao_shu_di=%d ceng\n",t++);
    fflush(stdout);
    showGrapih(current->node.m_gameMapVec);
    printf("parents_child_count:%d parents_child_count_WIN_count:%d p=%f\n",current->node.count,current->node.win_count,current->node.winning_probability);
    fflush(stdout);
}
void GameModel::ShowChild(MCTS_TREE_NODE rootx){

    printf("current child have:\n");
    fflush(stdout);
    for(int i=0;i<rootx.m_Child.size();i++){
       MCTS_TREE_NODE *temp=rootx.m_Child[i];
       showGrapih(temp->node.m_gameMapVec);
       printf("child_node_count:%d current_node_count_WIN:%d p=%f\n",temp->node.count,temp->node.win_count,temp->node.winning_probability);
       fflush(stdout);
    }
   printf("current child over!\n");
     fflush(stdout);
}

Node GameModel::MCTS(Node &node,unsigned int depth){

   flags=0;
   int count=1;
   bool isPocliy=false;
   bool temp_playerFlag=!playerFlag;//保存第一次该白棋棋下的标志
   MCTS_Flag=playerFlag;//防止MCTS破坏下棋方
   //std::vector<MCTS_TREE_NODE> NodeSet;
    root=new MCTS_TREE_NODE();
    root->parent=nullptr;
    root->node=node;
    root->node.node_isWin=false;
    //目前根节点没有子节点
    root->m_Child.clear();
     //产生当前节点
    MCTS_TREE_NODE *currentNode;;
       //currentNode=root;
    QTime time;
    time.start();
    //while(root->node.count<depth)
    while(1)
    {
          float delte_time=time.elapsed()/1000.0;
          if(delte_time>MCTS_runime){//程序计算时间大于30s
              break;
          }
          currentNode=root;
          int curNode=0;
          while(!currentNode->m_Child.empty())
          {
            //当前节点不是叶子节点
            Select(currentNode,curNode,root->node.count);
            //选择子节点节点
            currentNode=currentNode->m_Child[curNode];
          }
          if(currentNode->node.count==0){//当前节点没有被访问过
             playerFlag=!temp_playerFlag;//黑棋开始模拟
             if(Simulation(currentNode->node)==1){//如果随机模拟赢了当前局面加一分
              currentNode->node.win_count++;
             }
             currentNode->node.count++;
             float x=currentNode->node.win_count*1.0;
             float y=currentNode->node.count*1.0;
             currentNode->node.winning_probability=x/y;
             playerFlag=temp_playerFlag;//恢复由模拟破坏的

             feedback(currentNode);


             isPocliy=true;
          }
          else{
              if(isPocliy){
                flags++;
                isPocliy=false;
              }
              count=1;
              playerFlag=!temp_playerFlag;//扩展黑棋的局面
              Expand(currentNode,currentNode->node);
              //选择第一个节点进行模拟·
              currentNode=currentNode->m_Child[0];
              if(currentNode->node.count==0){//当前节点没有被访问过
                 playerFlag=!temp_playerFlag;//黑棋局面下该白棋模拟
                 if(Simulation(currentNode->node)==1){//如果随机模拟赢了当前局面加一分
                  currentNode->node.win_count++;
                 }
                 currentNode->node.count++;
                 float x=currentNode->node.win_count*1.0;
                 float y=currentNode->node.count*1.0;
                 currentNode->node.winning_probability=x/y;

                 playerFlag=temp_playerFlag;//恢复黑棋局面

                 feedback(currentNode);


            }
       }
    }
    printf("count number is:%d time is:%fs\n",root->node.count,time.elapsed()/1000.0);
    fflush(stdout);
    int index=getSelect(root);
    playerFlag=MCTS_Flag;//恢复正常下棋方
    return root->m_Child[index]->node;
}
void GameModel::getPostion(int &clickRow, int &clickCol){
    x=clickRow;
    y=clickCol;
}
bool GameModel::getThreadFlags(){
    return isThreadOver;
}
void GameModel::setThreadFlags(bool flags){
    isThreadOver=flags;
}
void GameModel::run(){

     isSearchOver=false;
     actionByAI(isSearchOver,x,y);
     //actionByAI1(x,y);
     isThreadOver=true;
}

void GameModel::actionByAI(bool &isSerachFinish,int &clickRow, int &clickCol)
{
   int no_thinking=0,iswin=0;
   float winning_probability=0.0;
   if(roundCount<Quikly_play){
       #ifdef chuangtongAI
       calculateScore();
       int maxScore = 0;
       std::vector<std::pair<int, int>> maxPoints;
       for (int row = 1; row < kBoardSizeNum; row++)
           for (int col = 1; col < kBoardSizeNum; col++)
           {
               if (gameMapVec[row][col] == 0)
               {
                   if (scoreMapVec[row][col] > maxScore)          // 鎵炬渶澶х殑鏁板拰鍧愭爣
                   {
                       maxPoints.clear();
                       maxScore = scoreMapVec[row][col];
                       maxPoints.push_back(std::make_pair(row, col));
                   }
                   else if (scoreMapVec[row][col] == maxScore)     // 濡傛灉鏈夊涓渶澶х殑鏁帮紝閮藉瓨璧锋潵
                       maxPoints.push_back(std::make_pair(row, col));
               }
           }
       // 闅忔満钀藉瓙锛屽鏋滄湁澶氫釜鐐圭殑璇?
       srand((unsigned)time(0));
       int index = rand() % maxPoints.size();

       std::pair<int, int> pointPair = maxPoints.at(index);
       clickRow = pointPair.first; // 璁板綍钀藉瓙鐐?
       clickCol = pointPair.second;
       updateGameMap(clickRow, clickCol);
       #else
         actionByAI1(clickRow, clickCol);
       #endif
   }
   else
   {
    //当前点一定会赢/一定会输
    for (int row = 0; row < kBoardSizeNum; row++) {
       for (int col = 0; col < kBoardSizeNum; col++) {//枚举当点状态所有情况
           if (gameMapVec[row][col] == 0){
                //无论这步棋对手下还是我下都赢则必须下这步棋
                //我方下该位置能赢
                gameMapVec[row][col]= (playerFlag==false)?-1:1;
                if(isWin(row,col)){
                    printf("Select_Max_index=%d,Select_node.winning_probability=%f\n",1,1.0);
                    fflush(stdout);
                    no_thinking=1;
                    playerFlag = !playerFlag;
                    iswin=1;
                    isSerachFinish=true;
                   break;
                }
                //敌方下该位置能赢
                gameMapVec[row][col]=(!(playerFlag)==false)?-1:1;
                if(isWin(row,col)){
                       gameMapVec[row][col]=(playerFlag==false)?-1:1;
                       no_thinking=1;
                       printf("Select_Max_index=%d,Select_node.winning_probability=%f\n",0,-1.0);
                       fflush(stdout);
                       playerFlag = !playerFlag;
                       iswin=1;
                       isSerachFinish=true;
                       break;
               }
               //还原棋盘
               gameMapVec[row][col]=0;
           }
           if(iswin==1){
               break;
           }
       }
       if(iswin==1){
           break;
       }
    }
    //没有一定会赢/一定会输的点，查看是否有敌方下子后有活4的情况
    if(no_thinking!=1){
        for (int row = 0; row < kBoardSizeNum; row++) {
           for (int col = 0; col < kBoardSizeNum; col++){
                if(gameMapVec[row][col]==0){
                    gameMapVec[row][col]=!(playerFlag)==false?-1:1;
                    if(writeChessHigh_P(row,col)){
                       no_thinking=1;
                       printf("Select_Max_index=%f,Select_node.winning_probability=%f\n",0.5,-0.87);
                       fflush(stdout);
                       gameMapVec[row][col]=playerFlag==false?-1:1;
                       playerFlag = !playerFlag;
                       iswin=1;
                       isSerachFinish=true;
                       break;
                   }else{
                      gameMapVec[row][col]=0;
                   }
                }
            }
        }
    }
    if(no_thinking!=1){
        isSearchOver=false;
        //unsigned int number=40000;//4e+4
        printf("\nMCTS AI Start\n");
        printf("MCTS AI is thinking,please wait......\n");
        fflush(stdout);
        Node node;
        node.count=0;
        node.win_count=0;
        node.winning_probability=0.0;
        node.m_gameMapVec=gameMapVec;
        gameMapVec=MCTS(node,serach_depth[depth_index]).m_gameMapVec;//50000
        winning_probability=node.winning_probability;
        playerFlag = !playerFlag;
        if(Destroy_MCTS_TREE(root)){
           printf("Destroy_Tree Finish\n");
           fflush(stdout);
        }
        if(roundCount%delta_value==delta_value-1){
            depth_index++;
            if(depth_index>Quikly_play_num-1){//如果在AI走45子时全力搜索
                depth_index=Quikly_play_num-1;
            }
        }
     }
   }
   //if(winning_probability!=-0.87&&winning_probability!=-1){
      roundCount++;
   //}
   isSerachFinish=true;
}

void GameModel::actionByAI1(int &clickRow, int &clickCol)
{
   #ifdef AI_1
    calculateScore();
    int maxScore = 0;
    std::vector<std::pair<int, int>> maxPoints;
    for (int row = 1; row < kBoardSizeNum; row++)
        for (int col = 1; col < kBoardSizeNum; col++)
        {
            if (gameMapVec[row][col] == 0)
            {
                if (scoreMapVec[row][col] > maxScore)          // 鎵炬渶澶х殑鏁板拰鍧愭爣
                {
                    maxPoints.clear();
                    maxScore = scoreMapVec[row][col];
                    maxPoints.push_back(std::make_pair(row, col));
                }
                else if (scoreMapVec[row][col] == maxScore)     // 濡傛灉鏈夊涓渶澶х殑鏁帮紝閮藉瓨璧锋潵
                    maxPoints.push_back(std::make_pair(row, col));
            }
        }

    // 闅忔満钀藉瓙锛屽鏋滄湁澶氫釜鐐圭殑璇?
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();

    std::pair<int, int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first; // 璁板綍钀藉瓙鐐?
    clickCol = pointPair.second;
    updateGameMap(clickRow, clickCol);
   #endif
   printf("Basic AI Start.\n");
   fflush(stdout);
   AI ai(this->gameMapVec);
   //ai.cb=this->gameMapVec;
   for (int row = 0; row < kBoardSizeNum; row++){
       for (int col = 0; col < kBoardSizeNum; col++)
       {
          if(ai.cb[row][col]==-1){
              ai.cb[row][col]=2;
          }
       }
   }
   //showGrapih(ai.cb);
   // man 1（白 1） computer 2(黑 -1)
   //此时计算机下 白棋先下
   int f1=playerFlag? 1:2;
   QTime time;
   time.start();
   ai.x_y(clickRow, clickCol,f1);
   float delte_time=time.elapsed()/1000.0;
   updateGameMap(clickRow, clickCol);
   printf("Current basic AI runtime:%fs.\n", delte_time);
   fflush(stdout);
}

// 最关键的计算评分函数
void GameModel::calculateScore()
{
    // 统计玩家或者电脑连成的子
    int personNum = 0; // 玩家连成子的个数
    int botNum = 0; // AI连成子的个数
    int emptyNum = 0; // 各方向空白位的个数

    // 清空评分数组
    scoreMapVec.clear();
    for (int i = 0; i < kBoardSizeNum; i++)
    {
        std::vector<int> lineScores;
        for (int j = 0; j < kBoardSizeNum; j++)
            lineScores.push_back(0);
        scoreMapVec.push_back(lineScores);
    }

    // 计分（此处是完全遍历，其实可以用bfs或者dfs加减枝降低复杂度，通过调整权重值，调整AI智能程度以及攻守风格）
    for (int row = 0; row < kBoardSizeNum; row++)
        for (int col = 0; col < kBoardSizeNum; col++)
        {
            // 空白点就算
            if (row > 0 && col > 0 &&
                gameMapVec[row][col] == 0)
            {
                // 遍历周围八个方向
                for (int y = -1; y <= 1; y++)
                    for (int x = -1; x <= 1; x++)
                    {
                        // 重置
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;

                        // 原坐标不算
                        if (!(y == 0 && x == 0))
                        {
                            // 每个方向延伸4个子

                            // 对玩家白子评分（正反两个方向）
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    gameMapVec[row + i * y][col + i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                         col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                         gameMapVec[row + i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    gameMapVec[row - i * y][col - i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                         gameMapVec[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (personNum == 1)                      // 杀二
                                scoreMapVec[row][col] += 10;
                            else if (personNum == 2)                 // 杀三
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 30;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 40;
                            }
                            else if (personNum == 3)                 // 杀四
                            {
                                // 量变空位不一样，优先级不一样
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 60;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 110;
                            }
                            else if (personNum == 4)                 // 杀五
                                scoreMapVec[row][col] += 10100;

                            // 进行一次清空
                            emptyNum = 0;

                            // 对AI黑子评分
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    gameMapVec[row + i * y][col + i * x] == 1) // 玩家的子
                                {
                                    botNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                         col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                         gameMapVec[row +i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    gameMapVec[row - i * y][col - i * x] == -1) // AI的子
                                {
                                    botNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                         gameMapVec[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (botNum == 0)                      // 普通下子
                                scoreMapVec[row][col] += 5;
                            else if (botNum == 1)                 // 活二
                                scoreMapVec[row][col] += 10;
                            else if (botNum == 2)
                            {
                                if (emptyNum == 1)                // 死三
                                    scoreMapVec[row][col] += 25;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 50;  // 活三
                            }
                            else if (botNum == 3)
                            {
                                if (emptyNum == 1)                // 死四
                                    scoreMapVec[row][col] += 55;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 100; // 活四
                            }
                            else if (botNum >= 4)
                                scoreMapVec[row][col] += 10000;   // 活五

                        }
                    }

            }
        }
}

bool GameModel::isWin(int row, int col)
{
    //qDebug()<<"isWinStrat\n";
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 水平方向
    for (int i = 0; i <5; i++)
    {
        // 往左5个，往右匹配4个子，20种情况
        if(col - i >= 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 1] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 2] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 3] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 4])
            return true;
    }

    // 竖直方向(上下延伸4个)
    for (int i = 0; i < 5; i++)
    {
        if (row - i >= 0 &&
            row - i + 4 < kBoardSizeNum &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 1][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 2][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 3][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 4][col])
            return true;
    }

    // 左斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row + i < kBoardSizeNum &&
            row + i - 4 >= 0 &&
            col - i >= 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 1][col - i + 1] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 2][col - i + 2] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 3][col - i + 3] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 4][col - i + 4])
            return true;
    }

    // 右斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row - i >= 0 &&
            row - i + 4 < kBoardSizeNum &&
            col - i >= 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 1][col - i + 1] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 2][col - i + 2] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 3][col - i + 3] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 4][col - i + 4])
            return true;
    }
    //qDebug()<<"isWinEND\n";
    return false;
}
bool GameModel::isWin(std::vector<std::vector<int>> map,int row, int col){
    //qDebug()<<"isWinStrat\n";
        // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
        // 水平方向
        for (int i = 0; i <5; i++)
        {
            // 往左5个，往右匹配4个子，20种情况
            if(col - i >= 0 &&
                col - i + 4 < kBoardSizeNum &&
                map[row][col - i] == map[row][col - i + 1] &&
                map[row][col - i] == map[row][col - i + 2] &&
                map[row][col - i] == map[row][col - i + 3] &&
                map[row][col - i] == map[row][col - i + 4])
                return true;
        }

        // 竖直方向(上下延伸4个)
        for (int i = 0; i < 5; i++)
        {
            if (row - i >= 0 &&
                row - i + 4 < kBoardSizeNum &&
                map[row - i][col] == map[row - i + 1][col] &&
                map[row - i][col] == map[row - i + 2][col] &&
                map[row - i][col] == map[row - i + 3][col] &&
                map[row - i][col] == map[row - i + 4][col])
                return true;
        }

        // 左斜方向
        for (int i = 0; i < 5; i++)
        {
            if (row + i < kBoardSizeNum &&
                row + i - 4 >= 0 &&
                col - i >= 0 &&
                col - i + 4 < kBoardSizeNum &&
                map[row + i][col - i] == map[row + i - 1][col - i + 1] &&
                map[row + i][col - i] == map[row + i - 2][col - i + 2] &&
                map[row + i][col - i] == map[row + i - 3][col - i + 3] &&
                map[row + i][col - i] == map[row + i - 4][col - i + 4])
                return true;
        }

        // 右斜方向
        for (int i = 0; i < 5; i++)
        {
            if (row - i >= 0 &&
                row - i + 4 < kBoardSizeNum &&
                col - i >= 0 &&
                col - i + 4 < kBoardSizeNum &&
                map[row - i][col - i] == map[row - i + 1][col - i + 1] &&
                map[row - i][col - i] == map[row - i + 2][col - i + 2] &&
                map[row - i][col - i] == map[row - i + 3][col - i + 3] &&
                map[row - i][col - i] == map[row - i + 4][col - i + 4])
                return true;
        }
        //qDebug()<<"isWinEND\n";
        return false;
}

bool GameModel::isDeadGame()
{
    // 所有空格全部填满
    for (int i = 1; i < kBoardSizeNum; i++)
        for (int j = 1; j < kBoardSizeNum; j++)
        {
            if (!(gameMapVec[i][j] == 1 || gameMapVec[i][j] == -1))
                return false;
        }
    return true;
}
//禁手（对先手有效）返回为1在禁手处
int  GameModel::do_not(int x, int y, int playerID)
{
    if (playerID == 2)
        return 0;
    int CBWIDTH=15;
    int cb[CBWIDTH][CBWIDTH];
    for(int i=0;i<CBWIDTH;i++){
        for(int j=0;j<CBWIDTH;j++){
            cb[i][j]=gameMapVec[i][j];
        }
    }
    cb[x][y] = playerID;
    {
        //长连禁手
        int l = 0;
        int count = 0;
        for (int k = -4; k <= 4; k++)		//横向判断
        {
            if (y + k < 0)
                continue;
            if (y + k >= CBWIDTH)
                break;
            if (cb[x][y + k] == playerID)
            {
                count++;
                if (count == 5 && k == 4)
                {
                    cb[x][y] = 0;
                    return -1;
                }
                if (count >= 6)
                {
                    l++;
                    break;
                }
            }
            else
            {
                if (count == 5)
                {
                    cb[x][y] = 0;
                    return -1;
                }
                count = 0;
                continue;
            }
        }
        count = 0;
        for (int k = -4; k <= 4; k++)		//纵向判断
        {
            if (x + k < 0)
                continue;
            if (x + k >= CBWIDTH)
                break;
            if (cb[x + k][y] == playerID)
            {
                count++;
                if (count == 5 && k == 4)
                {
                    cb[x][y] = 0;
                    return -1;
                }
                if (count >= 6)
                {
                    l++;
                    break;
                }
            }
            else
            {
                if (count == 5)
                {
                    cb[x][y] = 0;
                    return -1;
                }
                count = 0;
                continue;
            }
        }
        count = 0;
        for (int k = -4; k <= 4; k++)					//右下方判断
        {
            if (x + k < 0 || y + k < 0)
                continue;
            if (x + k >= CBWIDTH || y + k >= CBWIDTH)
                break;
            if (cb[x + k][x + k] == playerID)
            {
                count++;
                if (count == 5 && k == 4)
                {
                    cb[x][y] = 0;
                    return -1;
                }
                if (count >= 6)
                {
                    l++;
                    break;
                }
            }
            else
            {
                if (count == 5)
                {
                    cb[x][y] = 0;
                    return -1;
                }
                count = 0;
                continue;
            }
        }
        count = 0;
        for (int k = -4; k <= 4; k++)		//左下方判断
        {
            if (x - k >= CBWIDTH || y + k <= 0)
                continue;
            if (x - k < 0 || y + k >= CBWIDTH)
                break;
            if (cb[x - k][y + k] == playerID)
            {
                count++;
                if (count == 5 && k == 4)
                {
                    cb[x][y] = 0;
                    return -1;
                }
                if (count >= 6)
                {
                    l++;
                }
            }
            else
            {
                if (count == 5)
                {
                    cb[x][y] = 0;
                    return -1;
                }
                count = 0;
                continue;
            }
        }
        if (l >= 1)
        {
            cb[x][y] = 0;
            return 1;
        }
    }


    {
        //四四禁手
        int four = 0;
        int count = 0;
        int ocount = 0;
        int four1 = 0;
        int four2 = 0;
        int four3 = 0;
        int four4 = 0;
        for (int k = -4; k <= 0; k++)  //横向搜索
        {
            if (y + k < 0)
                continue;
            if (y + k >= CBWIDTH - 4)
                break;
            count = 0;
            ocount = 0;
            for (int i = 0; i < 5; i++)
            {
                if (cb[x][y + k + i] == playerID)
                    count++;
                else if (cb[x][y + k + i] == 0)
                    ocount++;
                else
                {
                    break;
                }

            }
            if (count == 4 && ocount == 1)
            {
                if (cb[x][y + k - 1] == playerID && cb[x][y + k + 5] == 0)
                {
                    if (cb[x][y + k] == 0)
                    {
                        four1++;
                        break;
                    }
                    else
                        break;
                }
                else if (cb[x][y + k - 1] == 0 && cb[x][y + k + 5] == playerID)
                {
                    if (cb[x][y + k + 4] == 0)
                    {
                        four1++;
                        break;
                    }
                    else
                        break;
                }
                else if (cb[x][y + k - 1] != playerID && cb[x][y + k + 5] != playerID)
                {
                    four1++;
                    if (cb[x][y + k + 4] == 0 || cb[x][y + k] == 0)
                    {
                        four1 = 1;
                        break;
                    }
                }
                else
                    break;
            }
        }
        for (int k = -4; k <= 0; k++)		//纵向搜索
        {
            if (x + k < 0)
                continue;
            if (x + k >= CBWIDTH - 4)
                break;
            count = 0;
            ocount = 0;
            for (int i = 0; i < 5; i++)
            {
                if (cb[x + k + i][y] == playerID)
                    count++;
                else if (cb[x + k + i][y] == 0)
                    ocount++;
                else
                {
                    break;
                }

            }
            if (count == 4 && ocount == 1)
            {
                if (cb[x + k - 1][y] == playerID && cb[x + k + 5][y] == 0)
                {
                    if (cb[x + k][y] == 0)
                    {
                        four2++;
                        break;
                    }
                    else
                        break;
                }
                else if (cb[x + k - 1][y] == 0 && cb[x + k + 5][y] == playerID)
                {
                    if (cb[x + k + 4][y] == 0)
                    {
                        four2++;
                        break;
                    }
                    else
                        break;
                }
                else if (cb[x + k - 1][y] != playerID && cb[x + k + 5][y] != playerID)
                {
                    four2++;
                    if (cb[x + k + 4][y] == 0 || cb[x + k][y] == 0)
                    {
                        four2 = 1;
                        break;
                    }
                }
                else
                    break;
            }
        }
        for (int k = -4; k <= 0; k++)			// 右下方搜索
        {
            if (x + k < 0 || y + k < 0)
                continue;
            if (x + k >= CBWIDTH - 4 || y + k >= CBWIDTH - 4)
                break;
            count = 0;
            ocount = 0;
            for (int i = 0; i < 5; i++)
            {
                if (cb[x + k + i][y + k + i] == playerID)
                    count++;
                else if (cb[x + k + i][y + k + i] == 0)
                    ocount++;
                else
                {
                    break;
                }
            }
            if (count == 4 && ocount == 1)
            {
                if (cb[x + k - 1][y + k - 1] == playerID && cb[x + k + 5][y + k + 5] == 0)
                {
                    if (cb[x + k][y + k] == 0)
                    {
                        four3++;
                        break;
                    }
                    else
                        break;
                }
                else if (cb[x + k - 1][y + k - 1] == 0 && cb[x + k + 5][y + k + 5] == playerID)
                {
                    if (cb[x + k + 4][y + k + 4] == 0)
                    {
                        four3++;
                        break;
                    }
                    else
                        break;
                }
                else if (cb[x + k - 1][y + k - 1] != playerID && cb[x + k + 5][y + k + 5] != playerID)
                {
                    four3++;
                    if (cb[x + k + 4][y + k + 4] == 0 || cb[x + k][y + k] == 0)
                    {
                        four3 = 1;
                        break;
                    }
                }
                else
                    break;
            }
        }
        for (int k = -4; k <= 0; k++)				//左下方搜索
        {
            if (x + k < 0 || y - k >= CBWIDTH)
                continue;
            if (x + k >= CBWIDTH - 4 || y - k < 4)
                break;
            count = 0;
            ocount = 0;
            for (int i = 0; i < 5; i++)
            {
                if (cb[x + k + i][y - k - i] == playerID)
                    count++;
                else if (cb[x + k + i][y - k - i] == 0)
                    ocount++;
                else
                {
                    break;
                }
            }
            if (count == 4 && ocount == 1)
            {
                if (cb[x + k - 1][y - k + 1] == playerID && cb[x + k + 5][y - k - 5] == 0)
                {
                    if (cb[x + k][y - k] == 0)
                    {
                        four4++;
                        break;
                    }
                    else
                        break;
                }
                else if (cb[x + k - 1][y - k + 1] == 0 && cb[x + k + 5][y - k - 5] == playerID)
                {
                    if (cb[x + k + 4][y - k - 4] == 0)
                    {
                        four4++;
                        break;
                    }
                    else
                        break;
                }
                else if (cb[x + k - 1][y - k + 1] != playerID && cb[x + k + 5][y - k - 5] != playerID)
                {
                    four4++;
                    if (cb[x + k + 4][y - k - 4] == 0 || cb[x + k][y - k] == 0)
                    {
                        four4 = 1;
                        break;
                    }
                }
                else
                    break;
            }
        }
        four = four1 + four2 + four3 + four4;
        if (four >= 2)
        {
            cb[x][y] = 0;
            return 1;
        }

    }



    {
        //三三禁手
        int count = 0;
        int ocount = 0;
        int three = 0;
        for (int k = -4; k <= 0; k++)			//横向搜索
        {
            if (y + k < 0)
                continue;
            if (y + k > CBWIDTH - 4)
                break;
            count = 0;
            ocount = 0;
            if (cb[x][y + k] == playerID)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (cb[x][y + k + i] == playerID)
                        count++;
                    else if (cb[x][y + k + i] == 0)
                        ocount++;
                    else
                    {
                        break;
                    }
                }
            }
            if (count == 3 && ocount == 2)
            {
                if (cb[x][y + k - 2] == playerID)
                    continue;
                if (cb[x][y + k - 1] == 0 && cb[x][y + k + 4] == 0)
                {
                    if (cb[x][y + k + 3] == playerID && cb[x][y + k + 5] == playerID)
                        break;
                    three++;
                    break;
                }
            }
            else if (count == 3 && ocount == 1)
            {
                if (cb[x][y + k + 3] == 0 && cb[x][y + k - 1] == 0 && cb[x][y + k - 2] == 0)
                {
                    three++;
                    break;
                }
            }

        }
        for (int k = -4; k <= 0; k++)			//纵向搜索
        {
            if (x + k < 0)
                continue;
            if (x + k > CBWIDTH - 4)
                break;
            count = 0;
            ocount = 0;
            if (cb[x + k][y] == playerID)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (cb[x + k + i][y] == playerID)
                        count++;
                    else if (cb[x + k + i][y] == 0)
                        ocount++;
                    else
                    {
                        break;
                    }
                }
            }
            if (count == 3 && ocount == 2)
            {
                if (cb[x + k - 2][y] == playerID)
                    continue;
                if (cb[x + k - 1][y] == 0 && cb[x + k + 4][y] == 0)
                {
                    if (cb[x + k + 3][y] == playerID && cb[x + k + 5][y] == playerID)
                        break;
                    three++;
                    break;
                }
            }
            else if (count == 3 && ocount == 1)
            {
                if (cb[x + k + 3][y] == 0 && cb[x + k - 1][y] == 0 && cb[x + k - 2][y] == 0)
                {
                    three++;
                    break;
                }
            }

        }
        for (int k = -4; k <= 0; k++)				//右下方搜索
        {
            if (x + k < 0 || y + k < 0)
                continue;
            if (x + k > CBWIDTH - 4 || y + k > CBWIDTH - 4)
                break;
            count = 0;
            ocount = 0;
            if (cb[x + k][y + k] == playerID)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (cb[x + k + i][y + k + i] == playerID)
                        count++;
                    else if (cb[x + k + i][y + k + i] == 0)
                        ocount++;
                    else
                    {
                        break;
                    }
                }
            }
            if (count == 3 && ocount == 2)
            {
                if (cb[x + k - 2][y + k - 2] == playerID)
                    continue;
                if (cb[x + k - 1][y + k - 1] == 0 && cb[x + k + 4][y + k + 4] == 0)
                {
                    if (cb[x + k + 3][y + k + 3] == playerID && cb[x + k + 5][y + k + 5] == playerID)
                        break;
                    three++;
                    break;
                }
            }
            else if (count == 3 && ocount == 1)
            {
                if (cb[x + k + 3][y + k + 3] == 0 && cb[x + k - 1][y + k - 1] == 0 && cb[x + k - 2][y + k - 2] == 0)
                {
                    three++;
                    break;
                }
            }
        }
        for (int k = -4; k <= 0; k++)				//左下方搜索
        {
            if (x + k < 0 || y - k >= CBWIDTH)
                continue;
            if (x + k > CBWIDTH - 4 || y - k < 4)
                break;
            count = 0;
            ocount = 0;
            if (cb[x + k][y - k] == playerID)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (cb[x + k + i][y - k - i] == playerID)
                        count++;
                    else if (cb[x + k + i][y - k - i] == 0)
                        ocount++;
                    else
                    {
                        break;
                    }
                }
            }
            if (count == 3 && ocount == 2)
            {
                if (cb[x + k - 2][y - k + 2] == playerID)
                    continue;
                if (cb[x + k - 1][y - k + 1] == 0 && cb[x + k + 4][y - k - 4] == 0)
                {
                    if (cb[x + k + 3][y - k - 3] == playerID && cb[x + k + 5][y - k - 5] == playerID)
                        break;
                    three++;
                    break;
                }
            }
            else if (count == 3 && ocount == 1)
            {
                if (cb[x + k + 3][y - k - 3] == 0 && cb[x + k - 1][y - k + 1] == 0 && cb[x + k - 2][y - k + 2] == 0)
                {
                    three++;
                    break;
                }
            }
        }
        if (three >= 2)
        {
            cb[x][y] = 0;
            return 1;
        }

    }

    cb[x][y] = 0;
    return 0;
}
