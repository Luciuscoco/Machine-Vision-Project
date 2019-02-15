#pragma execution_character_set("utf-8")
#include "color_transorm.h"
#include <qdebug.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include <iostream>
#include <qobject.h>
#include <string.h>

using namespace std;

extern ZvImage out_image;          //����ͼ�����
ZvImage convert_color;      //ת�����ͼ��

bool flag1 = false;     //ִ�б��
bool flag2 = false;     //ȷ�ϱ�� 

QString str = NULL;     //ת���ı��
QString input_string = "File";   //��qcomobox ���ó�ʼ��Ĭ��ֵ
QString choice_string = "Processed image";
QString out_string = NULL;

Color_Transorm::Color_Transorm(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

Color_Transorm::~Color_Transorm()
{
    
}


/*---------------------------------------�ۺ�����ʵ��-------------------------------------------*/
//����comboBox �ؼ��ĺ���
//ѡ��ԭʼͼ���������ļ���������ͷ
void Color_Transorm::On_comboBox_activated() {
    input_string = ui.comboBox->itemText(ui.comboBox->currentIndex());
}
//������ͼ��
void Color_Transorm::On_comboBox_4_activated() {
    choice_string = ui.comboBox_4->itemText(ui.comboBox_4->currentIndex());
}
//��ʾ����ѡ��
void Color_Transorm::On_comboBox_5_activated() {
    out_string = ui.comboBox_5->itemText(ui.comboBox_5->currentIndex());
}

//ִ�а�ť����
void Color_Transorm::on_run_clicked() {
    flag1 = true;
}
//ȡ������ʵ��
void Color_Transorm::on_cancel_clicked() {
    flag1 = false;
    flag2 = false;
}
//ȷ������ʵ��
void Color_Transorm::on_sure_clicked() {
    flag2 = true;
    if ((flag1 && flag2) && (out_string != NULL)) color_transorm();
}

//radioButton ����ʵ��
void Color_Transorm::on_radioButton_clicked() {   //RGB->GRAY
    str = "RGB->GRAY";
}
void Color_Transorm::on_radioButton2_clicked() {  //RGB->HSV
    str = "RGB->HSV";
}
void Color_Transorm::on_radioButton3_clicked() {  //RGB->HLS
    str = "RGB->HLS";
}
void Color_Transorm::on_radioButton4_clicked() {  //RGB->YCrCb
    str = "RGB->YCrCb";
}

extern QImage Mat4QImage(ZvImage img);

//��ʾ����
void Color_Transorm::show_print() {
    if (out_string == "Original image") {   //��ԭͼ src_iamge
        QImage image1 = Mat4QImage(out_image);
        ui.label_2->clear();
        ui.label_2->setPixmap(QPixmap::fromImage(image1));
        ui.label_2->setScaledContents(true);//ʹͼ��ߴ���label��Сƥ��
        ui.label_2->show();
    }
    if (out_string == "Processed image") {
        QImage image1 = Mat4QImage(convert_color);
        ui.label_2->clear();
        ui.label_2->setPixmap(QPixmap::fromImage(image1));
        ui.label_2->setScaledContents(true);    //ʹͼ��ߴ���label��Сƥ��
        ui.label_2->show();
    }
}

//��ɫת������
void Color_Transorm::color_transorm() {
    if (str == "RGB->GRAY") { /*cvtColor(out_image, convert_color, COLOR_RGB2GRAY)*/; show_print();}
    else if (str == "RGB->HSV") { /*cvtColor(out_image, convert_color, COLOR_RGB2HSV)*/; show_print();}
    else if (str == "RGB->HLS") { /*cvtColor(out_image, convert_color, COLOR_RGB2HLS)*/; show_print(); }
    else if (str == "RGB->YCrCb") { /*cvtColor(out_image, convert_color, COLOR_RGB2YCrCb)*/; show_print();}
    else { QMessageBox::information(this, tr("��Ϣ���ѿ�"), tr("��ѡ��Ҫִ�е���ɫת��������")); }   //�򿪱�����Ϣ���� 
}