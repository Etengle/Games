#ifndef BLOCK_H
#define BLOCK_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#define WIDTH 30

/* define all kinds of blocks */
class BLOCK : public QWidget
{
    Q_OBJECT

public:
    explicit BLOCK(QWidget *parent = 0, int k = 0);
    ~BLOCK();
    void paintEvent(QPaintEvent *);
    void rotate();
    void rotate_inv();
    void change(int k = 0);
    int getPrefix();
    QString src[8];
    int kind;
    int rot;      // rotate: choose 1st~4th pattern
    int sp[8][4]; // block pattern
    int getBlockSp();


private:


signals:

public slots:

};

#endif // BLOCK_H
