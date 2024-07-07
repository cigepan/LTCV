#pragma once
#include "CImgProc.h"

//---------------------------------CImgProc_GaussianBlur--------------------------------------
class CImgProc_GaussianBlur : public CImgProc
{
    Q_OBJECT
public:
    CImgProc_GaussianBlur(const QJsonObject& args={}, QWidget *parent = nullptr);
    static CImgProc* New(const QJsonObject& args) { return new CImgProc_GaussianBlur(args); }
    virtual const char* API() const { return "GaussianBlur"; }
    virtual const char* Title() const { return "高斯滤波"; }
    virtual const char* ToolTip() const { return "void GaussianBlur( InputArray src, OutputArray dst, Size ksize,\
 double sigmaX, double sigmaY = 0,\
 int borderType = BORDER_DEFAULT );"; }
    virtual const EmGroup Group() const { return EmGroup::FILTER; }

    virtual SPMSG Process(Mat& src, Mat& dst);
};

//---------------------------------CImgProc_medianBlur--------------------------------------
class CImgProc_medianBlur : public CImgProc
{
    Q_OBJECT
public:
    CImgProc_medianBlur(const QJsonObject& args={}, QWidget *parent = nullptr);
    static CImgProc* New(const QJsonObject& args) { return new CImgProc_medianBlur(args); }
    virtual const char* API() const { return "medianBlur"; }
    virtual const char* Title() const { return "中值滤波"; }
    virtual const char* ToolTip() const { return "void medianBlur( InputArray src, OutputArray dst, int ksize );"; }
    virtual const EmGroup Group() const { return EmGroup::FILTER; }

    virtual SPMSG Process(Mat& src, Mat& dst);
};
