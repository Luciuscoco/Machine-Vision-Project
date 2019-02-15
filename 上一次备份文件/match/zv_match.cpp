#include "zv_private.h"
#include "zv_match.h"
#include "tools/zv_tools.h"

#define ZV_MATCH_RST_TO_SCORE(rst)  ((rst) * 100)  //[-1,1]-->[-100, 100]
#define ZV_MATCH_SCORE_TO_RST(score)  ((score) * 0.01)
#define FillArea 10     // ���������С

//3*3����������������ֵ
int zv_match_subPix2d(float * line0, float *line1, float * line2, double &x, double &y, double &maxVal)
{
    ZV_CHK_NULL3(line0, line1, line2);
    //����9��
    double pixArr[3 * 9] = { -1.0, -1.0, line0[0], 0, -1.0, line0[1], 1.0, -1.0, line0[2],
        -1.0, 0, line1[0], 0, 0, line1[1], 1.0, 0, line1[2],
        -1.0, 1.0, line2[0], 0, 1.0, line2[1], 1.0, 1.0, line2[2] };
    return zv_tk_subPixInterp2d(pixArr, 9, x, y, maxVal); //ʧ���򷵻طǲ�ֵ����
}

// ����ƥ��
int zv_match_fastTempl(ZvImage img, ZvImage templ, int *x, int *y, ZvImage mask)
{
    ZV_CHK_NULL4(img, templ, x, y);
    if (1 != zv_img_channels(img) || 1 != zv_img_channels(templ))
    {
        return ZV_ERR_I_ONLY_CN1;
    }

    if (0 == method)
    {
        method = CV_TM_CCOEFF_NORMED;
    }
    else if (1 == method)
    {
        method = CV_TM_CCOEFF;
    }
    else
    {
        return ZV_ERR_MC_METHOD;
    }

    cv::Mat result;
    cv::matchTemplate(ZV_ADP_IMG_GETM(img), ZV_ADP_IMG_GETM(templ), result, method);

    cv::Point maxLoc;
    cv::minMaxLoc(result, NULL, NULL, NULL, &maxLoc);
    *x = maxLoc.x + zv_img_width(templ) / 2;
    *y = maxLoc.y + zv_img_height(templ) / 2;

    return ZV_ST_OK;
}

// �Ҷ�ģ��ƥ��
int zv_match_bestTempl(ZvImage img, ZvImage templ, double minScore, double *x,
    double *y, double *score, bool isSubPix, int polar)
{
    ZV_CHK_NULL6(img, templ, g_zv_adp_notNull, x, y, score);
    if (minScore > 100 || minScore <= 0)
    {
        return ZV_ERR_FA_RANGE_3;
    }

    if (1 != zv_img_channels(img) || 1 != zv_img_channels(templ))
    {
        return ZV_ERR_I_ONLY_CN1;
    }

    cv::Mat result;
    cv::matchTemplate(ZV_ADP_IMG_GETM(img), ZV_ADP_IMG_GETM(templ), result, cv::TM_CCOEFF_NORMED);

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    if (0 != polar)
    {
        maxVal = max(maxVal, -minVal);  //ȡmaxVal��minVal�ľ���ֵ�ϴ��
    }
    maxVal = ZV_MATCH_RST_TO_SCORE(maxVal);
    if (maxVal < minScore)
    {
        *x = 0;
        *y = 0;

        *score = 0;
        return ZV_ST_MATCH_FAIL;
    }

    *x = maxLoc.x;
    *y = maxLoc.y;
    *score = maxVal;

    if (isSubPix)
    {
        int idx = maxLoc.x - 1;
        int idy = maxLoc.y - 1;
        if (0 < maxLoc.x && result.cols - 1 > maxLoc.x
            && 0 < maxLoc.y && result.rows - 1 > maxLoc.y
            && ZV_ST_OK == zv_match_subPix2d(result.ptr<float>(idy, idx), result.ptr<float>(idy + 1, idx),
            result.ptr<float>(idy + 2, idx), *x, *y, *score))
        {
            *x += maxLoc.x;
            *y += maxLoc.y;
            *score = ZV_MATCH_RST_TO_SCORE(abs(*score));
            if (*score > 100.0)
            {
                *score = 100.0;
            }
        }
        *x += zv_img_width(templ) / 2.0 - 0.5;
        *y += zv_img_height(templ) / 2.0 - 0.5;
    }
    else
    {
        *x += zv_img_width(templ) / 2;
        *y += zv_img_height(templ) / 2;
    }

    return ZV_ST_OK;
}

