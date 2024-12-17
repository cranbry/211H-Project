#ifndef DOCTORNPC_H
#define DOCTORNPC_H

#include <QGraphicsPixmapItem>
#include <QObject>

class DoctorNPC : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)

public:
    explicit DoctorNPC(QObject *parent = nullptr);
    void setX(qreal x) { setPos(x, y()); }
    void setY(qreal y) { setPos(x(), y); }

signals:
    void doctorClicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int currentFrame;
    QPixmap spriteSheet;
    void updateAnimation();
    QTimer* animationTimer;
};

#endif // DOCTORNPC_H
