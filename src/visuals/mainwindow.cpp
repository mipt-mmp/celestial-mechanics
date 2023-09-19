#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "universedisplayer.hpp"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto* ud = new UniverseDisplayer(this);
    ud->setGeometry(0, 0, 500, 500);
    bool success = connect(ui->pushButton, SIGNAL(pressed()), ud, SLOT(redraw()));
    Q_ASSERT(success);
}

MainWindow::~MainWindow()
{
    delete ui;
}

