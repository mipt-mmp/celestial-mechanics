#ifndef UNIVERSEDISPLAYER_HPP
#define UNIVERSEDISPLAYER_HPP

#include "celestial.hpp"
#include "universe.hpp"
#include "physicsthread.hpp"
#include <QWidget>
#include <QVector>

class QSpinBox;
class QPushButton;

class UniverseDisplayer : public QWidget
{
    Q_OBJECT
public:
    explicit UniverseDisplayer(QWidget *parent = nullptr);
    ~UniverseDisplayer() override;

    void assignStopButton(QPushButton* button); //FIXME: No more valid way of doing things
    void assignSpeedBox(QSpinBox *box);

    [[nodiscard]] const phys::Universe::Metrics& getUniverseMetrics() const;

private:
    phys::Universe m_universe;
    phys::Universe::Metrics m_universeMetrics;

    QTimer* m_timer;

    PhysicsThread m_physThread;

    QPushButton* m_stoper = nullptr;

    QVector<Celestial* > m_celestials;

    void createCelestial(phys::Mass mass, phys::Length radius, phys::Position pos, phys::Velocity v, QColor color = Qt::white);
    void addCelestial(Celestial* celestial);
    void clear();

private slots:
    void setButtonState(bool);

public slots:
    void redraw();

signals:
    void recalced();
};

#endif // UNIVERSEDISPLAYER_HPP
