#pragma execution_character_set("utf-8")
// ������ͷ�ļ�
#include "color_transorm.h"
#include "filter_function.h"
#include "mainwindow.h"
#include "camera_setparm.h"

ZvImage out_image;                //��ֵ���Ӵ��ڵĿؼ�

int num = 0;

QString Data_anynas_type = NULL;      //ͼ�����ݵĽ�����ʽ
static int c_countOfImagesToGrab = 200;     //��������ץȡ 1000 ֡ͼ��

/* camId ��ʾҪ�����ĸ���� {0, 1, 2, 3};
scanId ��ȡֵΪ -1��δ��, 1����ʼ��
����ÿһ��camId ����һ��״̬��scanId;
*/
static int camId = 0;
static int scanId = 0;
static bool Flag_parmSet = false;

Camera_SetParm *C_S;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //����Ƕ��dock
    setDockNestingEnabled(true);
    //��¼���е�dockָ��
    m_docks.append(ui.dockWidget_3);
    m_docks.append(ui.dockWidget_2);

    // �Դ���Ĳ��ֵ�����

    addDockWidget(Qt::RightDockWidgetArea, ui.dockWidget_3);
    ui.dockWidget_3->setMinimumWidth(255);    // ���ҵ���Ҫ���� ���Ŀ����������Ҷ��룩 
    
    // �ڴ�������� ���οؼ�
    QTreeWidget *treeWidget = new QTreeWidget;  //�������οؼ�  
    QStringList headers;        //��ͷ  
    headers << "Property" << "Value";
    treeWidget->setHeaderLabels(headers);

    ui.dockWidget_3->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);  //ê�Ӳ��� ����ͣ������������  
    ui.dockWidget_3->setWidget(treeWidget);  //����ê�Ӳ��������� TreeWidget  


    addDockWidget(Qt::BottomDockWidgetArea, ui.dockWidget_2);
    ui.dockWidget_2->setMinimumHeight(145);      // ���µ���Ҫ���� ���ĸߣ��������ϡ��¶��룩

    // �ڴ�������� ���οؼ�
    QTreeWidget *treeWidget1 = new QTreeWidget;  //�������οؼ�  
    QStringList headers1;        //��ͷ  
    headers1 << "Level" << "Time" << "Source" << "Message";
    treeWidget1->setHeaderLabels(headers1);

    //��ȡʱ��
    QDateTime time = QDateTime::currentDateTime();             //��ȡϵͳ���ڵ�ʱ��
    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");   //������ʾ��ʽ

    QStringList rootTextList;       //���ĸ��ڵ�, �����ȡֵ���ǿ����Լ��Ժ��趨�� 
    rootTextList << "!Information" << str << "Basler acA2500-14gm(22533411)" << "Start grab one picture";    //���ѡ�� Root --- 0  
    QTreeWidgetItem *root = new QTreeWidgetItem(treeWidget1, rootTextList);

    ui.dockWidget_2->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);  //ê�Ӳ��� ����ͣ������������  
    ui.dockWidget_2->setWidget(treeWidget1);  //����ê�Ӳ��������� TreeWidget 

    this->setWindowTitle("Image process Porject");    //���������ñ���
    this->setWindowIcon(QIcon("./Qt ico/Windows Media Player.ico"));    //���������ͼ��

}

MainWindow::~MainWindow()
{
}


/*=======================================�������岼��====================================*/
void MainWindow::removeAllDock()
{
    for (int i = 0; i < 9; ++i)
    {
        removeDockWidget(m_docks[i]);
    }
}
/// \brief ��ʾָ����ŵ�dock
/// \param index ָ����ţ������ָ���������ʾ����
void MainWindow::showDock(const QList<int> &index)
{
    if (index.isEmpty())
    {
        for (int i = 0; i < 9; ++i)
        {
            m_docks[i]->show();
        }
    }
    else
    {
        foreach(int i, index) {
            m_docks[i]->show();
        }
    }
}

/*===========================================Help Menu===========================================*/
void MainWindow::ZV_Version_triggered() {
    QMessageBox::information(this, tr("ZVision �汾"), tr(ZV_VERSION));   //�򿪱�����Ϣ��
}

/*===========================================Set Menu===========================================*/
void MainWindow::ZV_CameraTyep_Gige_triggered() {
    Data_anynas_type = "GIGE";
}

