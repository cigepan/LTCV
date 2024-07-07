#include "CImgProcAdv.h"

//---------------------------------CImgProc_Canny--------------------------------------
CImgProc_Canny::CImgProc_Canny(const QJsonObject& args, QWidget *parent)
    : CImgProc(parent)
{
    {
        auto threshold1 = new CProcArgs_QDoubleSpinBox(this);
        Args["threshold1"] = threshold1;
        threshold1->Init(args.value("threshold1").toDouble(100.0));
        threshold1->UI<QDoubleSpinBox>()->setRange(0.0, 255.0);
    }
    {
        auto threshold2 = new CProcArgs_QDoubleSpinBox(this);
        Args["threshold2"] = threshold2;
        threshold2->Init(args.value("threshold2").toDouble(200.0));
        threshold2->UI<QDoubleSpinBox>()->setRange(0.0, 255.0);
    }
    {
        auto apertureSize = new CProcArgs_QSpinBox(this);
        Args["apertureSize"] = apertureSize;
        apertureSize->Init(args.value("apertureSize").toInt(3));
        apertureSize->UI<QSpinBox>()->setRange(3, 7);
    }
    InitUI(args);
}

SPMSG CImgProc_Canny::Process(Mat& src, Mat& dst)
{
    try{
        auto threshold1 = Arg("threshold1")->Get().toDouble();
        auto threshold2 = Arg("threshold2")->Get().toDouble();
        auto apertureSize = Arg("apertureSize")->Get().toInt()|0x00000001;
        Canny(src, dst, threshold1, threshold2, apertureSize);
        return _ok;
    } catch (cv::Exception e) {
        return SPFAIL("CImgProc_Canny::Process\r\n"+QString::fromLocal8Bit(e.what()));
    } return _null;
}
