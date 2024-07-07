#pragma once

#include <CMyDef.h>

class CProcArgs : public QObject
{
    Q_OBJECT
    StMap<int> smp; //<CV_DEFINE_STR, <CV_DEFINE_INT, CV_DEFINE_NAME>>
    QVariant dft; //default
signals:
    void sigChanged(CProcArgs*);
public:
    CProcArgs(){}
    void ReSet() { return Set(dft); }
    const StMap<int>& SMP() const { return smp; }
    template <typename T> T CvType() const { return T(Type().toInt()); }
    template <typename T> T* UI() { return dynamic_cast<T*>(Widget()); }
    virtual void Init(const QVariant& v, const StMap<int>& m={{}}){ dft=v; smp=m; }
    virtual QWidget* Widget() = 0;
    virtual QVariant Get() const = 0;
    virtual QVariant Type() const = 0;
    virtual void Set(const QVariant& v) = 0;
};

class CProcArgs_QComboBox : public CProcArgs
{
    Q_OBJECT
    QComboBox* ui{NULL};

public:
    CProcArgs_QComboBox(QWidget *parent = nullptr){
        ui = new QComboBox(parent);
    }
    virtual void Init(const QVariant& v, const StMap<int>& m={{}}){
        ui->clear(); ui->addItems(m.ezs);
        connect(ui, &QComboBox::currentIndexChanged, [&](){
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
    CProcArgs_QDoubleSpinBox(QWidget *parent = nullptr){
        ui = new QDoubleSpinBox(parent);
    }
    virtual void Init(const QVariant& v, const StMap<int>& m={{}}){
        connect(ui, &QDoubleSpinBox::valueChanged, [&](){
            CProcArgs::sigChanged((CProcArgs*)(this));
        });
        return CProcArgs::Init(v, m);
    }
    virtual QWidget* Widget() { return ui; }
    virtual QVariant Get() const { return ui->value();  }
    virtual QVariant Type() const { return double(0.0); }
    virtual void Set(const QVariant& v) { return ui->setValue(v.toDouble()); }
};
class CProcArgs_QSpinBox : public CProcArgs
{
    Q_OBJECT
    QSpinBox* ui{NULL};

public:

    CProcArgs_QSpinBox(QWidget *parent = nullptr){
        ui = new QSpinBox(parent);
    }
    virtual void Init(const QVariant& v, const StMap<int>& m={{}}){
        connect(ui, &QSpinBox::valueChanged, [&](){
            CProcArgs::sigChanged((CProcArgs*)(this));
        });
        return CProcArgs::Init(v, m);
    }
    virtual QWidget* Widget() { return ui; }
    virtual QVariant Get() const { return ui->value();  }
    virtual QVariant Type() const { return int(0); }
    virtual void Set(const QVariant& v) { return ui->setValue(v.toInt()); }
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
    void sigAddNext(CImgProc* p);
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
    QVBoxLayout* m_vbLayOut{new QVBoxLayout};
    QCheckBox* m_qEnable{new QCheckBox("启用")};
    QCheckBox* m_qEditEn{new QCheckBox("编辑")};
    QPushButton* m_btnReset{new QPushButton("重置")};
    QPushButton* m_btnRemove{new QPushButton("移除")};
    QPushButton* m_btnAddNext{new QPushButton("后加")};
    QMap<QString, CProcArgs*> Args{}; //处理参数UI管理

public:
    //static CImgProc* New(const QJsonObject& args) { return new CImgProc(args); }
    explicit CImgProc(QWidget *parent = nullptr);
    //~CImgProc();
    bool Enable() const { return m_qEnable->isChecked(); }
    SPMSG InitUI(const QJsonObject& args);
    SPMSG ResetArgs();
    CProcArgs* Arg(const QString arg){ return Args.value(arg); }
    virtual QJsonObject GetArgs() const;
    //virtual SPMSG SetArgs(const QJsonObject& args);
    virtual const char* API() const = 0;
    virtual const char* Title() const = 0;
    virtual const char* ToolTip() const = 0;
    virtual const EmGroup Group() const = 0;
    virtual SPMSG Process(Mat& src, Mat& dst) = 0;
};

