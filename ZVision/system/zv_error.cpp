#include "zv_private.h"
#include "zv_error.h"
#include "zv_system.h"


ZV_EXPORTS int g_zv_errStatus = ZV_ST_OK;
int g_zv_errError = ZV_ST_OK;
ZV_EXPORTS int g_zv_errLastError = ZV_ST_OK;

static char s_zv_err_statusMsg[ZV_ERR_MSG_BUF_LEN] = { 0 };
char g_zv_errErrorMsg[ZV_ERR_MSG_BUF_LEN] = { 0 };
ZV_EXPORTS char g_zv_errLastErrorMsg[ZV_ERR_MSG_BUF_LEN] = { 0 };

char *zv_err_statusStr(int status)
{
    switch (status)
    {
    case ZV_ST_OK:                  return "����";
    case ZV_WARN_RELE_NULL:         return "�������ͣ��ͷſ�ָ��";
    case ZV_WARN_RELE_NONE:         return "�ͷſ�����";
    case ZV_WARN_RELE_UNINIT:       return "�ͷ�δ��ʼ���Ľṹ";
    case ZV_WARN_FILE_NOT_EXIST:    return "�ļ�������";
    case ZV_WARN_NODE_NOT_EXIST:    return "�ڵ㲻����";

    case ZV_ERR_G_UNDEF:            return "δ�������";
    case ZV_ERR_G_CPP_EXCEP:        return "C++�쳣";
    case ZV_ERR_G_ASSERT:           return "���Դ���";
    case ZV_ERR_G_NULL_PTR:         return "��ָ��";
    case ZV_ERR_G_STD_EXCEP:        return "std���쳣";
    case ZV_ERR_G_LIB_EXCEP:        return "δ֪�ĵ��������쳣";
    case ZV_ERR_G_LIB_RUN:          return "δ֪�ĵ����������״̬";
    case ZV_ERR_G_DIV_ZERO:         return "�������";
    case ZV_ERR_G_VEC_SIZE:         return "vector���ȴ���";
    case ZV_ERR_G_BUF_SMALL:        return "���TABLE���Ȳ���";
    case ZV_ERR_G_OBJ_NOT_EXIST:    return "Ŀ�겻����";
    case ZV_ERR_G_LIB_ASSERT:       return "δ֪�ĵ����������ʧ��";
    case ZV_ERR_G_UNFINISHED:       return "δ��ɵĹ���";
    case ZV_ERR_G_NOT_SUPPORT:      return "��֧�ֵĹ���";
    case ZV_ERR_G_NSP_CL:           return "��֧�ֵ�OPENCLӦ��";
    case ZV_ERR_G_NSP_GL:           return "��֧�ֵ�OPENGLӦ��";
    case ZV_ERR_G_NSP_CUDA:         return "��֧�ֵ�CUDAӦ��";

    case ZV_ERR_T_PARAM_SIZE:       return "�����������������";
    case ZV_ERR_T_THREAD_ID:        return "�����߳�ID������Χ";

    case ZV_ERR_MEM_ALLOC:          return "�ڴ����ʧ��";
    case ZV_ERR_MEM_CORRUPTED:      return "������ڴ汻�ƻ�";

    case ZV_ERR_F_NOT_EXIST:        return "�ļ�������";
    case ZV_ERR_F_OPEN_FAIL:        return "�ļ��򿪳���";
    case ZV_ERR_F_SAVE_FAIL:        return "�ļ��������";
    case ZV_ERR_F_FORMAT:           return "�ļ���ʽ����";

    case ZV_ERR_CAM_GRAB:           return "�ɼ�����";
    case ZV_ERR_CAM_GRAB_TIMEOUT:   return "�ɼ���ʱ";
    case ZV_ERR_CAM_SCANID:         return "���ɨ������������Χ";
    case ZV_ERR_CAM_CAMID:          return "�������������Χ";
    case ZV_ERR_CAM_UNBOUND:        return "δ�󶨵����";
    case ZV_ERR_CAM_OPEN:           return "�豸�򿪳���";
    case ZV_ERR_CAM_SCAN:           return "���ɨ�����";
    case ZV_ERR_CAM_NONE:           return "δ�������";
    case ZV_ERR_CAM_UNKNOWN:        return "���״̬δ֪";
    case ZV_ERR_CAM_PARAM_UNDEF:    return "����������ò�֧��";
    case ZV_ERR_CAM_PARAM_RD:       return "�����ȡ��������";
    case ZV_ERR_CAM_PARAM_WR :      return "���д���������";
    case ZV_ERR_CAM_INIT_TIS:       return "��ʼ��TIS����ʧ��";

    case ZV_ERR_FA_TYPE:            return "�������ʹ���";
    case ZV_ERR_FA_TYPE_1:          return "����1���ʹ���";
    case ZV_ERR_FA_TYPE_2:          return "����2���ʹ���";
    case ZV_ERR_FA_TYPE_3:          return "����3���ʹ���";
    case ZV_ERR_FA_RANGE:           return "����������Χ";
    case ZV_ERR_FA_RANGE_1:         return "����1������Χ";
    case ZV_ERR_FA_RANGE_2:         return "����2������Χ";
    case ZV_ERR_FA_RANGE_3:         return "����3������Χ";
    case ZV_ERR_FA_RANGE_4:         return "����4������Χ";
    case ZV_ERR_FA_RANGE_5:         return "����5������Χ";
    case ZV_ERR_FA_RANGE_6:         return "����6������Χ";
    case ZV_ERR_FA_RANGE_7:         return "����7������Χ";
    case ZV_ERR_FA_RANGE_8:         return "����8������Χ";
    case ZV_ERR_FA_RANGE_9:         return "����9������Χ";
    case ZV_ERR_FA_RANGE_10:        return "����10������Χ";
    case ZV_ERR_FA_NULL:            return "����Ϊ��";
    case ZV_ERR_FA_NULL_1:          return "����1Ϊ��";
    case ZV_ERR_FA_NULL_2:          return "����2Ϊ��";
    case ZV_ERR_FA_NULL_3:          return "����3Ϊ��";
    case ZV_ERR_FA_NULL_4:          return "����4Ϊ��";
    case ZV_ERR_FA_NULL_5:          return "����5Ϊ��";
    case ZV_ERR_FA_NULL_6:          return "����6Ϊ��";
    case ZV_ERR_FA_NULL_7:          return "����7Ϊ��";
    case ZV_ERR_FA_NULL_8:          return "����8Ϊ��";

    case ZV_ERR_M_SIZE:             return "�ߴ粻����Ҫ��";
    case ZV_ERR_M_SIZE_INVALID:     return "�ߴ���Ч";
    case ZV_ERR_M_OVER_SIZE:        return "�ߴ糬����Χ";
    case ZV_ERR_M_EMPTY:            return "����Ϊ��";
    case ZV_ERR_M_DATA_FORMAT:      return "��֧�ֵ����ݸ�ʽ";
    case ZV_ERR_M_DIM:              return "ά�ȳ�����Χ";
    case ZV_ERR_M_ROI:              return "ROI�ߴ����";
    case ZV_ERR_M_SIZE_UNMATCH:     return "��������ߴ粻ƥ��";
    case ZV_ERR_M_TYPE_UNMATCH:     return "ͼ���������Ͳ�ƥ��";

    case ZV_ERR_I_ONLY_8UC1:        return "ͼ���ʽ���󣬽�֧��8λ��ͨ��";
    case ZV_ERR_I_DEPTH:            return "��֧�ֻ�δ�������������";
    case ZV_ERR_I_CN:               return "ͼ��ͨ��������";
    case ZV_ERR_I_CN_UNMATCH:       return "ͨ��������ƥ��";
    case ZV_ERR_I_CN_SRC:           return "Դͼ��ͨ��������";
    case ZV_ERR_I_CN_DST:           return "Ŀ��ͼ��ͨ��������";
    case ZV_ERR_I_ONLY_CN1:         return "��֧�ֵ�ͨ��ͼ��";
    case ZV_ERR_I_ALIGN:            return "��Ҫ�����ͼ������";
            
    case ZV_ERR_MAT_MUL_SIZE:       return "����˳ߴ粻ƥ��";
    case ZV_ERR_MAT_NOT_SQUARE:     return "�����Ƿ���";

    case ZV_ERR_DRA_COLOR_NAME:     return "��֧�ֵ���ɫ����:";
    case ZV_ERR_DRA_COLOR_RANGE:    return "��ɫֵ������Χ";
    case ZV_ERR_DRA_MARKER_TYPE:    return "��֧�ֵ�Marker����";

    case ZV_ERR_FT_SIZE:            return "�˲����ߴ糬��";
    case ZV_ERR_FT_ANCHOR:          return "�˲���ƫ�ƴ���";
    case ZV_ERR_FT_STRUCT:          return "�˲����ṹ����";
    case ZV_ERR_MP_TYPE:            return "�������̬ѧ����";
    case ZV_ERR_MP_SHAPE:           return "����ĽṹԪ����״";

    case ZV_ERR_MC_METHOD:          return "δ�����ƥ���㷨";

    case ZV_ERR_MR_MR_UNMATCH:      return "����������������ܲ�ƥ��";
    case ZV_ERR_MR_INVALID:         return "����������Ч";
    case ZV_ERR_MR_PTS_NUM:         return "������ϵ���������";
    case ZV_ERR_MR_PT_OUTER_X:      return "��ϵ�X���곬��ͼ��Χ";
    case ZV_ERR_MR_PT_OUTER_Y:      return "��ϵ�Y���곬��ͼ��Χ";
    case ZV_ERR_MR_CIRCLE:          return "����Բ����";
    case ZV_ERR_MR_LINE:            return "����ֱ�߳���";

    case ZV_ERR_FEAT_TYPE:          return "�������ʹ���";
    case ZV_ERR_FEAT_CALC:          return "����ֵ�������";
    case ZV_ERR_FEAT_MOMENT_ORDER:  return "�ؽ�������";
    case ZV_ERR_FEAT_MOMENT_TYPE:   return "�����ʹ���";

    case ZV_ERR_TK_FIT_PTS_NUM:     return "��ϵ���������";
    case ZV_ERR_TK_INTERP2D_VAL:    return "��ֵ�������";
    case ZV_ERR_TK_LINE_PT:         return "ֱ�������غ�";
    case ZV_ERR_TK_TRANS_MAT:       return "�任�������";
    }
    static char buf[32];
    snprintf(buf, sizeof(buf), "δ֪��%s�룺[0x%X]", status >= ZV_ERR_START ? "����" : "״̬", status);
    return buf;
}