int zv_match_multiTempl(ZvImage img, ZvImage templ, ZvMatrix matches, double minScore, int method, bool isSubPix)
{
    ZV_CHK_NULL3(img, templ, matches);
    
    cv::Mat _img = ZV_ADP_IMG_GETM(img);
    cv::Mat _temp1 = ZV_ADP_IMG_GETM(templ);
    cv::Mat _matRst = ZV_ADP_MAT_GETM(matches);
    cv::Mat result(_img.cols - _temp1.cols + 1, _img.rows - _temp1.rows + 1, CV_64FC1);
    cv::Mat Rst(_img.cols - _temp1.cols + 1, 3, CV_64FC1);
    
    if (method == 0) method += 5;
    cv::matchTemplate(_img, _temp1, result, method);     // result ��ȡֵΪ��0��1�� ��ֵ

    double maxVal;
    int maxLocX, maxLocY;
    if (minScore < 1 && minScore > 0)
    {
        for (int i = 0; i < result.rows; i++)
        {
            maxVal = 0;  maxLocX = 0;   maxLocY = 0;
            for (int j = 0; j < result.cols; j++)
            {
                // �ҵ�result[i,j��] �����ֵ
                if (maxVal < result.at<double>(i, j))
                {
                    maxVal = result.at<double>(i, j);
                    maxLocX = i;
                    maxLocY = j;
                }
            }

            // �Ƚ�ÿ�� �����ֵ ��> minScore, ����ͱ���: Rst[x_i, y_i, value]
            if (maxVal >= minScore)
            {
                Rst.at<double>(i, 0) = (double)maxLocX;
                Rst.at<double>(i, 1) = (double)maxLocY;
                Rst.at<double>(i, 2) = (double)maxVal;
            }
            else
            {
                Rst.at<double>(i, 0) = (double)0;
                Rst.at<double>(i, 1) = (double)0;
                Rst.at<double>(i, 2) = (double)0;
            }
        }

        ZV_ADP_MAT_SETM(matches, Rst);
        return ZV_ST_OK;
    }
    else
    {
        return ZV_ST_OUT_RANGE_P4;
    }
}

