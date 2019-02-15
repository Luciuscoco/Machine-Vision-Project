#ifndef ZV_TYPES_H
#define ZV_TYPES_H


typedef enum _ZvPixelType
{
    ZV_PIXEL_UNDEF = -1,
    ZV_PIXEL_8U = 0,
    ZV_PIXEL_16U,
    ZV_PIXEL_32S,
    ZV_PIXEL_64F,
}ZvPixelType;

typedef enum _ZvInterpolation
{
    ZV_INTERP_NEAREST = 0,
    ZV_INTERP_LINEAR,
    ZV_INTERP_CUBIC,
    ZV_INTERP_LANCZOS,
    ZV_INTERP_MAX,
}ZvInterpolation;

typedef enum _ZvReadImgType
{
    ZV_READ_RAW = 0,        //ԭ��ʽ
    ZV_READ_TO_GRAY = 1,    //�Ҷ�
    ZV_READ_TO_RGB = 2,    //��ɫ
    //ZV_READ_EXDEPTH = 3,    //֧��16��32λ���
    //ZV_READ_EXCOLOR = 4,    //֧����չ����ɫ�ռ�
    ZV_READ_TYPE_MAX,
}ZvReadImgType;

typedef enum _ZvCamPropertyVal
{
    ZV_CAM_PV_ROI_X = 0,        //ROI
    ZV_CAM_PV_ROI_Y,
    ZV_CAM_PV_ROI_W,
    ZV_CAM_PV_ROI_H,
    ZV_CAM_PV_EXPOSURE,         //�ع�
    ZV_CAM_PV_GAIN,             //����
    ZV_CAM_PV_GAMMA,            //Gamma
    ZV_CAM_PV_BIN_HORI,         //�ϲ�����X
    ZV_CAM_PV_BIN_VERT,         //�ϲ�����Y
    ZV_CAM_PV_FPS,              //֡��
    ZV_CAM_PV_GAIN_R,           //����R
    ZV_CAM_PV_GAIN_G,           //����G
    ZV_CAM_PV_GAIN_B,           //����B
    ZV_CAM_PV_WBALANCE_R,       //��ƽ��R
    ZV_CAM_PV_WBALANCE_G,       //��ƽ��G
    ZV_CAM_PV_WBALANCE_B,       //��ƽ��B

    ZV_CAM_PV_BITS = 80,        //ֻ�� λ��
    ZV_CAM_PV_BUF_SIZE,         //�ڴ��С

    ZV_CAM_PV_EXPOSURE_AUTO = 100, //���� �Զ��ع�
    ZV_CAM_PV_GAIN_AUTO,        //�Զ�����
    ZV_CAM_PV_WBALANCE_AUTO,    //�Զ���ƽ��
    ZV_CAM_PV_GAMMA_ENABLE,     //����Gamma

    ZV_CAM_PV_INPUT0 = 110, //IO
    ZV_CAM_PV_INPUT1,
    ZV_CAM_PV_INPUT2,

    ZV_CAM_PV_OUTPUT0 = 120,
    ZV_CAM_PV_OUTPUT1,
    ZV_CAM_PV_OUTPUT2,
}ZvCamPropertyVal;

typedef enum _ZvCamPropertyCmd
{
    ZV_CAM_PC_SOFT_TRIGGER = 0,
    ZV_CAM_PC_ACQUI_START,
}ZvCamPropertyCmd;

typedef enum _ZvCamPropertyStr
{
    ZV_CAM_PS_SN = 0,       //SN
    ZV_CAM_PS_MODEL,        //�ͺ�
    ZV_CAM_PS_DEV_ID,       //�豸ID
    ZV_CAM_PS_DEV_NAME,     //�豸����
    ZV_CAM_PS_DISPLAY,      //��ʾ����
    ZV_CAM_PS_INTERFACE,    //�ӿ�����
    ZV_CAM_PS_PORT,         //�˿ں�
    ZV_CAM_PS_MAC,          //Mac��ַ
    ZV_CAM_PS_IP,           //IP
    ZV_CAM_PS_HOST_IP,      //����IP
    ZV_CAM_PS_USER_NAME,    //�û��Զ�������
    ZV_CAM_PS_USER_PARAM,   //�û��Զ������

    ZV_CAM_PS_FORMAT = 100, //��ʽ
    ZV_CAM_PS_TRG_MODE,     //����ģʽ
    ZV_CAM_PS_TRG_POLAR,    //��������
    ZV_CAM_PS_TRG_SOURCE,   //����Դ
    ZV_CAM_PS_WBALANCE_T,   //��ƽ������
    ZV_CAM_PS_GAMMA_T,      //Gamma����
}ZvCamPropertyStr;

typedef enum _ZvCamStatus
{
    ZV_CAM_ST_ERROR = -1,    //����
    ZV_CAM_ST_NULL = 0,        //δ��
    ZV_CAM_ST_CONNECT,        //δ��
    ZV_CAM_ST_OPENED,        //�Ѵ�
    ZV_CAM_ST_REMOVED,        //�Ͽ�����
}ZvCamStatus;

