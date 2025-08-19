#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QIcon>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , fileOrganizer(new Organizer(this))  // initialize organizing core
{
    ui->setupUi(this);

    // ---------- Initialize UI state ----------
    ui->lblStatus->setText("Ready");
    ui->progressBar->setValue(0);
    ui->btnStart->setEnabled(true);  // ensure the button is clickable

    // ---------- Connect signals and slots ----------
    connect(ui->btnSelectDir, &QPushButton::clicked, this, &MainWindow::onSelectDir);
    connect(ui->chkWatch,     &QCheckBox::toggled,   this, &MainWindow::onWatchToggle);
    connect(ui->btnStart,     &QPushButton::clicked, this, &MainWindow::startOrganizing);

    // Organizer core signals
    connect(fileOrganizer, &Organizer::progressChanged, this, &MainWindow::updateProgress);
    connect(fileOrganizer, &Organizer::finished,        this, &MainWindow::onOrganizingFinished);

    // ---------- System Tray (menu bar) ----------
    // uses the PNG you added in resources.qrc :/icons/icon.png
    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/icons/icon.png"));
    tray->setToolTip("File Organizer Assistant");

    trayMenu   = new QMenu(this);
    actShowHide = trayMenu->addAction("Show/Hide Window");
    actStart    = trayMenu->addAction("Start Organizing");
    actStop     = trayMenu->addAction("Stop Watching");
    trayMenu->addSeparator();
    actQuit     = trayMenu->addAction("Quit");

    tray->setContextMenu(trayMenu);
    tray->show();

    // Tray actions
    connect(actShowHide, &QAction::triggered, this, [this]{
        if (isVisible()) {
            hide();
        } else {
            show();
            raise();
            activateWindow();
        }
    });

    connect(actStart, &QAction::triggered, this, [this]{
        startOrganizing();
    });

    connect(actStop, &QAction::triggered, this, [this]{
        if (ui->chkWatch->isChecked())
            ui->chkWatch->setChecked(false);
        ui->lblStatus->setText("Watching stopped");
    });

    connect(actQuit, &QAction::triggered, qApp, &QApplication::quit);

    // Optional: first-time hint
    tray->showMessage("Running in Menu Bar",
                      "Click the tray icon to open or quit.",
                      QSystemTrayIcon::Information, 3000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileOrganizer;
}

// ---------- Slots ----------
void MainWindow::onSelectDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory");
    if (!dir.isEmpty()) {
        ui->txtPath->setText(dir);
        ui->lblStatus->setText("Selected: " + dir);
    }
}

void MainWindow::onWatchToggle(bool checked)
{
    if (ui->txtPath->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a directory first!");
        ui->chkWatch->setChecked(false);  // auto uncheck
        return;
    }

    ui->lblStatus->setText(checked ? "Watching enabled" : "Watching stopped");

    // Actual watching logic
    if (checked) {
        fileOrganizer->startWatching(ui->txtPath->text());
    } else {
        fileOrganizer->stopWatching();
    }
}

void MainWindow::startOrganizing()
{
    if (ui->txtPath->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a directory first!");
        return;
    }

    ui->lblStatus->setText("Organizing...");
    ui->btnStart->setEnabled(false);  // disable to prevent repeated clicks
    ui->progressBar->setValue(0);     // reset progress bar

    // Kick off organizing
    fileOrganizer->organizeDirectory(ui->txtPath->text());
}

void MainWindow::updateProgress(int value)
{
    ui->progressBar->setValue(value);
    ui->lblStatus->setText(QString("Organizing... %1%").arg(value));
}

void MainWindow::onOrganizingFinished()
{
    ui->lblStatus->setText("Finished!");
    ui->btnStart->setEnabled(true);  // re-enable button
    ui->progressBar->setValue(100);

    // Optionally stop watching
    if (ui->chkWatch->isChecked()) {
        ui->chkWatch->setChecked(false);
    }
}

// ---------- Close -> minimize to tray ----------
void MainWindow::closeEvent(QCloseEvent* e)
{
    if (tray && tray->isVisible()) {
        hide();
        tray->showMessage("Still running",
                          "The app is minimized to the menu bar.",
                          QSystemTrayIcon::Information, 2000);
        e->ignore();
    } else {
        e->accept();
    }
}
