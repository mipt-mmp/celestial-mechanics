#include "celestial.hpp"
#include "celestialstatedialog.hpp"
#include <QPainter>

Celestial::Celestial(phys::Mass mass, phys::Length radius, QWidget *parent)
    : QWidget{parent}, m_object({}, {}, mass), m_radius(radius)
{
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::transparent);
    setAutoFillBackground(true);
    setPalette(pal);
    setDisabled(true);
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
    pen.setColor(m_color);
    painter.setPen(pen);
    QBrush brush(Qt::SolidPattern);
    brush.setColor(m_color);
    painter.setBrush(brush);
    painter.drawEllipse(1, 1, m_display_size, m_display_size);
}

void Celestial::updatePosition()
{
    QPoint pos = qobject_cast<QWidget *>(parent())->rect().center();
    QPoint offs{scaled(*m_object.getPos().X()), scaled(*m_object.getPos().Y())};
    pos += offs;
    setGeometry(pos.x(), pos.y(), m_display_size + 4, m_display_size + 4);
}

void Celestial::rescale(phys::num_t newScale)
{
    m_scale = newScale;
    m_display_size = 2 * scaled(*m_radius);
    m_display_size = std::max(2, m_display_size);
    updatePosition();
}

void Celestial::edit()
{
    CelestialStateDialog dialog;
}
