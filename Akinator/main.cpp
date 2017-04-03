#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <cstdio>
#include "gamelogic.h"
Tree tree;
int last;
int state;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    FILE* f;
    last=0;
    state=0;
    f=fopen("tree.txt", "r");
    tree.load(f);
    fclose(f);
    w.show();
    QLineEdit* ui_textIn = w.findChild<QLineEdit*>("textInput");
    QLabel* ui_textOut = w.findChild<QLabel*>("textOutput");
    ui_textOut->setText(tree.get());
    ui_textIn->hide();
    return a.exec();
}
