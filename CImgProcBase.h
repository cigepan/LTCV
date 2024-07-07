#pragma once
#include "CImgProc.h"

//---------------------------------CImgProc_cvtColor--------------------------------------
class CImgProc_cvtColor : public CImgProc
{
    Q_OBJECT
public:
    CImgProc_cvtColor(const QJsonObject& args={}, QWidget *parent = nullptr);
    static CImgProc* New(const QJsonObject& args) { return new CImgProc_cvtColor(args); }
    virtual const char* API() const { return "cvtColor"; }
    virtual const char* Title() const { return "图像转换"; }
    virtual const char* ToolTip() const { return "void cvtColor( InputArray src, OutputArray dst, int code, int dstCn = 0 );"; }
    virtual const EmGroup Group() const { return EmGroup::BASE; }

    virtual SPMSG Process(Mat& src, Mat& dst);
};

//---------------------------------CImgProc_adaptiveThreshold--------------------------------------
class CImgProc_adaptiveThreshold : public CImgProc
{
    Q_OBJECT
public:
    CImgProc_adaptiveThreshold(const QJsonObject& args={}, QWidget *parent = nullptr);
    static CImgProc* New(const QJsonObject& args) { return new CImgProc_adaptiveThreshold(args); }
    virtual const char* API() const { return "adaptiveThreshold"; }
    virtual const char* Title() const { return "阈值分割"; }
    virtual const char* ToolTip() const { return "adaptiveThreshold( InputArray src, OutputArray dst,double maxValue, int adaptiveMethod,int thresholdType, int blockSize, double C );"; }
    virtual const EmGroup Group() const { return EmGroup::BASE; }
    virtual SPMSG Process(Mat& src, Mat& dst);
};


