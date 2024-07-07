#include "CImgProc.h"
#include "CImgProcBase.h"

QHash<QString, Fun_CImgProc_New> CImgProc::hsProcNew;
const StMap<CImgProc::EmGroup> CImgProc::smGroup={
    {
        {BASE,{"BASE","基础"}},
        {FILTER,{"FILTER","滤波"}},
        {MPH,{"MPH","形态学"}},
        {ADV,{"ADV","高级"}},
        {ENDF,{"ENDF","未定义"}}
    }};

SPMSG CImgProc::Register(const char* api, Fun_CImgProc_New fun)
{
    QString sapi(api);
    if (hsProcNew.contains(sapi)){
        return SPFAIL("CImgProc::Register api is already->"+sapi);
    }
    hsProcNew[sapi] = fun;
    return SPOK("CImgProc::Register->"+sapi);
}

CImgProc* CImgProc::NewProc(const QString& key, const QJsonObject& args)
{
    auto func = hsProcNew.value(key);
    if(!func) { return NULL; }
    return (*func)(args);
}

SPMSG CImgProc::SelectProc()
{
    QMap<CImgProc::EmGroup, QComboBox*> mapCmb;
    QMap<QString, QStringList> mapInfos;
    QString strLastAPI;
    for (auto key : hsProcNew.keys()){
        auto prc = NewProc(key);
        if (!mapCmb.contains(prc->Group())){
            auto cmb = mapCmb[prc->Group()] = new QComboBox();
            QObject::connect(cmb, &QComboBox::currentTextChanged
            , [&,cmb](const QString& api){
                strLastAPI = api;
                if (api.isEmpty()){ cmb->setToolTip(prc->GroupZH()); return; }
                for (auto c : mapCmb){ if(c!=cmb) { c->setCurrentIndex(0); } }
                cmb->setToolTip(mapInfos.value(api).join("\r\n"));
            });
            cmb->addItem("");
        }
        mapCmb.value(prc->Group())->addItem(prc->API());
        mapInfos[prc->API()] = prc->Infos();
        delete prc;
    }
    QVBoxLayout* lay = new QVBoxLayout;
    for (auto cmb : mapCmb){ lay->addWidget(cmb); }
    QPushButton* ok = new QPushButton("OK");
    ok->setMinimumHeight(50);
    lay->addWidget(ok);
    QDialog dlg;
    dlg.setLayout(lay);
    QObject::connect(ok, &QPushButton::clicked, [&](){ dlg.accept(); });
    if (QDialog::Accepted == dlg.exec()
            && !strLastAPI.isEmpty()){ return SPOK(strLastAPI); }
    return SPNULL("");
}
