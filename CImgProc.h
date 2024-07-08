#pragma once

#include <CMyDef.h>

class CProcArgs : public QObject
{
    Q_OBJECT
    QString nm; //arg name
    StMap<int> smp; //<CV_DEFINE_STR, <CV_DEFINE_INT, CV_DEFINE_NAME>>
    QVariant dft; //default
signals:
    void sigChanged(CProcArgs*);
public:
    CProcArgs(const QString& name):nm(name){}
    void ReSet() { return Set(dft); }
    const QString& Name() const { return nm; }
    const StMap<int>& SMP() const { return smp; }
    virtual QVariant Type() const { return QVariant(); }
    template <typename T> T CvType() const { return T(Type().toInt()); }
    template <typename T> T* UI() { return dynamic_cast<T*>(Widget()); }
    virtual void Init(const QVariant& v, const StMap<int>& m={{}}){ dft=v; smp=m; }
    virtual QWidget* Widget() = 0;
    virtual QVariant Get() const = 0;
    virtual void Set(const QVariant& v) = 0;
};

class CProcArgs_QComboBox : public CProcArgs
{
    Q_OBJECT
    QComboBox* ui{NULL};

public:
    CProcArgs_QComboBox(const QString& title, QWidget *parent = nullptr)
        : CProcArgs(title) {
        ui = new QComboBox(parent);
    }
    virtual void Init(const QVariant& v, const StMap<int>& m={{}}){
        ui->clear(); ui->addItems(m.ezs);
        connect(ui, &QComboBox::currentIndexChanged, [&](){
            ui->setToolTip(QString("%1 = %2").arg(Name()).arg(Get().toString()));
            CProcArgs::sigChanged((CProcArgs*)(this));
        });
        return CProcArgs::Init(v, m);
    }
    virtual QWidget* Widget() { return ui; }
    virtual QVariant Get() const { return SMP().ens[ui->currentIndex()];  }
    virtual QVariant Type() const { return SMP().tps[ui->currentIndex()]; }
    virtual void Set(const QVariant& v) {
        auto idx = SMP().IDX(v.toString());
        if (0>idx){ return ReSet(); }
        return ui->setCurrentIndex(idx);
    }
};

class CProcArgs_QDoubleSpinBox : public CProcArgs
{
    Q_OBJECT
    QDoubleSpinBox* ui{NULL};

public:
    CProcArgs_QDoubleSpinBox(const QString& title, QWidget *parent = nullptr)
        : CProcArgs(title) {
        ui = new QDoubleSpinBox(parent);
    }
    virtual void Init(const QVariant& v, const StMap<int>& m={{}}){
        connect(ui, &QDoubleSpinBox::valueChanged, [&](){
            ui->setToolTip(QString("%1 = %2").arg(Name()).arg(Get().toDouble()));
            CProcArgs::sigChanged((CProcArgs*)(this));
        });
        return CProcArgs::Init(v, m);
    }
    virtual QWidget* Widget() { return ui; }
    virtual QVariant Get() const { return ui->value();  }
    virtual void Set(const QVariant& v) { return ui->setValue(v.toDouble()); }
};
class CProcArgs_QSpinBox : public CProcArgs
{
    Q_OBJECT
    QSpinBox* ui{NULL};

public:

    CProcArgs_QSpinBox(const QString& title, QWidget *parent = nullptr)
        : CProcArgs(title) {
        ui = new QSpinBox(parent);
    }
    virtual void Init(const QVariant& v, const StMap<int>& m={{}}){
        connect(ui, &QSpinBox::valueChanged, [&](){
            ui->setToolTip(QString("%1 = %2").arg(Name()).arg(Get().toInt()));
            CProcArgs::sigChanged((CProcArgs*)(this));
        });
        return CProcArgs::Init(v, m);
    }
    virtual QWidget* Widget() { return ui; }
    virtual QVariant Get() const { return ui->value();  }
    virtual void Set(const QVariant& v) { return ui->setValue(v.toInt()); }
};

