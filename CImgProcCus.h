#pragma once
#include "CImgProc.h"

//---------------------------------CImgProc_findContours--------------------------------------
class CImgProc_findContours : public CImgProc
{
    Q_OBJECT
public:
    static CImgProc* New(const QJsonObject& args) { return new CImgProc_findContours(args); }
    virtual const char* API() const { return "findContours"; }
    virtual const char* Title() const { return "边界查找"; }
    virtual const char* ToolTip() const { return "CV_EXPORTS void findContours( InputArray image, OutputArrayOfArrays contours,\
 int mode, int method, Point offset = Point());"; }
    virtual const EmGroup Group() const { return EmGroup::CUS; }
    CImgProc_findContours(const QJsonObject& args={}, QWidget *parent = nullptr);
    // virtual QJsonObject GetArgs() const ;
    // virtual SPMSG SetArgs(const QJsonObject& args);
    virtual SPMSG Process(Mat& src, Mat& dst);
};
