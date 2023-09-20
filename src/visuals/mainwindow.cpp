#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_ud = new UniverseDisplayer(this);
    m_ud->setGeometry(rect());

    ui->setupUi(this);

    connect(m_ud, SIGNAL(displayEnergy(double)), ui->doubleSpinBox, SLOT(setValue(double)));
    m_ud->assignSpinBox(ui->spinBox);
    m_ud->assignStopButton(ui->pushButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    m_ud->setGeometry(rect());
}

