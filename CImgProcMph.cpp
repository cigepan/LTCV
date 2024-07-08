#include "CImgProcMph.h"

//---------------------------------CImgProc_morphologyEx--------------------------------------
CImgProc_morphologyEx::CImgProc_morphologyEx(const QJsonObject& args, QWidget *parent)
    : CImgProc(args, parent)
{
    auto grp = NewGrp<QVBoxLayout>();
    {
        auto op = NewArg<CProcArgs_QComboBox>("op", grp);
        op->Init(args.value("op").toString("MORPH_ERODE"), {{
                                                   {MORPH_ERODE,{"MORPH_ERODE","腐蚀"}},
                                                   {MORPH_DILATE,{"MORPH_DILATE","膨胀"}},
                                                   {MORPH_OPEN,{"MORPH_OPEN","开操作"}},
                                                   {MORPH_CLOSE,{"MORPH_CLOSE","闭操作"}},
                                                   {MORPH_GRADIENT,{"MORPH_GRADIENT","MORPH_GRADIENT"}},
                                                   {MORPH_TOPHAT,{"MORPH_TOPHAT","顶帽"}},
                                                   {MORPH_BLACKHAT,{"MORPH_BLACKHAT","黑帽"}},
                                                   {MORPH_HITMISS,{"MORPH_HITMISS","MORPH_HITMISS"}},
                                               }});
    }
    {
        auto kernel = NewArg<CProcArgs_QSpinBox>("kernel", grp);
        kernel->Init(args.value("kernel").toInt(5));
        kernel->UI<QSpinBox>()->setRange(1, 255);
    }
}

SPMSG CImgProc_morphologyEx::Process(Mat& src, Mat& dst)
{
    try{
        auto op = Arg("op")->CvType<MorphTypes>();
        auto kernel = Arg("kernel")->Get().toInt();
        morphologyEx(src, dst, op
                     , getStructuringElement(MORPH_RECT, Size(kernel, kernel), Point(-1, -1)));
        return _ok;
    } catch (cv::Exception e) {
        return SPFAIL("CImgProc_morphologyEx::Process\r\n"+QString::fromLocal8Bit(e.what()));
    } return _null;
}
