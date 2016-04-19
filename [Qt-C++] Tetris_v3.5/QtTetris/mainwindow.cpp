#include "mainwindow.h"
#include <ctime>
#include <QPainter>
#include <QPixmap>
#include <QSignalMapper>
#include <QMessageBox>
#include <iostream>
#define SMALLEST_TIME 150
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    win_width = (NUM_X+PADDING)*WIDTH;
    win_height = NUM_Y*WIDTH;
    this->setFixedSize(win_width, win_height);	//視窗大小
    this->setWindowIcon(QIcon(":/Image/1.png"));
    this->setWindowTitle("Qt Double Tetris");
    qsrand(time(NULL));

    pool = new POOL(this);
    for (int i = 0; i < 2; i++){
        int init_block = (qrand() % 7) + 1;
        block[i] = new BLOCK(this, init_block);
    }
    bgm = new easyMusic((char*)"Music/Tetris Battle Music.wav", 50, 1);
    bgm->play();
    drop_sound = new easyMusic((char*)"Music/drop.wav", 100, 0);
    elim_sound = new easyMusic((char*)"Music/eliminate.wav", 100, 0);
    rotate_sound = new easyMusic((char*)"Music/rotate.wav", 100, 0);
    gamemsg = new GAMEMSG(this);
    Number = new number(this);
    gameRedy();
}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // 在這裡設定按下鍵盤要觸發的功能
    // close game
    // cout << event->key() << endl;

    switch (gamemod) {
        case start:
            if (event->key() == Qt::Key_Escape) {gameLose(); gameRedy(); return;}
            if (event->key() == Qt::Key_W){
                // rotate first
                block[0]->rotate();
                QPoint original = block[0]->pos();
                int prefix = block[0]->getPrefix();
                // if at left boundary shift back
                if (block[0]->pos().x() < (-prefix)*WIDTH)
                    block[0]->move((-prefix)*WIDTH, block[0]->pos().y());
                // if collide someone rotate and move bcak
                if (collide(0,0,0)) {
                    block[0]->rotate_inv();
                    block[0]->move(original);
                }
                else {rotate_sound->stop(); rotate_sound->play();}
                block[0]->update();
            }
            if (event->key() == Qt::Key_S)
                blockAction(1); // (block, down) = (0, 1)
            if (event->key() == Qt::Key_A){
                int res = collide(0,-1,0);
                if (!res) block[0]->move(block[0]->pos().x() - WIDTH, block[0]->pos().y());
                else if (res == 2) pushBlock(0, 1, -1, 0);
            }
            if (event->key() == Qt::Key_D){
                int res = collide(0,1,0);
                if (!res) block[0]->move(block[0]->pos().x() + WIDTH, block[0]->pos().y());
                else if (res == 2) pushBlock(0, 1, 1, 0);
            }
            if (event->key() == Qt::Key_Space) gamePause();
            if (event->key() == Qt::Key_E) {
                int res = collide(0,0,1);
                while (res != 1) {
                    if (!res) blockAction(1);
                    else pushBlock(0, 1, 0, 1);
                    res = collide(0,0,1);
                    //cout << res << endl;
                }
            }
            if (event->key() == Qt::Key_0) {
                int res = collide(1,0,1);
                while (res != 1) {
                    if (!res) blockAction(3);
                    else pushBlock(1, 0, 0, 1);
                    res = collide(1,0,1);
                    //cout << res << endl;
                }
            }
            if (event->key() == Qt::Key_Up){
                block[1]->rotate();
                QPoint original = block[1]->pos();
                int prefix = block[1]->getPrefix();
                if (block[1]->pos().x() < (-prefix)*WIDTH)
                    block[1]->move((-prefix)*WIDTH, block[1]->pos().y());
                if (collide(1,0,0)) {
                    block[1]->rotate_inv();
                    block[1]->move(original);
                }
                else {rotate_sound->stop(); rotate_sound->play();}
                block[1]->update();
            }
            if (event->key() == Qt::Key_Down)
                blockAction(3); // (block, down) = (1, 1)
            if (event->key() == Qt::Key_Left){
                int res = collide(1,-1,0);
                if (!res) block[1]->move(block[1]->pos().x() - WIDTH, block[1]->pos().y());
                else if (res == 2) pushBlock(1, 0, -1, 0);
            }
            if (event->key() == Qt::Key_Right){
                int res = collide(1,1,0);
                if (!res) block[1]->move(block[1]->pos().x() + WIDTH, block[1]->pos().y());
                else if (res == 2) pushBlock(1, 0, 1, 0);
            }
            break;
        case lose:
            gameRedy();
            break;
        case redy:
            if (event->key() == Qt::Key_Escape) close();
            else gameStart();
            break;
        default:
            gameStart();
            break;
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // 在這裡設定按下滑鼠要觸發的功能
    switch (gamemod) {
            case start:
                gamePause();
                break;
            case lose:
                gameRedy();
                break;
            case redy:
                gameStart();
                break;
            default:
                gameStart();
                break;
     }

}


