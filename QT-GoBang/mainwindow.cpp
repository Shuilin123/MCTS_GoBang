#include <QPainter>
#include <QTimer>
#include <QSound>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <math.h>
#include "mainwindow.h"
#include<QinputDialog>
#include<QDialog>
#include<QSpinBox>
#include<QFormLayout>
#include<QDialogButtonBox>
#include<QLabel>
// -------全局遍历-------//
#define CHESS_ONE_SOUND ":/res/sound/chessone.wav"
#define WIN_SOUND ":/res/sound/win.wav"
#define LOSE_SOUND ":/res/sound/lose.wav"

const int kBoardMargin = 30; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kMarkSize = 6; // 落子标记边长
const int kBlockSize = 40; // 格子的大小
const int kPosDelta = 20; // 鼠标点击的模糊距离上限

const int kAIDelay = 700; // AI下棋的思考时间//700

// -------------------- //

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
     Ai_isFinish=true;
    //system("cls");
    // 设置棋盘大小
    setFixedSize(kBoardMargin * 2 + kBlockSize * kBoardSizeNum, kBoardMargin * 2 + kBlockSize * kBoardSizeNum);
    //setStyleSheet("background-color:yellow;");

    // 开启鼠标hover功能，这两句一般要设置window的
    this->setWindowTitle("重庆三峡学院");
    setMouseTracking(true);
//    centralWidget()->setMouseTracking(true);

    // 添加菜单
    QMenu *gameMenu = menuBar()->addMenu(tr("Game")); // menuBar默认是存在的，直接加菜单就可以了
    QAction *actionPVP = new QAction("Person VS Person", this);
    connect(actionPVP, SIGNAL(triggered()), this, SLOT(initPVPGame()));
    gameMenu->addAction(actionPVP);

    QAction *actionPVE = new QAction("Person VS Computer", this);
    connect(actionPVE, SIGNAL(triggered()), this, SLOT(initPVEGame()));
    gameMenu->addAction(actionPVE);

    QAction *actionMCTS = new QAction("MCTS VS TestAI", this);
    //不使用
    connect(actionMCTS, SIGNAL(triggered()), this, SLOT(initMCTSGame()));
    //gameMenu->addAction(actionMCTS);


    // 开始游戏
    initGame();
}

MainWindow::~MainWindow()
{
    if (game)
    {
        delete game;
        game = nullptr;
    }
}

void MainWindow::initGame()
{   
    // 初始化游戏模型
    game = new GameModel;
    initPVPGame();
}

void MainWindow::initPVPGame()
{
    game_type = PERSON;
    game->gameStatus = PLAYING;
    game->roundCount=0;
    game->depth_index=0;
    game->startGame(game_type);
    update();
}
bool MainWindow::MessageBox(int &quikly_play,float &runtime){
       /*QString dlgTitle="输入快速走子数";
       QString txtLabel="快速走子";
       int minValue=6, maxValue=50,stepValue=1; //范围，步长
       bool ok=false;
       int inputValue = QInputDialog::getInt(this, dlgTitle,txtLabel,
                                  defaultValue, minValue,maxValue,stepValue,&ok);
       if (ok) //是否确认输入
       {
          quikly_play=inputValue;
       }*/
    QDialog dialog(this);
    QFormLayout form(&dialog);
    //form.addRow(new QLabel("输入游戏参数"));
    // Value1
    QString value1 = QString("快速走子数: ");
    QSpinBox *spinbox1 = new QSpinBox(&dialog);
    spinbox1->setValue(5);
    form.addRow(value1, spinbox1);
    // Value2
    QString value2 = QString("MCTS每步计算时间: ");
    QSpinBox *spinbox2 = new QSpinBox(&dialog);
    spinbox2->setValue(15);
    form.addRow(value2, spinbox2);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        // Do something here
        quikly_play=fabs(spinbox1->value())==0?1:fabs(spinbox1->value());
        runtime=abs(spinbox2->value())==0? 1:abs(spinbox2->value());
        return true;
    }
    return false;
}

