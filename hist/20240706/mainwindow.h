#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <CWidget2D.h>
#include <CImgProcRegister.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CLog : public QLineEdit
{
    Q_OBJECT
    QList<SPMSG> listMsg;
public:
    CLog(const QString &text, QWidget *parent=nullptr):QLineEdit(text, parent){
    }
    CLog& operator<<(SPMSG msg){
        listMsg << msg;
        setText(msg->body);
        return *this;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QVBoxLayout* m_vLayWay{new QVBoxLayout};
    QMap<QString, QFileInfo> m_mapFiles;
    QMap<QString, QFileInfo> m_mapSaved;
    QJsonObject m_objCfg;
    QJsonArray m_aryWay;
    QList<Mat> m_listMat;
    QList<CImgProc*> m_listProc;
    SPIMG m_spIMG;
    QPushButton* m_Readme{new QPushButton("使用说明")};
    QPushButton* m_Version{new QPushButton("版本记录")};
    SPMSG ShowText(QString path);
    SPMSG AddProc(CImgProc* pProc);
    SPMSG ClearProc();
    const QJsonObject& Cfg() const { return m_objCfg; }
    const QString Cfg(QString key) const { return m_objCfg.value(key).toString(); }
    const QJsonObject& SetCfg(QString key, QJsonValue value) { m_objCfg[key]=value; return Cfg(); }
    virtual void keyPressEvent(QKeyEvent *event);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    SPMSG InitUI();

private slots:
    SPMSG slotProcess2D();
    void on_pushButton_opendir_clicked();
    void on_pushButton_opendir_clicked(QString dir);
    void on_pushButton_savedir_clicked();
    void on_pushButton_savedir_clicked(QString dir);

    void on_listWidget_files_currentTextChanged(const QString &currentText);

    void on_comboBox_way_currentTextChanged(const QString &arg1);

    void on_pushButton_newway_clicked();

    void on_pushButton_procadd_clicked();

    void on_pushButton_saveway_clicked();



    void on_pushButton_saveimg_clicked();

    void on_radioButton_orgsize_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
