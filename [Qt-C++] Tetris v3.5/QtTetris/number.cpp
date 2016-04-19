#include "number.h"
#include <QPainter>
#include <QPixmap>
#include <math.h>
number::number(QWidget *parent) : QWidget(parent)
{
 this->setFixedSize((NUM_X+PADDING)*WIDTH,(NUM_Y)*WIDTH);
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
 highscore = 0;
 level = 0;
}

number::~number()
{

}

void number::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap no;
    int height = 0;
    no.load(":Image/score.png");
    painter.drawPixmap(SCREEN_RIGHT-(PADDING+0.5)*WIDTH,height,(PADDING-1)*WIDTH,NUM_HEIGHT-10,no);
    height += NUM_HEIGHT-10;
    int num2=num,t=0;
    /*double a=0.5;
    while(num2/=10)a+=0.5;
    num2=num;*/
    do{
     t++;
    no.load(numString[num2%10]);
    painter.drawPixmap(SCREEN_RIGHT+NUM_WIDTH/3*(-t),height,NUM_WIDTH/3,NUM_HEIGHT/3,no);
    num2/=10;
    }while(num2>0);
    height += NUM_HEIGHT/3;
    no.load(":Image/highscore.png");
    painter.drawPixmap(SCREEN_RIGHT-(PADDING)*WIDTH,height,(PADDING-1)*WIDTH,NUM_HEIGHT-10,no);
    height += NUM_HEIGHT-10;
    if (num >= highscore){
    highscore = num;
    }
    num2=highscore;
    t=0;
    do{
         t++;
        no.load(numString[num2%10]);
        painter.drawPixmap(SCREEN_RIGHT+NUM_WIDTH/3*(-t),height,NUM_WIDTH/3,NUM_HEIGHT/3,no);
        num2/=10;
   }while(num2>0);
    height += NUM_HEIGHT/3;
    level = num/1000;
    no.load(":Image/level.png");
    painter.drawPixmap(SCREEN_RIGHT-(PADDING+0.3)*WIDTH,height,(PADDING-1)*WIDTH,NUM_HEIGHT-10,no);
    height += NUM_HEIGHT-10;
    num2=level;
    t=0;
        do{
             t++;
            no.load(numString[num2%10]);
            painter.drawPixmap(SCREEN_RIGHT+NUM_WIDTH/3*(-t),height,NUM_WIDTH/3,NUM_HEIGHT/3,no);
            num2/=10;
       }while(num2>0);

}
