#include "test_function.h"
#include "test_frame.h"
#include <windows.h>
#include <string>
#include <opencv.hpp>

////��Ԫ������־��
//���ԣ���ѧ���� - ������
//���뺯�� ---->testScalarAdd<----
//�������1��[ͼ��]��������1(ROOT/1.jpg)
//�������2��[��ֵ]18.0000
//�������1��[ͼ��]�������1
//���Ժ�����zvScalarAdd
//����������zvReadImage
//    ���λ��[100,100] 134.000 == 134.000
//    ���ͼ��� 134335.0000 == 134335.0000
//���Խ����PASS
//�뿪����
//

ZvImage g_test_imgFrame = NULL;
ZvMatrix g_test_matFrame = NULL;

ZvImage g_test_imgStdOut = NULL;
ZvMatrix g_test_matStdOut = NULL;

ZvMatrix g_test_matSrc = NULL;
ZvMatrix g_test_matSrc2 = NULL;
ZvMatrix g_test_matDst = NULL;


//���Ժ���Demo
TEST_FUNC(testBase)  //test_testBase
{
    TEST_TRACE("Version:%s\n", ZV_VERSION);
    g_zv_errStatus = 0x1001;
    TEST_CHK_TRUE(0x1001 == g_zv_errStatus);
    g_zv_errStatus = 0;
    TEST_CHK_TRUE(0 == g_zv_errStatus);
   
    g_zv_errLastError = 0x1002;
    TEST_CHK_TRUE(0x1002 == g_zv_errLastError);
    g_zv_errLastError = 0;
    TEST_CHK_TRUE(0 == g_zv_errLastError);
    TEST_CHK_TRUE('\0' == g_zv_errLastErrorMsg[0]);
    TEST_CHK_TRUE(false == g_zv_sysIsDrawFill);
    TEST_CHK_TRUE(26066955 == zv_sysGetParamInt("Test"));
    zv_sysSetParamInt("Test", 888);
    TEST_CHK_TRUE(888 == zv_sysGetParamInt("Test"));
    TEST_CHK_NEAR(ZV_PI, zv_sysGetParamDouble("Test"));

    TEST_RTN_PASS;
}

TEST_FUNC(testPass)
{
    TEST_TRACE("ģ����Գɹ����\n");
    Sleep(300);
    TEST_RTN_PASS;
}

TEST_FUNC(testFail)
{
    TEST_TRACE("ģ�����ʧ�����\n");
    TEST_TRACE("������ͳ������\n");
    Sleep(200);
    ZvImage img1 = test_frm_getImgSrc(0);
    ZvImage img2 = test_frm_getImgSrc(2);

    ZvImage dst1 = test_frm_getImgDst(false);
    ZvImage dst2 = test_frm_getImgDst(true);
    zv_imgCopy(img1, dst1);
    zv_imgCopy(img2, dst2);

    TEST_RTN_FAIL;
}

/*========================ͼ��Ļ�������======================*/
TEST_FUNC(zv_matGenEye)
{
    TEST_CHK_RUN(zv_matGenEye(g_test_matSrc, 5));

    // ��ȡ�Եľ���Ȼ�����ƥ��
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matDst, TEST_DATA_PATH"./TrueResultData/test_eye_mat.xml", "ZvMat", 0));
  
    TEST_CHK_MAT_NEAR(g_test_matSrc, g_test_matDst);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_matGetData)    
{
    double Src2Data[9] = { 0, 2, 4, -3, -1, 1, -6, -4, -2 };
    int width = 3, height = 3;
    zv_matGenData(g_test_matSrc, height, width, Src2Data);

    // �޸�src �����ֵ
    double data[9] = { 0 };
    TEST_CHK_RUN(zv_matGetData(g_test_matSrc, data, 9));

    TEST_CHK_ARR_NEAR(Src2Data, data);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_matCopy)
{
    // ��.xml �ļ��ж�ȡ���ݵ� ZvMatrxi��������ȥ
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matSrc, TEST_DATA_PATH"./TrueResultData/test_data_mat.xml", "ZvMat", 0));

    TEST_CHK_RUN(zv_matCopy(g_test_matSrc, g_test_matDst));

    // ��ȡ�Եľ���Ȼ�����ƥ��
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"./TrueResultData/test_copy_mat.xml", "ZvMat", 0));

    TEST_CHK_MAT_NEAR(g_test_matStdOut, g_test_matDst);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_matGetSub)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);
    
    double x = 50, y = 100, width = 240, height = 320;
    TEST_CHK_RUN(zv_matGetSub(src, dst, x, y, width, height));

    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/sub_image.bmp", 0, 0));
    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/sub_image.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_imgConvert)
{
    ZvImage img = test_frm_getImgSrc(0);
    ZvImage tmp = test_frm_getImgDst(false);
    ZvImage dst = test_frm_getImgDst(true);
    zv_imgConvert(img, tmp, ZV_PIXEL_32S, 1, 0);
    zv_imgConvert(tmp, dst, ZV_PIXEL_8U, 1, 0);
    ZvImage tmp1 = test_frm_getImgDst(false);
    ZvImage dst1 = test_frm_getImgDst(true);
    zv_imgConvert(img, tmp1, ZV_PIXEL_64F, 1, 0);
    zv_imgConvert(tmp1, dst1, ZV_PIXEL_8U, 1, 0);

    TEST_RTN_PASS;
}

