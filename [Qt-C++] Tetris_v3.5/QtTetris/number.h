#ifndef NUMBER_H
#define NUMBER_H

#include <QWidget>
#include <QTimer>
#define WIDTH 30
#define NUM_X 10
#define NUM_Y 20
#define X_SPACE (NUM_X+3+1)
#define Y_SPACE (NUM_Y+4+1)
#define PADDING 5
#define SCREEN_RIGHT (NUM_X+PADDING)*WIDTH
#define NUM_WIDTH 45
#define NUM_HEIGHT 63


class number : public QWidget
{
    Q_OBJECT
public:
    explicit number(QWidget *parent = 0);
    ~number();
    void setnum(int n){
        num=n;
    }

    int getnum(){
        return num;
    }

    int getHighScore(){
        return highscore;
    }
    int setHighScore(int n){
        highscore=n;
    }
int level;

protected:
    void paintEvent(QPaintEvent *);
    int highscore;

    int num;
    QTimer *numTimer;
    QString numString[10];

};

#endif // NUMBER_H
