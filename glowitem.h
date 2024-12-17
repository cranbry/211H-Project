#ifndef GLOWITEM_H
#define GLOWITEM_H

#include <QObject>
#include <QGraphicsEllipseItem>

class GlowItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

public:
    GlowItem(QObject* parent = nullptr) : QObject(parent), QGraphicsEllipseItem() {}
    qreal scale() const { return m_scale; }
    void setScale(qreal scale) {
        m_scale = scale;
        QGraphicsEllipseItem::setScale(scale);
    }

private:
    qreal m_scale = 1.0;
};
#endif
