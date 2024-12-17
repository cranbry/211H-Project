#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QAudioOutput>

class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    void setCanMove(bool can);
    void stopMovement();
    explicit Player(QObject *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void setMovingRight(bool moving) { movingRight = moving; updateSprite(); }
    void setMovingLeft(bool moving) { movingLeft = moving; updateSprite(); }

signals:
    void positionChanged();

private:
    qreal sceneWidth = 1060;  // Match your scene width
    bool canMove = true;      // To control when player can move
    QMediaPlayer *walkingSound;
    QAudioOutput *audioOutput;
    bool facingRight;
    int animationSpeed;
    int animationCounter;
    void updateSprite();
    QPixmap spriteSheet;
    int currentFrame;
    bool movingLeft;
    bool movingRight;
    qreal speed;
    QTimer *animationTimer;

    int footstepTimer;
    void playFootstep();
};
#endif // PLAYER_H