class CProcArgs_QCheckBox : public CProcArgs
{
    Q_OBJECT
    QCheckBox* ui{NULL};

public:
    CProcArgs_QCheckBox(const QString& title, QWidget *parent = nullptr)
        : CProcArgs(title) {
        ui = new QCheckBox(title, parent);
    }
    virtual void Init(const QVariant& v, const StMap<int>& m={{}}){
        connect(ui, &QCheckBox::clicked, [&](){
            ui->setToolTip(QString("%1 = %2").arg(Name()).arg(Get().toBool()));
            CProcArgs::sigChanged((CProcArgs*)(this));
        });
        return CProcArgs::Init(v, m);
    }
    virtual QWidget* Widget() { return ui; }
    virtual QVariant Get() const { return ui->isChecked();  }
    virtual void Set(const QVariant& v) { return ui->setChecked(v.toBool()); }
};
//---------------------------------CImgProc--------------------------------------
class CImgProc;
using Fun_CImgProc_New = CImgProc*(*)(const QJsonObject&);
class CImgProc : public QGroupBox
{
    Q_OBJECT
    static QHash<QString, Fun_CImgProc_New> hsProcNew;
signals:
    void sigUpdate(CImgProc* p, CProcArgs* a=NULL);
    void sigRemove(CImgProc* p);
    void sigAddPrev(CImgProc* p);
public:
    enum EmGroup : char
    {
        BASE, FILTER, MPH, ADV, CUS, ENDF
    };
    static SPMSG Register(const char* api, Fun_CImgProc_New fun);
    static CImgProc* NewProc(const QString& key, const QJsonObject& args={});
    static SPMSG SelectProc();
    static const StMap<EmGroup> smGrp;
    static QString GrpEN(EmGroup e) { return smGrp.EN(e); }
    static QString GrpZH(EmGroup e) { return smGrp.ZH(e); }
    const QString GrpEN() const { return GrpEN(Group()); }
    const QString GrpZH() const { return GrpZH(Group()); }
    QStringList Infos() const {
        QStringList ifs;
        ifs << QString("组类 ")+ GrpZH();
        ifs << QString("名称 ")+Title();
        ifs << QString("接口 ")+API();
        ifs << QString("提示 ")+ToolTip();
        return ifs;
    }
protected:
    QVBoxLayout* m_vbLayOut;
    QCheckBox* m_qEnable;
    QCheckBox* m_qEditEn;
    QPushButton* m_btnReset;
    QPushButton* m_btnRemove;
    QPushButton* m_btnAddPrev;
    QMap<QString, CProcArgs*> Args{}; //处理参数UI管理

public:
    //static CImgProc* New(const QJsonObject& args) { return new CImgProc(args); }
    explicit CImgProc(const QJsonObject& args, QWidget *parent = nullptr);
    //~CImgProc();
    bool Enable() const { return m_qEnable->isChecked(); }
    virtual SPMSG InitUI();
    SPMSG ResetArgs();
    CProcArgs* Arg(const QString& arg) const { return Args.value(arg); }
    QVariant GetArg(const QString& arg) const { return Arg(arg)->Get(); }
    template<class T> T* NewGrp(const QString& title=""){
        T* lay = new T;
        QGroupBox* p = new QGroupBox(title.isEmpty()?Title():title);
        p->setToolTip(ToolTip());
        p->setContentsMargins(0,0,0,0);
        p->setLayout(lay);
        m_vbLayOut->addWidget(p);
        return lay;
    }
    template<class T> T* NewArg(const QString& title, QLayout *lay=nullptr){
        auto p = new T(title);
        if (p) { Args[title]=p; }
        if (lay) { lay->addWidget(p->Widget()); }
        return p;
    }
    virtual QJsonObject GetArgs() const;
    //virtual SPMSG SetArgs(const QJsonObject& args);
    virtual const char* API() const = 0;
    virtual const char* Title() const = 0;
    virtual const char* ToolTip() const = 0;
    virtual const EmGroup Group() const = 0;
    virtual const char* GrpTitle() const { return Title(); }
    virtual SPMSG Process(Mat& src, Mat& dst) = 0;
};

