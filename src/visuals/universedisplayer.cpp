#include "universedisplayer.hpp"
#include "celestial.hpp"
#include <QTimer>
#include <QSpinBox>
#include <QPushButton>
#include <QDateTimeEdit>>

UniverseDisplayer::UniverseDisplayer(QWidget *parent)
    : QWidget{parent}, m_physThread(m_universe, this)
{

    createCelestial(7.36e24_kg, 6.3e6_m,
                    {0_m, 0_m},                    // pos
                    {0_m / 1_sec, 0_m / 1_sec}, // v
                    Qt::green
                    );

    createCelestial(5.36e24_kg, 6.7e6_m,
                    {4e8_m, 0_m},                  // pos
                    {0_m / 1_sec, 8e2_m / 1_sec}, // v
                    Qt::blue
                    );

    createCelestial(5.36e24_kg, 6.7e6_m,
                    {-4e8_m, 0_m},                  // pos
                    {0_m / 1_sec, -8.001e2_m / 1_sec}, // v
                    Qt::red
                    );



    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000 / 60); // 60 fps
    m_timer->setSingleShot(false);
    connect(m_timer,SIGNAL(timeout()),this, SLOT(redraw()));
    m_timer->start();
}

UniverseDisplayer::~UniverseDisplayer()
{
    m_physThread.terminate();
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

void UniverseDisplayer::assignUniverseTimer(QDateTimeEdit* timer)
{
    m_watch = timer;
}

void UniverseDisplayer::createCelestial(phys::Mass mass, phys::Length radius, phys::Position pos, phys::Velocity v, QColor color)
{
    const phys::num_t Scale = 2e6l;
    Celestial* cel = new Celestial(mass, radius, this);
    cel->setPosition(pos);
    cel->setVelocity(v);
    cel->setColor(color);
    cel->rescale(Scale);
    m_celestials.push_back(cel);
    m_universe.addMaterialPoint(cel->getObject());
}

void UniverseDisplayer::setButtonState(bool running)
{
    m_stoper->setText(running ? "Stop" : "Run");
}

void UniverseDisplayer::redraw()
{
    bool stopped = m_physThread.getStopped();
    if(!stopped) {
        m_physThread.stop();
    }

    for(auto* celestial : m_celestials) {
        celestial->updatePosition();
    }

    emit displayEnergy(m_universe.getEnergy()->getVal() / 1e20);

    if(m_watch) {
        m_watch->setDateTime(QDateTime::fromSecsSinceEpoch(static_cast<uint64_t>(m_universe.getTime()->getVal())));
    }

    if(!stopped) {
        m_physThread.cont();
    }
}
