#include "Test.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Test w;                    //����Testz��Ĺ��캯��
    w.showMaximized();
    return a.exec();
}
