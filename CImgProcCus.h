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

//---------------------------------CImgProc_findContours--------------------------------------
class CImgProc_findContours : public CImgProc_Canny
{
    Q_OBJECT
public:
    CImgProc_findContours(const QJsonObject& args={}, QWidget *parent = nullptr);
    static CImgProc* New(const QJsonObject& args) { return new CImgProc_findContours(args); }
    virtual const char* API() const { return "Canny>findContours"; }
    virtual const char* Title() const { return "轮廓查找"; }
    virtual const char* GrpTitle() const { return "边缘检测 轮廓查找"; }
    virtual const char* ToolTip() const { return "CV_EXPORTS void findContours( InputArray image, OutputArrayOfArrays contours,\
 int mode, int method, Point offset = Point());"; }
    virtual const EmGroup Group() const { return EmGroup::CUS; }

    virtual SPMSG Process(Mat& src, Mat& dst);
};
//---------------------------------CImgProc_findContours1--------------------------------------
class CImgProc_findContours1 : public CImgProc_findContours
{
    Q_OBJECT
public:
    CImgProc_findContours1(const QJsonObject& args={}, QWidget *parent = nullptr);
    static CImgProc* New(const QJsonObject& args) { return new CImgProc_findContours1(args); }
    virtual const char* API() const { return "Canny>findContours>filterCirAreaLen"; }
    virtual const char* Title() const { return "面积长度圆度过滤"; }
    virtual const char* GrpTitle() const { return "边缘检测 轮廓查找 面积长度圆度过滤"; }
    virtual const char* ToolTip() const { return "filterCirAreaLen(area_min, len_close, len_min, cir_rio_max)"; }

    virtual SPMSG Process(Mat& src, Mat& dst);
};