/*================��ѧ����======================*/
TEST_FUNC(zv_arithAbs)
{
    ZvImage img = test_frm_getImgSrc(0);
    ZvImage tmp = test_frm_getImgDst(false);
    ZvImage dst = test_frm_getImgDst(true);
    zv_imgConvert(img, tmp, ZV_PIXEL_32S, 1, -127);

    TEST_CORE_RUN(zv_arithAbs(tmp, dst));
    zv_imgConvert(tmp, tmp, ZV_PIXEL_8U, 1, 0);
    zv_imgConvert(dst, dst, ZV_PIXEL_8U, 1, 0);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_arithAbsDiff)
{
    double Src1Data[9] = { 0, 1, 2, -15, -14, -13, -30, -29, -28 };
    double Src2Data[9] = { 0, 2, 4, -3, -1, 1, -6, -4, -2 };
    int width = 3, height = 3;
    zv_matGenData(g_test_matSrc, height, width, Src1Data);
    zv_matGenData(g_test_matSrc2, height, width, Src2Data);

    TEST_CHK_RUN(zv_arithAbsDiff(g_test_matSrc, g_test_matSrc2, g_test_matDst, 1));    // �������ֵ�ľ���ֵ�ĺ���     

    // ��ȡ�Եľ���Ȼ�����ƥ��
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"./TrueResultData/test_absdiff_mat.xml", "ZvMat", 0));

    TEST_CHK_MAT_NEAR(g_test_matDst, g_test_matStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_arithAdd)
{
    double Src1Data[9] = { 0, 1, 2, -15, -14, -13, -30, -29, -28 };
    double Src2Data[9] = { 0, 2, 4, -3, -1, 1, -6, -4, -2 };
    int width = 3, height = 3;
    zv_matGenData(g_test_matSrc, height, width, Src1Data);
    zv_matGenData(g_test_matSrc2, height, width, Src2Data);
    TEST_CHK_RUN(zv_arithAdd(g_test_matSrc, g_test_matSrc2, g_test_matDst, 1, 0));

    // ��ȡ�Եľ���Ȼ�����ƥ��
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"./TrueResultData/test_ADD_mat.xml", "ZvMat", 0));

    TEST_CHK_MAT_NEAR(g_test_matDst, g_test_matStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_arithAddWeighted)
{
    double Src1Data[9] = { 0, 1, 2, -15, -14, -13, -30, -29, -28 };
    double Src2Data[9] = { 0, 2, 4, -3, -1, 1, -6, -4, -2 };
    int width = 3, height = 3;
    zv_matGenData(g_test_matSrc, height, width, Src1Data);
    zv_matGenData(g_test_matSrc2, height, width, Src2Data);

    TEST_CHK_RUN(zv_arithAddWeighted(g_test_matSrc, g_test_matSrc2, g_test_matDst, 2, 3, 4));    // �������ֵ�ľ���ֵ�ĺ���     

    // ��ȡ�Եľ���Ȼ�����ƥ��
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"./TrueResultData/test_AddWeighted_mat.xml", "ZvMat", 0));

    TEST_CHK_MAT_NEAR(g_test_matDst, g_test_matStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_arithSin)    // �����������ֵ
{
    double Src1Data[9] = { 10, 20, 35, 45, 60, 75, 90, 135, 155 };
    int width = 3, height = 3;
    zv_matGenData(g_test_matSrc, height, width, Src1Data);

    TEST_CHK_RUN(zv_arithSin(g_test_matSrc, g_test_matDst));    // �������ֵ�ľ���ֵ�ĺ���     

    // ������� .xml �ļ���ȥ, ���Լ��Ƿ�ԣ��ԵĻ�����Ϊ���յı�׼
    //TEST_CHK_RUN(zv_fileWriteMatrix(g_test_matSrc, TEST_DATA_PATH"./TrueResultData/test_sin_mat.xml", "ZvMat", 0));

    // ��ȡ�Եľ���Ȼ�����ƥ��
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"./TrueResultData/test_sin_mat.xml", "ZvMat", 0));

    TEST_CHK_MAT_NEAR(g_test_matDst, g_test_matStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_arithAsin)
{
    ZvImage src = test_frm_getImgSrc(3);
    ZvImage cvtDb = test_frm_getImgDst(false);
    zv_imgConvert(src, cvtDb, ZV_PIXEL_64F, 1.0 / 255, 0);

    TEST_CORE_RUN(zv_arithAsin(cvtDb, g_test_matDst));

    //TEST_CHK_RUN(zv_fileWriteMatrix(g_test_matDst, TEST_DATA_PATH"TrueResultData/test_asin_mat.xml", "ASin", 0));
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"TrueResultData/test_asin_mat.xml", "ASin", 0));
    TEST_CHK_MAT_NEAR(g_test_matDst, g_test_matStdOut);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_arithAcos)
{
    ZvImage src = test_frm_getImgSrc(3);
    ZvImage cvtDb = test_frm_getImgDst(false);
    zv_imgConvert(src, cvtDb, ZV_PIXEL_64F, 1.0 / 255, 0);

    TEST_CORE_RUN(zv_arithAcos(cvtDb, g_test_matDst));

    //TEST_CHK_RUN(zv_fileWriteMatrix(g_test_matDst, TEST_DATA_PATH"./TrueResultData/test_acos_mat.xml", "ACos", 0));
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"./TrueResultData/test_acos_mat.xml", "ACos", 0));
    TEST_CHK_MAT_NEAR(g_test_matDst, g_test_matStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_arithAnd)
{
    ZvImage tmp = test_frm_getImgDst(false);
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);

    double val = 0;
    TEST_CHK_RUN(zv_imgGenConst(tmp, zv_imgWidth(src), zv_imgHeight(src), zv_imgDataType(src), zv_imgChannels(src), &val));
    g_zv_sysIsDrawFill = 1;
    zv_draCircle(tmp, 400, 500, 300, 0xFFFFFF);
    zv_draCircle(tmp, 400, 500, 120, 0);
    g_zv_sysIsDrawFill = 0;
    TEST_CHK_TRUE(0 == g_zv_errLastError);

    TEST_CHK_RUN(zv_arithAnd(src, tmp, dst));
    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/and_image.bmp", 0, 0));
    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/and_image.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_arithHist)
{
    ZvImage src = test_frm_getImgSrc(6);
    TEST_CHK_RUN(zv_arithHist(src, g_test_matDst, 256, 0, 255));
    //TEST_CHK_RUN(zv_fileWriteMatrix(g_test_matDst, TEST_DATA_PATH"TrueResultData/test_std_mat.xml", "hist_data_6", 0));
    zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"TrueResultData/test_std_mat.xml", "hist_data_6", 0);
    TEST_CHK_MAT_NEAR(g_test_matDst, g_test_matStdOut);
    TEST_RTN_PASS;
}

