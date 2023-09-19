#include "universedisplayer.hpp"
#include "celestial.hpp"
#include <QTimer>

UniverseDisplayer::UniverseDisplayer(QWidget *parent)
    : QWidget{parent}
{
    m_earth = new Celestial(5.9e24_kg, 6.3e6_m, this);
    m_earth->setColor(Qt::green);
    m_earth->setVelocity({0_m / 1_sec, -1e1_m / 1_sec});
    m_universe.addMaterialPoint(m_earth->getObject());
    connect(this, SIGNAL(recalced()), m_earth, SLOT(updatePosition()));

    m_moon = new Celestial(7.36e22_kg, 1.7e6_m, this);
    m_moon->setColor(Qt::blue);
    m_moon->setVelocity({0_m / 1_sec, 1e3_m / 1_sec});
    m_moon->setPosition({4e8_m});
    m_universe.addMaterialPoint(m_moon->getObject());
    connect(this, SIGNAL(recalced()), m_moon, SLOT(updatePosition()));


    m_timer=new QTimer(this);
    m_timer->setInterval(10);
    m_timer->setSingleShot(false);
    connect(m_timer,SIGNAL(timeout()),this, SLOT(redraw()));
    m_timer->start();
}

void UniverseDisplayer::redraw()
{
    m_universe.simulateStep();
    emit recalced();
}