void MainWindow::paintEvent(QPaintEvent *)		//繪圖事件, 用来產生背景
{
    // 可以在這裡新增背景圖片
    QPainter painter(this);
    QPixmap bgImg[2];
    bgImg[0].load(":/Image/grid4.png");
    bgImg[1].load(":/Image/grid5.png");
    painter.setOpacity(0.8);
    for (int i = 0; i < NUM_X; i++)
        for (int j = 0; j < NUM_Y; j++)
            painter.drawPixmap(i*WIDTH, j*WIDTH, WIDTH, WIDTH, bgImg[(i+j)%2]);
}

void MainWindow::createBlock(){
    Number->level = 0;
    thesholdscore = 1000;
    // reset pool map
    Number->setnum(0);
    pool->clean();
    pool->update();
    for (int i = 0; i < 2; i++){
        blockTimer[i] = new QTimer(this);
        droptime[i] = 500;
        // int x = (qrand() % (7 + block[i]->getPrefix())) - block[i]->getPrefix();
        block[i]->move((i*NUM_X/2+qrand()%(NUM_X/2-4+1))*WIDTH, -4*WIDTH);
        QSignalMapper* signalMapper = new QSignalMapper (this) ;
        connect(blockTimer[i], SIGNAL(timeout()), signalMapper, SLOT(map()));
        signalMapper->setMapping(blockTimer[i], i << 1);
        connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(blockAction(int)));
    }
    connect(blockTimer[0], SIGNAL(timeout()), this, SLOT(scoreCheck()));
}

void MainWindow::scoreCheck(){
    if (droptime[0] >= SMALLEST_TIME && Number->getnum() >= thesholdscore) {
        thesholdscore += 1000;
        for (int i = 0; i < 2; i++){
            droptime[i] -= 50;
            // cout << droptime[i] << endl;
            blockTimer[i]->stop();
            blockTimer[i]->start(droptime[i]);
        }
        bgm->louderOrlower(1);
    }
}

void MainWindow::changeBlock(int i){
    int new_block = (qrand() % 7) + 1;
    block[i]->change(new_block);
    // int x = (qrand() % (7 + block[i]->getPrefix())) - block[i]->getPrefix();
    block[i]->move((i*NUM_X/2+qrand()%(NUM_X/2-4+1))*WIDTH, -4*WIDTH);
}

void MainWindow::pushBlock(int pusher, int pushee, int x, int y){
    if (collide(pushee, x, y) == 1) {blockAction(2*pushee+1); return;}
    else {
        block[pushee]->move(block[pushee]->pos().x() + x*WIDTH, block[pushee]->pos().y() + y*WIDTH);
        block[pusher]->move(block[pusher]->pos().x() + x*WIDTH, block[pusher]->pos().y() + y*WIDTH);
    }
}


