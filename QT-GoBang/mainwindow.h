#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameModel.h"
#include <synchapi.h>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void MCTS();
    bool is_Win();
    void Simulation();
    ~MainWindow();

protected:
    bool Ai_isFinish;
    // 绘制
    void paintEvent(QPaintEvent *event);
    // 监听鼠标移动情况，方便落子
    void mouseMoveEvent(QMouseEvent *event);
    // 实际落子
    void mouseReleaseEvent(QMouseEvent *event);
    bool MessageBox(int &quikly_play,float &runtime);
private:
    GameModel *game; // 游戏指针
    GameType game_type; // 存储游戏类型
    int clickPosRow, clickPosCol; // 存储将点击的位置
    void initGame();
    void checkGame(int y, int x);

private slots:
    bool chessOneByPerson(); // 人执行
    bool chessOneByAI(); // AI下棋

    void initPVPGame();
    void initPVEGame();
    void initMCTSGame();
};

#endif // MAINWINDOW_H