/*====================ͼ��/����Ķ�ȡ�ͱ���=================*/
TEST_FUNC(zv_fileReadImage)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);

    TEST_CHK_RUN(zv_fileReadImage(dst, TEST_DATA_PATH"./TrueResultData/gray_image.bmp", 0));    // ��ʽ���øú���  
  
    TEST_CHK_MAT_NEAR(src, dst);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_fileWriteImage)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);

    TEST_CHK_RUN(zv_fileWriteImage(src, TEST_DATA_PATH"Temp/write_test.bmp", 100));
    Sleep(10);
    TEST_CHK_RUN(zv_fileReadImage(dst, TEST_DATA_PATH"Temp/write_test.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_EQ(src, dst);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_fileReadMatrix)
{
    TEST_CHK_RUN(zv_matGenEye(g_test_matSrc, 5));
    // ��
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"./TrueResultData/test_eye_mat.xml", "ZvMat", 0));
    TEST_CHK_MAT_NEAR(g_test_matSrc, g_test_matStdOut);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_fileWriteMatrix)
{
    double Src1Data[9] = { 10, 11, 2, -15, 34, -13, 47, -29, 58 };
    zv_matGenData(g_test_matSrc, 3, 3, Src1Data);

    // д
    TEST_CHK_RUN(zv_fileWriteMatrix(g_test_matSrc, TEST_DATA_PATH"./TrueResultData/test_fileWriteMat_mat.xml", "WriteMatrix", 0));

    // ��
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"./TrueResultData/test_fileWriteMat_mat.xml", "WriteMatrix", 0));
    TEST_CHK_MAT_NEAR(g_test_matSrc, g_test_matStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_fileMatrix)
{
    double Src1Data[9] = { 10, 11, 2, -15, 34, -13, 47, -29, 58 };
    zv_matGenData(g_test_matSrc, 3, 3, Src1Data);

    // д
    TEST_CHK_RUN(zv_fileWriteMatrix(g_test_matSrc, TEST_DATA_PATH"./TrueResultData/test_fileWrite_mat.yml", "WriteMatrix", 0));

    // ��
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"./TrueResultData/test_fileWrite_mat.yml", "WriteMatrix", 0));
    TEST_CHK_MAT_NEAR(g_test_matSrc, g_test_matStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_fileMatrix_update)
{
    double Src1Data[9] = { 10, 11, 2, -15, 34, -13, 47, -29, 58 };
    zv_matGenData(g_test_matSrc, 3, 3, Src1Data);

    // д
    TEST_CHK_RUN(zv_fileWriteMatrix(g_test_matSrc, TEST_DATA_PATH"Temp/test_fileWrite_mat.yml", "WriteMatrix", 0));
    // ׷��
    TEST_CHK_RUN(zv_fileWriteMatrix(g_test_matSrc, TEST_DATA_PATH"Temp/test_fileWrite_mat.yml", "WriteMatrix1", 1));

    // ��
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"Temp/test_fileWrite_mat.yml", "WriteMatrix", 0));
    TEST_CHK_MAT_NEAR(g_test_matSrc, g_test_matStdOut);
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"Temp/test_fileWrite_mat.yml", "WriteMatrix1", 0));
    TEST_CHK_MAT_NEAR(g_test_matSrc, g_test_matStdOut);

    zv_matSetValue(g_test_matSrc, 0, 2, 100);
    // ����
    TEST_CHK_RUN(zv_fileWriteMatrix(g_test_matSrc, TEST_DATA_PATH"Temp/test_fileWrite_mat.yml", "WriteMatrix1", 1));
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"Temp/test_fileWrite_mat.yml", "WriteMatrix1", 0));
    TEST_CHK_MAT_NEAR(g_test_matSrc, g_test_matStdOut);

    TEST_RTN_PASS;
}

