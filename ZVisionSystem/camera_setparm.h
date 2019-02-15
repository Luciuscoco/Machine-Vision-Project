#ifndef CAMERA_SETPARM_H
#define CAMERA_SETPARM_H

//����QT API.
#include <qdebug.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include "qprocess.h" 
#include <qdir.h>
#include <qtextcodec.h>
#include <qdatetime.h>

#include <QWidget>
#include "ui_camera_setparm.h"

#include "zv.h"
using namespace std;

class CameraSetParam {
public:
    CameraSetParam() {};
    ~CameraSetParam() {};

    int offsetX;
    int offsetY;
    int m_Depth;
    int Width;
    int Height;
    int ExpTime;    //�ع�ʱ��
    int Heartbeat;  //����ʱ��

    QString m_strPixelFormat;    //���صĸ�ʽ
    QString m_strUserId;
    QString m_strCamName;        //���������
};

class CCameraParam    //param ����
{
public:
    CCameraParam();
    virtual ~CCameraParam() {};

    int m_X;
    int m_Y;
    int m_Width;
    int m_Height;
    int m_Depth;
    int m_Channel;
    int m_ExpTime;    //�ع�ʱ��
    int m_Heartbeat;  //����ʱ��

    QString m_strPixelFormat;    //���صĸ�ʽ
    QString m_strUserId;
    QString m_strCamName;        //���������

    bool m_isTrigger;
    bool m_isAttach;
};

class Camera_SetParm : public QWidget
{
    Q_OBJECT

public:
    Camera_SetParm(QWidget *parent = 0);
    ~Camera_SetParm();

    int GrabOne(int index, ZvImage img);
    //void GrabStopAll();
    //bool GrabStop(int index);
    //bool GrabContinue(int index);
    void SetExposureTime(int index, int exposureTime);
    int GetExposureTime(int index);
    //void GetExposureTimeMinMax(int index, int &min, int &max);
    int GetCameraCount();
    int GetCameraActCount();
    bool GetImageSize(int index, int &width, int &height);
    bool GetImageInfo(int index, int *width, int *height, int *channel, int *depth);
    int GetImageLength(int index);
    //QString GetCameraName(int index);
    //int GetDepth(QString pixFormat);
    //int GetDepth(int index);

    //���Ӳ������
    //void SetTrigger(int index, bool isTrigger);
    
    CameraSetParam *camsetparam;

private:
    Ui::Camera_SetParm ui;
    CCameraParam * m_pCamParams;
    int m_CamActNum;    //���ص��������
    int m_CamParamsNum;
    int m_ParamsBufSize;   //�������Ĵ�С
    QString m_strConfigFile;
    int m_isInit;

    bool IsCameraReady(int index);    //�ж�camera �Ƿ�׼������
    bool GetParaFromCamera(int index);
    void SetParaToCamera(int index);
    void AllocDataBuffer();
    bool InitSystem(int cameraCnt);
    void Destroy();
    bool WriteIni();
    bool ReadIniFile();
    void InitCamerasAndGetPara(void);
    int64_t Adjust(int64_t val, int64_t minimum, int64_t maximum, int64_t inc);
    
private slots :
    void on_load_clicked();
    void on_update_clicked();
};


#endif // CAMERA_SETPARM_H
