#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMouseEvent>
#include <BLOCK.h>
#include <POOL.h>
#include <GAMEMSG.h>
#include "easymusic.h"
#include "number.h"
#define WIDTH 30
#define NUM_X 10
#define NUM_Y 20
#define X_SPACE (NUM_X+3+1)
#define Y_SPACE (NUM_Y+4+1)
#define PADDING 5

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);    //繪製背景圖
    void keyPressEvent(QKeyEvent *event);	//鍵盤事件
    void mousePressEvent(QMouseEvent *event);	//滑鼠點下事件

private slots:
    void blockAction(int i2 = 0);
    void scoreCheck();


private:
    int win_width;      // 視窗寬度
    int win_height;     // 視窗高度
    void gameLose();			 //遊戲結束
    void gameStart();			 //遊戲開始
    void gameRedy();             //遊戲預備
    void gamePause();             //遊戲預備
    enum{lose = 0, start = 1, redy = 2, pause = 3};  // Enum三個參數, 代表遊戲狀態
    // 遊戲狀態預設流程： redy -> start -> lose -> redy -> start .... 不斷循環
    int gamemod;		//目前遊戲狀態, 0 = lose, 1 = start, 2 = redy
    BLOCK *block[2];

    void createBlock();          // 產生方塊
    void changeBlock(int i = 0);
    QTimer *blockTimer[2];
    int droptime[2];
    int collide(int i, int dx, int dy);
    void pushBlock(int pusher, int pushee, int x, int y);

    POOL *pool;
    easyMusic *bgm, *drop_sound, *elim_sound, *rotate_sound;

    GAMEMSG *gamemsg;
    number *Number;     //建立數字
    int thesholdscore;
};

#endif // MAINWINDOW_H