// ���ڻҶ�ֵ�������ƥ�� ~ NCC
int zv_match_grayScale(ZvImage img, ZvImage templ, int NumLevel, double AngleStart, double AngleExtent, double AngleStep, double *x, double *y, double MinScore)   
{
    ZV_CHK_NULL4(img, templ, x, y);

    // ��������Լ�д��NCC�㷨����Ҫ�涨ͼ���ͨ����Ϊ ~ ��ͨ��
    // ����Opencv ��NCC�㷨����Ҫ�����ж�
    //if (ZV_ADP_IMG_GETM(img).channels() != 1)
    //    return ZV_ERR_I_ONLY_8UC1;

    // ģ����ת����
    RotateTempl(templ, &AngleStart, &AngleExtent, &AngleStep);

    // �õ�������ͼ����
    PYRDOWNIMAGE pyrDownImage;
    PYPDOWNTEMPL pyrDownTempl;

    //pyrDownImage.imgLevel = NumLevel;
    pyrDownImage.pyrImg = new cv::Mat[NumLevel];
    //pyrDownTempl.templLevel = NumLevel;
    pyrDownTempl.pyrTempl = new cv::Mat[NumLevel];

    // �²����õ�������ͼ��
    zv_pyrDown(img, templ, NumLevel, pyrDownImage, pyrDownTempl);

    /*MinScore: ƥ�����
    Ҫ�ҵ���ģ��ʵ������ͷ�����
    Ĭ��ֵ��0.8
    ����ֵ��0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0
    ����ֵ��Χ��0��MinScore��1
    ��С������0.01
    ����������0.05*/

    double matchX, matchY;
    cv::Mat ROI;

    double A_x1 = 0,  A_y1 = 0;                             // (A_x1, A_y1) Ϊ��ʼ���꣬ ��A_x2, A_y2) Ϊ��ֹ����;
    double A_y2 = pyrDownImage.pyrImg[NumLevel - 1].rows;   // height
    double A_x2 = pyrDownImage.pyrImg[NumLevel - 1].cols;   // width
    double nccInterScore;
    int i;

    // ���ͼ���ģ��ͼ������ظ��� >= 8
    if ((A_y2 * A_x2 < 8) || (pyrDownTempl.pyrTempl[NumLevel - 1].rows * pyrDownTempl.pyrTempl[NumLevel - 1].cols < 8))
    {
        return -1;
    }
   
    // ���Եĳ�������
    for (i = NumLevel - 1; i >= 0; i--)   // �Ӷ���ĵ����ڶ�����ʼ����
    {
        // ������������������Խ�������
        if (A_x1 < 0)
        {
            A_x1 = 0;
        }
        if (A_y1 < 0)
        {
            A_y1 = 0;
        }
        if (A_x2 > pyrDownImage.pyrImg[i].cols)
        {
            A_x2 = pyrDownImage.pyrImg[i].cols;
        }
        if (A_y2 > pyrDownImage.pyrImg[i].rows)
        {
            A_y2 = pyrDownImage.pyrImg[i].rows;
        }

        ROI = pyrDownImage.pyrImg[i](cv::Rect(A_x1, A_y1, A_x2 - A_x1, A_y2 - A_y1));

         // �ڽ�������ÿһ�� �ҵ����ƥ���
        nccInterScore = getNccInterScore(&ROI, &pyrDownTempl.pyrTempl[i], &matchX, &matchY);

        if (nccInterScore < MinScore)
        {
            // û���ҵ���ѵ�ƥ��λ��
            break;
            return -1;
        }
        
        // λ��ӳ�䣺���¼����ϼ���λ��ӳ��
        // ��Χ������������
        if (i >= 1)
        {
            A_x2 = (matchX + A_x1) * 2 + pyrDownTempl.pyrTempl[i - 1].cols + FillArea;
            A_y2 = (matchY + A_y1) * 2 + pyrDownTempl.pyrTempl[i - 1].rows + FillArea;
            A_x1 = (matchX + A_x1) - FillArea;
            A_y1 = (matchY + A_y1) - FillArea;
        }
    }
   
    *x = matchX + A_x1;
    *y = matchY + A_y1;
    return ZV_ST_OK;
}

// ����ƥ�䣨��ת������
//int zv_match_contourScale(ZvImage img, ZvImage dst)
//{
//    return ZV_ST_OK;
//}
//===========================================================================================================================================================================
//typedef PYPDOWNTEMPL ZvModel;

