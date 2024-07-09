#include "CImgProcFilter.h"

//---------------------------------CImgProc_GaussianBlur--------------------------------------
CImgProc_GaussianBlur::CImgProc_GaussianBlur(const QJsonObject& args, QWidget *parent)
    : CImgProc(args, parent)
{
    auto grp = NewGrp<QHBoxLayout>();
    {
        auto ksize = NewArg<CProcArgs_QSpinBox>("ksize", grp);
        ksize->Init(args.value("ksize").toInt(5));
        ksize->UI<QSpinBox>()->setRange(1, 255);
    }
    {
        auto sigmaX = NewArg<CProcArgs_QDoubleSpinBox>("sigmaX", grp);
        sigmaX->Init(args.value("sigmaX").toDouble(0.0));
        sigmaX->UI<QDoubleSpinBox>()->setRange(0.0, 255.0);
    }
    {
        auto sigmaY = NewArg<CProcArgs_QDoubleSpinBox>("sigmaY", grp);
        sigmaY->Init(args.value("sigmaY").toDouble(0.0));
        sigmaY->UI<QDoubleSpinBox>()->setRange(0.0, 255.0);
    }
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
    : CImgProc(args, parent)
{
    auto grp = NewGrp<QHBoxLayout>();
    {
        auto ksize = NewArg<CProcArgs_QSpinBox>("ksize", grp);
        ksize->Init(args.value("ksize").toInt(5));
        ksize->UI<QSpinBox>()->setRange(1, 255);
    }
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
