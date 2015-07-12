#include "gamemsg.h"

GAMEMSG::GAMEMSG(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(WIDTH*NUM_X, WIDTH*NUM_Y);
    gamemod = redy;

}

void GAMEMSG::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    switch (gamemod){
        case redy:
        case pause:
            pix.load(":Image/startbar.png");
            break;
        case lose:
            pix.load(":Image/gameover.png");
            break;
    }
    painter.drawPixmap(0, (NUM_Y/2-MSG_HEIGHT/2)*WIDTH, WIDTH*NUM_X, WIDTH*MSG_HEIGHT, pix);
}

void GAMEMSG::setGameMod(int mode){
    gamemod = mode;
}

GAMEMSG::~GAMEMSG()
{

}
