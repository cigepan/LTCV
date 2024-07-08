#include "CImgProcBase.h"

//---------------------------------CImgProc_cvtColor--------------------------------------
CImgProc_cvtColor::CImgProc_cvtColor(const QJsonObject& args, QWidget *parent)
    : CImgProc(args, parent)
{
    auto grp = NewGrp<QHBoxLayout>();
    {
        auto code = NewArg<CProcArgs_QComboBox>("code", grp);
        code->Init(args.value("code").toString("COLOR_BGR2GRAY"), {{
                                                   {COLOR_BGR2GRAY,{"COLOR_BGR2GRAY","COLOR_BGR2GRAY"}},
                                                   {COLOR_RGB2GRAY,{"COLOR_RGB2GRAY","COLOR_RGB2GRAY"}},
                                                   {COLOR_BGR2RGB,{"COLOR_BGR2RGB","COLOR_BGR2RGB"}},
                                               }});
    }
}

SPMSG CImgProc_cvtColor::Process(Mat& src, Mat& dst)
{
    try{
        auto code = Arg("code")->CvType<ColorConversionCodes>();
        cvtColor(src, dst, code);
        return _ok;
    } catch (cv::Exception e) {
        return SPFAIL("CImgProc_cvtColor::Process\r\n"+QString::fromLocal8Bit(e.what()));
    } return _null;
}

//---------------------------------CImgProc_adaptiveThreshold--------------------------------------
CImgProc_adaptiveThreshold::CImgProc_adaptiveThreshold(const QJsonObject& args, QWidget *parent)
    : CImgProc(args, parent)
{
    auto grp = NewGrp<QVBoxLayout>();
    {
        auto maxValue = NewArg<CProcArgs_QDoubleSpinBox>("maxValue", grp);
        maxValue->Init(args.value("maxValue").toDouble(255.0));
        maxValue->UI<QDoubleSpinBox>()->setRange(0.0, 255.0);
    }
    {
        auto adaptiveMethod = NewArg<CProcArgs_QComboBox>("adaptiveMethod", grp);
        adaptiveMethod->Init(args.value("adaptiveMethod").toString("ADAPTIVE_THRESH_MEAN_C"), {{
                                                   {ADAPTIVE_THRESH_MEAN_C,{"ADAPTIVE_THRESH_MEAN_C","ADAPTIVE_THRESH_MEAN_C"}},
                                                   {ADAPTIVE_THRESH_GAUSSIAN_C,{"ADAPTIVE_THRESH_GAUSSIAN_C","ADAPTIVE_THRESH_GAUSSIAN_C"}},
                                               }});
    }
    {
        auto thresholdType = NewArg<CProcArgs_QComboBox>("thresholdType", grp);
        thresholdType->Init(args.value("thresholdType").toString("THRESH_BINARY"), {{
                                                   {THRESH_BINARY,{"THRESH_BINARY","THRESH_BINARY"}},
                                                   {THRESH_BINARY_INV,{"THRESH_BINARY_INV","THRESH_BINARY_INV"}},
                                               }});
    }
    {
        auto blockSize = NewArg<CProcArgs_QSpinBox>("blockSize", grp);
        blockSize->Init(args.value("blockSize").toInt(5));
        blockSize->UI<QSpinBox>()->setRange(3, 255);
    }
    {
        auto C = NewArg<CProcArgs_QDoubleSpinBox>("C", grp);
        C->Init(args.value("C").toDouble(0.0));
        C->UI<QDoubleSpinBox>()->setRange(0.0, 255.0);
    }
}

SPMSG CImgProc_adaptiveThreshold::Process(Mat& src, Mat& dst)
{
    try{
        auto maxValue = Arg("maxValue")->Get().toDouble();
        auto adaptiveMethod = Arg("adaptiveMethod")->CvType<AdaptiveThresholdTypes>();
        auto thresholdType = Arg("thresholdType")->CvType<ThresholdTypes>();
        auto blockSize = Arg("blockSize")->Get().toInt()|0x00000001;
        auto C = Arg("C")->Get().toDouble();
        adaptiveThreshold(src, dst, maxValue, adaptiveMethod, thresholdType, blockSize, C);
        return _ok;
    } catch (cv::Exception e) {
        return SPFAIL("CImgProc_adaptiveThreshold::Process\r\n"+QString::fromLocal8Bit(e.what()));
    } return _null;
}


