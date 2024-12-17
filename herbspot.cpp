#include "herbspot.h"
#include <QPainter>
#include <QGraphicsEffect>

HerbSpot::HerbSpot(qreal x, qreal y, QGraphicsItem* parent)
    : QGraphicsEllipseItem(0, 0, 30, 30, parent)
{
    setPos(x, y);
    setAcceptHoverEvents(true);
    setBrush(QBrush(QColor(100, 255, 100, 100)));  // Semi-transparent green
    setPen(QPen(Qt::white, 2));
    setAcceptedMouseButtons(Qt::LeftButton);
    updateAppearance();
}

void HerbSpot::setCollected(bool isCollected) {
    collected = isCollected;
    updateAppearance();
}

void HerbSpot::updateAppearance() {
    if (collected) {
        setBrush(QBrush(QColor(50, 50, 50, 100)));  // Grayed out when collected
        setEnabled(false);
    }
}

void HerbSpot::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    if (!collected) {
        setBrush(QBrush(QColor(150, 255, 150, 150)));  // Brighter when hovered
    }
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

void HerbSpot::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    if (!collected) {
        setBrush(QBrush(QColor(100, 255, 100, 100)));  // Normal state
    }
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

void HerbSpot::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (!collected) {
        emit herbCollected();
        setCollected(true);
    }
    QGraphicsEllipseItem::mousePressEvent(event);
}
