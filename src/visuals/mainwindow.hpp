#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "universedisplayer.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void resizeEvent(QResizeEvent* event) override;

private slots:
    void updateMetrics();

private:
    UniverseDisplayer* m_ud;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
