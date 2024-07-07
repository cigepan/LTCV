#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScreen>
#include <QStatusBar>

#define FILE_CFG QFile(QCoreApplication::applicationDirPath()+"/config.json")
#define FOLDER_WAY QDir(QCoreApplication::applicationDirPath()+"/way")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto ssz = qApp->primaryScreen()->size();
    StMsg::pLog = new QLineEdit();
    StMsg::pLog->setMinimumWidth(ssz.width()-300);
    //-------------------------------------------------------
    setWindowState(Qt::WindowMaximized);
    ui->tabWidget_2D->setMinimumSize(ssz-QSize{300,100});
    ui->tabWidget_2D->setMaximumSize(ssz-QSize{300,100});
    ui->listWidget_files->setStyleSheet("QListWidget::Item:selected { background-color: yellow; }");
}

SPMSG MainWindow::ShowText(QString path)
{
    QFile file(path);
    file.open(QFile::ReadOnly);
    static QTextBrowser tb;
    tb.setText(file.readAll());
    tb.show();
    return SPOK(path);
}

SPMSG MainWindow::InitUI()
{
    ui->scrollAreaWidgetContents->setLayout(m_vLayWay);
    m_Readme->setMinimumWidth(100);
    m_Version->setMinimumWidth(100);
    m_Readme->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_Version->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    statusBar()->addWidget(StMsg::pLog);
    statusBar()->addWidget(m_Readme);
    statusBar()->addWidget(m_Version);
    connect(m_Version, QPushButton::clicked, [&](){ ShowText(":/version.txt"); });
    connect(m_Readme, QPushButton::clicked, [&](){ ShowText(":/readme.txt"); });
    RegisterImgProc();
    //------------------------------------------------------------
    ui->comboBox_save_imgsuffix->addItems({"png","jpg","tif","icon","bin"});
    //----------------------------CFG---------------------------
    auto cfg = FILE_CFG;
    cfg.open(QFile::ReadOnly);
    m_objCfg = QJsonDocument::fromJson(cfg.readAll()).object();
    on_pushButton_opendir_clicked(Cfg("pushButton_opendir").toString());
    on_pushButton_savedir_clicked(Cfg("pushButton_savedir").toString());
    on_listWidget_files_currentTextChanged(Cfg("listWidget_files").toString());
    //---------------------------WAY----------------------------
    if (!FOLDER_WAY.exists() && !FOLDER_WAY.mkpath(".")){
        return SPFAIL("无法创建方案配置文件夹->"+FOLDER_WAY.path());
    }
    auto block = ui->comboBox_way->blockSignals(true);
    for (auto it : FOLDER_WAY.entryInfoList({"*.json"}, QDir::Files))
    {
        ui->comboBox_way->addItem(it.fileName());
    }
    ui->comboBox_way->setCurrentIndex(-1);
    ui->comboBox_way->blockSignals(block);
    ui->comboBox_way->setCurrentText(Cfg("comboBox_way").toString());
    ui->spinBox_pensize->setValue(Cfg("spinBox_pensize").toInt());
    ui->checkBox_editen->setChecked(Cfg("checkBox_editen").toBool());
    return _ok;
}

MainWindow::~MainWindow()
{
    if (!QFile(FOLDER_WAY.filePath(ui->comboBox_way->currentText())).exists()){
        on_pushButton_saveway_clicked();
    }
    auto cfg = FILE_CFG;
    cfg.open(QFile::WriteOnly|QFile::Truncate);
    cfg.write(QJsonDocument(m_objCfg).toJson());
    cfg.close();
    for (auto p : m_listProc) { p->deleteLater(); }
    this->deleteLater();
    delete ui;
}

void MainWindow::on_pushButton_opendir_clicked()
{
    auto dir = QFileDialog::getExistingDirectory(ui->pushButton_opendir);
    on_pushButton_opendir_clicked(dir);
}
void MainWindow::on_pushButton_opendir_clicked(QString dir)
{
    if (dir.isEmpty()){return;}
    SetCfg("pushButton_opendir", dir);
    ui->pushButton_opendir->setText(dir);
    ui->pushButton_opendir->setToolTip(dir);
    for (auto file : QDir(dir).entryInfoList()){
        if (!file.isFile()){continue;}
        m_mapFiles[file.baseName()] = file;
    }
    ui->listWidget_files->clear();
    ui->listWidget_files->addItems(m_mapFiles.keys());
}

