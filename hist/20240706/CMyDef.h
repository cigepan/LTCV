#pragma once

#include <QObject>
#include <QMap>
#include <QHash>
#include <QPair>
#include <QVector>
#include <QLabel>
#include <QSharedPointer>
#include <QImage>
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QDesktopServices>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QScrollBar>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QTextBrowser>
#include <QInputDialog>
#include <QKeyEvent>
#include "opencv2/opencv.hpp"

using namespace cv;

template<typename ET>
struct StMap : public QMap<ET,QPair<QString, QString>>
{
    QMap<QString, QString> ENZH;
    StMap(const QMap<ET,QPair<QString, QString>>& map)
        : QMap<ET,QPair<QString, QString>>(map){
        ENZH.clear();
        for (auto it : map) { ENZH[it.first] = it.second; }
    }
    const QMap<ET,QPair<QString, QString>>& Map() const { return *this; }
    QString EN(ET key) const { return Map().value(key).first; }
    QString ZH(ET key) const { return Map().value(key).second; }
    QString EN(const QString& key) const { return ENZH.value(key); }
};

#define SPIMG QSharedPointer<QImage>
inline void Log(const Mat& mat)
{
    qDebug() <<"mat.rows" << mat.rows;
    qDebug() <<"mat.cols" << mat.cols;
    qDebug() <<"mat.channels" << mat.channels();
    qDebug() <<"mat.step" << mat.step;
    qDebug() <<"mat.type" << mat.type();
}
inline SPIMG MatToIMG(const Mat& mat)
{
    Log(mat);
    switch (mat.type())
    {
    case CV_8UC1:return SPIMG(new QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8));
    case CV_8UC3: return SPIMG(new QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_BGR888));
    default: return NULL;
    }
}

enum EMsgType : char
{
    MSG_NULL=0,
    MSG_OK = 1,
    MSG_YES = 1,
    MSG_FAIL = 2,
    MSG_NO = 2,
    MSG_ENDF
};

const StMap<EMsgType> g_smEMsgType{{
    {MSG_NULL,{"NULL","NULL"}},
    {MSG_OK,{"OK","OK"}},
    {MSG_FAIL,{"FAIL","FAIL"}},
    {MSG_ENDF,{"ENDF","ENDF"}}
}};
#define SMT g_smEMsgType
struct StMsg
{
    EMsgType type{MSG_NULL};
    QByteArray body;
    StMsg(const QString& str, EMsgType tp=MSG_NULL){
        type = tp;
        if (!str.isEmpty()) { body = str.toLocal8Bit(); }
    }
    StMsg(const char* str, EMsgType tp=MSG_NULL){
        type = tp;
        if (str) { body = QByteArray(str); }
    }
    void tip() const {
        QString str = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss ");
        str.append(SMT.EN(type)).append(" ").append(body);
        qDebug() << str;
        QMessageBox::warning(NULL, "", str);
    }
    bool null() const { return (type==MSG_NULL); }
    bool ok() const { return (type==MSG_OK); }
    bool yes() const { return (type==MSG_YES); }
    bool fail() const { return (type==MSG_FAIL); }
    bool no() const { return (type==MSG_NO); }
};

#define SPMSG QSharedPointer<StMsg>
#define SPNULL(XXX) SPMSG(new StMsg(XXX, MSG_NULL))
#define _null SPNULL("")
#define _unknow SPNULL("")
#define SPOK(XXX) SPMSG(new StMsg(XXX, MSG_OK))
#define _ok SPOK("")
#define _yes SPOK("")
#define SPFAIL(XXX) SPMSG(new StMsg(XXX, MSG_FAIL))
#define _fail SPFAIL("")
#define _no SPFAIL("")

#define BORDER_GREEN "border:1px solid green"
#define BORDER_RED "border:1px solid red"

