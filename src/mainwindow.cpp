#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_model = new QStandardItemModel(this);
    ui->tvFile->setModel(m_model);
    ui->tvFile->setHeaderHidden(true);
    loadModelNames();
    loadConfig();
}

MainWindow::~MainWindow()
{
    saveConfig();
    delete ui;
}

void MainWindow::loadModelNames()
{
    ui->lvProjects->clear();
    QDir dir(QApplication::applicationDirPath() + "/models");

    QFileInfoList files = dir.entryInfoList();
    for (QFileInfo info : files) {
        if ((info.isDir()) && (info.fileName() != ".") && (info.fileName() != "..")) {
            ui->lvProjects->addItem(info.fileName());
        }
    }
}

void MainWindow::showFiles(QString parentPath, QStandardItem* parentItem)
{
    QStandardItem* item = nullptr;
    if (parentItem == nullptr) {
        m_model->clear();
    }
    QDir dir(parentPath);
    QFileInfoList files = dir.entryInfoList();

    for (QFileInfo info : files) {
        if ((info.fileName() != ".") && (info.fileName() != "..")) {
            item = new QStandardItem();
            item->setText(info.fileName());
            item->setData(info.absoluteFilePath());
            if (parentItem == nullptr) {

                m_model->appendRow(item);
            } else {
                parentItem->appendRow(item);
            }
            if (info.isDir()) {
                item->setIcon(QIcon(":/res/folder.ico"));
                showFiles(info.absoluteFilePath(), item);
            } else if (info.isFile()) {

                item->setIcon(QIcon(":/res/file.png"));
            }
        }
    }
}

void MainWindow::loadConfig()
{
    QSettings ini(QApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
    ini.beginGroup("set");
    ui->edtPath->setText(ini.value("path", "").toString());
    ini.endGroup();
}

void MainWindow::saveConfig()
{

    QSettings ini(QApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
    ini.beginGroup("set");
    ini.setValue("path", ui->edtPath->text());
    ini.endGroup();
}

void MainWindow::copyFiles(QStandardItem* parentItem)
{
    QStandardItem* item = nullptr;
    if (parentItem == nullptr) {
        for (int i = 0; i < m_model->rowCount(); i++) {
            item = m_model->item(i);
            QFileInfo info(item->data().toString());
            QString path = info.absoluteFilePath();
            path = path.replace(m_srcPath, m_destPath);

            if (info.isDir()) {
                QDir dir;
                dir.mkpath(path);
                copyFiles(item);
            } else {
                QFile file(item->data().toString());
                if (!file.exists()) {
                    file.copy(path);
                }
            }
        }
    } else {
        for (int i = 0; i < parentItem->rowCount(); i++) {
            item = parentItem->child(i);
            QFileInfo info(item->data().toString());
            QString path = info.absoluteFilePath();
            path = path.replace(m_srcPath, m_destPath);

            if (info.isDir()) {
                QDir dir;
                dir.mkpath(path);
                copyFiles(item);
            } else {
                QFile file(item->data().toString());
                if (!file.exists()) {
                    file.copy(path);
                }
            }
        }
    }
}

void MainWindow::on_lvProjects_itemClicked(QListWidgetItem* item)
{

    m_srcPath = QApplication::applicationDirPath() + "/models/" + item->text();
    showFiles(QApplication::applicationDirPath() + "/models/" + item->text());

    ui->tvFile->expandAll();
}

void MainWindow::on_btnOpenParentFolder_clicked()
{
    QString path = QFileDialog::getExistingDirectory();
    if (path == "") {
        return;
    }
    ui->edtPath->setText(path + "/");
}

void MainWindow::on_btnRun_clicked()
{
    m_destPath = ui->edtPath->text() + ui->edtName->text();
    QDir dir(m_destPath);
    if (!dir.exists()) {
        dir.mkpath(m_destPath);
    }
    copyFiles(nullptr);
}

void MainWindow::on_btnOpenFolder_clicked()
{
    m_destPath = ui->edtPath->text() + ui->edtName->text();
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_destPath));
}
