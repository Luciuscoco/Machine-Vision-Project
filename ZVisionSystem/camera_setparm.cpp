#pragma execution_character_set("utf-8")
#include "camera_setparm.h"
//����QT API.
#include <qdebug.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include "qprocess.h" 
#include <qdir.h>
#include <qtextcodec.h>
#include <qdatetime.h>
#include <qsettings.h>
#include <qmutex.h>

#define USE_NOW_PYLON     //ʹ�����ڵĹ�����Ŀ��������������ĳ�ʼ������

#define DEFAULT_DEPTH 8

//�����ռ�.  

using namespace std;

//ȫ�ֱ���
QMutex mutex;
bool flag_parea_set = true;
QString iniFilePath = "MyINI.ini";      //д����������õ������ļ�
QSettings settings(iniFilePath, QSettings::IniFormat);

/*====================��ʼ������Ĳ���====================*/
//��ʼ������Ĳ���
CCameraParam::CCameraParam()
{
    m_X = 0;
    m_Y = 0;
    m_Width = 64;
    m_Height = 64;
    m_Depth = DEFAULT_DEPTH;
    m_Channel = 1;
    m_ExpTime = 1000;
    m_Heartbeat = 3000;
    m_strPixelFormat = "Mono 8";
    m_strCamName = "δ����";
    m_strUserId = "-1";

    m_isTrigger = false;
    m_isAttach = false;
}

Camera_SetParm::Camera_SetParm(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_CamParamsNum = 1;
    m_CamActNum = 1;
    m_pCamParams = NULL;
    m_ParamsBufSize = 0;
    m_isInit = false;
}

//���������ﵽ�ͷ���Դ������
Camera_SetParm::~Camera_SetParm(){
    Destroy();
}

//������� ׼�����˾ͻ᷵��һ�� true
bool Camera_SetParm::IsCameraReady(int index)
{
    if (index < 0 || index >= m_CamParamsNum)   // index ��Ч�����
    {
        return false;
    }

    if (!m_pCamParams[index].m_isAttach)    // ��������������Ҳ����Ч�����
    {
        return false;
    }

    return true;
}

//�����ݷ��仺����
void Camera_SetParm::AllocDataBuffer()
{
    if (m_CamParamsNum > m_ParamsBufSize && 0 != m_ParamsBufSize)  //���������ɼ���ͼ���� ������ ��������Ҫ��������
    {
        Destroy();
    }

    if (NULL == m_pCamParams)    // ��ʼ���� m_pCamParams Ϊ NULL
    {
        m_pCamParams = new CCameraParam[m_CamParamsNum];   //���� ����ָ��
        m_ParamsBufSize = m_CamParamsNum;
    }
    else
    {
        for (int i = 0; i<m_ParamsBufSize; i++)
        {
            m_pCamParams[i] = CCameraParam();   //�Ի�������ÿ�����Դ�ͼ��Ŀ� ���г�ʼ������
        }
    }
}

//��ʼ�� ϵͳ����
bool Camera_SetParm::InitSystem(int cameraCnt)
{
    ReadIniFile();   // ����ʼ���豸���ļ�


    InitCamerasAndGetPara();   //�����˴�����ͷ����
    GetParaFromCamera(0);


    return true;
}

//�õ�����ͷ������
int Camera_SetParm::GetCameraCount()
{
    return m_CamParamsNum;
}

int Camera_SetParm::GetCameraActCount()
{
    return m_CamActNum;
}

//���� ֵ
int64_t Camera_SetParm::Adjust(int64_t val, int64_t minimum, int64_t maximum, int64_t inc)
{
    // Check the input parameters.
    if (inc <= 0)
    {
        return minimum;
    }
    if (minimum > maximum)
    {
        return minimum;
    }

    // Check the lower bound.
    if (val < minimum)
    {
        return minimum;
    }

    // Check the upper bound.
    if (val > maximum)
    {
        return maximum;
    }

    // Check the increment.
    if (inc == 1)
    {
        return val;
    }
    else
    {
        return minimum + (((val - minimum) / inc) * inc);
    }
}

void Camera_SetParm::InitCamerasAndGetPara(void)
{

}

