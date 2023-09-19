#include "celestial.hpp"
#include <QPainter>

Celestial::Celestial(phys::Mass mass, phys::Length radius, QWidget *parent)
    : QWidget{parent}, m_object({}, {}, mass), m_radius(radius)
{
    setGeometry(0, 0, 100, 100);
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::transparent);
    setAutoFillBackground(true);
    setPalette(pal);
    updatePosition();
//    connect(this, SIGNAL(update()), this, SLOT(updatePosition()));
}

phys::MaterialPoint* Celestial::getObject()
{
    return &m_object;
}

void Celestial::setColor(const QColor &newColor)
{
    m_color = newColor;
}

void Celestial::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(3);
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(m_color);
    painter.setPen(pen);
    painter.drawEllipse(rect().center(), std::max(2,scaled(2**m_radius)), std::max(2, scaled(2**m_radius)));
}

void Celestial::updatePosition()
{
    QPoint pos = qobject_cast<QWidget *>(parent())->rect().center();
    QPoint offs{scaled(*m_object.getPos().X()), scaled(*m_object.getPos().Y())};
    pos += offs;
    setGeometry(pos.x(), pos.y(), 100, 100);
}
