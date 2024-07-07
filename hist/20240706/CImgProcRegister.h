#include "CImgProc.h"
#include "CImgProcBase.h"

template<class T>
SPMSG Register(){ return CImgProc::Register(T().API(), T::New); }

template<class Log>
inline SPMSG RegisterImgProc(Log& log)
{
    log << Register<CImgProc_cvtColor>();
    log << Register<CImgProc_adaptiveThreshold>();
    return SPOK("RegisterImgProc");
}
