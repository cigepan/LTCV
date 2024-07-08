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

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QVBoxLayout* m_vLayWay{new QVBoxLayout};
    QMap<QString, QFileInfo> m_mapFiles;
    QMap<QString, QFileInfo> m_mapSaved;
    QJsonObject m_objCfg;
    QJsonArray m_aryWay;
    QList<CImgProc*> m_listProc;
    QList<Mat> m_listMat;
    QPushButton* m_Readme{new QPushButton("使用说明")};
    QPushButton* m_Version{new QPushButton("版本记录")};
    SPMSG ShowText(QString path);
    SPMSG AddProc(CImgProc* pProc, int nIDX=-1);
    SPMSG ClearProc();
    const QJsonObject& Cfg() const { return m_objCfg; }
    const QJsonValue Cfg(QString key) const { return m_objCfg.value(key); }
    const QJsonObject& SetCfg(QString key, QJsonValue value) { m_objCfg[key]=value; return Cfg(); }
    virtual void keyPressEvent(QKeyEvent *event);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    SPMSG InitUI();

private slots:
    SPMSG slotProcess2D(CImgProc* pCurProc=NULL);
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

    void on_spinBox_pensize_valueChanged(int arg1);

    void on_tabWidget_2D_currentChanged(int index);

    void on_checkBox_editen_clicked();

    void on_checkBox_follow_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
