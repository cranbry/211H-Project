#ifndef HERBSPOT_H
#define HERBSPOT_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QGraphicsSceneHoverEvent>

class HerbSpot : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    explicit HerbSpot(qreal x, qreal y, QGraphicsItem* parent = nullptr);
    void setCollected(bool collected);
    bool isCollected() const { return collected; }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

signals:
    void herbCollected();

private:
    bool collected = false;
    void updateAppearance();
};

#endif // HERBSPOT_H
