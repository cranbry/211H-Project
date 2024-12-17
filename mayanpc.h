#ifndef MAYANPC_H
#define MAYANPC_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsSceneMouseEvent>

class MayaNPC : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)

public:
    explicit MayaNPC(QGraphicsItem* parent = nullptr) : QObject(), QGraphicsPixmapItem(parent) {
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::LeftButton);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        emit mayaClicked();
        QGraphicsItem::mousePressEvent(event);
    }

signals:
    void mayaClicked();
};

#endif // MAYANPC_H
