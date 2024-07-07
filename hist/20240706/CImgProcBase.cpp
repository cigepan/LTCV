#include "CImgProcBase.h"

//---------------------------------CImgProc_cvtColor--------------------------------------
CImgProc_cvtColor::CImgProc_cvtColor(const QJsonObject& args, QWidget *parent)
    : CImgProc(parent)
{
    {
        auto code = new QComboBox;
        connect(code, &QComboBox::currentIndexChanged,[&](){ sigUpdate(this); });
        code->addItems({"COLOR_BGR2GRAY","COLOR_RGB2GRAY","COLOR_BGR2RGB"});
        AUI["code"] = code;
    }
    InitUI(args);
}
QJsonObject CImgProc_cvtColor::GetArgs() const {
    auto args = CImgProc::GetArgs();
    args["code"] = dynamic_cast<QComboBox*>(AUI.value("code"))->currentText();
    return args;
}
SPMSG CImgProc_cvtColor::SetArgs(const QJsonObject& args) {
    dynamic_cast<QComboBox*>(AUI.value("code"))
            ->setCurrentText(args.value("code").toString("COLOR_BGR2GRAY"));
    return CImgProc::SetArgs(args);
}
SPMSG CImgProc_cvtColor::Process(Mat& src, Mat& dsc)
{
    try{
        auto args = GetArgs();
        auto code = [&args](){
            auto v = args.value("code").toString();
            if ("COLOR_BGR2GRAY"==v){ return COLOR_BGR2GRAY; }
            if ("COLOR_RGB2GRAY"==v){ return COLOR_RGB2GRAY; }
            if ("COLOR_BGR2RGB"==v){ return COLOR_BGR2RGB; }
            return COLOR_BGR2GRAY;
        }();
        cvtColor(src, dsc, code);
        return _ok;
    } catch (std::exception e) {
        return SPFAIL(e.what());
    } return _null;
}

//---------------------------------CImgProc_adaptiveThreshold--------------------------------------
CImgProc_adaptiveThreshold::CImgProc_adaptiveThreshold(const QJsonObject& args, QWidget *parent)
    : CImgProc(parent)
{
    {
        auto maxValue = new QDoubleSpinBox;
        connect(maxValue, &QDoubleSpinBox::valueChanged,[&](){ sigUpdate((CImgProc*)this); });
        maxValue->setRange(0, 255);
        AUI["maxValue"] = maxValue;
    }
    {
        auto adaptiveMethod = new QComboBox;
        connect(adaptiveMethod, &QComboBox::currentIndexChanged,[&](){ sigUpdate((CImgProc*)this); });
        adaptiveMethod->addItems({"ADAPTIVE_THRESH_MEAN_C","ADAPTIVE_THRESH_GAUSSIAN_C"});
        AUI["adaptiveMethod"] = adaptiveMethod;
    }
    {
        auto thresholdType = new QComboBox;
        connect(thresholdType, &QComboBox::currentIndexChanged,[&](){ sigUpdate((CImgProc*)this); });
        thresholdType->addItems({"THRESH_BINARY","THRESH_TRUNC","THRESH_TOZERO","THRESH_MASK"});
        AUI["thresholdType"] = thresholdType;
    }
    {
        auto blockSize = new QSpinBox;
        connect(blockSize, &QSpinBox::valueChanged,[&](){ sigUpdate((CImgProc*)this); });
        blockSize->setRange(1,255);
        AUI["blockSize"] = blockSize;
    }
    {
        auto C = new QDoubleSpinBox;
        connect(C, &QDoubleSpinBox::valueChanged,[&](){ sigUpdate((CImgProc*)this); });
        C->setRange(0.0,255.0);
        AUI["C"] = C;
    }
    InitUI(args);
}
QJsonObject CImgProc_adaptiveThreshold::GetArgs() const {
    auto args = CImgProc::GetArgs();
    args["maxValue"] = dynamic_cast<QDoubleSpinBox*>(AUI.value("maxValue"))->value();
    args["adaptiveMethod"] = dynamic_cast<QComboBox*>(AUI.value("adaptiveMethod"))->currentText();
    args["thresholdType"] = dynamic_cast<QComboBox*>(AUI.value("thresholdType"))->currentText();
    args["blockSize"] = dynamic_cast<QSpinBox*>(AUI.value("blockSize"))->value()|0x00000001;
    args["C"] = dynamic_cast<QDoubleSpinBox*>(AUI.value("C"))->value();
    return args;
}
SPMSG CImgProc_adaptiveThreshold::SetArgs(const QJsonObject& args) {
    dynamic_cast<QDoubleSpinBox*>(AUI.value("maxValue"))
            ->setValue(args.value("maxValue").toDouble(255.0));
    dynamic_cast<QComboBox*>(AUI.value("adaptiveMethod"))
            ->setCurrentText(args.value("adaptiveMethod").toString("ADAPTIVE_THRESH_MEAN_C"));
    dynamic_cast<QComboBox*>(AUI.value("thresholdType"))
            ->setCurrentText(args.value("thresholdType").toString("THRESH_BINARY"));
    dynamic_cast<QSpinBox*>(AUI.value("blockSize"))->setValue(args.value("blockSize").toInt(15));
    dynamic_cast<QDoubleSpinBox*>(AUI.value("C"))->setValue(args.value("C").toDouble(0.0));
    return CImgProc::SetArgs(args);
}
SPMSG CImgProc_adaptiveThreshold::Process(Mat& src, Mat& dsc)
{
    try{
        auto args = GetArgs();
        auto maxValue = args.value("maxValue").toDouble();
        auto adaptiveMethod = [&args](){
            auto v = args.value("adaptiveMethod").toString();
            if ("ADAPTIVE_THRESH_MEAN_C"==v){ return ADAPTIVE_THRESH_MEAN_C; }
            if ("ADAPTIVE_THRESH_GAUSSIAN_C"==v){ return ADAPTIVE_THRESH_GAUSSIAN_C; }
            return ADAPTIVE_THRESH_MEAN_C;
        }();
        auto thresholdType = [&args](){
            auto v = args.value("thresholdType").toString();
            if ("THRESH_BINARY"==v){ return THRESH_BINARY; }
            if ("THRESH_TRUNC"==v){ return THRESH_TRUNC; }
            if ("THRESH_TOZERO"==v){ return THRESH_TOZERO; }
            if ("THRESH_MASK"==v){ return THRESH_MASK; }
            return THRESH_BINARY;
        }();
        auto blockSize = args.value("blockSize").toInt();
        auto C = args.value("C").toDouble();
        adaptiveThreshold(src, dsc, maxValue, adaptiveMethod, thresholdType, blockSize, C);
        return _ok;
    } catch (std::exception e) {
        return SPFAIL(e.what());
    } return _null;
}