/*====================ͼ������==========================*/
TEST_FUNC(zv_ipMirror)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);
    
    TEST_CHK_RUN(zv_ipMirror(src, dst, 0));   

    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_mirror_img.bmp", 0, 0));
    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_mirror_img.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipRotate)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);

    double angle = 45;         // ��ת�Ķ���
    int interp = 0;            // ��ֵ������Ĭ����ת����ڲ�ֵ����
    TEST_CHK_RUN(zv_ipRotate(src, dst, angle, interp));

    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_rotate_img.bmp", 0, 0));
    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_rotate_img.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipZoom)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);

    double scaleW = 0.5, scaleH = 0.5;    // Ϊͼ��������ŵı�������,����0 ��ͼ����зŴ�С��0��ͼ�������С;
    int interp = 0;            // ��ֵ������Ĭ����ת����ڲ�ֵ����
    TEST_CHK_RUN(zv_ipZoom(src, dst, scaleW, scaleH, interp));

    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_zoom_img.bmp", 0, 0));
    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_zoom_img.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipAffine)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage rgb = test_frm_getImgSrc(9);

    int width = zv_imgWidth(rgb);
    int height = zv_imgHeight(rgb);
    double from[6], to[6];
    from[0] = 0; from[1] = 0;                   // ͼ���ϵ� A ��
    from[2] = width - 1; from[3] = 0;    //ͼ���ϵ� B ��
    from[4] = 0; from[5] = height - 1;    //ͼ���ϵ� C ��

    to[0] = width*0.0; to[1] = height*0.33;       //�任���ͼ������Ӧԭͼ A �ĵ�ΪA��
    to[2] = width*0.85; to[3] = height*0.25;      //  �任���ͼ������Ӧԭͼ B �ĵ�ΪB��
    to[4] = width*0.15; to[5] = height*0.7;         //�任���ͼ������Ӧԭͼ C�ĵ�ΪC��

    zv_tkGetAffine(g_test_matDst, from, to);
    int interp = 0;     // ������ͼ��Ŀ�͸�
    char *border = NULL;                                                            // �߽�Ĵ�����
    TEST_CHK_RUN(zv_ipAffine(rgb, dst, g_test_matDst, width, height, interp, border));       //���з���任

    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_affine_img.bmp", 0, 0));
    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_affine_img.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipPerspective)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage rgb = test_frm_getImgSrc(9);

    int width = zv_imgWidth(rgb);
    int height = zv_imgHeight(rgb);
    // double from[8], to[8];
   // from[0] = 0;                            from[1] = 0;    // A
   // from[2] = (*(cv::Mat*)rgb).cols - 1;    from[3] = 0;   // B
   // from[4] = 0;                            from[5] = (*(cv::Mat*)rgb).rows - 1;   // C
   // from[6] = (*(cv::Mat*)rgb).cols - 1;    from[7] = (*(cv::Mat*)rgb).rows - 1;    //D

   // to[0] = (*(cv::Mat*)rgb).cols*0.05;     to[1] = (*(cv::Mat*)rgb).cols*0.33;
   // to[2] = (*(cv::Mat*)rgb).cols*0.9;      to[3] = (*(cv::Mat*)rgb).rows*0.25;           // 
   // to[4] = (*(cv::Mat*)rgb).cols*0.2;      to[5] = (*(cv::Mat*)rgb).rows*0.7;             // 
   // to[6] = (*(cv::Mat*)rgb).cols*0.8;      to[7] = (*(cv::Mat*)rgb).rows*0.9;             // 
   // zv_tkGetProjective(g_test_matDst, from, to);
   // int dstW = (*(cv::Mat*)rgb).cols, dstH = (*(cv::Mat*)rgb).rows, interp = 0;
   // char *border = NULL;
   // TEST_CHK_RUN(zv_ipPerspective(rgb, dst, g_test_matDst, dstW, dstH, interp, border));       //���з���任
   // 
   //// TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_perspective_img.bmp", 0, 0));
   // TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_perspective_img.bmp", 0));    // ��ʽ���øú���  
   // TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipRgbToGray)
{
    ZvImage dst = test_frm_getImgDst(true);
    TEST_CORE_RUN(zv_ipRgbToGray(test_frm_getImgSrc(4), dst));
    //zv_fileWriteImage(dst, TEST_DATA_PATH"4_to_gray.bmp", NULL, 0); //�����˹��жϵ���ȷͼƬ
    ZvImage stdGray = test_frm_loadImgStd("4_to_gray.bmp");
    TEST_CHK_MAT_EQ(dst, stdGray);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipThreshold)
{
    ZvImage dst = test_frm_getImgDst(true);
    TEST_CORE_RUN(zv_ipThreshold(test_frm_getImgSrc(5), dst, 100, 255, false));
    //zv_fileWriteImage(dst, TEST_DATA_PATH"5_thresh_100_255_F.bmp", NULL, 0); //�����˹��жϵ���ȷͼƬ
    ZvImage imgStd = test_frm_loadImgStd("5_thresh_100_255_F.bmp");
    TEST_CHK_MAT_EQ(dst, imgStd);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipThreshold1)
{
    ZvImage dst = test_frm_getImgDst(true);
    TEST_CHK_RUN(zv_ipThreshold(test_frm_getImgSrc(5), dst, 100, 255, true));
    ZvImage stdGray = test_frm_loadImgStd("5_thresh_100_255_T.bmp");
    TEST_CHK_MAT_EQ(dst, stdGray);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipEqualizeHist)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);

    TEST_CHK_RUN(zv_ipEqualizeHist(src, dst));

    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_equalizeHist_img.bmp", 0, 0));
    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_equalizeHist_img.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipMeanBlur)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);

    TEST_CHK_RUN(zv_ipMeanBlur(src, dst, 3));

    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_meanBlur_img.bmp", 0, 0));

    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_meanBlur_img.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipGaussBlur)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);

    TEST_CHK_RUN(zv_ipGaussBlur(src, dst, 3));

    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_guassBlur_img.bmp", 0, 0));

    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_guassBlur_img.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_NEAR(dst, g_test_imgStdOut);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipCanny)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);
    
    /// ʹ�� 3x3�ں˽���
    int size = 3;
    TEST_CHK_RUN(zv_ipMeanBlur(src, dst, size));    

    /// ����Canny����
    int max_lowThreshold = 100, ratio = 3, lowThreshold = 45, kernel_size = 3;
    TEST_CHK_RUN(zv_ipCanny(dst, dst, lowThreshold, lowThreshold*ratio, kernel_size));
    
    // д
    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_canny_img.bmp", 0, 0));
    // ��
    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_canny_img.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipSobel)
{
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);

    /// �ֱ�� x�� �� y�����sobel���ݶ���
    TEST_CHK_RUN(zv_ipSobel(src, g_test_matSrc, 1, 0, 3));
    zv_imgConvert(g_test_matSrc, g_test_matSrc, ZV_PIXEL_8U, 1, 0);

    TEST_CHK_RUN(zv_ipSobel(src, g_test_matSrc2, 0, 1, 3));
    zv_imgConvert(g_test_matSrc2, g_test_matSrc2, ZV_PIXEL_8U, 1, 0);

    /// �ϲ��ݶ�(����)
    TEST_CHK_RUN(zv_arithAddWeighted(g_test_matSrc, g_test_matSrc2, dst, 0.5, 0.5, 0));

    // д
    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_Soble_img.bmp", 0, 0));
    
    // ��
    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_Soble_img.bmp", 0));    // ��ʽ���øú���  

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipOpening)
{
    // ��ûҶ�ͼ��
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);

    TEST_CHK_RUN(zv_ipOpening(src, dst, 15, 15));

    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_Opening_img.bmp", 0, 0));

    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_Opening_img.bmp", 0));

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_ipClosing)
{
    // ��ûҶ�ͼ��
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(9);

    TEST_CHK_RUN(zv_ipClosing(src, dst, 15, 15));
    //TEST_CHK_RUN(zv_fileWriteImage(dst, TEST_DATA_PATH"./TrueResultData/ip_Closing_img.bmp", 0, 0));
    TEST_CHK_RUN(zv_fileReadImage(g_test_imgStdOut, TEST_DATA_PATH"./TrueResultData/ip_Closing_img.bmp", 0));

    TEST_CHK_MAT_EQ(dst, g_test_imgStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_contGen)
{
    // ��ûҶ�ͼ��
    ZvImage dst = test_frm_getImgDst(true);     // ͨ����������� ZvImage �����ȿ�����ʾ����
    ZvImage src = test_frm_getImgSrc(5);

    // ��ͼ�������ֵ������
    TEST_CHK_RUN(zv_ipThreshold(src, dst, 100, 255, false));
    //TEST_CORE_RUN(zv_ipThreshold(test_frm_getImgSrc(5), dst, 100, 255, false));

    ZvContList contours = zv_modContListCreate();

    // �õ�ͼ�������
    TEST_CHK_RUN(zv_contGen(dst, contours, 1, 0));     // error

    zv_matGenConst(g_test_matDst, zv_contCount(contours), 2, 0);
    for (int i = 0; i < zv_contCount(contours); i++)
    {
        ZvContour cont2 = zv_contGetContour(contours, i);
        // �������������� д�뵽 Matrix ������ȥ
        double pt[2];
        zv_contGetPoint(cont2, 0, pt);
        TEST_CHK_RUN(zv_matSetRow(g_test_matDst, i, pt));
    }
    // �Ѿ�������д�뵽.xml �ļ���ȥ
    //TEST_CHK_RUN(zv_fileWriteMatrix(g_test_matDst, TEST_DATA_PATH"./TrueResultData/test_contours_mat.xml", "contours", 0));
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matStdOut, TEST_DATA_PATH"./TrueResultData/test_contours_mat.xml", "contours", 0));
    // �Ա����������Ƿ�ƥ��
    TEST_CHK_MAT_EQ(g_test_matDst, g_test_matStdOut);

    TEST_RTN_PASS;
}