void MainWindow::on_pushButton_savedir_clicked()
{
    auto dir = QFileDialog::getExistingDirectory(ui->pushButton_savedir);
    if (dir == ui->pushButton_opendir->text()){
        SPFAIL("保存路径不允许与加载路径相同")->tip(); return;
    }
    on_pushButton_savedir_clicked(dir);
}
void MainWindow::on_pushButton_savedir_clicked(QString dir)
{
    if (dir.isEmpty()){return;}
    SetCfg("pushButton_savedir", dir);
    ui->pushButton_savedir->setText(dir);
    ui->pushButton_savedir->setToolTip(dir);
    for (auto file : QDir(dir).entryInfoList()){
        if (!file.isFile()){continue;}
        m_mapSaved[file.baseName()] = file;
    }
    auto files = m_mapFiles.keys();
    for (auto save : m_mapSaved.keys()){
        auto idx = files.indexOf(save);
        if (idx>-1){
            ui->listWidget_files->item(idx)->setBackground(Qt::green);
        }
    }
}

void MainWindow::on_listWidget_files_currentTextChanged(const QString &currentText)
{
    if (!ui->listWidget_files->currentItem()
        || ui->listWidget_files->currentItem()->text()!=currentText){
        QSignalBlocker block(ui->listWidget_files);
        ui->listWidget_files->setCurrentRow(m_mapFiles.keys().indexOf(currentText));
    }
    setWindowTitle(currentText);
    SetCfg("listWidget_files", currentText);
    slotProcess2D();
}

SPMSG MainWindow::slotProcess2D(CImgProc* p)
{
    auto item = ui->listWidget_files->currentItem();
    if (!item) { return _null; }
    auto img_fio = m_mapFiles.value(item->text());
    m_listMat.clear();
    m_listMat << cv::imread(img_fio.absoluteFilePath().toStdString().c_str());
    auto lay_way = ui->scrollAreaWidgetContents->layout();
    auto size = lay_way->count();
    const auto last_tab_idx = [&](){
        if (ui->tabWidget_2D->currentIndex()>-1){
            return ui->tabWidget_2D->currentIndex();
        } else {
            return Cfg("tabWidget_2D").toInt();
        }
    }();
    for (auto i=0; i<ui->tabWidget_2D->count(); i++){
        ui->tabWidget_2D->widget(i)->deleteLater();
    } ui->tabWidget_2D->clear();
    auto src = MatToIMG(m_listMat.last());
    ui->tabWidget_2D->addTab(new CWidget2D(src), "原图");
    for (auto idx=0; idx<size; idx++){
        auto item = lay_way->itemAt(idx);
        auto pw = item->widget();
        if (!pw) { continue; }
        auto proc = dynamic_cast<CImgProc*>(pw);
        if (!proc){ continue; }
        if (!proc->Enable()) { proc->setStyleSheet(GSS_COLOR("#808080")); continue; }
        m_listMat << Mat();
        auto res = proc->Process(m_listMat[m_listMat.size()-2], m_listMat.last());
        proc->setStyleSheet((res->ok()?GSS_COLOR("#00AA00"):GSS_COLOR("#AA0000")));
        if (!res->ok()) { return res; }
        auto sp = MatToIMG(m_listMat.last());
        if (!sp) { continue; }
        ui->tabWidget_2D->addTab(new CWidget2D(sp), proc->Title());
    }
    auto& dsc = m_listMat.last();
    if (ui->radioButton_orgsize->isChecked()){
        ui->spinBox_save_imgw->setValue(dsc.cols);
        ui->spinBox_save_imgh->setValue(dsc.rows);
        ui->spinBox_save_imgch->setValue(dsc.channels());
        ui->comboBox_save_imgsuffix->setCurrentText(img_fio.suffix());
    }
    ui->tabWidget_2D->setCurrentIndex(last_tab_idx);
    return _ok;
}

