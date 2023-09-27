#ifndef CELESTIAL_HPP
#define CELESTIAL_HPP

#include "materialpoint.hpp"
#include <QWidget>
class QTextStream;
class Celestial : public QWidget
{
    Q_OBJECT
public:
    explicit Celestial(phys::Mass mass, phys::Length radius, QWidget *parent = nullptr);

    [[nodiscard]] phys::MaterialPoint* getObject();

    void setPosition(const phys::Position& pos) {m_object.setPos(pos);}

    void setVelocity(const phys::Velocity& v) {m_object.setVelocity(v);}

    void setColor(const QColor &newColor);

    [[nodiscard]] QString getName() const;
    void setName(const QString& newName);

    phys::num_t getZ() const { return *m_object.getPos().Z(); }

private:
    QString m_name;
    QColor m_color;
    phys::MaterialPoint m_object = {{}, {}, 1_kg};
    phys::Length m_radius = 1_m;

    int m_display_size;

    phys::num_t m_scale;

    void paintEvent(QPaintEvent* event) override;

public slots:
    void updatePosition();

    void rescale(phys::num_t newScale);

    int scaled(phys::num_t x) {
        return static_cast<int>(x / m_scale);
    }

private slots:
    void edit();
signals:

};


QTextStream& operator>>(QTextStream& in, Celestial& cst);

#endif // CELESTIAL_HPP