// ��Camare �л�ȡ����
bool Camera_SetParm::GetParaFromCamera(int index)
{
    if (!IsCameraReady(index))
    {
        return false;
    }

    return true;
}

//������Camare �ڲ��Ĳ���
void Camera_SetParm::SetParaToCamera(int index)
{
    if (!IsCameraReady(index))
    {
        return;
    }

    return ;
    zv_camSetROI(index, m_pCamParams[index].m_X, m_pCamParams[index].m_Y, m_pCamParams[index].m_Width, m_pCamParams[index].m_Height);
    zv_camSetExposure(index, m_pCamParams[index].m_ExpTime);
    double exposure = 0;
    zv_camGetExposure(index, &exposure);
    m_pCamParams[index].m_ExpTime = (int)exposure;
}


//�����ع�ʱ��͵õ��ع�ʱ��
int Camera_SetParm::GetExposureTime(int index)
{
    if (index < 0 || index >= m_CamParamsNum)
    {
        return -1;
    }

    return m_pCamParams[index].m_ExpTime;
}

void Camera_SetParm::SetExposureTime(int index, int ExposureTime)
{
    if (!IsCameraReady(index))
    {
        return;
    }

    if (ExposureTime == m_pCamParams[index].m_ExpTime)
    {
        return;
    }

    zv_camSetExposure(index, ExposureTime);
    m_pCamParams[index].m_ExpTime = ExposureTime;
    WriteIni();
}


//�õ�ͼ���ʵ�ʳߴ�
bool Camera_SetParm::GetImageSize(int index, int &width, int &height)
{
    width = height = 0;
    if (index < 0 || index >= m_CamParamsNum)
    {
        return false;
    }
    width = m_pCamParams[index].m_Width;
    height = m_pCamParams[index].m_Height;
    return true;
}

bool Camera_SetParm::GetImageInfo(int index, int *width, int *height, int *channel, int *depth)
{
    int w = 0, h = 0, c = 0, d = 0;
    if (index >= 0 && index < m_CamParamsNum)  //index ��Ч�������
    {
        w = m_pCamParams[index].m_Width;
        h = m_pCamParams[index].m_Height;
        c = m_pCamParams[index].m_Channel;
        d = m_pCamParams[index].m_Depth;
    }
    //��ʼ�� w �� h ��c ��d ��ֵΪ 0
    if (width) *width = w;
    if (height) *height = h;
    if (channel) *channel = c;
    if (depth) *depth = d;
    return (0 != w);
}

//�õ�ͼ��ĳ���
int Camera_SetParm::GetImageLength(int index)
{
    if (index < 0 || index >= m_CamParamsNum)   // �ж�index �Ƿ��ǶԵ�ֵ
    {
        return 0;
    }

    CCameraParam *p = m_pCamParams + index;   // ָ��Camare ���ָ�� + ƫ�Ƶ�ֵ
    return p->m_Width*p->m_Height*p->m_Channel*p->m_Depth / (sizeof(char) * 8);    //�õ�ͼ��ĳ��� = I_h * I_w * I_D * I_C / (sizeof(char)*8)
}

//����������ͷŴ���
void Camera_SetParm::Destroy()
{
    // m_Cameras ��camera ��ʱ�Ķ���
    zv_modExit();

    if (m_pCamParams != NULL)   //���������ı�������ʼ����ֵ����Ϊ NULL
    {
        delete[] m_pCamParams;       //  m_pCamParams ��ָ�� CamParams ���ָ����� 
        m_pCamParams = NULL;
        m_ParamsBufSize = 0;    //�ѻ���ȥ�Ĵ�С���㻯
    }
}


//ֹͣץȡͼ��
//void Camera_SetParm::GrabStopAll()
//{
//    m_Cameras.StopGrabbing();   // Camare ֹͣץȡ����
//}

//bool Camera_SetParm::GrabStop(int index){
//    if (!IsCameraReady(index))  //���жϡ�camare �Ƿ�׼������
//    {
//        return false;
//    }
//    m_Cameras[index].StopGrabbing();
//    return false;
//}

