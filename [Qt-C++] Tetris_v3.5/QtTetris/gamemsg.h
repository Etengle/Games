#ifndef GAMEMSG_H
#define GAMEMSG_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#define WIDTH 30
#define NUM_X 10
#define NUM_Y 20
#define MSG_HEIGHT 4

class GAMEMSG : public QWidget
{
    Q_OBJECT
public:
    explicit GAMEMSG(QWidget *parent = 0);
    ~GAMEMSG();
    void paintEvent(QPaintEvent *);
    enum{lose = 0, start = 1, redy = 2, pause = 3};
    int gamemod;
    void setGameMod(int);


signals:

public slots:
};

#endif // GAMEMSG_H
