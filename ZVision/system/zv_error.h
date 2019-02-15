#ifndef ZV_ERROR_H
#define ZV_ERROR_H


typedef enum _ZvStatus
{
    ZV_ST_OK = 0x0,
//    1    ���������Զ�ת��
//    2    ������ֹ�ȴ��ź�
    ZV_ST_MATCH_FAIL = 0x400,

    ZV_WARN_RELE_NULL = 0x800,          //�������ͣ��ͷſ�ָ��
    ZV_WARN_RELE_NONE = 0x801,          //�ͷſ�����
    ZV_WARN_RELE_UNINIT = 0x802,        //�ͷ�δ��ʼ���Ľṹ
    ZV_WARN_FILE_NOT_EXIST = 0x900,     //�ļ�������
    ZV_WARN_NODE_NOT_EXIST = 0x901,     //�ڵ㲻����

    ZV_ERR_START = ZV_SYS_ERR_START,     //0x1000 ������뿪ʼ
    ZV_ERR_GENERAL_START = 0x1001,      //һ����� 0x1001-1DFF
    ZV_ERR_G_UNDEF = ZV_ERR_GENERAL_START,  //δ֪�Ĵ���
    ZV_ERR_G_CPP_EXCEP = 0x1002,        //C++�쳣
    ZV_ERR_G_ASSERT = 0x1003,           //���Դ���
    ZV_ERR_G_NULL_PTR = 0x1004,
    ZV_ERR_G_STD_EXCEP = 0x1005,        //std���쳣
    ZV_ERR_G_LIB_EXCEP = 0x1006,        //δ֪�ĵ��������쳣 �����������������
    ZV_ERR_G_LIB_RUN = 0x1007,          //δ֪�ĵ����������״̬
    ZV_ERR_G_DIV_ZERO = 0x1008,         //�������
    ZV_ERR_G_VEC_SIZE = 0x1009,         //vector���ȴ���
    ZV_ERR_G_BUF_SMALL = 0x100C,        //���TABLE���Ȳ���
    ZV_ERR_G_OBJ_NOT_EXIST = 0x100D,    //Ŀ�겻����
    ZV_ERR_G_LIB_ASSERT,
    
    ZV_ERR_G_UNFINISHED = 0x1D30,
    ZV_ERR_G_NOT_SUPPORT = 0x1D40,      //��֧�ֵĹ���
    ZV_ERR_G_NSP_CL = 0x1D50,           //��֧�ֵ�OPENCLӦ��
    ZV_ERR_G_NSP_GL = 0x1D51,           //��֧�ֵ�OPENGLӦ��
    ZV_ERR_G_NSP_CUDA = 0x1D52,         //��֧�ֵ�CUDAӦ��

    ZV_ERR_TASK_START = 0x1E00,         //����
    ZV_ERR_T_PARAM_SIZE = 0x1E00,       //�����������������
    ZV_ERR_T_THREAD_ID = 0x1E01,        //�����߳�ID������Χ

    ZV_ERR_MEN_START = 0x2000,              //0x2000-0x20FF
    ZV_ERR_MEM_ALLOC = ZV_ERR_MEN_START,    //�ڴ����ʧ��    �ڴ�MEM
    ZV_ERR_MEM_CORRUPTED = 0x2001,          //������ڴ汻�ƻ�
    
    ZV_ERR_FILE_START = 0x2100,
    ZV_ERR_F_NOT_EXIST = ZV_ERR_FILE_START, //�ļ�������
    ZV_ERR_F_OPEN_FAIL = 0x2101,            //�ļ��򿪳���
    ZV_ERR_F_SAVE_FAIL = 0x2102,
    ZV_ERR_F_FORMAT = 0x2121,               //������ļ���ʽ

    ZV_ERR_CAM_START = 0x2200,
    ZV_ERR_CAM_GRAB = ZV_ERR_CAM_START,
    ZV_ERR_CAM_GRAB_TIMEOUT = 0x2201,
    ZV_ERR_CAM_SCANID = 0x2202,         //�豸ö��ID������Χ
    ZV_ERR_CAM_CAMID = 0x2203,          //�豸ID������Χ
    ZV_ERR_CAM_UNBOUND = 0x2204,        //δ���豸
    ZV_ERR_CAM_OPEN = 0x2205,           //�豸�򿪳���
    ZV_ERR_CAM_SCAN,
    ZV_ERR_CAM_NONE = 0x2209,           //δ�����豸
    ZV_ERR_CAM_UNKNOWN = 0x220F,        //�豸״̬δ֪
    ZV_ERR_CAM_PARAM_UNDEF = 0x2210,    //�������ֲ�֧��
    ZV_ERR_CAM_PARAM_RD = 0x2211,       //��ȡ��������
    ZV_ERR_CAM_PARAM_WR = 0x2212,       //д���������
    ZV_ERR_CAM_INIT_TIS = 0x22E0,       //��ʼ��TIS����ʧ��

    ZV_ERR_FUN_ARG_START = 0x2600,      //������������
    ZV_ERR_FA_TYPE = 0x2640,            //�������ʹ���
    ZV_ERR_FA_TYPE_1 = 0x2641,
    ZV_ERR_FA_TYPE_2 = 0x2642,
    ZV_ERR_FA_TYPE_3 = 0x2643,
    ZV_ERR_FA_RANGE = 0x2660,           //����������Χ����־�����֧��
    ZV_ERR_FA_RANGE_1 = 0x2661,
    ZV_ERR_FA_RANGE_2 = 0x2662,
    ZV_ERR_FA_RANGE_3 = 0x2663,
    ZV_ERR_FA_RANGE_4 = 0x2664,
    ZV_ERR_FA_RANGE_5 = 0x2665,
    ZV_ERR_FA_RANGE_6 = 0x2666,
    ZV_ERR_FA_RANGE_7 = 0x2667,
    ZV_ERR_FA_RANGE_8 = 0x2668,
    ZV_ERR_FA_RANGE_9 = 0x2669,
    ZV_ERR_FA_RANGE_10 = 0x266A,
    ZV_ERR_FA_NULL = 0x2680,            //����Ϊ��
    ZV_ERR_FA_NULL_1 = 0x2681,          //����1Ϊ��
    ZV_ERR_FA_NULL_2 = 0x2682,
    ZV_ERR_FA_NULL_3 = 0x2683,
    ZV_ERR_FA_NULL_4 = 0x2684,
    ZV_ERR_FA_NULL_5 = 0x2685,
    ZV_ERR_FA_NULL_6 = 0x2686,
    ZV_ERR_FA_NULL_7 = 0x2687,
    ZV_ERR_FA_NULL_8 = 0x2688,

    ZV_ERR_M_START = 0x3000,            //M Mat����ͨ�ò���
    ZV_ERR_M_SIZE = 0x3001,             //�ߴ粻����Ҫ��
    ZV_ERR_M_SIZE_INVALID = 0x3002,     //�ߴ���Ч
    ZV_ERR_M_OVER_SIZE = 0x3003,        //�ߴ糬����Χ
    ZV_ERR_M_EMPTY = 0x3004,            //����Ϊ��
    ZV_ERR_M_DATA_FORMAT = 0x3005,      //��֧�ֵ����ݸ�ʽ   //�粻�������
    ZV_ERR_M_DIM = 0x300B,              //ά�ȳ�����Χ
    ZV_ERR_M_ROI = 0x3020,              //ROI�ߴ����
    ZV_ERR_M_SIZE_UNMATCH = 0x3030,     //��������ߴ粻ƥ��
    ZV_ERR_M_TYPE_UNMATCH = 0x3031,     //����������Ͳ�ƥ��

    ZV_ERR_IMG_START = 0x3200,              //IMG ͼ���������I
    ZV_ERR_I_ONLY_8UC1 = ZV_ERR_IMG_START,  //ͼ���ʽ���󣬽�֧��8λ��ͨ��
    ZV_ERR_I_DEPTH = 0x3201,                //��֧�ֻ�δ�������������
    ZV_ERR_I_CN = 0x3204,
    ZV_ERR_I_CN_UNMATCH = 0x3205,           //ͨ��������ƥ��
    ZV_ERR_I_CN_SRC = 0x3206,               //Դͼ��ͨ��������
    ZV_ERR_I_CN_DST = 0x3207,
    ZV_ERR_I_ONLY_CN1 = 0x3208,             //��֧�ֵ�ͨ��ͼ��
    ZV_ERR_I_ALIGN = 0x3209,

    ZV_ERR_MAT_START = 0x3300,              //MAT ����
    ZV_ERR_MAT_MUL_SIZE = ZV_ERR_MAT_START, //����˳ߴ粻ƥ��
    ZV_ERR_MAT_NOT_SQUARE = 0x3301,         //�����Ƿ���
    
    ZV_ERR_C_START = 0x3400,                //CONT ����C
    
    ZV_ERR_DRA_START = 0x3500,              //��ͼdra
    ZV_ERR_DRA_COLOR_NAME = ZV_ERR_DRA_START,   //��֧�ֵ���ɫ����
    ZV_ERR_DRA_COLOR_RANGE = 0x3501,        // ��ɫֵ������Χ
    ZV_ERR_DRA_MARKER_TYPE = 0x3502,        //��֧�ֵ�Marker����

    ZV_ERR_FILTER_START = 0x3600,           //�˲�FT
    ZV_ERR_FT_SIZE = ZV_ERR_FILTER_START,   //�˲����ߴ糬��
    ZV_ERR_FT_ANCHOR = 0x3601,              //�˲���ƫ�ƴ���
    ZV_ERR_FT_STRUCT = 0x3602,              //�˲����ṹ����
    
    ZV_ERR_MP_START = 0x3700,               //��̬ѧMP
    ZV_ERR_MP_TYPE = ZV_ERR_MP_START,       //�������̬ѧ����
    ZV_ERR_MP_SHAPE = 0x3701,               //����ĽṹԪ����״
   
    ZV_ERR_SEG_START = 0x3780,              //�ָ� S

    ZV_ERR_MC_START = 0x3900,               //ƥ��MC
    ZV_ERR_MC_METHOD = ZV_ERR_MC_START,     //ƥ���㷨��֧��

    ZV_ERR_MR_START = 0x3A00,               //3A00   ����MR
    ZV_ERR_MR_MR_UNMATCH = ZV_ERR_MR_START, //����������������ܲ�ƥ��
    ZV_ERR_MR_INVALID = 0x3A01,             //����������Ч
    ZV_ERR_MR_PTS_NUM = 0x3A02,             //������ϵ���������
    ZV_ERR_MR_PT_OUTER_X = 0x3A03,          //��ϵ�X���곬��ͼ��Χ
    ZV_ERR_MR_PT_OUTER_Y = 0x3A04,          //��ϵ�Y���곬��ͼ��Χ
    ZV_ERR_MR_CIRCLE = 0x3A10,
    ZV_ERR_MR_LINE = 0x3A11,

    ZV_ERR_FEAT_START = 0x3A80,             //����
    ZV_ERR_FEAT_TYPE = ZV_ERR_FEAT_START,   //�������ʹ���
    ZV_ERR_FEAT_CALC,                       //����ֵ�������
    ZV_ERR_FEAT_MOMENT_ORDER,               //�ؽ�������
    ZV_ERR_FEAT_MOMENT_TYPE,                //�����ʹ���

    ZV_ERR_TK_START = 0x3B00,               //3B00 tools
    ZV_ERR_TK_FIT_PTS_NUM = ZV_ERR_TK_START,
    ZV_ERR_TK_INTERP2D_VAL = 0x3B01,            //��ֵ�������
    ZV_ERR_TK_LINE_PT,
    ZV_ERR_TK_TRANS_MAT,

    ZV_ST_MAX = 0xFFFF,
}ZvStatus;

#endif