// ����ץȡ
//bool Camera_SetParm::GrabContinue(int index){
//    if (!IsCameraReady(index)){
//        return false;
//    }
//
//    if (!m_Cameras[index].IsGrabbing())   // �ж���û�п�ʼץȡ��ͼ�񣬼����Ƿ�����������ͼ���ץȡ
//    {
//        m_Cameras[index].StartGrabbing(GrabStrategy_OneByOne);   // ���������one-by-on ��ģʽ����ץȡͼ��
//    }
//    return m_Cameras[index].IsGrabbing();   // ����ץȡ
//}

//Camera ץȡһ֡ͼ��
int Camera_SetParm::GrabOne(int index, ZvImage img)   //index ��һ�����
{
    zv_camGrab(img, index);
    return 0;
}

//void Camera_SetParm::SetTrigger(int index, bool isTrigger)
//{
//    if (!IsCameraReady(index))
//    {
//        return;
//    }
//
//    GenApi::INodeMap& nodemap1 = m_Cameras[index].GetNodeMap();
//
//    m_pCamParams[index].m_isTrigger = isTrigger;
//    GenApi::CEnumerationPtr PtrTriggerSelector(nodemap1.GetNode("TriggerSelector"));
//    PtrTriggerSelector->FromString("FrameStart");
//    GenApi::CEnumerationPtr ptrTriggerMode(nodemap1.GetNode("TriggerMode"));
//    if (isTrigger)
//    {
//        ptrTriggerMode->FromString("On");//*/
//    }
//    else
//    {
//        ptrTriggerMode->FromString("Off");//*/
//    }
//}

//���õ�camare ������
//QString Camera_SetParm::GetCameraName(int index)
//{
//    if (index < 0 || index >= m_CamParamsNum)
//    {
//        return "";
//    }
//    return m_pCamParams[index].m_strCamName;
//}

// ����ʼ���豸���ļ�
bool Camera_SetParm::ReadIniFile() {

    if (m_CamParamsNum <= 0)   //û�а������������϶��д���
    {
        return false;
    }

#ifdef USE_AGO_PYLON
    AllocDataBuffer();        // �������ݻ����� 
    mutex.lock();       //must lockbuffer, otherwise the buffer will be shared with other QString �����������
    for (int i = 0; i < m_CamParamsNum; i++)    // m_CamParamsNum Ϊ��������ʵ��ͼ������
    {
        // ����ͼ���һЩ����,����ļ�.ini ����ֵ��ֱ�Ӵ��л�ö�Ӧ��������Ӧ��ֵ������=Ĭ��ֵ
        if (ui.lineEdit->text() != NULL) {     //֮ǰ������ֵ�����
            QString PixelFormat = settings.value("Basler Camera/PixelFormat").toString();
            m_pCamParams[i].m_strPixelFormat = PixelFormat;
            m_pCamParams[i].m_Depth = DEFAULT_DEPTH;

            m_pCamParams[i].m_X = settings.value("Basler Camera/OffsetX").toInt();
            m_pCamParams[i].m_Y = settings.value("Basler Camera/OffsetY").toInt();
            m_pCamParams[i].m_Width = settings.value("Basler Camera/Width").toInt();
            m_pCamParams[i].m_Height = settings.value("Basler Camera/Height").toInt();
            m_pCamParams[i].m_ExpTime = settings.value("Basler Camera/ExposureTime").toInt();
            m_pCamParams[i].m_Heartbeat = settings.value("Basler Camera/Heartbeat").toInt();

            QString UserID = settings.value("Basler Camera/UserID").toString();
            m_pCamParams[i].m_strUserId = UserID;

            QString CameraName = settings.value("Basler Camera/CameraName").toString();
            m_pCamParams[i].m_strCamName = CameraName;
        }
        else {    //һֱû�н��г�ʼ��ʱ��
            QString PixelFormat = "Mono 8";
            m_pCamParams[i].m_strPixelFormat = PixelFormat;
            m_pCamParams[i].m_Depth = DEFAULT_DEPTH;

            m_pCamParams[i].m_X = 0;
            m_pCamParams[i].m_Y = 0;
            m_pCamParams[i].m_Width = 1024;
            m_pCamParams[i].m_Height = 768;
            m_pCamParams[i].m_ExpTime = 35000;
            m_pCamParams[i].m_Heartbeat = 500;

            QString UserID = "00";
            m_pCamParams[i].m_strUserId = UserID;

            QString CameraName = "δ����";
            m_pCamParams[i].m_strCamName = CameraName;
        }
    }
    mutex.unlock();   //��������
    return true;
#elif defined USE_NOW_PYLON
    camsetparam = new CameraSetParam;
    mutex.lock();       //must lockbuffer, otherwise the buffer will be shared with other QString �����������
    for (int i = 0; i < m_CamParamsNum; i++)    // m_CamParamsNum Ϊ��������ʵ��ͼ������
    {
        // ����ͼ���һЩ����,����ļ�.ini ����ֵ��ֱ�Ӵ��л�ö�Ӧ��������Ӧ��ֵ������=Ĭ��ֵ
        if (ui.lineEdit->text() != NULL) {     //֮ǰ������ֵ�����
            QString PixelFormat = settings.value("Basler Camera/PixelFormat").toString();
            camsetparam->m_strPixelFormat = PixelFormat;
            camsetparam->m_Depth = DEFAULT_DEPTH;
            camsetparam->offsetX = settings.value("Basler Camera/OffsetX").toInt();
            camsetparam->offsetY = settings.value("Basler Camera/OffsetY").toInt();
            camsetparam->Width = settings.value("Basler Camera/Width").toInt();
            camsetparam->Height = settings.value("Basler Camera/Height").toInt();
            camsetparam->ExpTime = settings.value("Basler Camera/ExposureTime").toInt();
            camsetparam->Heartbeat = settings.value("Basler Camera/Heartbeat").toInt();

            QString UserID = settings.value("Basler Camera/UserID").toString();
            camsetparam->m_strUserId = UserID;

            QString CameraName = settings.value("Basler Camera/CameraName").toString();
            camsetparam->m_strCamName = CameraName;
        }
        else {
            QString PixelFormat = "Mono 8";
            camsetparam->m_strPixelFormat = PixelFormat;
            camsetparam->m_Depth = DEFAULT_DEPTH;

            camsetparam->offsetX = 0;
            camsetparam->offsetY = 0;
            camsetparam->Width = 2592;
            camsetparam->Height = 1944;
            camsetparam->ExpTime = 35000;
            camsetparam->Heartbeat = 500;

            QString UserID = "00";
            camsetparam->m_strUserId = UserID;

            QString CameraName = "Basler acA2500-14gm(22533411)";
            camsetparam->m_strCamName = CameraName;
        }
    }
    mutex.unlock();   //��������
    return true;
#endif    
}

