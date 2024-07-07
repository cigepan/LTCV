#include "CImgProcCus.h"

//---------------------------------CImgProc_findContours--------------------------------------
CImgProc_findContours::CImgProc_findContours(const QJsonObject& args, QWidget *parent)
    : CImgProc(parent)
{
    {
        auto mode = new QComboBox;
        connect(mode, &QComboBox::currentIndexChanged,[&](){ sigUpdate((CImgProc*)this); });
        mode->addItems({"RETR_EXTERNAL","RETR_LIST","RETR_CCOMP","RETR_TREE","RETR_FLOODFILL"});
        AUI["mode"] = mode;
    }
    {
        auto method = new QComboBox;
        connect(method, &QComboBox::currentIndexChanged,[&](){ sigUpdate((CImgProc*)this); });
        method->addItems({"CHAIN_APPROX_NONE","CHAIN_APPROX_SIMPLE","CHAIN_APPROX_TC89_L1","CHAIN_APPROX_TC89_KCOS"});
        AUI["method"] = method;
    }
    {
        auto draw_clr_r = new QSpinBox;
        connect(draw_clr_r, &QSpinBox::valueChanged,[&](){ sigUpdate((CImgProc*)this); });
        draw_clr_r->setRange(0,255);
        AUI["draw_clr_r"] = draw_clr_r;
        auto draw_clr_g = new QSpinBox;
        connect(draw_clr_g, &QSpinBox::valueChanged,[&](){ sigUpdate((CImgProc*)this); });
        draw_clr_g->setRange(0,255);
        AUI["draw_clr_g"] = draw_clr_g;
        auto draw_clr_b = new QSpinBox;
        connect(draw_clr_b, &QSpinBox::valueChanged,[&](){ sigUpdate((CImgProc*)this); });
        draw_clr_b->setRange(0,255);
        AUI["draw_clr_b"] = draw_clr_b;
    }
    {
        auto draw_thickness = new QSpinBox;
        connect(draw_thickness, &QSpinBox::valueChanged,[&](){ sigUpdate((CImgProc*)this); });
        draw_thickness->setRange(-128,128);
        AUI["draw_thickness"] = draw_thickness;
    }
    {
        auto draw_lineType = new QComboBox;
        connect(draw_lineType, &QComboBox::currentIndexChanged,[&](){ sigUpdate((CImgProc*)this); });
        draw_lineType->addItems({"FILLED","LINE_4","LINE_8","LINE_AA"});
        AUI["draw_lineType"] = draw_lineType;
    }
    InitUI(args);
}
// QJsonObject CImgProc_findContours::GetArgs() const {
//     auto args = CImgProc::GetArgs();
//     args["mode"] = dynamic_cast<QComboBox*>(AUI.value("mode"))->currentText();
//     args["method"] = dynamic_cast<QComboBox*>(AUI.value("method"))->currentText();
//     args["draw_clr_r"] = dynamic_cast<QSpinBox*>(AUI.value("draw_clr_r"))->value();
//     args["draw_clr_g"] = dynamic_cast<QSpinBox*>(AUI.value("draw_clr_g"))->value();
//     args["draw_clr_b"] = dynamic_cast<QSpinBox*>(AUI.value("draw_clr_b"))->value();
//     args["draw_thickness"] = dynamic_cast<QSpinBox*>(AUI.value("draw_thickness"))->value();
//     args["draw_lineType"] = dynamic_cast<QComboBox*>(AUI.value("draw_lineType"))->currentText();
//     return args;
// }
// SPMSG CImgProc_findContours::SetArgs(const QJsonObject& args) {
//     dynamic_cast<QComboBox*>(AUI.value("mode"))
//             ->setCurrentText(args.value("mode").toString("RETR_EXTERNAL"));
//     dynamic_cast<QComboBox*>(AUI.value("method"))
//             ->setCurrentText(args.value("method").toString("CHAIN_APPROX_NONE"));
//     dynamic_cast<QSpinBox*>(AUI.value("draw_clr_r"))->setValue(args.value("draw_clr_r").toInt(126));
//     dynamic_cast<QSpinBox*>(AUI.value("draw_clr_g"))->setValue(args.value("draw_clr_g").toInt(126));
//     dynamic_cast<QSpinBox*>(AUI.value("draw_clr_b"))->setValue(args.value("draw_clr_b").toInt(126));
//     dynamic_cast<QSpinBox*>(AUI.value("apertureSize"))->setValue(args.value("apertureSize").toInt(3));
//     return CImgProc::SetArgs(args);
// }
SPMSG CImgProc_findContours::Process(Mat& src, Mat& dst)
{
    try{
        // QJsonObject args = GetArgs();
        // Scalar color(1.0,1.0,1.0);
        // int thickness = 1;
        // int lineType = LINE_8;
        // Mat find_ctr;
        // findContours(src, find_ctr, mode, method);
        // Mat tmp=Mat::zeros(src.size(), src.type());
        // for (auto ctr: find_ctr){
        //     drawContours(tmp, ctr, -1, color, thickness, lineType);
        // }
        return _ok;
    } catch (cv::Exception e) {
        return SPFAIL(e.what());
    } return _null;
}

// SPMSG CImgProc_findContours1::Process(Mat& src, Mat& dst)
// {
//     try{
//         const QJsonObject args = GetArgs();
//         auto mode = RETR_EXTERNAL;
//         auto method = CHAIN_APPROX_SIMPLE;
//         auto len_close = true;
//         auto area_min = 100;
//         auto len_min = 100;
//         auto circle_rio_min = 0.3;
//         vector<vector<Point>> find_ctr;
//         vector<vector<Point>> clear_ctr;
//         findContours(src, find_ctr, mode, method);
//         for (auto& ctr : find_ctr){
//             auto area = contourArea(ctr);
//             auto len = arcLength(ctr, len_close);
//             if (len_min>len || area_min>area
//                     || circle_rio_min>((4*CV_PI*area)/(len*len))){
//                 clear_ctr.push_back(ctr);
//             }
//         }
//         Mat mask=Mat::ones(src.size(), src.type());
//         drawContours(mask, clear_ctr, -1, 0, -1);
//         bitwise_and(src, src, dst, mask);
//         return _ok;
//     } catch (cv::Exception e) {
//         return SPFAIL(e.what());
//     } return _null;
// }
