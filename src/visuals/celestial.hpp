#ifndef CELESTIAL_HPP
#define CELESTIAL_HPP

#include "materialpoint.hpp"
#include <QWidget>

const long double Scale = 3e6l;

class Celestial : public QWidget
{
    Q_OBJECT
public:
    explicit Celestial(phys::Mass mass, phys::Length radius, QWidget *parent = nullptr);

    [[nodiscard]] phys::MaterialPoint* getObject();

    void setPosition(const phys::Position& pos) {m_object.setPos(pos);}

    void setVelocity(const phys::Velocity& v) {m_object.setVelocity(v);}

    void setColor(const QColor &newColor);

private:
    QColor m_color;
    phys::MaterialPoint m_object = {{}, {}, 1_kg};
    phys::Length m_radius = 1_m;


    void paintEvent(QPaintEvent* event) override;
public slots:
    void updatePosition();

    static int scaled(long double x) {
        return static_cast<int>(x / Scale);
    }
signals:

};

#endif // CELESTIAL_HPP
