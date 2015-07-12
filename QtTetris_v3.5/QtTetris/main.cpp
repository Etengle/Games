#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.showNormal();

    QMessageBox msgBox;
    msgBox.setText("---------------- How to play ----------------");
    msgBox.setInformativeText("W, A, S, D, E: control the leftside block\n"
                              "↑,←,↓,→,0: control the rightside block\n"
                              "Space:            pause\n"
                              "Esc:                 restart\n");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

    return a.exec();
}
