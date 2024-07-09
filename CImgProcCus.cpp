#include "CImgProcCus.h"

//---------------------------------CImgProc_Canny--------------------------------------
CImgProc_Canny::CImgProc_Canny(const QJsonObject& args, QWidget *parent)
    : CImgProc(args, parent)
{
    auto grp = NewGrp<QHBoxLayout>();
    {
        auto threshold1 = NewArg<CProcArgs_QDoubleSpinBox>("threshold1", grp);
        threshold1->Init(args.value("threshold1").toDouble(100.0));
        threshold1->UI<QDoubleSpinBox>()->setRange(0.0, 255.0);
    }
    {
        auto threshold2 = NewArg<CProcArgs_QDoubleSpinBox>("threshold2", grp);
        threshold2->Init(args.value("threshold2").toDouble(200.0));
        threshold2->UI<QDoubleSpinBox>()->setRange(0.0, 255.0);
    }
    {
        auto apertureSize = NewArg<CProcArgs_QSpinBox>("apertureSize", grp);
        apertureSize->Init(args.value("apertureSize").toInt(3));
        apertureSize->UI<QSpinBox>()->setRange(3, 7);
    }
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

//---------------------------------CImgProc_findContours--------------------------------------
CImgProc_findContours::CImgProc_findContours(const QJsonObject& args, QWidget *parent)
    : CImgProc_Canny(args, parent)
{
    auto grp = NewGrp<QVBoxLayout>();
    {
        auto mode = NewArg<CProcArgs_QComboBox>("mode",grp);
        mode->Init(args.value("mode").toString("RETR_EXTERNAL"), {{
                                                   {RETR_EXTERNAL,{"RETR_EXTERNAL","RETR_EXTERNAL"}},
                                                   {RETR_LIST,{"RETR_LIST","RETR_LIST"}},
                                                   {RETR_CCOMP,{"RETR_CCOMP","RETR_CCOMP"}},
                                                   {RETR_TREE,{"RETR_TREE","RETR_TREE"}},
                                                   {RETR_FLOODFILL,{"RETR_FLOODFILL","RETR_FLOODFILL"}},
                                               }});
    }
    {
        auto method = NewArg<CProcArgs_QComboBox>("method",grp);
        method->Init(args.value("method").toString("CHAIN_APPROX_NONE"), {{
                                                   {CHAIN_APPROX_NONE,{"CHAIN_APPROX_NONE","CHAIN_APPROX_NONE"}},
                                                   {CHAIN_APPROX_SIMPLE,{"CHAIN_APPROX_SIMPLE","CHAIN_APPROX_SIMPLE"}},
                                                   {CHAIN_APPROX_TC89_L1,{"CHAIN_APPROX_TC89_L1","CHAIN_APPROX_TC89_L1"}},
                                                   {CHAIN_APPROX_TC89_KCOS,{"CHAIN_APPROX_TC89_KCOS","CHAIN_APPROX_TC89_KCOS"}},
                                               }});
    }
}

SPMSG CImgProc_findContours::Process(Mat& src, Mat& dst)
{
    try{
        auto msg = CImgProc_Canny::Process(src, dst);
        if (!msg->ok()){ return msg; }
        auto mode = Arg("mode")->CvType<RetrievalModes>();
        auto method = Arg("method")->CvType<ContourApproximationModes>();
        vector<vector<Point>> find_ctr;
        findContours(dst, find_ctr, mode, method);
        dst = Mat::zeros(dst.rows, dst.cols, dst.type());
        drawContours(dst, find_ctr, -1, Scalar(255.0));
        return _ok;
    } catch (cv::Exception e) {
        return SPFAIL("CImgProc_findContours::Process\r\n"+QString::fromLocal8Bit(e.what()));
    } return _null;
}

//---------------------------------CImgProc_findContours1--------------------------------------
CImgProc_findContours1::CImgProc_findContours1(const QJsonObject& args, QWidget *parent)
    :CImgProc_findContours(args, parent)
{
    auto grp = NewGrp<QHBoxLayout>();
    {
        auto area_min = NewArg<CProcArgs_QDoubleSpinBox>("area_min", grp);
        area_min->Init(args.value("area_min").toDouble(100.0));
        area_min->UI<QDoubleSpinBox>()->setRange(0.0, 9999.0);
    }
    {
        auto len_min = NewArg<CProcArgs_QDoubleSpinBox>("len_min", grp);
        len_min->Init(args.value("len_min").toDouble(100.0));
        len_min->UI<QDoubleSpinBox>()->setRange(0.0, 9999.0);
    }
    {
        auto cir_rio_max = NewArg<CProcArgs_QDoubleSpinBox>("cir_rio_max", grp);
        cir_rio_max->Init(args.value("cir_rio_max").toDouble(0.3));
        cir_rio_max->UI<QDoubleSpinBox>()->setSingleStep(0.1);
        cir_rio_max->UI<QDoubleSpinBox>()->setRange(0.1, 10.0);
    }
    {
        auto len_close = NewArg<CProcArgs_QCheckBox>("len_close", grp);
        len_close->Init(args.value("len_close").toBool(false));
    }
}
SPMSG CImgProc_findContours1::Process(Mat& src, Mat& dst)
{
    try{
        auto msg = CImgProc_Canny::Process(src, dst);
        if (!msg->ok()){ return msg; }
        //find
        auto mode = Arg("mode")->CvType<RetrievalModes>();
        auto method = Arg("method")->CvType<ContourApproximationModes>();
        vector<vector<Point>> find_ctr;
        vector<Vec4i> hierachy;
        findContours(dst, find_ctr, hierachy, mode, method);
        //draw
        auto len_close = Arg("len_close")->Get().toBool();
        auto area_min = Arg("area_min")->Get().toDouble();
        auto len_min = Arg("len_min")->Get().toDouble();
        auto cir_rio_max = Arg("cir_rio_max")->Get().toDouble();
        Mat mask=Mat::ones(src.size(), src.type());
        for (auto i=0; i<find_ctr.size(); i++){
            auto& ctr = find_ctr[i];
            double area = contourArea(ctr);
            auto len = arcLength(ctr, len_close);
            if (len_min>len || area_min>area
                    || cir_rio_max<((4*CV_PI*area)/(len*len))){
                drawContours(mask, find_ctr, i, 0, -1, LINE_8, hierachy, 0, Point(0,0));
            }
        }
        bitwise_and(src, src, dst, mask);
        return _ok;
    } catch (cv::Exception e) {
        return SPFAIL("CImgProc_findContours1::Process\r\n"+QString::fromLocal8Bit(e.what()));
    } return _null;
}
