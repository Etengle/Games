#include "floor.h"
#include "mainwindow.h"

FLOOR::FLOOR(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1079,600);
}

void FLOOR::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap floor;
    floor.load("Image/floor.png");
    painter.drawPixmap(0,0,1079,150,floor);
}

FLOOR::~FLOOR()
{

}

