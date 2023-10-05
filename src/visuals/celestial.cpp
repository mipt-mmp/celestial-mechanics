#include "celestial.hpp"
#include "celestialstatedialog.hpp"
#include <QPainter>
#include <QTextStream>

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

QString Celestial::getName() const
{
    return m_name;
}

void Celestial::setName(const QString& newName)
{
    m_name = newName;
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
    painter.drawEllipse(7, 7, m_display_size, m_display_size);
}

void Celestial::updatePosition()
{
    QPoint pos = qobject_cast<QWidget *>(parent())->rect().center();
    QPoint offs{scaled(*m_object.getPos().X()), scaled(*m_object.getPos().Y())};
    pos += offs;
    setGeometry(pos.x(), pos.y(), m_display_size + 16, m_display_size + 16);
}

void Celestial::rescale(phys::num_t newScale)
{
    m_scale = newScale;
    m_display_size = 2 * scaled(*m_radius);
    m_display_size = std::max(8, m_display_size);
    updatePosition();
}

void Celestial::edit()
{
    CelestialStateDialog dialog;
}

#define CHECK_CHAR(c)                               \
    do {                                            \
    QChar _ch;                                      \
    in.skipWhiteSpace();                            \
    in >> _ch;                                      \
    if(_ch != c) {                                  \
        in.setStatus(QTextStream::ReadCorruptData); \
        return in;                                  \
    }                                               \
    } while(0)

QTextStream& operator>>(QTextStream& in, Celestial& cst) {
    CHECK_CHAR('{');
    QString str;
    if(!(in >> str).status()) {
        return in;
    }
    CHECK_CHAR(',');

    uint32_t int_color;
    if(!(in >> int_color).status()) {return in;}
    QColor color(int_color);

    phys::Mass mass;
    if(!(in >> mass).status()) {return in;}

    phys::Position pos;
    {
        CHECK_CHAR('{');
        if(!(in >> pos[0]).status()) {return in;}
        CHECK_CHAR(',');
        if(!(in >> pos[1]).status()) {return in;}
        CHECK_CHAR(',');
        if(!(in >> pos[2]).status()) {return in;}
        CHECK_CHAR('}');
    }

    phys::Velocity v;
    {
        CHECK_CHAR('{');
        if(!(in >> v[0]).status()) {return in;}
        CHECK_CHAR(',');
        if(!(in >> v[1]).status()) {return in;}
        CHECK_CHAR(',');
        if(!(in >> v[2]).status()) {return in;}
        CHECK_CHAR('}');
    }
    CHECK_CHAR('}');

    cst.getObject()->setMass(mass);
    cst.getObject()->setPos(pos);
    cst.getObject()->setVelocity(v);
    cst.setColor(color);


    return in;

}

#undef CHECK_CHAR
