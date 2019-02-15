#include "zv_private.h"
#include "zv_match.h"
#include "tools/zv_tools.h"

#define FillArea 10
#define ZV_MATCH_RST_TO_SCORE(rst)  ((rst) * 100)  //[-1,1]-->[-100, 100]
#define ZV_MATCH_SCORE_TO_RST(score)  ((score) * 0.01)

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

int zv_match_fastTempl(ZvImage img, ZvImage templ, int *x, int *y, int method)
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

int zv_match_multiTempl(ZvImage img, ZvImage templ, ZvMatrix matches,
    double minScore, int nums, double minDist, bool isSubPix, int polar)
{
    ZV_CHK_NULL3(img, templ, matches);
    if (minScore > 100 || minScore <= 0)
    {
        return ZV_ERR_FA_RANGE_4;
    }
    if (nums < 1)   //ƥ������
    {
        nums = -1;
    }
    if (1 != zv_img_channels(img) || 1 != zv_img_channels(templ))
    {
        return ZV_ERR_I_ONLY_CN1;
    }

    cv::Mat result;
    std::vector<cv::Point3d> vecRst;
    cv::matchTemplate(ZV_ADP_IMG_GETM(img), ZV_ADP_IMG_GETM(templ), result, CV_TM_CCOEFF_NORMED);

    int templWidth = zv_img_width(templ);
    int templHeight = zv_img_height(templ);
    float minRst = (float)ZV_MATCH_SCORE_TO_RST(minScore);
    for (int i = 0; i < result.rows; i++)
    {
        float * data = (float *)(result.data + i * result.step);
        for (int j = 0; j < result.cols; j++)
        {
            float cur = 0 == polar ? data[j] : fabs(data[j]);
            float prev = 0 == j ? 0 : 0 == polar ? data[j-1] : fabs(data[j-1]);
            float next = result.cols-1 == j ? 0 : 0 == polar ? data[j+1] : fabs(data[j+1]);

            if (minRst <= cur && cur > prev && cur > next)
            {
                cv::Point3d ptRst(j, i, ZV_MATCH_RST_TO_SCORE(cur));
                int size = (int)vecRst.size();
                if (0 == size || minDist <= zv_tk_distancePP(j, i, vecRst[size - 1].x, vecRst[size - 1].y)) //�Ϸ���׷�ӣ�����ȡ���ֵ
                {
                    vecRst.push_back(ptRst);
                }
                else
                {
                    if (ptRst.z > vecRst[size - 1].z)
                    {
                        vecRst[size - 1] = ptRst;
                    }
                }
            }
        }
    }

    //ȥ�ء�����
    int size = (int)vecRst.size();
    for (int i = 0; i < size;)
    {
        int minId = i;
        double minVal = vecRst[i].z;
        for (int j = i + 1; j < size; j++)  //�ҳ����ֵ����
        {
            if (minVal < vecRst[j].z)
            {
                minVal = vecRst[j].z;
                minId = j;
            }
        }

        bool isErase = false;
        for (int k = 0; k < i; k++)
        {
            double dist = zv_tk_distancePP(vecRst[k].x, vecRst[k].y, vecRst[minId].x, vecRst[minId].y);
            if (dist < minDist)  //�����е���벻����
            {
                isErase = true;
                vecRst.erase(vecRst.begin() + minId);
                size--;
                break;
            }
        }

        if (!isErase)
        {
            if (minId != i)
            {
                cv::Point3d tmp = vecRst[i];
                vecRst[i] = vecRst[minId];
                vecRst[minId] = tmp;
            }
            i++;
        }
    }

    //��������
    if (vecRst.size() > (uint)nums)
    {
        vecRst.resize((uint)nums);
    }

    for (int k = 0; k < (int)vecRst.size(); k++)
    {
        cv::Point3d &ptRst = vecRst[k];
        if (isSubPix)
        {
            int i = cvRound(ptRst.y);
            int j = cvRound(ptRst.x);
            if (0 < j && result.cols - 1 > j
                && 0 < i && result.rows - 1 > i
                && ZV_ST_OK == zv_match_subPix2d(result.ptr<float>(i - 1, j - 1), result.ptr<float>(i, j - 1),
                result.ptr<float>(i + 1, j - 1), ptRst.x, ptRst.y, ptRst.z))
            {
                ptRst.x += j;
                ptRst.y += i;
                ptRst.z = ZV_MATCH_RST_TO_SCORE(abs(ptRst.z));
                if (ptRst.z > 100.0)
                {
                    ptRst.z = 100.0;
                }
            }
            ptRst.x += templWidth * 0.5 - 0.5;
            ptRst.y += templHeight * 0.5 - 0.5;
        }
        else
        {
            ptRst.x += templWidth >> 1; // tW/2
            ptRst.y += templHeight >> 1;
        }
    }

    cv::Mat matRst(vecRst, true);
    ZV_ADP_MAT_SETM(matches, matRst.reshape(1));
 
    return ZV_ST_OK;
}

