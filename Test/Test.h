#pragma once

#include <QtWidgets/QMainWindow>
#include "test_function.h"
#include "ui_Test.h"
#include <QThread>
#include <iostream>

using namespace std;

// ��Qt�н����̵߳���ҪĿ�ľ���Ϊ�����߳���������Щ��ʱ�ĺ�̨������
// �Ӷ����������ܼ�ʱ��Ӧ�û������������
class TestThread : public QThread
{
    Q_OBJECT

public:
    TestThread(QObject * param = NULL);
    ~TestThread();
    void run();

    QObject * param;

signals:
    void functionTested(int set, int func, int rst);
    void allTested();
};

class Test : public QMainWindow
{
    Q_OBJECT

public:
    Test(QWidget *parent = Q_NULLPTR);

    QVector<QPoint> testId;
    bool isRun;
    bool isExit;

    int testFunc(int set, int func);
    void updateStat();
    void updateSrcCboItem();
    void updateDstCboItem();
    void showImage(bool isSrc, int id);
    void addLog(QString log);
    void showNgInfo(int);

private slots:
    void updateTestedInfo(int set, int func, int rst);
    void testFinished();

    /***************��ť�źź���***************/
    void on_btnTest_clicked();
    void on_btnPause_clicked();
    void on_cboSrcInfo_currentIndexChanged(int index);
    void on_cboDstInfo_currentIndexChanged(int index);
    void on_listNG_itemDoubleClicked(QListWidgetItem *item);
    void on_treeTest_itemClicked(QTreeWidgetItem *item, int column);
    void on_treeTest_itemSelectionChanged();
    void on_treeTest_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::TestClass ui;
    TestThread *thread;

    int testTotal;
    int tested;
    int testNG;
    QString state;
    QString testLogStr;

    void init();
    void release();
    void clearData(); //�����������
    void stringToHtml(QString &string);
    QString getSrcCboString(int id);
    QString getDstCboString(int _id);
    QString getColorString(QString str, QString color);
    void closeEvent(QCloseEvent *ev);
};
