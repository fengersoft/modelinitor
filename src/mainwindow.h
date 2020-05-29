#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QListWidget>
#include <QMainWindow>
#include <QSettings>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void loadModelNames();
    void showFiles(QString parentPath, QStandardItem* parentItem = nullptr);
    void loadConfig();
    void saveConfig();
    void copyFiles(QStandardItem* parentItem = nullptr);

private slots:
    void on_lvProjects_itemClicked(QListWidgetItem* item);

    void on_btnOpenParentFolder_clicked();

    void on_btnRun_clicked();

    void on_btnOpenFolder_clicked();

private:
    Ui::MainWindow* ui;
    QStandardItemModel* m_model;
    QString m_srcPath;
    QString m_destPath;
};
#endif // MAINWINDOW_H
