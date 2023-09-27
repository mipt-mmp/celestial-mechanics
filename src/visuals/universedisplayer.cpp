#include "universedisplayer.hpp"
#include "celestial.hpp"
#include <QTimer>
#include <QSpinBox>
#include <QPushButton>
#include <QDateTimeEdit>

UniverseDisplayer::UniverseDisplayer(QWidget *parent)
    : QWidget{parent}, m_physThread(m_universe, this)
{

    {
    createCelestial(1.99e30_kg, 6.9e8_m,
                    {0_m, 0_m},                    // pos
                    {0_m / 1_sec, 0_m / 1_sec}, // v
                    Qt::yellow
                    );

    createCelestial(5.9e24_kg, 6.3e6_m,
                    {1.5e11_m, 0_m},                  // pos
                    {0_m / 1_sec, 30e3_m/ 1_sec}, // v
                    Qt::blue
                    );

    createCelestial(7.36e22_kg, 1.7e6_m,
                    {1.5e11_m + 3.8e8_m, 0_m},                  // pos
                    {0_m / 1_sec, 30e3_m / 1_sec, 1.03e3_m / 1_sec}, // v
                    Qt::white
                    );
}

//{
//    createCelestial(5.9e24_kg, 6.3e6_m,
//                    {1.5e11_m, 0_m},                  // pos
//                    {0_m / 1_sec}, // v
//                    Qt::blue
//                    );

//    createCelestial(7.36e2_kg, 1.7e6_m,
//                    {1.5e11_m + 3.8e8_m, 0_m},                  // pos
//                    {0_m / 1_sec, 1.23e3_m / 1_sec}, // v
//                    Qt::white
//                    );
//}
    m_universe.shiftMassCenter();
    m_universe.recalcOptimalDt();
    redraw();

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000 / 60); // 60 fps
    m_timer->setSingleShot(false);
    connect(m_timer,SIGNAL(timeout()), this, SLOT(redraw()));
    m_timer->start();
}

UniverseDisplayer::~UniverseDisplayer()
{
    m_physThread.terminate();
}

void UniverseDisplayer::assignSpeedBox(QSpinBox *box)
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

const phys::Universe::Metrics& UniverseDisplayer::getUniverseMetrics() const
{
    return m_universeMetrics;
}

void UniverseDisplayer::createCelestial(phys::Mass mass, phys::Length radius, phys::Position pos, phys::Velocity v, QColor color)
{
    Celestial* cel = new Celestial(mass, radius, this);
    cel->setPosition(pos);
    cel->setVelocity(v);
    cel->setColor(color);
    addCelestial(cel);
}

void UniverseDisplayer::addCelestial(Celestial* celestial)
{
    celestial->setParent(this);
    const phys::num_t Scale = 3.3e8l;
//    const phys::num_t Scale = 2e6l;
    celestial->rescale(Scale);
    m_celestials.push_back(celestial);
    m_universe.addMaterialPoint(celestial->getObject());
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

    std::sort(m_celestials.begin(), m_celestials.end(), [](const Celestial* lhs, const Celestial* rhs) -> bool{
        return lhs->getZ() < rhs->getZ();
    });

    for(qsizetype i = 0; i < m_celestials.size()-1; ++i) {
        m_celestials[i]->stackUnder(m_celestials[i+1]);
    }

    for(auto* celestial : m_celestials) {
        celestial->updatePosition();
    }

    m_universeMetrics = m_universe.getMetrics();

    emit recalced();

    if(!stopped) {
        m_physThread.cont();
    }
}
