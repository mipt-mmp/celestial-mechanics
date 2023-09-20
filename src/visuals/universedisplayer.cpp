#include "universedisplayer.hpp"
#include "celestial.hpp"
#include <QTimer>
#include <QSpinBox>
#include <QPushButton>

UniverseDisplayer::UniverseDisplayer(QWidget *parent)
    : QWidget{parent}, m_physThread(m_universe, this)
{
    m_earth = new Celestial(5.9e24_kg, 6.3e6_m, this);
    m_earth->setColor(Qt::green);
    m_earth->setVelocity({0_m / 1_sec, 0_m / 1_sec});
    m_universe.addMaterialPoint(m_earth->getObject());

    m_moon = new Celestial(7.36e2_kg, 1.7e6_m, this);
    m_moon->setColor(Qt::blue);
    m_moon->setVelocity({0_m / 1_sec, 1.2e3_m / 1_sec});
    m_moon->setPosition({4e8_m});
    m_universe.addMaterialPoint(m_moon->getObject());

    const phys::num_t Scale = 2e6l;
    m_moon->rescale(Scale);
    m_earth->rescale(Scale);

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000 / 60); // 60 fps
    m_timer->setSingleShot(false);
    connect(m_timer,SIGNAL(timeout()),this, SLOT(redraw()));
    m_timer->start();

    m_physThread.cont();
}

void UniverseDisplayer::assignSpinBox(QSpinBox *box)
{
    box->setValue(m_physThread.getPeriod());
    connect(box, SIGNAL(valueChanged(int)), &m_physThread, SLOT(setPeriod(int)));
}

void UniverseDisplayer::assignStopButton(QPushButton *button)
{
    assert(m_stoper == nullptr);
    m_stoper = button;
    setButtonState(true);
    connect(button, SIGNAL(clicked()), &m_physThread, SLOT(toggle()));
    connect(&m_physThread, SIGNAL(toggled(bool)), this, SLOT(setButtonState(bool)));
}

void UniverseDisplayer::setButtonState(bool running)
{
    m_stoper->setText(running ? "Stop" : "False");
}

void UniverseDisplayer::redraw()
{
    bool stopped = m_physThread.getStopped();
    if(!stopped) {
        m_physThread.stop();
    }

    m_earth->updatePosition();
    m_moon-> updatePosition();
    emit displayEnergy(static_cast<double>(*m_universe.getEnergy()));

    if(!stopped) {
        m_physThread.cont();
    }
}
