#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>

#include "organizer.h"     // 整理核心

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onSelectDir();            // 选择目录
    void onWatchToggle(bool);      // 监控开关
    void startOrganizing();        // 开始整理
    void updateProgress(int);      // 更新进度
    void onOrganizingFinished();   // 整理完成

protected:
    void closeEvent(QCloseEvent* e) override;

private:
    Ui::MainWindow *ui = nullptr;
    Organizer *fileOrganizer = nullptr;        // 整理核心对象

    // Tray
    QSystemTrayIcon* tray = nullptr;
    QMenu* trayMenu = nullptr;
    QAction* actShowHide = nullptr;
    QAction* actStart = nullptr;
    QAction* actStop = nullptr;
    QAction* actQuit = nullptr;
};

#endif // MAINWINDOW_H
