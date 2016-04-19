#ifndef FLOOR_H
#define FLOOR_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>



class FLOOR : public QWidget
{
    Q_OBJECT
public:
    explicit FLOOR(QWidget *parent = 0);
    ~FLOOR();

protected:
    void paintEvent(QPaintEvent *);

public:
    QTimer *floorTimer;

private:
    enum{lose=0,start=1,redy=2,pause=3};  // Enum三個參數, 代表遊戲狀態

};

#endif // FLOOR_H