TEST_FUNC(zv_contFromMatrix)
{
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matSrc, TEST_DATA_PATH"./TrueResultData/test_contours_mat.xml", "contours", 0));

    ZvContour contours = zv_modContCreate();
    TEST_CHK_RUN(zv_contFromMatrix(contours, g_test_matSrc));    // �Ѿ��������ת������������

    TEST_CHK_RUN(zv_contToMatrix(contours, g_test_matStdOut));

    TEST_CHK_MAT_EQ(g_test_matSrc, g_test_matStdOut);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_contArea)
{
    // �õ�����������
    TEST_CHK_RUN(zv_fileReadMatrix(g_test_matSrc, TEST_DATA_PATH"./TrueResultData/test_contours_mat.xml", "contours", 0));
    ZvContour contours = zv_modContCreate();
    TEST_CHK_RUN(zv_contFromMatrix(contours, g_test_matSrc));    // �Ѿ��������ת������������

    // �����������
    double area;
    TEST_CHK_RUN(zv_contArea(contours, false, &area));
    TEST_TRACE("Area:%.8f\n", area);

    TEST_CHK_TRUE(fabs(area - 123211) < 0.1);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_tkFitPolyn)
{
    double data[] = { 5, 9, 6, 11, 7, 0 };
    zv_matGenData(g_test_matSrc, 3, 2, data);
    double param[3] = { 0 };
    TEST_CORE_RUN(zv_tkFitPolyn(g_test_matSrc, 2, param));
    double paramStd[] = { -196, 73.5, -6.5 };
    TEST_CHK_ARR_NEAR(param, paramStd);
    TEST_RTN_PASS;
}

