#ifndef PHYSICSTHREAD_HPP
#define PHYSICSTHREAD_HPP

#include <QThread>
#include <QMutex>

#include "universe.hpp"

class PhysicsThread : public QThread
{
    Q_OBJECT
public:
    PhysicsThread(phys::Universe& universe, QObject* parent = nullptr);
    ~PhysicsThread() override;

public slots:
    void stop() {
        m_mutex.lock();
        m_stopped = true;
        m_mutex.unlock();
        wait();
        emit toggled(false);
    }

    void cont() {
        m_mutex.lock();
        if(m_stopped) {
            m_stopped = false;
            start(LowPriority);
            emit toggled(true);
        }
        m_mutex.unlock();
    }

    void toggle() {
        m_mutex.lock();
        m_stopped ^= 1;
        if(m_stopped) {
            m_mutex.unlock();
            wait();
            emit toggled(false);
            return;
        } else {
            start(LowPriority);
            emit toggled(true);
        }
        m_mutex.unlock();
    }

public:
    [[nodiscard]] unsigned long getPeriod() const;
    [[nodiscard]] bool getStopped();

signals:
    void toggled(bool);

public slots:
    void setPeriod(int newPeriod);

protected:
    void run() override;

private:
    phys::Universe& m_universe;
    QMutex m_mutex;
    bool m_stopped = true;
    unsigned long m_period = 5;
};

#endif // PHYSICSTHREAD_HPP