// ��ÿ������level)��ʹ�ö����ת�Ķ��ͼ��������ȼ�����ģ�Ͳ�����洢�ڴ洢����(mod)��
//������halcon�еģ�create_ncc_model (ImageReduced, 'auto', 0, 0, 'auto', 'use_polarity', ModelID) ����;
int zv_match_nccCreate(ZvModel mod, ZvImage templ, double angleStart, double angleExt, double angleStep, int numLevel)
{
    ZV_CHK_NULL2(mod, templ);
    cv::Mat _templ = ZV_ADP_IMG_GETM(templ);

    //�������
    if (angleStep < 0 || numLevel < 0)    // angleStep > PI / 16
    {
        //����Ĳ�������
        return -1;
    }
  
    int numRotateTempl;
    if (angleStep == 0) 
        numRotateTempl = (int)(abs(angleExt - angleStart) / 0.11111) + 1;
    else
        numRotateTempl = (int)(abs(angleExt - angleStart) / angleStep) + 1;
    
    mod->angleStart = angleStart;
    mod->angleExt = angleExt;
    mod->angleStep = angleStep;
    mod->numRotateTempl = numRotateTempl;
    mod->numLevel = numLevel;
    //���� numRotateTempl ��vector<cv::Mat> ����, ÿ�����������洢������תͼ��Ľ������²���ͼ��;
    if (!mod->createZvModel(numRotateTempl))
    {
        return -1;
    }

    //ע�⣺[]ֻ�ܷ���vector����ֵ������������ͨ��[]�������еı�����ֵ;
    //�õ�������תͼ��Ľ�����ͼ��
    cv::Point2f center(_templ.cols / 2.f, _templ.rows / 2.f);
    
    cv::Mat temp1, temp2, rot;
    cv::Rect bbox;

    for (int i = 0; i < numRotateTempl; i++)
    {
        rot = cv::getRotationMatrix2D(center, angleStart + i * angleStep, 1);
        bbox = cv::RotatedRect(center, _templ.size(), angleStart + i * angleStep).boundingRect();

        rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
        rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

        cv::warpAffine(_templ, temp1, rot, bbox.size());     //��ת�õ���ͼ�����нض�����;

        mod->Zvmod[i].push_back(temp1);

        for (int j = 0; j < numLevel; j++)
        { 
            cv::pyrDown(mod->Zvmod[i][j], temp2, cv::Size((mod->Zvmod[i][j].cols + 1) / 2, (mod->Zvmod[i][j].rows + 1) / 2));
            mod->Zvmod[i].push_back(temp2);
        }
    }

    return ZV_ST_OK;
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
int zv_match_nccFind(ZvModel mod, ZvImage img, ZvMatrix matchs, int NumLevel, 
    double minScore, double *x, double *y, double minDist, int isSubPix)
{
    ZV_CHK_NULL4(img, matchs, x, y);
    //1. ��ԭͼ img ���н������²�����������ÿһ�������ļ��𱣴浽vector��ȥ
    cv::Mat _img = ZV_ADP_IMG_GETM(img);
    std::vector<cv::Mat> pyrDownImg;
    pyrDownImg.push_back(_img);
    for (int i = 0; i < NumLevel; i++)   // 2 x 2 / 4 ���²�������
    {
        cv::pyrDown(pyrDownImg[i], _img, cv::Size((pyrDownImg[i].cols + 1) / 2, (pyrDownImg[i].rows + 1) / 2));
        pyrDownImg.push_back(_img);
    }

    ////2. ��ʼ�ڽ�������ÿһ�� ƥ��ģ��ͼ�� �� ԭͼ��
    ////* �������ҵ������Ƶĵ��λ��(���ֵ�㣩
    ////������������ֵ�㣬���Ǽ����ÿ����ķ���ֵ�����ƵĹ�һ�����룩 > minScore �����Խ��ܣ�����������кܶ�ĵ㣻
    ////��ô��ĵ��ָ����ȷ����������һ��ԭͼ��ROI�����أ�
    //for (int i = 0; i < mod->numRotateTempl; i++)   //numRotateTempl����תͼ��;
    //{
    //    for (int j = 0; j < NumLevel; j++)    //ÿ����תͼ����� NumLevel���²���ͼ��;
    //    {
    //        //��Ӧ�㼶�� ԭͼ ��-�� ģ��ͼ����в������Ƶĵ��λ��;
    //        //��1�����ǽ�������߲� ԭͼ �� ģ��ͼ�� �ҵ�һ�������Ƶĵ㣨�������ֵ�㣩�����;

    //    }
    //}

    double matchX, matchY;
    cv::Mat ROI;

    double A_x1 = 0, A_y1 = 0;                             // (A_x1, A_y1) Ϊ��ʼ���꣬ ��A_x2, A_y2) Ϊ��ֹ����;
    double A_y2 = pyrDownImg[NumLevel - 1].rows;   // height
    double A_x2 = pyrDownImg[NumLevel - 1].cols;   // width
    double nccInterScore;
    double maxNccInterScore = 0.0;
    double jiaodu = 0.0;
    int i;

    // ���ͼ���ģ��ͼ������ظ��� >= 8 
    // ����ѡ��ͼ�񲢲��ı�ͼ����Ĵ�С�����ֻ��ĳһ���Ƕȵ�ͼ���������µ�������������;
    for (int i = 0; i < mod->numRotateTempl; i++)   //numRotateTempl����תͼ��;
    {
        if((ZV_ADP_IMG_GETM(zv_match_nccTempl(mod, i, NumLevel - 1)).rows * ZV_ADP_IMG_GETM(zv_match_nccTempl(mod, i, NumLevel - 1)).cols < 8) || (A_y2 * A_x2 < 8))
        {
            return -1;
        }

        // ���Եĳ�������
        for (int j = NumLevel - 1; j >= 0; j--)   // �Ӷ���ĵ����ڶ�����ʼ����
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
            if (A_x2 > pyrDownImg[j].cols)
            {
                A_x2 = pyrDownImg[j].cols;
            }
            if (A_y2 > pyrDownImg[j].rows)
            {
                A_y2 = pyrDownImg[j].rows;
            }

            //�õ����������򣬼���ԭͼ���ROI����
            ROI = pyrDownImg[j](cv::Rect(A_x1, A_y1, A_x2 - A_x1, A_y2 - A_y1));

            /*cv::imshow("Templ", ZV_ADP_IMG_GETM(zv_match_nccTempl(mod, 60, j)));
            cv::imshow("Roi", ROI);
            cv::waitKey(0);*/

            if (i == 179)
            {
                cv::imshow("x2", ZV_ADP_IMG_GETM(zv_match_nccTempl(mod, i, j)));
                cv::waitKey(0);
                cout << "hellp" << endl;
            }

            // �ڽ�������ÿһ�� �ҵ����ƥ��� 
            nccInterScore = getNccInterScore(&ROI, zv_match_nccTempl(mod, i, j), &matchX, &matchY);

            if (nccInterScore < minScore)
            {
                // û���ҵ���ѵ�ƥ��λ��
                break;
                return -1;
            }

            // λ��ӳ�䣺���¼����ϼ���λ��ӳ��
            // ��Χ������������
            if (j >= 1)
            {
                A_x2 = (matchX + A_x1) * 2 + ZV_ADP_IMG_GETM(zv_match_nccTempl(mod, i, j - 1)).cols + FillArea;
                A_y2 = (matchY + A_y1) * 2 + ZV_ADP_IMG_GETM(zv_match_nccTempl(mod, i, j - 1)).rows + FillArea;
                A_x1 = (matchX + A_x1) - FillArea;
                A_y1 = (matchY + A_y1) - FillArea;
            }
        }
     
        if (maxNccInterScore < nccInterScore)
        {
            maxNccInterScore = nccInterScore;
            jiaodu = mod->angleStart + i * mod->angleStep;
            *x = matchX + A_x1;
            *y = matchY + A_y1;
        }
    }

    cout << "maxNccInterScore = " << maxNccInterScore << endl;
    cout << "jiaodu = " << jiaodu << endl;
    cout << "*x = " << *x << endl;
    cout << "*y = " << *y << endl;

    return ZV_ST_OK;
}