typedef enum _ZvColorSpace
{
    ZV_CLR_YUV = 0,
    ZV_CLR_HSV,
    ZV_CLR_LAB,
    ZV_CLR_HLS,
    ZV_CLR_YCRCB,
    ZV_CLR_LUV,
    ZV_CLR_XYZ,
    ZV_CLR_RGBA,
    ZV_CLR_MAX,
}ZvColorSpace;

typedef enum _ZvBayerType
{
    ZV_BAYER_BG = 0,
    ZV_BAYER_GB,
    ZV_BAYER_RG,
    ZV_BAYER_GR,
}ZvBayerType;

typedef enum _ZvGeoFeature
{
    ZV_FEAT_AREA = 0,       //���
    ZV_FEAT_GX,             //����X
    ZV_FEAT_GY,             //����Y
    ZV_FEAT_PERIMETER,      //�ܳ�
    ZV_FEAT_CIRCULARITY,    //Բ��
    ZV_FEAT_COMPACT,        //���¶�
    ZV_FEAT_MEAN,           //ԭͼ�ҶȾ�ֵ
    ZV_FEAT_HOLE_NUMS,      //������
    ZV_FEAT_ISCONVEX,       //͹��
    ZV_FEAT_HULL_AREA,      //͹�����
    ZV_FEAT_CONVEXITY,      //͹��
    ZV_FEAT_RECT_X,         //��Ӿ���x
    ZV_FEAT_RECT_Y,         //��Ӿ���y
    ZV_FEAT_RECT_W,         //��Ӿ��ο��
    ZV_FEAT_RECT_H,         //��Ӿ��θ߶�
    ZV_FEAT_RECT_RX,        //��Ӿ���x + w
    ZV_FEAT_RECT_BY,        //��Ӿ���y + h
    ZV_FEAT_RECT_RATIO,     //��Ӿ���h / w
    ZV_FEAT_EA_MAJOR,       //��������
    ZV_FEAT_EA_MINOR,       //��������
    ZV_FEAT_EA_ANGLE,       //����Ƕ�
    ZV_FEAT_EA_RATIO,       //����/����
    ZV_FEAT_RECT2_CX,       //��С��������X
    ZV_FEAT_RECT2_CY,       //��С��������Y
    ZV_FEAT_RECT2_W,        //��С���ο��
    ZV_FEAT_RECT2_H,        //��С���θ߶�
    ZV_FEAT_RECT2_ANGLE,    //��С���νǶ�
    ZV_FEAT_RECT2_RATIO,    //��С����h/w
    ZV_FEAT_OC_X,           //���ԲX
    ZV_FEAT_OC_Y,           //���ԲY
    ZV_FEAT_OC_R,           //���ԲR

    ZV_FEAT_M_S00 = 36,          //�ռ�� 10��
    ZV_FEAT_M_S10,
    ZV_FEAT_M_S01,
    ZV_FEAT_M_S20,
    ZV_FEAT_M_S11,
    ZV_FEAT_M_S02,
    ZV_FEAT_M_S30,
    ZV_FEAT_M_S21,
    ZV_FEAT_M_S12,
    ZV_FEAT_M_S03,

    ZV_FEAT_M_C20,          //���ľ� 7��
    ZV_FEAT_M_C11,
    ZV_FEAT_M_C02,
    ZV_FEAT_M_C30,
    ZV_FEAT_M_C21,
    ZV_FEAT_M_C12,
    ZV_FEAT_M_C03,
    ZV_FEAT_M_N20,          //��һ�����ľ� 7��
    ZV_FEAT_M_N11,
    ZV_FEAT_M_N02,
    ZV_FEAT_M_N30,
    ZV_FEAT_M_N21,
    ZV_FEAT_M_N12,
    ZV_FEAT_M_N03,

    ZV_FEAT_HU0,            //hu��7��
    ZV_FEAT_HU1,
    ZV_FEAT_HU2,
    ZV_FEAT_HU3,
    ZV_FEAT_HU4,
    ZV_FEAT_HU5,
    ZV_FEAT_HU6,
    ZV_FEAT_MAX,
}ZvGeoFeature;

typedef enum _ZvDefColor
{
    ZV_RED = 0,
    ZV_GREEN,
    ZV_BLUE,
    ZV_WHITE,
    ZV_BLACK,
    ZV_DEF_CLR_MAX,
}ZvDefColor;

typedef enum _ZvThreshPolar //��Ե����
{
    ZV_POLAR_TODARK,    //�׺�
    ZV_POLAR_TOLIGHT,   //�ڰ�
    ZV_POLAR_ALL,       //����
}ZvThreshPolar;

typedef enum _ZvEdgePos //��Եλ��
{
    ZV_EDGE_FIRST,      //��һ��
    ZV_EDGE_LAST,       //����
    ZV_EDGE_MAX,        //��ǿ��
    ZV_EDGE_ALL,        //���е�
}ZvEdgePos;

#endif // ZV_DEFINE_H
