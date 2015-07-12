#ifndef POOL_H
#define POOL_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#define WIDTH 30
#define NUM_X 10
#define NUM_Y 20
#define X_SPACE (NUM_X+3+1)
#define Y_SPACE (NUM_Y+4+1)

/* store the dropped and stopped blocks in pool */
class POOL : public QWidget
{
    Q_OBJECT

public:
    explicit POOL(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    ~POOL();
    int map[X_SPACE][Y_SPACE];
    int getPoolSp(int, int);
    void clean();
    int eliminate();

private:
    QString src[8];
    void reduce(int y = 0);

signals:


public slots:
};

#endif // POOL_H
