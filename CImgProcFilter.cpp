#include "CImgProcFilter.h"

//---------------------------------CImgProc_GaussianBlur--------------------------------------
CImgProc_GaussianBlur::CImgProc_GaussianBlur(const QJsonObject& args, QWidget *parent)
    : CImgProc(parent)
{
    {
        auto ksize = new CProcArgs_QSpinBox(this);
        Args["ksize"] = ksize;
        ksize->Init(args.value("ksize").toInt(5));
        ksize->UI<QSpinBox>()->setRange(1, 255);
    }
    {
        auto sigmaX = new CProcArgs_QDoubleSpinBox(this);
        Args["sigmaX"] = sigmaX;
        sigmaX->Init(args.value("sigmaX").toDouble(0.0));
        sigmaX->UI<QDoubleSpinBox>()->setRange(0.0, 255.0);
    }
    {
        auto sigmaY = new CProcArgs_QDoubleSpinBox(this);
        Args["sigmaY"] = sigmaY;
        sigmaY->Init(args.value("sigmaY").toDouble(0.0));
        sigmaY->UI<QDoubleSpinBox>()->setRange(0.0, 255.0);
    }
    InitUI(args);
}

SPMSG CImgProc_GaussianBlur::Process(Mat& src, Mat& dst)
{
    try{
        auto ksize = Arg("ksize")->Get().toInt()|0x00000001;
        auto sigmaX = Arg("sigmaX")->Get().toDouble();
        auto sigmaY = Arg("sigmaY")->Get().toDouble();
        GaussianBlur(src, dst, Size(ksize,ksize), sigmaX, sigmaY);
        return _ok;
    } catch (cv::Exception e) {
        return SPFAIL("CImgProc_GaussianBlur::Process\r\n"+QString::fromLocal8Bit(e.what()));
    } return _null;
}

//---------------------------------CImgProc_medianBlur--------------------------------------
CImgProc_medianBlur::CImgProc_medianBlur(const QJsonObject& args, QWidget *parent)
    : CImgProc(parent)
{
    {
        auto ksize = new CProcArgs_QSpinBox(this);
        Args["ksize"] = ksize;
        ksize->Init(args.value("ksize").toInt(5));
        ksize->UI<QSpinBox>()->setRange(1, 255);
    }
    InitUI(args);
}

SPMSG CImgProc_medianBlur::Process(Mat& src, Mat& dst)
{
    try{
        auto ksize = Arg("ksize")->Get().toInt()|0x00000001;
        medianBlur(src, dst, ksize);
        return _ok;
    } catch (cv::Exception e) {
        return SPFAIL("CImgProc_medianBlur::Process\r\n"+QString::fromLocal8Bit(e.what()));
    } return _null;
}
