#include "mainwindow.h"
#include "color_transorm.h"
#include "filter_function.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <iostream>
#include <stdio.h>
#include <qdebug.h>

int main03(int argc, char* argv[]){
    QApplication a(argc, argv);
    MainWindow w1;        //������������Ķ��󣬲���ʾ
    w1.show();
    return a.exec();
}

