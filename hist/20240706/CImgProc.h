#pragma once

#include <CMyDef.h>

//---------------------------------CImgProc--------------------------------------
class CImgProc;
using Fun_CImgProc_New = CImgProc*(*)(const QJsonObject&);
class CImgProc : public QGroupBox
{
    Q_OBJECT
    static QHash<QString, Fun_CImgProc_New> hsProcNew;
signals:
    void sigUpdate(CImgProc* p);
    void sigRemove(CImgProc* p);
public:
    enum EmGroup : char
    {
        BASE, FILTER, MPH, ADV, ENDF
    };
    static SPMSG Register(const char* api, Fun_CImgProc_New fun);
    static CImgProc* NewProc(const QString& key, const QJsonObject& args={});
    static SPMSG SelectProc();
    static const StMap<EmGroup> smGroup;
    static QString GroupEN(EmGroup e) { return smGroup.EN(e); }
    static QString GroupZH(EmGroup e) { return smGroup.ZH(e); }
    const QString GroupEN() const { return GroupEN(Group()); }
    const QString GroupZH() const { return GroupZH(Group()); }
    QStringList Infos() const {
        QStringList ifs;
        ifs << QString("组类 ")+ GroupZH();
        ifs << QString("名称 ")+Title();
        ifs << QString("接口 ")+API();
        ifs << QString("提示 ")+ToolTip();
        return ifs;
    }
protected:
    QVBoxLayout* m_vbLayOut{new QVBoxLayout};
    QCheckBox* m_qEnable{new QCheckBox("启用")};
    QCheckBox* m_qEditEn{new QCheckBox("编辑")};
    QPushButton* m_btnReset{new QPushButton("重置")};
    QPushButton* m_btnRemove{new QPushButton("移除")};
    QMap<QString, QWidget*> AUI{};
    QJsonObject m_objArgs;
public:
    //static CImgProc* New(const QJsonObject& args) { return new CImgProc(args); }
    explicit CImgProc(QWidget *parent = nullptr){ }
    const QJsonValue Arg(QString key) const { return m_objArgs.value(key); }
    void Reset() { SetArgs(m_objArgs); }
    bool Enable() const { return m_qEnable->isChecked(); }
    virtual QJsonObject GetArgs() const {
        return {{"m_qEnable", m_qEnable->isChecked()}};
    }
    virtual SPMSG SetArgs(const QJsonObject& args) {
        m_qEnable->setChecked(Arg("m_qEnable").toBool(true));
        m_objArgs=args;
        return _ok;
    }
    virtual SPMSG InitUI(const QJsonObject& args)
    {
        //menu
        QHBoxLayout* opt = new QHBoxLayout;
        opt->addWidget(m_qEnable);
        opt->addWidget(m_qEditEn);
        opt->addWidget(m_btnReset);
        opt->addWidget(m_btnRemove);
        m_vbLayOut->addLayout(opt);
        QGroupBox::setLayout(m_vbLayOut);
        QGroupBox::setTitle(Title());
        QGroupBox::setToolTip(ToolTip());
        connect(m_btnReset, &QPushButton::clicked, [&](){ Reset(); });
        connect(m_btnRemove, &QPushButton::clicked, [&](){ sigRemove(this); });
        connect(m_qEnable, &QPushButton::clicked, [&](){
            sigUpdate(this);
        });
        connect(m_qEditEn, &QPushButton::clicked, [&](){
            for (auto p : AUI){
            p->setEnabled(m_qEditEn->isChecked());
            } sigUpdate(this);
        });
        //args
        for (auto it=AUI.begin(); it!=AUI.end(); it++){
            it.value()->setToolTip(it.key());
            m_vbLayOut->addWidget(it.value());
        }
        m_qEditEn->clicked(m_qEditEn->isChecked());
        return SetArgs(args);
    }
    virtual const char* API() const = 0;
    virtual const char* Title() const = 0;
    virtual const char* ToolTip() const = 0;
    virtual const EmGroup Group() const = 0;
    virtual SPMSG Process(Mat& src, Mat& dsc) = 0;
};