void MainWindow::initPVEGame()
{
    game_type = BOT;
    game->roundCount=0;
    game->depth_index=0;
    game->gameStatus = PLAYING;
    if(MessageBox(game->Quikly_play,game->MCTS_runime)){
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("提示");//对话框标题
        msg.setText("设置完成，如果算法收敛很慢请改小MCTS运行时间，");//对话框提示文本
        msg.setIcon(QMessageBox::Information);//设置图标类型
        msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮
        msg.exec();
    }
    game->startGame(game_type);
    update();
}
void MainWindow::Simulation(){
    while(1)
    {
        if(Ai_isFinish){
          chessOneByPerson();
          Ai_isFinish=false;
        }
        if(is_Win()){
            break;
        }
        chessOneByAI();
        //repaint();
        Sleep(150);
    }
}

void MainWindow::initMCTSGame()
{
    game_type = MCTSX;
    game->gameStatus = PLAYING;
    game->roundCount=0;
    srand((unsigned)time(0));
    if(game->startGame(game_type)==1){
        Simulation();
    }

}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // 绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
//    QPen pen; // 调整线条宽度
//    pen.setWidth(2);
//    painter.setPen(pen);
    for (int i = 1; i < kBoardSizeNum + 2; i++)
    {
        QString str=QString('A'+i-1);
        QFont font(this->font());
        font.setPixelSize(24);
        painter.setFont(font);
        painter.drawText(kBoardMargin + kBlockSize * i-5, kBoardMargin+kBlockSize,str);
        painter.drawLine(kBoardMargin + kBlockSize * i, kBoardMargin+kBlockSize, kBoardMargin + kBlockSize * i, size().height() - kBoardMargin);
        str=str.number(i);
        if(i!=0){
        painter.drawText(kBlockSize+kBoardMargin-25, kBoardMargin + kBlockSize * i+10,str);
        }
        painter.drawLine(kBlockSize+kBoardMargin, kBoardMargin + kBlockSize * i, size().width() - kBoardMargin, kBoardMargin + kBlockSize * i);
    }
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    // 绘制落子标记(防止鼠标出框越界)
    if(clickPosRow > 0 && clickPosRow <= kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol <= kBoardSizeNum &&
        game->gameMapVec[clickPosRow-1][clickPosCol-1] == 0)
    {
        if (game->playerFlag)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
    }
    // 绘制棋子 
    for (int i = 0; i < kBoardSizeNum; i++)
        for (int j=0; j < kBoardSizeNum; j++)
        {
            if (game->gameMapVec[i][j] == 1)
            {
                /*QRadialGradient radgd(kBoardMargin + kBlockSize * (j+1) - kRadius, kBoardMargin + kBlockSize * (i+1) - kRadius,kRadius * 2,kBoardMargin + kBlockSize * (j+1) - kRadius+kRadius, kBoardMargin + kBlockSize * (i+1) - kRadius+kRadius);
                radgd.setSpread(QGradient::PadSpread);
                radgd.setColorAt(1,Qt::white);
                radgd.setColorAt(0,Qt::black);
                QBrush burush(radgd);
                */
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * (j+1) - kRadius, kBoardMargin + kBlockSize * (i+1) - kRadius, kRadius * 2, kRadius * 2);
                //printf("i=%d j=%d\n",i,j);
            }
            else if (game->gameMapVec[i][j] == -1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * (j+1) - kRadius, kBoardMargin + kBlockSize * (i+1) - kRadius, kRadius * 2, kRadius * 2);
                //printf("i=%d j=%d\n",i,j);
            }
        }
    //is_Win(clickPosRow,clickPosCol);
    // 判断死局
    if (game->isDeadGame())
    {
        QSound::play(LOSE_SOUND);
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "oops", "dead game!");
        if (btnValue == QMessageBox::Ok)
        {
            game->startGame(game_type);
            game->gameStatus = PLAYING;
        }

    }
    // 存了坐标后也要重绘
    if(game->gameType!=MCTSX){
      update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // 人下棋，并且不能抢机器的棋
    if (!(game_type == BOT && !game->playerFlag))
    {
        chessOneByPerson();
        // 如果是人机模式，需要调用AI下棋
        if (game->gameType == BOT && !game->playerFlag)
        {
            // 用定时器做一个延迟
            QTimer::singleShot(kAIDelay, this, SLOT(chessOneByAI()));
        }
    }

}

