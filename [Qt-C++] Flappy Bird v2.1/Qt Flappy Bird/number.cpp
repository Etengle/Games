#include "number.h"
#include <QPainter>
#include <QPixmap>
#include <math.h>
number::number(QWidget *parent) : QWidget(parent)
{
 this->setFixedSize(440,600);
 numString[0]=":/Image/no0.png";
 numString[1]=":/Image/no1.png";
 numString[2]=":/Image/no2.png";
 numString[3]=":/Image/no3.png";
 numString[4]=":/Image/no4.png";
 numString[5]=":/Image/no5.png";
 numString[6]=":/Image/no6.png";
 numString[7]=":/Image/no7.png";
 numString[8]=":/Image/no8.png";
 numString[9]=":/Image/no9.png";
 numTimer=new QTimer(this);
 connect(numTimer,SIGNAL(timeout()),this,SLOT(update()));
 numTimer->start(100);

}

number::~number()
{

}

void number::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap no;
    int num2=num,t=0;
    double a=0.5;
    while(num2/=10)a+=0.5;
    num2=num;
    do{
     t++;
    no.load(numString[num2%10]);
    painter.drawPixmap(220+45*(a-t),500,45,63,no);
    num2/=10;
    }while(num2>0);
}