int MainWindow::collide(int i, int dx, int dy){
    // block1 and pool
    int x, y;
    x = block[i]->pos().x()/WIDTH + 3 + dx;
    y = block[i]->pos().y()/WIDTH + 4 + dy;
    int blksp = block[i]->getBlockSp(), poolsp = pool->getPoolSp(x, y);

    // block2
    int x2, y2;
    x2 = block[1-i]->pos().x()/WIDTH + 3;
    y2 = block[1-i]->pos().y()/WIDTH + 4;
    int blk2sp = block[1-i]->getBlockSp();

    // store block2 to tmp pool
    POOL *tmppool = new POOL();
    for (int k = 3; k >= 0; k--)
        for (int j = 3; j >= 0; j--, blk2sp >>= 1)
           if (blk2sp & 1) tmppool->map[x2+j][y2+k] = block[1-i]->kind;
    int pool2sp = tmppool->getPoolSp(x, y);
    delete tmppool;

    // cmp block1 with pool and then block1 with block2
    return (blksp & poolsp) ? 1 : (blksp & pool2sp) ? 2 : 0;
}

void MainWindow::blockAction(int i2){
    // i2->(block,down): 0->(0,0), 1->(0,1), 2->(1,0), 3->(1,1)
    int i = i2 >> 1;
    int down = i2 & 1;
    // lose
    for (int k = 3; k < X_SPACE-1; k++) if (pool->map[k][3]) {
        if (Number->getnum() > Number->getHighScore()) Number->setHighScore(Number->getnum());
        gameLose();
        return;
    }
    // touch bottom
    if (collide(i,0,1) == 1){
        drop_sound->stop();
        drop_sound->play();
        int x, y, blksp = block[i]->getBlockSp();
        x = block[i]->pos().x()/WIDTH + 3;
        y = block[i]->pos().y()/WIDTH + 4;
        // store in pool
        for (int k = 3; k >= 0; k--)
            for (int j = 3; j >= 0; j--, blksp >>= 1)
               if (blksp & 1) pool->map[x+j][y+k] = block[i]->kind;
        pool->update();

        // judge if some row(s) can be eliminated
        int res = pool->eliminate();
        if (res){
            Number->setnum(Number->getnum()+(1<<(res-1))*100);
            // the more row(s) are eliminated, the larger the volumn will be.
            elim_sound->setVolume(60+10*res);
            elim_sound->stop();
            elim_sound->play();
        }
        pool->update();
        // change the block back to top
        changeBlock(i);
    }
    // the upper block touches the lower block and the upper one wants to descend
    else if (collide(i,0,1) == 2 && down) pushBlock(i, 1-i, 0, 1);
    // the upper block touches the lower block and the lower one touch the bottom
    else if (collide(i,0,1) == 2 && collide(1-i,0,1) == 1);
    else block[i]->move(block[i]->pos().x(), block[i]->pos().y() + WIDTH);
}

void MainWindow::gameRedy()
{
    gamemod=redy;
    bgm->setVolume(60);
    createBlock();
    gamemsg->setGameMod(gamemod);
    gamemsg->show();
}

void MainWindow::gameLose()
{
    gamemod=lose;
    bgm->setVolume(60);
    for (int i = 0; i < 2; i++) blockTimer[i]->stop();
    gamemsg->setGameMod(gamemod);
    gamemsg->show();
}

void MainWindow::gamePause()
{
    gamemod=pause;
    bgm->setVolume(40);
    for (int i = 0; i < 2; i++) blockTimer[i]->stop();
    gamemsg->setGameMod(gamemod);
    gamemsg->show();
}


void MainWindow::gameStart()
{
    gamemod=start;
    bgm->setVolume(50);
    for (int i = 0; i < 2; i++) blockTimer[i]->start(droptime[i]);
    gamemsg->hide();
}