//�����ⲿ���õĲ����������ȥ
bool Camera_SetParm::WriteIni(){
    for (int i = 0; i<m_CamParamsNum; i++){
        if (ui.lineEdit->text() != NULL) {
            settings.setValue("Basler Camera/PixelFormat", ui.lineEdit->text());
        
            settings.setValue("Basler Camera/OffsetX", ui.lineEdit_4->text().toInt());
            settings.setValue("Basler Camera/OffsetY", ui.lineEdit_5->text().toInt());        
            settings.setValue("Basler Camera/Width", ui.lineEdit_9->text().toInt());
            settings.setValue("Basler Camera/Height", ui.lineEdit_10->text().toInt());    
            settings.setValue("Basler Camera/ExposureTime", ui.lineEdit_11->text().toInt());
            settings.setValue("Basler Camera/Heartbeat", ui.lineEdit_12->text().toInt());

            settings.setValue("Basler Camera/UserID", ui.lineEdit_13->text());
            settings.setValue("Basler Camera/CameraName", ui.lineEdit_14->text());
        }
    }

    return true;
}

void Camera_SetParm::on_load_clicked() {
    int cameraCnt = 1;         //ΪҪ�����ܵ������Ŀ

    if(flag_parea_set) m_CamParamsNum = ui.comboBox->itemText(ui.comboBox->currentIndex()).toInt();   //������õ�����ͷ����
    //InitSystem(cameraCnt);     //��ʼ��, ���°汾��ʹ�õ�ʱ����Ҫ�������
    ReadIniFile();   // ����ʼ���豸���ļ�
}

void Camera_SetParm::on_update_clicked() {
    m_CamParamsNum = ui.comboBox->itemText(ui.comboBox->currentIndex()).toInt();   //������õ�����ͷ����
    WriteIni();
    flag_parea_set = false;
}