bool MainWindow::chessOneByPerson()
{
    // 根据当前存储的坐标下子
    // 只有有效点击才下子，并且该处没有子
    if(game->gameType==MCTSX){
        //game->actionByAI1(clickPosRow, clickPosCol);
        bool tempx=true;
        game->actionByAI(tempx,clickPosRow, clickPosCol);
    }
    else{
        if (clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow-1][clickPosCol-1] == 0)
        {
            game->actionByPerson(clickPosRow-1, clickPosCol-1);
        }
    }
    //QSound::play(CHESS_ONE_SOUND);
    is_Win();
    //update();
    repaint();
    /*if (clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow-1][clickPosCol-1] == 0)
    {

        game->actionByPerson(clickPosRow-1, clickPosCol-1);
        is_Win();
        QSound::play(CHESS_ONE_SOUND);
        // 重绘
        update();
    }*/

}

bool MainWindow::chessOneByAI()
{
     bool temp;
    //game->actionByAI(clickPosRow, clickPosCol);
    if(!game->isRunning()){
        game->start();
    }
    if(game->getThreadFlags())
    {
        game->getPostion(clickPosRow, clickPosCol);
        game->setThreadFlags(false);
        is_Win();
        Ai_isFinish=true;
       //QSound::play(CHESS_ONE_SOUND);
    }
    //update();
    repaint();
    return temp;
}
bool MainWindow::is_Win(){

    for (int row = 0; row < kBoardSizeNum; row++){
       for (int col = 0; col < kBoardSizeNum; col++){
           if (game->gameMapVec[row][col]!=0&&game->isWin(row, col) &&game->gameStatus == PLAYING)
           {
              game->gameStatus = WIN;
              QSound::play(WIN_SOUND);
              QString str;
              if (game->gameMapVec[row][col] == 1)
                str = "white player";
              else if (game->gameMapVec[row][col] == -1)
                str = "black player";
              QMessageBox::StandardButton btnValue = QMessageBox::information(this, "congratulations", str + " win!");
             // 重置游戏状态，否则容易死循环
            if (btnValue == QMessageBox::Ok)
            {
               game->startGame(game_type);
               game->gameStatus = PLAYING;
               game->gameType=BOT;
               game->roundCount=0;
               game->depth_index=0;
               game->exit();
               game->wait();
            }
            return true;
         }
      }
   }
    return false;
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 通过鼠标的hover确定落子的标记
    int x = event->x();
    int y = event->y();

    // 棋盘边缘不能落子
    if (x >= kBoardMargin + kBlockSize / 2 &&
            x < size().width() - kBoardMargin &&
            y >= kBoardMargin + kBlockSize / 2 &&
            y < size().height()- kBoardMargin)
    {
        // 获取最近的左上角的点
        int col = x / kBlockSize;
        int row = y / kBlockSize;

        int leftTopPosX = kBoardMargin + kBlockSize * col;
        int leftTopPosY = kBoardMargin + kBlockSize * row;

        // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
        clickPosRow = -1; // 初始化最终的值
        clickPosCol = -1;
        int len = 0; // 计算完后取整就可以了

        // 确定一个误差在范围内的点，且只可能确定一个出来
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col + 1;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
        }
    }
}