// �Ҷ�ƥ��
TEST_FUNC(zv_matchFastTempl)
{
    ZvImage src = test_frm_getImgSrc(9);
    ZvImage temp = test_frm_getImgSrc(8);

    int pos[2];
    TEST_CHK_RUN(zv_matchFastTempl(src, temp, pos, 0));
    int stdPos[2] = { 65, 115 };

    TEST_CHK_ARR_EQ(pos, stdPos);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_matchBestTempl)
{
    ZvImage src = test_frm_getImgSrc(9);
    ZvImage temp = test_frm_getImgDst(true);
    zv_imgGetSub(src, temp, 180, 543, 40, 40);

    double pos[3];
    TEST_CORE_RUN(zv_matchBestTempl(src, temp, 60, pos, true, 0));
    double stdPos[3] = { 199.5, 562.5, 100 };
    ZvImage dst = test_frm_getImgDst(true);
    zv_ipGrayToRgb(src, dst);
    zv_draRect2(dst, pos[0], pos[1], zv_imgWidth(temp), zv_imgHeight(temp), 0, 0x00FF00);
    TEST_CHK_ARR_NEAR(pos, stdPos, 0.5);
    TEST_RTN_PASS;
}

TEST_SUB_FUNC(matchMultiTrace, ZvImage src, ZvImage temp, ZvMatrix rst, double minScore, int num, int polar)
{
    TEST_CORE_RUN(zv_matchMultiTempl(src, temp, rst, minScore, num, 20, true, polar));
    ZvImage dst = test_frm_getImgDst(true);
    zv_ipGrayToRgb(src, dst);
    double maxPos[3] = { 0 };
    for (int i = 0; i < zv_matRows(rst); i++)
    {
        zv_matGetRow(rst, i, maxPos, 3);
        zv_draRect2(dst, maxPos[0], maxPos[1], zv_imgWidth(temp), zv_imgHeight(temp), 0, 0x00FF00);
        char text[32] = { 0 };
        sprintf(text, "%.2f", maxPos[2]);
        zv_draText(dst, text, maxPos[0], maxPos[1], 1, 0xFF);
    }
    TEST_TRACE("Total Match %d\n", zv_matRows(rst));
    TEST_RTN_CHK;
}

TEST_FUNC(zv_matchMultiTempl)
{
    ZvImage src = test_frm_getImgSrc(9);
    ZvImage temp = test_frm_getImgDst(true);
    zv_imgGetSub(src, temp, 180, 543, 40, 40);
    double stdPos[3] = { 199.5, 562.5, 100 };

    TEST_SUB_CALL(matchMultiTrace, src, temp, g_test_matDst, 70, 0, 0);
    double maxPos[3] = { 0 };
    zv_matGetRow(g_test_matDst, 0, maxPos, 3);
    TEST_CHK_ARR_NEAR(maxPos, stdPos, 0.5);
    TEST_RTN_PASS;
}

