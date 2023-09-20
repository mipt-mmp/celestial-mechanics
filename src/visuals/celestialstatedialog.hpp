#ifndef CELESTIALSTATEDIALOG_H
#define CELESTIALSTATEDIALOG_H

#include <QDialog>

namespace Ui {
class CelestialStateDialog;
}

class CelestialStateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CelestialStateDialog(QWidget *parent = nullptr);
    ~CelestialStateDialog() override;

private:
    Ui::CelestialStateDialog *ui;
};

#endif // CELESTIALSTATEDIALOG_H
