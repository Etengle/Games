#include "pool.h"
#include <iostream>
using namespace std;

POOL::POOL(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(WIDTH*NUM_X, WIDTH*NUM_Y);
    src[1] = ":/Image/1.png";
    src[2] = ":/Image/L.png";
    src[3] = ":/Image/J.png";
    src[4] = ":/Image/T.png";
    src[5] = ":/Image/S.png";
    src[6] = ":/Image/Z.png";
    src[7] = ":/Image/#.png";
    clean();
}

int POOL::getPoolSp(int x, int y){
    int sp = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            sp = sp*2 + (map[x+j][y+i] != 0 ? 1 : 0);
    return sp;
}

void POOL::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    for (int i = 3; i < X_SPACE-1; i++){
        for (int j = 4; j < Y_SPACE-1; j++){
            if (map[i][j]){
                pix.load(src[map[i][j]]);
                painter.drawPixmap((i-3)*WIDTH, (j-4)*WIDTH, WIDTH, WIDTH, pix);
            }
        }
    }
}

void POOL::clean(){
    for (int i = 0; i < X_SPACE; i++)
            for (int j = 0; j < Y_SPACE; j++)
                map[i][j] = i < 3 || i == X_SPACE-1 || j == Y_SPACE-1;
}

int POOL::eliminate(){
    int i, res = 0;
    for (int j = Y_SPACE-2; j >= 4; j--){
            for (i = 3; i < X_SPACE-1; i++)
                if (!map[i][j]) break;
            if (i == X_SPACE-1) {reduce(j++); res++;}
    }
    return res;
}

void POOL::reduce(int y){
    for (int j = y; j >= 4; j--)
        for (int i = 3; i < X_SPACE-1; i++)
            map[i][j] = map[i][j-1];
}

POOL::~POOL()
{

}