void MainWindow::ZV_CameraTyep_Opencv_triggered() {
    Data_anynas_type = "OPENCV";

}

void MainWindow::ZV_CameraTyep_V4L2_triggered() {
    Data_anynas_type = "V4L2";
}

void MainWindow::ZV_CameraTyep_Aravis_triggered() {
    Data_anynas_type = "Aravis";
}

void MainWindow::ZV_CameraParmSet_triggered() {
    // �½�pylon ImageFormatConverter����.
    C_S = new Camera_SetParm;
    C_S->show();    
    Flag_parmSet = true;
}

/*===========================================Define Tools===========================================*/
void MainWindow::on_action_Cammera() {
    if(Data_anynas_type == NULL)
        QMessageBox::critical(NULL, "critical", "�������ú�ͼ�����ݵĽ�����ʽ������", QMessageBox::Ok);
    else {
        if (!Flag_parmSet) {
            if (QMessageBox::information(this, tr("ZVision"), tr(" ����Ĭ�ϲ�����Camera ? "), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == 65536)  //�򿪱�����Ϣ��
                ZV_CameraParmSet_triggered();       // ���� opencv�� �����ʼ���������� ����
            QMessageBox::information(this, tr("����"), tr("Please reopen Camera!"));   //�򿪱�����Ϣ��
        }
        else {
            /* ����Opencv �ķ�ʽ
            ��������г�ʼ���Ĳ�������
            */
            //C_S->camsetparam;   //������QT �������õĲ���
            int64_t time_out;

            ZvImage img = zv_imgCreate();
            zv_modInit(true);
            //��ʼɨ�����, ����ϵͳĬ�������� ���� ���
            if (!zv_camScan("TIS")) 
            {    // type ��ȡֵ������  "All","binding","GIGE", "USB" , ���Ե������õ���������=1��
                int n = zv_camCount();
                if (!zv_camOpen(camId, scanId))
                {
                        //��� ��ʼ�� ��������
                        //zvSetCameraExposureTime(camId, C_S->camsetparam->ExpTime);

                        if (!zv_camSetROI(camId, 200, 100, 640, 480)) {

                            // �õ� ����Ĳ�����Ϣ���ع�ʱ��
                            int offsetX_out, offsetY_out, width_out, height_out;
                            if (1)//!zv_camGetROI(camId, &offsetX_out, &offsetY_out, &width_out, &height_out)) 
                            {
                                double dtime = 0;
                                zv_camGetExposure(camId, &dtime);     // �õ��ع��ʱ����Ϣ   error 
                                time_out = dtime;
                                ///////////////��ӡ��������Ϣ/////////////////
                                cout << "Now Use ID : " << C_S->camsetparam->m_strUserId.toStdString() << endl;
                                cout << "Camera name : " << C_S->camsetparam->m_strCamName.toStdString() << endl;
                                cout << "Banding camId : " << camId << endl;
                                cout << "Image format : " << C_S->camsetparam->m_strPixelFormat.toStdString() << endl;
                                cout << "Image depth : " << C_S->camsetparam->m_Depth << endl;
                                //cout << "offsetX : " << offsetX_out << endl;
                                //cout << "offsetY : " << offsetY_out << endl;
                                //cout << "width : " << width_out << endl;
                                //cout << "height : " << height_out << endl;
                                cout << "ExposureTime : " << time_out << endl;
                            }
                        }
                    }
                    else
                        QMessageBox::critical(NULL, "critical", "���û�б���ȷ�Ĵ򿪣�", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }

            //ֱ�Ӵ���� ��ȡÿһ֡����
            cout << "start to grab each image !" << endl;
            int cnt = 0;
            while (c_countOfImagesToGrab--) {
                zv_camGrab(img, camId);
                cout << "use grab" << endl;

                cout << "Read " << 200 - c_countOfImagesToGrab << "frame picture!" << endl;
                
                zv_fileWriteImage(img, QString("E:/aa%1.jpg").arg(cnt).toLocal8Bit().data(), NULL, 0);
                cnt++;

                if (c_countOfImagesToGrab == 0)
                {
                    if (!zv_modExit())
                        cout << " Release resources is succeed!" << endl;
                    break;
                }
            }
        }
    }
}    
