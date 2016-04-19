#ifndef ROLE_H
#define ROLE_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <math.h>

class ROLE : public QWidget
{
    Q_OBJECT
public:
    explicit ROLE(QWidget *parent = 0);
    ~ROLE();
public:
    void paintEvent(QPaintEvent *);
    void setbirdV(double v){
        birdV = v;
        }
private:
    double birdV;
    QString *src;
    int j;

};

#endif // ROLE_H
