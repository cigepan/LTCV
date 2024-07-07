#pragma once
#include "CImgProc.h"

//---------------------------------CImgProc_Canny--------------------------------------
class CImgProc_Canny : public CImgProc
{
    Q_OBJECT
public:
    CImgProc_Canny(const QJsonObject& args={}, QWidget *parent = nullptr);
    static CImgProc* New(const QJsonObject& args) { return new CImgProc_Canny(args); }
    virtual const char* API() const { return "Canny"; }
    virtual const char* Title() const { return "Canny边缘检测"; }
    virtual const char* ToolTip() const { return "CV_EXPORTS_W void Canny( InputArray image, OutputArray edges,\
 double threshold1, double threshold2,\
 int apertureSize = 3, bool L2gradient = false);"; }
    virtual const EmGroup Group() const { return EmGroup::ADV; }

    virtual SPMSG Process(Mat& src, Mat& dst);
};