//��ÿ������level)��ʹ�ö����ת�Ķ��ͼ��������ȼ�����ģ�Ͳ�����洢�ڴ洢����(mod)��
//������halcon�еģ�create_ncc_model (ImageReduced, 'auto', 0, 0, 'auto', 'use_polarity', ModelID) ����;
int zv_match_nccCreate(ZvModel mod, ZvImage templ, double angleStart, double angleExt, double angleStep, int numLevel)
{
    ZV_CHK_NULL3(templ, mod, mod->next);
    cv::Mat _templ = ZV_ADP_IMG_GETM(templ);

    if (angleExt < 0 || angleStep < 0 || numLevel < 0)    // angleStep > PI / 16
    {
        //����Ĳ�������
        return -1;
    }

    //Ϊÿ����תͼ����� numLevel ��С�����飬���ڴ�Ž������²��������㼶��ͼ��;
    for (int i = 0; i < mod->numRotateTempl; i++)
    {
        //mod->next[i].templImage = (cv::Mat *)malloc(sizeof(cv::Mat) * numLevel);
        mod->next[i].templImage = new _ZvImage[numLevel];
        if (mod->next[i].templImage == NULL)
        {
            return -1;
        }
    }

    ////�õ�������תͼ��Ľ�����ͼ��
    cv::Mat dst1;
    cv::Point2f center(_templ.cols / 2.f, _templ.rows / 2.f);
    for (int i = 0; i < mod->numRotateTempl; i++)
    {
        cv::Mat rot = cv::getRotationMatrix2D(center, angleStart + i * angleStep, 1);
        cv::warpAffine(_templ, mod->next[i].templImage[0], rot, _templ.size(), 0);

        mod->next[i].angleRotate = angleStart + i * angleStep;
        mod->next[i].numLevel = numLevel;
        //�õ�һ����ת��ͼ���ʼ����numLevelˮƽ���²���,�����²������ͼ�񱣴浽������ȥ
        for (int j = 0; j < numLevel - 1; j++)
        {
            cv::pyrDown(mod->next[i].templImage[j], mod->next[i].templImage[j + 1], cv::Size((mod->next[i].templImage[j].cols + 1) / 2, (mod->next[i].templImage[j].rows + 1) / 2));
        }
    }

    for (int i = 0; i < mod->numRotateTempl; i++)
    {
        for (int j = 0; j < numLevel - 1; j++)
        {
            cv::imshow("xxx", mod->next[i].templImage[j]);
            cv::waitKey(0);
        }
    }

    return ZV_ST_OK;
}

//��ȡģ���ͼƬ
ZvImage zv_match_nccTempl(ZvModel mod, double angleStart, double angleExt, double angleStep, int numLevel)
{
    //��ĳһ�����ģ��ͼ�񱣴浽�ļ���ȥ;
    //zv_match_nccWrite(ZvModel mod, char * name);
    //���ļ��ж�ȡ�ض������ģ��ͼ��;
    //zv_match_nccRead(ZvModel mod, char * name);

    //if (mod == NULL || numLevel < 0)
    //    return NULL;

    //int num = (angleExt - angleStart) / angleStep;
    //return &mod[num].templImage[numLevel];
    return NULL;
}


//��ȡ����ģ��Ĳ���
int zv_match_nccParams(ZvModel mod, double *angleStart, double *angleExt, double *angleStep, int *numLevel)
{
    //��ȡģ��Ĳ�����������Щ�������浽һ���ṹ�������ȥ;
    //node->img�� node->angleStart��....
    ZV_CHK_NULL1(mod);

    //*angleStart = mod->angleStart;
    //*angleExt = mod->angleExt;
    //*angleStep = mod->angleStep;
    //*numLevel = mod->numLevel;

    return ZV_ST_OK;
}

//int zv_match_nccWrite(ZvModel mod, char * name);
//int zv_match_nccRead(ZvModel mod, char * name);


