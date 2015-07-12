#include "role.h"
#define rate 8

ROLE::ROLE(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(35,35);
    src= new QString[3];
    src[0]=":/Image/bird1.png";
    src[1]=":/Image/bird2.png";
    src[2]=":/Image/bird3.png";

    j=2;
}

ROLE::~ROLE()
{

}

void ROLE::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;

       j=(j+1)%(3*rate);
       pix.load(src[j/rate]);

    painter.translate(17.5,17.5);                //使图片的中心作为旋转的中心
    painter.rotate(birdV*10);                //顺时针旋转90°
    painter.translate(-17.5,-17.5);

    painter.drawPixmap(0,0,35,35,pix);
}

