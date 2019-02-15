#include "zv_private.h"
#include "zv_system.h"

ZV_EXPORTS int g_zv_sysIsDrawFill = 0;
ZV_EXPORTS int g_zv_sysLineWidth = 1;

//����������Ŀ
int g_zv_sysTestInt = 26066955;         //��ͨ����������Ҳ����ͨ����������
double g_zv_sysTestDouble = ZV_PI;
static string s_zv_sys_testString = "ZMOTION"; //�漰��Խ�籣�������ṩֱ�ӷ���

double g_zv_sys_grabTimeout = 2000;
int g_zv_sys_errorLevel = 0;
int g_zv_sys_lineType = 8;       //����
int g_zv_sys_optLevel = 0;  //�����ã��Ż�����0-Ĭ�ϵ� 1-���������� 2-opencv 3-�����ȫ�� 4-����

int zv_sys_getParamInt(char *name, int *value)
{
    ZV_CHK_NULL2(name, value);

    string strName(name);
    if (ZV_SYS_P_ERROR_LEVEL == strName)
    {
        *value = g_zv_sys_errorLevel;
    }
    else if (ZV_SYS_P_TEST == strName)
    {
        *value = g_zv_sysTestInt;
    }
    else if (ZV_SYS_P_OPT_LEVEL == strName)
    {
        *value = g_zv_sys_optLevel;
    }
    else if (ZV_SYS_P_LINE_TYPE == strName)
    {
        *value = g_zv_sys_lineType;
    }
    else
    {
        return ZV_ERR_FA_RANGE_1;
    }

    return ZV_ST_OK;
}

int zv_sys_setParamInt(char *name, int value)
{
    ZV_CHK_NULL1(name);
    string strName(name);
    if (ZV_SYS_P_TEST == strName)
    {
        g_zv_sysTestInt = value;
    }
    else if (ZV_SYS_P_OPT_LEVEL == strName)
    {
        g_zv_sys_optLevel = value;
    }
    else if (ZV_SYS_P_LINE_TYPE == strName)
    {
        if (0 <= value && 2 >= value)
        {
            g_zv_sys_lineType = 4 << value;
        }
    }
    else
    {
        return ZV_ERR_FA_RANGE_1;
    }
    return ZV_ST_OK;
}

int zv_sys_getParamDouble(char *name, double *value)
{
    ZV_CHK_NULL2(name, value);
    string strName(name);
    if (ZV_SYS_P_TEST == strName)
    {
        *value = g_zv_sysTestDouble;
        return ZV_ST_OK;
    }
    return ZV_ERR_FA_RANGE_1;
}

int zv_sys_setParamDouble(char *name, double value)
{
    ZV_CHK_NULL1(name);
    string strName(name);
    if (ZV_SYS_P_TEST == strName)
    {
        g_zv_sysTestDouble = value;
        return ZV_ST_OK;
    }
    return ZV_ERR_FA_RANGE_1;
}

int zv_sys_getParamString(char *name, char *value, int maxValLen)
{
    ZV_CHK_NULL2(name, value);
    string &tmp = s_zv_sys_testString;
    string strName(name);
    if (ZV_SYS_P_TEST == strName)
    {
        tmp = s_zv_sys_testString;
    }
    else
    {
        return ZV_ERR_FA_RANGE_1;
    }
    if (tmp.size() >= maxValLen)
    {
        return ZV_ERR_G_BUF_SMALL;
    }
    strcpy_s(value, maxValLen, tmp.c_str());
    return ZV_ST_OK;
}

int zv_sys_setParamString(char *name, char *value)
{
    ZV_CHK_NULL2(name, value);
    string strName(name);
    if (ZV_SYS_P_TEST == strName)
    {
        s_zv_sys_testString = value;
        return ZV_ST_OK;
    }
    return ZV_ERR_FA_RANGE_1;
}
