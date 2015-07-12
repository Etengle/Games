#include "block.h"
#include <iostream>
using namespace std;

BLOCK::BLOCK(QWidget *parent, int k) : QWidget(parent)
{
    this->setFixedSize(WIDTH*4, WIDTH*4);
    src[1] = ":/Image/1.png";
    src[2] = ":/Image/L.png";
    src[3] = ":/Image/J.png";
    src[4] = ":/Image/T.png";
    src[5] = ":/Image/S.png";
    src[6] = ":/Image/Z.png";
    src[7] = ":/Image/#.png";
    kind = k;
    rot = 0;
    int map[8][4] = {
             {15,4369,15,4369},
             {23,785,116,547},
             {71,275,113,802},
             {39,305,114,562},
             {54,561,54,561},
             {99,306,99,306},
             {51,51,51,51},
             {-1}};
    // 1LJTSZ#
    memcpy(sp, map, sizeof(int)*32);
}

BLOCK::~BLOCK()
{

}

void BLOCK::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(src[kind]);
    int bp = sp[kind-1][rot];
    for (int i = 3; i >= 0; i--)
        for (int j = 3; j >= 0; j--, bp >>= 1)
            if (bp & 1) painter.drawPixmap(j*WIDTH, i*WIDTH, WIDTH, WIDTH, pix);
}

void BLOCK::rotate(){
    rot = (rot + 1) % 4;
}

void BLOCK::rotate_inv(){
    rot = (rot + 3) % 4;
}

void BLOCK::change(int k){
    kind = k;
}

// get left space to left edge of the widget
int BLOCK::getPrefix(){
    int res = 4;
    int bp = sp[kind-1][rot];
    bp |= bp >> 4;
    bp |= bp >> 8;
    bp &= 15;
    for (; bp; bp >>= 1, res--);
    return res;
}

int BLOCK::getBlockSp(){
    return sp[kind-1][rot];
}
