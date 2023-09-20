#ifndef UNIVERSEDISPLAYER_HPP
#define UNIVERSEDISPLAYER_HPP

#include "celestial.hpp"
#include "universe.hpp"
#include "physicsthread.hpp"
#include <QWidget>

class QSpinBox;
class QPushButton;

class UniverseDisplayer : public QWidget
{
    Q_OBJECT
public:
    explicit UniverseDisplayer(QWidget *parent = nullptr);

    void assignSpinBox(QSpinBox* box);
    void assignStopButton(QPushButton* button);
private:
    phys::Universe m_universe;
    QTimer* m_timer;

    PhysicsThread m_physThread;

    Celestial* m_earth;
    Celestial* m_moon;

    QPushButton* m_stoper = nullptr;
private slots:
    void setButtonState(bool);

public slots:
    void redraw();
signals:
    void recalced();

    void displayEnergy(double e);
};

#endif // UNIVERSEDISPLAYER_HPP
