#ifndef NUMBER_H
#define NUMBER_H

#include <QWidget>
#include <QTimer>

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

protected:
    void paintEvent(QPaintEvent *);
    int num;
    QTimer *numTimer;
    QString numString[10];
};

#endif // NUMBER_H
