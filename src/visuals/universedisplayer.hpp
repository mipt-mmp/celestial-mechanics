#ifndef UNIVERSEDISPLAYER_HPP
#define UNIVERSEDISPLAYER_HPP

#include "celestial.hpp"
#include "universe.hpp"
#include "physicsthread.hpp"
#include <QWidget>
#include <QVector>

class QSpinBox;
class QPushButton;
class QDateTimeEdit;

class UniverseDisplayer : public QWidget
{
    Q_OBJECT
public:
    explicit UniverseDisplayer(QWidget *parent = nullptr);
    ~UniverseDisplayer() override;

    void assignSpinBox(QSpinBox* box);
    void assignStopButton(QPushButton* button);
    void assignUniverseTimer(QDateTimeEdit* timer);
private:
    phys::Universe m_universe;
    QTimer* m_timer;

    PhysicsThread m_physThread;

    QPushButton* m_stoper = nullptr;
    QDateTimeEdit* m_watch= nullptr;

    QVector<Celestial* > m_celestials;

    void createCelestial(phys::Mass mass, phys::Length radius, phys::Position pos, phys::Velocity v, QColor color = Qt::white);

private slots:
    void setButtonState(bool);

public slots:
    void redraw();
signals:
    void recalced();

    void displayEnergy(double e);
};

#endif // UNIVERSEDISPLAYER_HPP