void MainWindow::on_comboBox_way_currentTextChanged(const QString &arg1)
{
    if (arg1.isEmpty()) { return; }
    SetCfg("comboBox_way", arg1);
    QFile way(FOLDER_WAY.filePath(arg1));
    way.open(QFile::ReadOnly);
    auto aryProc = QJsonDocument::fromJson(way.readAll()).array();
    ClearProc();
    for (auto itProc : aryProc){
        auto obj = itProc.toObject();
        for (auto it=obj.begin(); it!=obj.end(); it++){
            auto pProc = CImgProc::NewProc(it.key(), it.value().toObject());
            if (!pProc) { SPFAIL("Failed NewProc->"+it.key())->tip(); }
            auto add = AddProc(pProc);
            if (!add->ok()) { add->tip(); }
        }
    }
    slotProcess2D();
}
SPMSG MainWindow::ClearProc()
{
    m_listProc.clear();
    QLayoutItem* item;
    while(item=m_vLayWay->takeAt(0)){
        delete item->widget();
    }
    ui->scrollAreaWidgetContents->repaint();
    ui->scrollArea_way->repaint();
    m_vLayWay->update();
    return _ok;
}
SPMSG MainWindow::AddProc(CImgProc* pProc, int nIDX)
{
    if (!pProc) { return _fail; }
    if (nIDX<0){
        m_listProc << pProc;
        m_vLayWay->addWidget(pProc);
    } else {
        m_listProc.insert(nIDX, pProc);
        m_vLayWay->insertWidget(nIDX, pProc);
    }
    connect(pProc, &CImgProc::sigUpdate, this, &MainWindow::slotProcess2D);
    connect(pProc, &CImgProc::sigRemove, [&](CImgProc* p){
        m_listProc.removeAll(p);
        m_vLayWay->removeWidget(p);
        delete p;
        slotProcess2D();
    });
    // connect(pProc, &CImgProc::sigAddNext, [&](CImgProc* p){
    //     auto idx = m_listProc.indexOf(p);
    //     if (idx<0) { return; }
    //     auto sel_proc = CImgProc::SelectProc();
    //     if (!sel_proc->ok()){ return; }
    //     auto new_proc = CImgProc::NewProc(sel_proc->body);
    //     if (!new_proc){ return; }
    //     AddProc(new_proc, idx);
    //     slotProcess2D();
    // });
    return _ok;
}
void MainWindow::on_pushButton_newway_clicked()
{
    auto way_name = QInputDialog::getText(this, "请输入方案名", "格式：大小写字母 下划线 数字");
    if (way_name.isEmpty()) { return; }
    way_name += ".json";
    ui->comboBox_way->addItem(way_name);
    ui->comboBox_way->setCurrentText(way_name);
}


void MainWindow::on_pushButton_procadd_clicked()
{
    if (ui->comboBox_way->currentIndex()<0){
        return SPFAIL("请先创建选定一个方案")->tip();
    }
    auto sel_proc = CImgProc::SelectProc();
    if (!sel_proc->ok()){ return; }
    auto new_proc = CImgProc::NewProc(sel_proc->body);
    if (!new_proc){ return; }
    AddProc(new_proc);
    slotProcess2D(new_proc);
}

void MainWindow::on_pushButton_saveway_clicked()
{
    if (ui->comboBox_way->currentIndex()<0){
        return SPFAIL("请先创建选定一个方案")->tip();
    }
    QJsonArray aryProc;
    for (auto p : m_listProc){
        aryProc << QJsonObject{{p->API(), p->GetArgs()}};
    }
    QFile way(FOLDER_WAY.filePath(ui->comboBox_way->currentText()));
    way.open(QFile::WriteOnly|QFile::Truncate);
    way.write(QJsonDocument(aryProc).toJson());
    way.close();
}

void MainWindow::on_pushButton_saveimg_clicked()
{
    auto item = ui->listWidget_files->currentItem();
    if (!item) { return; }
    QDir save_dir(ui->pushButton_savedir->text());
    if (!save_dir.exists()){ return on_pushButton_savedir_clicked(); }
    QFileInfo save_file(save_dir.filePath(item->text())+"."+ui->comboBox_save_imgsuffix->currentText());
    auto pCurWidget = ui->tabWidget_2D->currentWidget();
    if (!pCurWidget) { return; }
    auto pWidget2D = dynamic_cast<CWidget2D*>(pCurWidget);
    if (!pWidget2D) { return; }
    auto spIMG = pWidget2D->GetImg();
    if (!spIMG) { return; }
    bool save_ok = [&](){
        if (ui->radioButton_newsize->isChecked()){
        auto dsc = spIMG->scaled({ui->spinBox_save_imgw->value(),ui->spinBox_save_imgh->value()});
        return dsc.save(save_file.absoluteFilePath());
        } else {
            return spIMG->save(save_file.absoluteFilePath());
        }
    }();
    if (!save_ok || !save_file.exists()){
        return SPFAIL("save image failed->"+save_file.absoluteFilePath())->tip();
    }
    SPOK("save image ok->"+save_file.absoluteFilePath())->tip();
    auto idx = m_mapFiles.keys().indexOf(item->text());
    if (idx>-1){
        ui->listWidget_files->item(idx)->setBackground(Qt::green);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Space: return on_pushButton_saveimg_clicked();
    }
    QWidget::keyPressEvent(event);
}




void MainWindow::on_radioButton_orgsize_clicked()
{
    slotProcess2D();
}


void MainWindow::on_spinBox_pensize_valueChanged(int arg1)
{
    SetCfg("spinBox_pensize", arg1);
    CWidget2D::Tool().size = arg1;
}


void MainWindow::on_tabWidget_2D_currentChanged(int index)
{
    SetCfg("tabWidget_2D", index);
}


void MainWindow::on_checkBox_editen_clicked()
{
    SetCfg("checkBox_editen", ui->checkBox_editen->isChecked());
    CWidget2D::Tool().enable = ui->checkBox_editen->isChecked();
}

