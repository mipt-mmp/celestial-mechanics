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

    connect(m_ud, SIGNAL(recalced()), this, SLOT(updateMetrics()));
    connect(ui->fluctuationPeriodSetter, SIGNAL(valueChanged(double)), m_ud, SLOT(setFluctuationPeriod(double)));

    m_ud->assignStopButton(ui->pushButton);
    m_ud->assignSpeedBox(ui->speedBox);
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

void MainWindow::updateMetrics()
{
    ui->time->setDateTime(QDateTime::fromSecsSinceEpoch(static_cast<uint64_t>(m_ud->getUniverseMetrics().time->getVal())));

    QString str;
    QTextStream ss(&str);
    ss << m_ud->getUniverseMetrics().energy;
    ui->eDisplay->setText(str);

    str.clear();
    ss << m_ud->getUniverseMetrics().impulse.Len();
    ui->pDisplay->setText(str);

    str.clear();
    ss << m_ud->getUniverseMetrics().impulsemoment.Len();
    ui->pmDisplay->setText(str);
}

