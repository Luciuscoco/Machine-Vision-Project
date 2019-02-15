#ifndef ZV_MATCH_H
#define ZV_MATCH_H


#ifdef __cplusplus
extern "C" {
#endif

    int zv_match_fastTempl(ZvImage img, ZvImage templ, int *x, int *y, int method);
    int zv_match_bestTempl(ZvImage img, ZvImage templ, double minScore, double *x,
        double *y, double *score, bool isSubPix, int polar);
    int zv_match_multiTempl(ZvImage img, ZvImage templ, ZvMatrix matches,
        double minScore, int nums, double minDist, bool isSubPix, int polar);

    int zv_match_nccCreate(ZvModel mod, ZvImage templ, double angleStart, double angleExt, double angleStep, int numLevel);

    int zv_match_nccFind(ZvModel mod, ZvImage img, ZvMatrix matchs, int NumLevel,
        double minScore, double *x, double *y, double minDist, int isSubPix);
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

    //��ȡ�� n ����תͼ��� ��������level���ϵ�ģ��ͼ��
    inline ZvImage zv_match_nccTempl(ZvModel mod, int n, int level)
    {
        if (mod == NULL || n < 0 || n >= mod->numRotateTempl || level < 0 || level >= mod->numLevel)
        {
            return NULL;
        }

        //��ĳһ�����ģ��ͼ�񱣴浽�ļ���ȥ;
        //zv_match_nccWrite(ZvModel mod, char * name);
        //���ļ��ж�ȡ�ض������ģ��ͼ��;
        //zv_match_nccRead(ZvModel mod, char * name);

        return &mod->Zvmod[n][level];
    }

    //��ò���
    inline int zv_match_nccParams(ZvModel mod, double *angleStart, double *angleExt, double *angleStep, int *numLevel, int *polar)
    {
        ZV_CHK_NULL4(mod, angleStart, angleExt, angleStep, numLevel, polar);
        *angleStart = mod->angleStart;
        *angleExt = mod->angleExt;
        *angleStep = mod->angleStep;
        *numLevel = mod->numLevel;
        return ZV_ST_OK;
    }
    
    //int zv_match_nccCreate(ZvModel mod, ZvImage templ, double angleStart, double angleExt, double angleStep = -1, int numLevel = -1, int polar = 0);
    //int zv_match_nccWrite(ZvModel mod, char * name);
    //int zv_match_nccRead(ZvModel mod, char * name);
    //int zv_match_nccFind(ZvModel mod, ZvImage img, ZvMatrix matchs, double minScore, int nums, double minDist, int isSubPix);

#ifdef __cplusplus
}
#endif

#endif