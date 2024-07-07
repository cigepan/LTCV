#pragma once
#include "CImgProc.h"

//---------------------------------CImgProc_morphologyEx--------------------------------------
class CImgProc_morphologyEx : public CImgProc
{
    Q_OBJECT
public:
    CImgProc_morphologyEx(const QJsonObject& args={}, QWidget *parent = nullptr);
    static CImgProc* New(const QJsonObject& args) { return new CImgProc_morphologyEx(args); }
    virtual const char* API() const { return "morphologyEx"; }
    virtual const char* Title() const { return "形态学处理"; }
    virtual const char* ToolTip() const { return "void morphologyEx( InputArray src, OutputArray dst,\r\n\
 int op, InputArray kernel,\r\n\
 Point anchor = Point(-1,-1), int iterations = 1,\r\n\
 int borderType = BORDER_CONSTANT,\r\n\
 const Scalar& borderValue = morphologyDefaultBorderValue() );"; }
    virtual const EmGroup Group() const { return EmGroup::MPH; }

    virtual SPMSG Process(Mat& src, Mat& dst);
};
