#include "CImgProc.h"
#include "CImgProcBase.h"

QHash<QString, Fun_CImgProc_New> CImgProc::hsProcNew;
const StMap<CImgProc::EmGroup> CImgProc::smGrp={
    {
        {BASE,{"BASE","基础"}},
        {FILTER,{"FILTER","滤波"}},
        {MPH,{"MPH","形态学"}},
        {ADV,{"ADV","高级"}},
        {CUS,{"CUS","定制"}},
        {ENDF,{"ENDF","未定义"}}
    }};

CImgProc::CImgProc(const QJsonObject& args, QWidget *parent)
    : QGroupBox(parent)
{
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setMinimumWidth(200);
    //new menu
    m_vbLayOut = new QVBoxLayout(this);
    m_qEnable = new QCheckBox("启用");
    m_qEditEn = new QCheckBox("编辑");
    m_btnReset = new QPushButton("重置");
    m_btnRemove = new QPushButton("移除");
    m_btnAddPrev = new QPushButton("前+");
    //menu
    QHBoxLayout* opt = new QHBoxLayout;
    opt->addWidget(m_qEnable);
    opt->addWidget(m_qEditEn);
    opt->addWidget(m_btnReset);
    opt->addWidget(m_btnAddPrev);
    opt->addWidget(m_btnRemove);
    m_vbLayOut->addLayout(opt);
    connect(m_btnReset, &QPushButton::clicked, [&](){ ResetArgs(); });
    connect(m_btnAddPrev, &QPushButton::clicked, [&](){ sigAddPrev(this); });
    connect(m_btnRemove, &QPushButton::clicked, [&](){ sigRemove(this); });
    connect(m_qEnable, &QPushButton::clicked, [&](){
        sigUpdate(this);
    });
    connect(m_qEditEn, &QPushButton::clicked, [&](){
        for (auto it : Args){
            it->Widget()->setEnabled(m_qEditEn->isChecked());
        } sigUpdate(this);
    });
    m_qEnable->setChecked(args.value("m_qEnable").toBool(true));
}
// CImgProc::~CImgProc(){
//     for (auto it : Args){ it->deleteLater(); }
// }

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
    auto proc = (*func)(args);
    if (!proc) { return NULL; }
    proc->InitUI();
    return proc;
}

SPMSG CImgProc::SelectProc()
{
    QDialog dlg;
    QMap<CImgProc::EmGroup, QComboBox*> mapCmb;
    QMap<QString, QStringList> mapInfos;
    QString strLastAPI;
    for (auto key : hsProcNew.keys()){
        auto prc = NewProc(key);
        if (!mapCmb.contains(prc->Group())){
            auto cmb = mapCmb[prc->Group()] = new QComboBox();
            QObject::connect(cmb, &QComboBox::currentTextChanged
            , [&,cmb](const QString& api){
                for (auto c : mapCmb){
                    if (c==cmb){ continue; }
                    QSignalBlocker b(c); c->setCurrentIndex(0);
                }
                strLastAPI = QString(api).remove(" ").trimmed().split("|").first();
                if (strLastAPI.isEmpty()){
                    dlg.setWindowTitle("请选择"); dlg.setToolTip(""); return;
                } else {
                    dlg.setWindowTitle(api);
                }
                dlg.setToolTip(mapInfos.value(api).join("\r\n"));
            });
            cmb->addItem("");
        }
        mapCmb.value(prc->Group())->addItem(QString("%1 | %2").arg(prc->API()).arg(prc->Title()));
        mapInfos[prc->API()] = prc->Infos();
        delete prc;
    }
    QFormLayout* lay = new QFormLayout;
    for (auto it=mapCmb.begin(); it!=mapCmb.end(); it++){
        lay->addRow(CImgProc::GrpZH(it.key()), it.value());
    }
    QPushButton* ok = new QPushButton("OK");
    ok->setMinimumHeight(50);
    lay->addWidget(ok);
    dlg.setLayout(lay);
    QObject::connect(ok, &QPushButton::clicked, [&](){ dlg.accept(); });
    if (QDialog::Accepted == dlg.exec()
            && !strLastAPI.isEmpty()){ return SPOK(strLastAPI); }
    return SPNULL("");
}

SPMSG CImgProc::InitUI()
{
    //args
    for (auto it=Args.begin(); it!=Args.end(); it++){
        it.value()->Widget()->setToolTip(it.key());
        connect(it.value(), &CProcArgs::sigChanged, [&](CProcArgs* a){ sigUpdate(this, a); });
    }
    QGroupBox::setLayout(m_vbLayOut);
    QGroupBox::setTitle(GrpTitle());
    QGroupBox::setToolTip(ToolTip());
    m_qEditEn->clicked(m_qEditEn->isChecked());
    return ResetArgs();
}

SPMSG CImgProc::ResetArgs() { for (auto& it : Args){ it->ReSet(); } return _ok; }

QJsonObject CImgProc::GetArgs() const
{
    QJsonObject cfg;
    cfg["m_qEnable"] = m_qEnable->isChecked();
    for (auto it=Args.begin(); it!=Args.end(); it++){
        cfg[it.key()] = it.value()->Get().toJsonValue();
    } return cfg;
}
// SPMSG CImgProc::SetArgs(const QJsonObject& args)
// {
//     for (auto it=Args.begin(); it!=Args.end(); it++){
//         it.value()->Set(args.value(it.key()).toVariant());
//     } return _ok;
// }