int zv_err_statusStrToFile(char *fileName)
{
    //���ļ�
    for (int i = 0; i < ZV_ST_MAX; i++)
    {
        char * str = zv_err_statusStr(i);
        //if ()
    }
    return ZV_ST_OK;
}

void zv_err_checkStatus(int status, char * file, char *func, int line, char *format, ...)
{
    if (ZV_ERR_START >= status)
    {
        return;
    }

    char * stStr = zv_err_statusStr(status);
    g_zv_errLastError = status;
    //20180702 10:24:36.145211 ����[0x2100]���ļ������ڣ�������Ϣ��
    //λ��[0478]:zv_module.cpp/zv_fileReadImage
    string dt = zv_sys_dateTimeString(true);
    int msgIdx = snprintf(g_zv_errLastErrorMsg, sizeof(g_zv_errLastErrorMsg), "%s ����[0x%04X]��%s", dt.c_str(), status, stStr);
    if (format && (format[0] != 0))
    {
        va_list args;
        msgIdx += snprintf(&g_zv_errLastErrorMsg[msgIdx], sizeof(g_zv_errLastErrorMsg) - msgIdx, "��");
        va_start(args, format);
        msgIdx += vsnprintf(&g_zv_errLastErrorMsg[msgIdx], sizeof(g_zv_errLastErrorMsg)-msgIdx, format, args);
        va_end(args);
        msgIdx += snprintf(&g_zv_errLastErrorMsg[msgIdx], sizeof(g_zv_errLastErrorMsg) - msgIdx,"�� ");
    }

#ifdef ZV_TRACE_FUNC_NAME
    if (string(func).size() > 0)
    {
        msgIdx += snprintf(&g_zv_errLastErrorMsg[msgIdx], sizeof(g_zv_errLastErrorMsg) - msgIdx,
            "������%s��", func);
    }
#endif
    msgIdx += snprintf(&g_zv_errLastErrorMsg[msgIdx], sizeof(g_zv_errLastErrorMsg) - msgIdx,
        "λ��[%04d]��%s\n", line, file);

    if (ZV_ST_OK == g_zv_errError)
    {
        g_zv_errError = status;
        memcpy(g_zv_errErrorMsg, g_zv_errLastErrorMsg, sizeof(g_zv_errErrorMsg));
    }
}