TEST_FUNC(zv_matchMultiTempl1)
{
    ZvImage src = test_frm_getImgSrc(9);
    ZvImage temp = test_frm_getImgDst(true);
    zv_imgGetSub(src, temp, 180, 543, 40, 40);
    double stdPos[3] = { 199.5, 562.5, 100 };

    TEST_SUB_CALL(matchMultiTrace, src, temp, g_test_matDst, 70, 0, 0);

    double maxPos[3] = { 0 };
    zv_matGetRow(g_test_matDst, 0, maxPos, 3);
    TEST_CHK_ARR_NEAR(maxPos, stdPos, 0.5);

    TEST_SUB_CALL(matchMultiTrace, src, temp, g_test_matSrc, 60, 0, 0);
    zv_matGetRow(g_test_matSrc, 0, maxPos, 3);
    TEST_CHK_ARR_NEAR(maxPos, stdPos, 0.5);

    TEST_SUB_CALL(matchMultiTrace, src, temp, g_test_matSrc2, 60, 10, 0);
    zv_matGetRow(g_test_matSrc2, 0, maxPos, 3);
    TEST_CHK_ARR_NEAR(maxPos, stdPos, 0.5);
    TEST_CHK_TRUE(10 == zv_matRows(g_test_matSrc2));

    TEST_TRACE("����ǰ10��ƥ���Ƿ���ͬ\n");
    for (int i = 0; i < 10; i++)
    {
        zv_matGetRow(g_test_matDst, i, stdPos, 3);
        zv_matGetRow(g_test_matSrc, i, maxPos, 3);
        TEST_CHK_ARR_NEAR(maxPos, stdPos, 0.5);
        zv_matGetRow(g_test_matSrc2, i, maxPos, 3);
        TEST_CHK_ARR_NEAR(maxPos, stdPos, 0.5);
    }

    TEST_RTN_CHK;
}

/*=============================����Ӵ���=======================*/
TEST_FUNC(zv_matchNccFind)
{
    /**********************************************************************************
    **                      Test1 ��Ŀ��ƥ�䣬����Ŀ���ǲ���ת
    ***********************************************************************************
    ZvImage src = test_frm_getImgSrc(9);
    ZvImage templ = test_frm_getImgSrc(8);
    ZvImage dst = test_frm_getImgSrc(9);
    //zv_imgGetSub(src, temp, 180, 543, 40, 40);

    // error ����д���϶��Ǵ���ģ�
    int T_w = zv_imgWidth(templ);
    int T_h = zv_imgHeight(templ);

    ZvModel mod = zv_zvModelCreate();
    double angleStart = 0, angleExt = 0, angleStep = 0;
    int NumLevel = 2;
    double x, y;
    double minScore = 0.5, minDist = 0;
    int isSubPix = 0;

    zv_matchNccCreate(mod, templ, angleStart, angleExt, angleStep, NumLevel);
    zv_matchNccFind(mod, src, g_test_matFrame, NumLevel, minScore, &x, &y, minDist, isSubPix);

    //�����ҵ�������ROI����
    zv_draRect(dst, x, y, T_w, T_h, 255);

    //��ʾ���ͼ��
    cv::imshow("x1", *(cv::Mat *)dst);
    cv::waitKey(0);
    */

    /**********************************************************************************
    ***                      Test2 ��Ŀ��ƥ�䣬����Ŀ�����нǶȵ�                    ***
    ***********************************************************************************/
    ZvImage src = test_frm_getImgSrc(9);
    ZvImage dst = test_frm_getImgSrc(9);
    ZvImage temp = test_frm_getImgDst(true);
    zv_imgGetSub(src, temp, 53, 34, 67, 149);
    //zv_fileWriteImage(temp, TEST_DATA_PATH"./Temp/TemplImg.bmp", 0);

    int T_w = zv_imgWidth(temp);
    int T_h = zv_imgHeight(temp);

    ZvModel mod = zv_zvModelCreate();
    double angleStart = -180, angleExt = 180, angleStep = 1;
    int NumLevel = 1;            //����û�в��ý������²���ʱ��ģ���תͼ���ƥ��
    double x, y;
    double minScore = 0.98, minDist = 0;
    int isSubPix = 0;

    zv_matchNccCreate(mod, temp, angleStart, angleExt, angleStep, NumLevel);


    clock_t start = clock();   //ʱ�ӿ�ʼ��ʱ�� 
    // ���Եĳ������� 
    zv_matchNccFind(mod, src, g_test_matFrame, NumLevel, minScore, &x, &y, minDist, isSubPix);
    clock_t ends = clock();   //ʱ�ӵĽ���ʱ��
    std::cout << "Running Time : " << (double)(ends - start) / CLOCKS_PER_SEC * 1000 << "ms!" << std::endl;


    //�����ҵ�������ROI����
    zv_draRect(dst, x, y, T_w, T_h, 0);

    //��ͼ���ϻ���ת����
    //double angle = -64;
    //zv_draRect2(dst, x+T_w/2 + 30, y+T_h/2 + 80, T_w, T_h, angle, 255);

    //��ʾ���ͼ��
    std::cout << "(x, y) = " << "(" << x << ", " << y << ")" << std::endl;
    cv::imshow("x1", *(cv::Mat *)dst);
    cv::waitKey(0);

    TEST_RTN_CHK;
}