//������halcon�еģ�find_ncc_model (Image, ModelID, 0, 0, 0.5, 1, 0.5, 'true', 0, Row, Column, Angle, Score) ����;
//ƥ��ԭʼͼ���ģ��ͼ���ҵ������Ƶĵ��λ��
int zv_match_nccFind(ZvModel mod, ZvImage img, ZvMatrix matchs, int NumLevel, double minScore, double *x, double *y, double minDist, int isSubPix)
{
    //ZV_CHK_NULL4(img, matchs, x, y);
    ////1. ��ԭͼ img ���н������²�����������ÿһ�������ļ��𱣴浽һ���ṹ����ȥ
    //ZvPYDOWNIMG pydownImg;
    //pydownImg.srcImg = (ZvImage)malloc(sizeof(_ZvImage) * NumLevel);
    //ZV_CHK_NULL1(pydownImg.srcImg);
    //ZV_ADP_IMG_GETM(img).copyTo(pydownImg.srcImg[0]);
    //pydownImg.numLevel = NumLevel;

    //for (int i = 0; i < NumLevel - 1; i++)   // 2 x 2 / 4 ���²�������
    //{
    //    cv::pyrDown(pydownImg.srcImg[i], pydownImg.srcImg[i + 1], cv::Size((pydownImg.srcImg[i].cols + 1) / 2, (pydownImg.srcImg[i].rows + 1) / 2));
    //}

    ////2. ��ʼ�ڽ�������ÿһ�� ƥ��ģ��ͼ�� �� ԭͼ��
    ////* �������ҵ������Ƶĵ��λ��(���ֵ�㣩
    ////������������ֵ�㣬���Ǽ����ÿ����ķ���ֵ�����ƵĹ�һ�����룩 > minScore �����Խ��ܣ�����������кܶ�ĵ㣻
    ////��ô��ĵ��ָ����ȷ����������һ��ԭͼ��ROI�����أ�
    //double matchX, matchY;
    //cv::Mat ROI;

    //double A_x1 = 0, A_y1 = 0;                             // (A_x1, A_y1) Ϊ��ʼ���꣬ ��A_x2, A_y2) Ϊ��ֹ����;
    //double A_y2 = pydownImg.srcImg[NumLevel - 1].rows;   // height
    //double A_x2 = pydownImg.srcImg[NumLevel - 1].cols;   // width
    //double nccInterScore;
    //int i;

    //// ���ͼ���ģ��ͼ������ظ��� >= 8 
    //// ����ѡ��ͼ�񲢲��ı�ͼ����Ĵ�С�����ֻ��ĳһ���Ƕȵ�ͼ���������µ�������������;
    //if ((A_y2 * A_x2 < 8) || (mod[0].templImage[NumLevel - 1].rows *  mod[0].templImage[NumLevel - 1].cols < 8))
    //{
    //    return -1;
    //}

    //// ���Եĳ�������
    //for (i = NumLevel - 1; i >= 0; i--)   // �Ӷ���ĵ����ڶ�����ʼ����
    //{
    //    // ������������������Խ�������
    //    if (A_x1 < 0)
    //    {
    //        A_x1 = 0;
    //    }
    //    if (A_y1 < 0)
    //    {
    //        A_y1 = 0;
    //    }
    //    if (A_x2 > pydownImg.srcImg[i].cols)
    //    {
    //        A_x2 = pydownImg.srcImg[i].cols;
    //    }
    //    if (A_y2 > pydownImg.srcImg[i].rows)
    //    {
    //        A_y2 = pydownImg.srcImg[i].rows;
    //    }

    //    ROI = pydownImg.srcImg[i](cv::Rect(A_x1, A_y1, A_x2 - A_x1, A_y2 - A_y1));

    //    // �ڽ�������ÿһ�� �ҵ����ƥ���
    //    nccInterScore = getNccInterScore(&ROI, &mod[0].templImage[i], &matchX, &matchY);

    //    if (nccInterScore < minScore)
    //    {
    //        // û���ҵ���ѵ�ƥ��λ��
    //        break;
    //        return -1;
    //    }

    //    // λ��ӳ�䣺���¼����ϼ���λ��ӳ��
    //    // ��Χ������������
    //    if (i >= 1)
    //    {
    //        A_x2 = (matchX + A_x1) * 2 + mod[0].templImage[i - 1].cols + FillArea;
    //        A_y2 = (matchY + A_y1) * 2 + mod[0].templImage[i - 1].rows + FillArea;
    //        A_x1 = (matchX + A_x1) - FillArea;
    //        A_y1 = (matchY + A_y1) - FillArea;
    //    }
    //}

    //*x = matchX + A_x1;
    //*y = matchY + A_y1;
    return ZV_ST_OK;

}

