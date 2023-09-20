#include "physicsthread.hpp"
#include <QMutexLocker>
PhysicsThread::PhysicsThread(phys::Universe& universe, QObject* parent) : QThread(parent), m_universe(universe)
{
}

PhysicsThread::~PhysicsThread()
{
    stop();
}

void PhysicsThread::run()
{
    forever {
        {
            QMutexLocker lock(&m_mutex);
            if(m_stopped) {
                break;
            }
            m_universe.simulateStep();
        }
        msleep(m_period);
    }
}

bool PhysicsThread::getStopped()
{
    QMutexLocker lock(&m_mutex);
    return m_stopped;
}

unsigned long PhysicsThread::getPeriod() const
{
    return m_period;
}

void PhysicsThread::setPeriod(int newPeriod)
{
    QMutexLocker lock(&m_mutex);
    m_period = newPeriod;
}