int testRegisterCPP()
{
    g_test_imgFrame = zv_modImgCreate();
    g_test_matFrame = zv_modMatCreate();

    g_test_matStdOut = zv_modMatCreate();
    g_test_imgStdOut = zv_modImgCreate();

    g_test_matSrc = zv_modMatCreate();
    g_test_matSrc2 = zv_modMatCreate();
    g_test_matDst = zv_modMatCreate();

    test_frm_setSetDisp(TEST_TEST, "��ܲ���");
    REG_FUNCTION(TEST_TEST, "��������", testBase);
    REG_FUNCTION(TEST_TEST, "ͨ������", testPass);
    REG_FUNCTION(TEST_TEST, "ʧ������", testFail);

    test_frm_setSetDisp(TEST_CORE, "��������");
    REG_FUNCTION(TEST_CORE, "���ɵ�λ����", zv_matGenEye);
    REG_FUNCTION(TEST_CORE, "�޸ľ����ֵ", zv_matGetData);
    REG_FUNCTION(TEST_CORE, "����Ŀ���", zv_matCopy);
    REG_FUNCTION(TEST_CORE, "���ͼ���������", zv_matGetSub);
    REG_FUNCTION(TEST_CORE, "���ͼ������", zv_contGen);
    REG_FUNCTION(TEST_CORE, "��������תΪ����", zv_contFromMatrix);
    REG_FUNCTION(TEST_CORE, "ͼ���ʽת��", zv_imgConvert);

    test_frm_setSetDisp(TEST_ARITH, "��ѧ����");
    REG_FUNCTION(TEST_ARITH, "����ֵ", zv_arithAbs);
    REG_FUNCTION(TEST_ARITH, "���Բ�", zv_arithAbsDiff);
    REG_FUNCTION(TEST_ARITH, "�������", zv_arithAdd);
    REG_FUNCTION(TEST_ARITH, "��Ȩ��", zv_arithAddWeighted);
    REG_FUNCTION(TEST_ARITH, "����", zv_arithSin);
    REG_FUNCTION(TEST_ARITH, "������", zv_arithAsin);
    REG_FUNCTION(TEST_ARITH, "������", zv_arithAcos);
    REG_FUNCTION(TEST_ARITH, "λ��", zv_arithAnd);
    REG_FUNCTION(TEST_ARITH, "ֱ��ͼ", zv_arithHist);

    test_frm_setSetDisp(TEST_FILE, "�ļ�");
    REG_FUNCTION(TEST_FILE, "��ȡͼ��", zv_fileReadImage);
    REG_FUNCTION(TEST_FILE, "����ͼ��", zv_fileWriteImage);
    REG_FUNCTION(TEST_FILE, "��ȡ����", zv_fileReadMatrix);
    REG_FUNCTION(TEST_FILE, "�������", zv_fileWriteMatrix);
    REG_FUNCTION(TEST_FILE, "YAML��д", zv_fileMatrix);
    REG_FUNCTION(TEST_FILE, "YAML����", zv_fileMatrix_update);

    test_frm_setSetDisp(TEST_IP, "ͼ����");
    REG_FUNCTION(TEST_IP, "����任", zv_ipMirror);
    REG_FUNCTION(TEST_IP, "��ת�任", zv_ipRotate);
    REG_FUNCTION(TEST_IP, "���ű任", zv_ipZoom);
    REG_FUNCTION(TEST_IP, "����任", zv_ipAffine);
    REG_FUNCTION(TEST_IP, "͸��任", zv_ipPerspective);
    REG_FUNCTION(TEST_IP, "��ɫת�Ҷ�", zv_ipRgbToGray);
    REG_FUNCTION(TEST_IP, "��ֵ��", zv_ipThreshold);
    REG_FUNCTION(TEST_IP, "��ֵ��ȡ��", zv_ipThreshold1);
    REG_FUNCTION(TEST_IP, "ֱ��ͼ���⻯", zv_ipEqualizeHist);
    REG_FUNCTION(TEST_IP, "��ֵ�˲�", zv_ipMeanBlur);
    REG_FUNCTION(TEST_IP, "��˹�˲�", zv_ipGaussBlur);
    REG_FUNCTION(TEST_IP, "Canny���ӱ�Ե��ȡ", zv_ipCanny);
    REG_FUNCTION(TEST_IP, "Soble���ӱ�Ե��ȡ", zv_ipSobel);
    REG_FUNCTION(TEST_IP, "��̬ѧ������", zv_ipOpening);
    REG_FUNCTION(TEST_IP, "��̬ѧ������", zv_ipClosing);

    test_frm_setSetDisp(TEST_FEATURE, "����");
    REG_FUNCTION(TEST_FEATURE, "�������", zv_contArea);

    test_frm_setSetDisp(TEST_TK, "��������");
    REG_FUNCTION(TEST_TK, "��϶���ʽ", zv_tkFitPolyn);

    test_frm_setSetDisp(TEST_MATCH, "ƥ��");
    REG_FUNCTION(TEST_MATCH, "����ģ��ƥ��", zv_matchFastTempl);
    REG_FUNCTION(TEST_MATCH, "ģ��ƥ��", zv_matchBestTempl);
    REG_FUNCTION(TEST_MATCH, "��Ŀ��ƥ��", zv_matchMultiTempl);
    REG_FUNCTION(TEST_MATCH, "��Ŀ��ƥ��1", zv_matchMultiTempl1);

    /*=============================����Ӵ���=======================*/
    REG_FUNCTION(TEST_MATCH, "NCC�Ҷ�ƥ��", zv_matchNccFind);


    return 0;
}
