#ifndef ZV_MATCH_H
#define ZV_MATCH_H


#ifdef __cplusplus
extern "C"{
#endif
    typedef struct _PYRDOWNIMAGE
    {
        ZvImage pyrImg;
    }PYRDOWNIMAGE;

    typedef struct _PYPDOWNTEMPL
    {
        ZvImage pyrTempl;
    }PYPDOWNTEMPL;

    typedef struct _ZvPYDOWNIMG
    {
        ZvImage srcImg;
        int numLevel;
    }ZvPYDOWNIMG;

    // ��תģ��, ʹ����ͼ���ϵ��ӿ����
    inline int RotateTempl(ZvImage templ, double *AngleStart, double *AngleExtent, double *AngleStep)
    {
        return ZV_ST_OK;
    }

    // ncc �����ϵ��
    inline double getNccInterScore(ZvImage img, ZvImage templ, double *matchX, double *matchY)
    {
        /*--------------------------�Լ�д��NCC ����--------------------------------------*/
        /*
        ZV_CHK_NULL2(img, templ);
        cv::Mat _img = ZV_ADP_IMG_GETM(img);
        cv::Mat _templ = ZV_ADP_IMG_GETM(templ);
        cv::Mat childImg;
        double T_h = _templ.rows, T_w = _templ.cols;
        double matchValue, maxValue = 0;
        double M_T = cv::mean(_templ)[0];

        for (int i = A_x; i < A_h; i++)   // 
        {
            for (int j = A_y; j < A_w; j++)   // ÿ���ƶ��ľ���Ϊ��һ������
            {
                // 1. �õ�ͼ���ϵ�һ�� ��templͬ�ȴ�С��ͼ���ӿ��
                childImg = _img(cv::Rect(j, i, T_w, T_h));  

                // 2. ��������Է���
                double M_C = cv::mean(childImg)[0];
                double interRelateValue = 0.0, C_energy = 0.0, T_energy = 0.0;

                const uchar *MT = NULL;
                const uchar *MC = NULL;
                for (int k = 0; k < _templ.rows; k++)
                {
                    MT = _templ.ptr<uchar>(k);
                    MC = childImg.ptr<uchar>(k);
                    for (int m = 0; m < _templ.cols; m++)
                    {
                        interRelateValue += ((double)MT[m] - M_T) * ((double)MC[m] - M_C);
                        C_energy += ((double)MC[m] - M_C) * ((double)MC[m] - M_C);
                        T_energy += ((double)MT[m] - M_T) * ((double)MT[m] - M_T);
                    }
                }

                matchValue = (1.0 / (T_w * T_h)) * (interRelateValue / (sqrt(C_energy)*sqrt(T_energy)));

                // ��ncc �ĸ���ֵ��[-1, 1] ӳ�䵽 [0, 1]֮��
                matchValue = (matchValue + 1) / 2;
                if (matchValue > maxValue)
                {
                    maxValue = matchValue;
                    *matchX = i;
                    *matchY = j;
                }
            }
        }
        return maxValue;
        */

        // ����Opencv �� NCC ������ʵ��
        ZV_CHK_NULL4(img, templ, matchX, matchX);
        cv::Mat result;

        // img Ϊһ��ROI������
        cv::matchTemplate(ZV_ADP_IMG_GETM(img), ZV_ADP_IMG_GETM(templ), result, cv::TM_CCORR_NORMED);
 
        double minVal; double maxVal;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
        
        *matchX = maxLoc.x;
        *matchY = maxLoc.y;

        return maxVal;
    }

    // ������ƥ����
    inline int zv_pyrDown(ZvImage img, ZvImage templ, int level, PYRDOWNIMAGE &pyrDownImage, PYPDOWNTEMPL pyrDownTempl)
    {
        ZV_CHK_NULL2(img, templ);

        ZV_ADP_IMG_GETM(img).copyTo(pyrDownImage.pyrImg[0]);
        ZV_ADP_IMG_GETM(templ).copyTo(pyrDownTempl.pyrTempl[0]);

        for (int i = 0; i < level - 1; i++)   // 2 x 2 / 4 ���²�������
        {
            cv::pyrDown(pyrDownImage.pyrImg[i], pyrDownImage.pyrImg[i+1], cv::Size((pyrDownImage.pyrImg[i].cols + 1) / 2, (pyrDownImage.pyrImg[i].rows + 1) / 2));  
        }

        return ZV_ST_OK;
    }

    int zv_match_fastTempl(ZvImage img, ZvImage templ, int *x, int *y, ZvImage mask);
    int zv_match_bestTempl(ZvImage img, ZvImage templ, double minScore, double *x, double *y, double *score, int method, bool isSubPix);
    int zv_match_multiTempl(ZvImage img, ZvImage templ, ZvMatrix matches, double minScore, int method, bool isSubPix);
    int zv_match_grayScale(ZvImage img, ZvImage templ, int NumLevel, double AngleStart, double AngleExtent, double AngleStep, double *x, double *y, double MinScore = 0.8);
   
    int zv_match_nccCreate(ZvModel mod, ZvImage templ, double angleStart, double angleExt, double angleStep, int numLevel);
    ZvImage zv_match_nccTempl(ZvModel mod, double angleStart, double angleExt, double angleStep, int numLevel);
    int zv_match_nccParams(ZvModel mod, double *angleStart, double *angleExt, double *angleStep, int *numLevel);
    int zv_match_nccFind(ZvModel mod, ZvImage img, ZvMatrix matchs, int NumLevel, double minScore, double *x, double *y, double minDist, int isSubPix);

#ifdef __cplusplus
}
#endif

#endif