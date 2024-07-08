#include "CImgProc.h"
#include "CImgProcBase.h"
#include "CImgProcFilter.h"
#include "CImgProcMph.h"
#include "CImgProcAdv.h"
#include "CImgProcCus.h"

template<class T>
SPMSG Register(){ return CImgProc::Register(T().API(), T::New); }

inline SPMSG RegisterImgProc()
{
    //BASE
    Register<CImgProc_cvtColor>();
    Register<CImgProc_adaptiveThreshold>();

    //FILTER
    Register<CImgProc_GaussianBlur>();
    Register<CImgProc_medianBlur>();

    //MPH
    Register<CImgProc_morphologyEx>();

    //ADV
    Register<CImgProc_Canny>();

    //CUSTOM
    Register<CImgProc_findContours>();
    Register<CImgProc_findContours1>();

    return SPOK("RegisterImgProc");
}
