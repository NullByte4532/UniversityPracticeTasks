#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdio>
#include "gamelogic.h"
extern Tree tree;
extern int last;
extern int state;
char tmp1[128];
char tmp2[128];
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_yesBtn_clicked()
{
    QLabel* ui_textOut = findChild<QLabel*>("textOutput");
    QLineEdit* ui_textIn = findChild<QLineEdit*>("textInput");
    QPushButton* ui_noBtn = findChild<QPushButton*>("noBtn");
    QPushButton* ui_yesBtn = findChild<QPushButton*>("yesBtn");
    if(state==0){
                if(!last){
                last=tree.step(YES);
                char tmp[128];
                if(last) sprintf(tmp, "Is it %s?", tree.get());
                else sprintf(tmp, "%s", tree.get());
                ui_textOut->setText(tmp);
                }else{
                last=0;

                ui_textOut->setText("YAY! try something else.");
                tree.rewind();
                last=0;
                state=100;
                ui_yesBtn->hide();
                ui_noBtn->setText("Try again");
                }
    }
    else if(state==1){
        QByteArray ba;
        ba = ui_textIn->text().toLatin1();
        strcpy(tmp1, ba.data());
        ui_textIn->clear();
        char tmp[256];
        sprintf(tmp, "What should I ask to tell the difference between %s and %s?", tree.get(), tmp1);
        ui_textOut->setText(tmp);
        state=2;

    }
    else if(state==2){
        QByteArray ba;
        ba = ui_textIn->text().toAscii();
        strcpy(tmp2, ba.data());
        ui_textIn->clear();
        ui_textIn->hide();
        char tmp[256];
        sprintf(tmp, "And what is the answer for %s?", tree.get());
        ui_textOut->setText(tmp);
        state=3;
        ui_noBtn->show();
        ui_yesBtn->setText("Yes");
    }else if(state==3){
        tree.add(tmp2, YES, tmp1);
        FILE* file;
        file=fopen("tree.txt", "w");
        tree.save(file);
        fclose(file);
        tree.rewind();
        last=0;
        state=100;
        ui_textOut->setText("I'll try to remember.");
        ui_yesBtn->hide();
        ui_noBtn->setText("Try again");
    }

}

void MainWindow::on_noBtn_clicked()
{
    QLabel* ui_textOut = findChild<QLabel*>("textOutput");
    QLineEdit* ui_textIn = findChild<QLineEdit*>("textInput");
    QPushButton* ui_noBtn = findChild<QPushButton*>("noBtn");
    QPushButton* ui_yesBtn = findChild<QPushButton*>("yesBtn");
    if(state==0){
                if(!last){
                last=tree.step(NO);
                char tmp[128];
                if(last) sprintf(tmp, "Is it %s?", tree.get());
                else sprintf(tmp, "%s", tree.get());
                ui_textOut->setText(tmp);
                }else{

                    ui_textOut->setText("Okay, you got me. What is it?");
                    state=1;

                    ui_noBtn->hide();
                    ui_yesBtn->setText("Ok");
                    ui_textIn->show();
                }
    }else if(state==3){
        tree.add(tmp2, NO, tmp1);
        FILE* file;
        file=fopen("tree.txt", "w");
        tree.save(file);
        fclose(file);
        tree.rewind();
        last=0;
        state=100;
        ui_textOut->setText("I'll try to remember.");
        ui_yesBtn->hide();
        ui_noBtn->setText("Try again");
    }else if (state==100){
        ui_textOut->setText(tree.get());
        ui_textIn->hide();
        state=0;
        ui_yesBtn->show();
        ui_noBtn->show();
        ui_noBtn->setText("No");
        ui_yesBtn->setText("Yes");

    }
}
