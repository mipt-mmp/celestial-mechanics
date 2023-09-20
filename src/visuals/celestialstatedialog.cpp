#include "celestialstatedialog.hpp"
#include "ui_celestialstatedialog.h"

CelestialStateDialog::CelestialStateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CelestialStateDialog)
{
    ui->setupUi(this);
}

CelestialStateDialog::~CelestialStateDialog()
{
    delete ui;
}
