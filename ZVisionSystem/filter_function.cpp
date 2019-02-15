#pragma execution_character_set("utf-8")

#include "filter_function.h"
#include <qdebug.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include <iostream>
#include <qobject.h>
#include <string.h>

#include "zv.h"

using namespace std;

QString Filter_choice_string = NULL;
QString Input_string = NULL;

extern ZvImage out_image;    //���������ж�ȡͼ��

bool flag3 = false;    //ִ�б��
bool flag4 = false;    //ȷ�ϱ��
int number;     //��ȡ������ֵ
QString out_choice_type = NULL;

Filter_Function::Filter_Function(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.MedianBlurSlider->setMinimum(0);
    ui.MedianBlurSlider->setMaximum(20);
    ui.MedianBlurSlider->setValue(2);

    ui.MeanBlurSlider->setMinimum(0);
    ui.MeanBlurSlider->setMaximum(20);
    ui.MeanBlurSlider->setValue(2);

    ui.GaussianBlurSlider->setMinimum(0);
    ui.GaussianBlurSlider->setMaximum(20);
    ui.GaussianBlurSlider->setValue(2);

    ui.BoxFilterlSlider->setMinimum(0);
    ui.BoxFilterlSlider->setMaximum(20);
    ui.BoxFilterlSlider->setValue(2);

    ui.BilateralFilterSlider->setMinimum(0);
    ui.BilateralFilterSlider->setMaximum(20);
    ui.BilateralFilterSlider->setValue(2);

    // ������̬ģʽ
    ui.checkBox->setTristate();
    convert_filter = zv_imgCreate();
}

Filter_Function::~Filter_Function()
{
    zv_imgRelease(&convert_filter);
}

//ͼ���ʽת������Mat����-��QImage���͵ı任
QImage Mat4QImage(ZvImage img)
{
    //8-bitsunsigned,NO.OFCHANNELS=1
    int channel = zv_imgChannels(img);
    uchar* qImageBuffer = new uchar(zv_imgHeight(img)*zv_imgWidth(img)*channel);
    if (channel == 1)
    {
        //cout<<"1"<<endl;
        //Setthecolortable(usedtotranslatecolourindexestoqRgbvalues)
        QVector<QRgb>colorTable;
        for (int i = 0; i<256; i++)
            colorTable.push_back(qRgb(i, i, i));
        //CopyinputMat
        zv_imgGetData(img, qImageBuffer, zv_imgHeight(img)*zv_imgWidth(img)*channel, true);
        //CreateQImagewithsamedimensionsasinputMat
        QImage qimg(qImageBuffer, zv_imgHeight(img), zv_imgWidth(img), QImage::Format_Indexed8);
        qimg.setColorTable(colorTable);
        return qimg;
    }
    //8-bitsunsigned,NO.OFCHANNELS=3
    else if (channel == 3)
    {
        //cout<<"3"<<endl;
        //CopyinputMat
        //CreateQImagewithsamedimensionsasinputMat
        QImage qimg(qImageBuffer, zv_imgHeight(img), zv_imgWidth(img), QImage::Format_RGB888);
        return qimg.rgbSwapped();

    }
    else
    {
        qDebug() << "ERROR:MatcouldnotbeconvertedtoQImage.";
        return QImage();
    }
}

/*-------------------------ʵ�ֲۺ���------------------------------*/
//����ͼ��ѡ�񴥷�����
void Filter_Function::activate_comboBox_triggred() {
    Input_string = ui.comboBox->itemText(ui.comboBox->currentIndex());   //��ô�comobox��ѡ�����������
}
//�˲���ѡ����
void Filter_Function::activate_comboBox_2_triggred() {
    Filter_choice_string = ui.comboBox_2->itemText(ui.comboBox_2->currentIndex());
}
//ִ�к���
void Filter_Function::on_run_clicked() {
    flag3 = true;
}
//ȷ�Ϻ���
void Filter_Function::on_sure_clicked() {
    flag4 = true;
    if ((flag3 && flag4) && (Filter_choice_string != NULL)) Filter_Processing();
}
//ȡ������
void Filter_Function::on_cancel_clicked() {
    flag3 = false;
    flag4 = false;
}

/*-----------------�˲���������----------------------------*/
void Filter_Function::set_medianblur_value(int) {
    int pos = ui.MedianBlurSlider->value();     //��lable ����ʾ����������
    number = pos;
    QString str = QString("%1").arg(pos);
    ui.label_12->setText(str);
}
void Filter_Function::set_meanblur_value(int) {
    int pos = ui.MeanBlurSlider->value();     //��lable ����ʾ����������
    number = pos;
    QString str = QString("%1").arg(pos);
    ui.label_10->setText(str);
}
void Filter_Function::set_gaussianblur_value(int) {
    int pos = ui.GaussianBlurSlider->value();     //��lable ����ʾ����������
    number = pos;
    QString str = QString("%1").arg(pos);
    ui.label_11->setText(str);
}
void Filter_Function::set_boxfilter_value(int) {
    int pos = ui.BoxFilterlSlider->value();     //��lable ����ʾ����������
    number = pos;
    QString str = QString("%1").arg(pos);
    ui.label_9->setText(str);
}
void Filter_Function::set_bilateralfilter(int) {
    int pos = ui.BilateralFilterSlider->value();     //��lable ����ʾ����������
    number = pos;
    QString str = QString("%1").arg(pos);
    ui.label_13->setText(str);
}

//��ʾ���ѡ��
void Filter_Function::on_statechanged(int state) {
    if (state == Qt::Checked) { out_choice_type = "processed image";  } // "ѡ��"
    else if (state == Qt::PartiallyChecked) {out_choice_type = "original image"; print_show1();}  // "��ѡ"
    else { ui.label_14->setText("δѡ��");  ui.label_8->clear();}   //����ʾ���ڽ��в���
}

//ͼ���˲�������
void Filter_Function::Filter_Processing() {
    if (Filter_choice_string == "BoxFilter") {zv_ipMeanBlur(out_image, convert_filter, number + 1); number = 0; print_show1();}
    else if (Filter_choice_string == "MeanBlur") { zv_ipMeanBlur(out_image, convert_filter, number + 1); number = 0; print_show1();}
    else if (Filter_choice_string == "GaussianBlur") { zv_ipGaussBlur(out_image, convert_filter, number * 2 + 1); number = 0; print_show1();}
    else if (Filter_choice_string == "MedianBlur") { zv_ipMedianBlur(out_image, convert_filter, number * 2 + 1); number = 0; print_show1();}
    else if (Filter_choice_string == "BilateralFilter") { zv_ipBilateralFilter(out_image, convert_filter, number / 2, number * 2); number = 0; print_show1(); }
    else  QMessageBox::information(this, tr("��Ϣ���ѿ�"), tr("��ѡ��Ҫִ�е��˲�������")); 
}

//ͼ����ʾ����
void Filter_Function::print_show1() {
    if (out_choice_type == "processed image") {
        QImage image1 = Mat4QImage(convert_filter);
        ui.label_8->setPixmap(QPixmap::fromImage(image1));
        ui.label_8->setScaledContents(true);    //ʹͼ��ߴ���label��Сƥ��
        ui.label_8->show();
        ui.label_14->setText("ȫѡ��ʾ������ͼ");
    }
    if (out_choice_type == "original image") {
        QImage image1 = Mat4QImage(out_image);
        ui.label_8->setPixmap(QPixmap::fromImage(image1));
        ui.label_8->setScaledContents(true);    //ʹͼ��ߴ���label��Сƥ��
        ui.label_8->show();
        ui.label_14->setText("��ѡ��ʾԭͼ");
    }
}