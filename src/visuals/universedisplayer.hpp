#ifndef UNIVERSEDISPLAYER_HPP
#define UNIVERSEDISPLAYER_HPP

#include "celestial.hpp"
#include "universe.hpp"
#include <QWidget>

class UniverseDisplayer : public QWidget
{
    Q_OBJECT
public:
    explicit UniverseDisplayer(QWidget *parent = nullptr);

private:
    phys::Universe m_universe;
    QTimer* m_timer;

    Celestial* m_earth;
    Celestial* m_moon;
public slots:
    void redraw();
signals:
    void recalced();
};

#endif // UNIVERSEDISPLAYER_HPP
