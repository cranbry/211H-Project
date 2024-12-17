#include "doctornpc.h"
#include <QTimer>
#include <QGraphicsSceneMouseEvent>

DoctorNPC::DoctorNPC(QObject *parent)
    : QObject(parent)
    , currentFrame(0)
{
    // Load the doctor's sprite sheet
    spriteSheet.load(":/res/doctor.png");
    setPixmap(spriteSheet.copy(0, 0, 100, 150));  // size of sprite. Get better sprite this one sucks...

    // Animation timer
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &DoctorNPC::updateAnimation);
    animationTimer->start(100);  // Lower = faster

    // doesnt do anything but breaks the code?
    setAcceptedMouseButtons(Qt::LeftButton);
}

void DoctorNPC::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit doctorClicked();
    QGraphicsItem::mousePressEvent(event);
}

// doesn't work yet.

void DoctorNPC::updateAnimation()
{
    // frame update on walk
    currentFrame = (currentFrame + 1) % 8;  // I think 8 frames no time work on later

    // Update the sprite
    setPixmap(spriteSheet.copy(currentFrame * 100, 0, 100, 150));  // sprite size
}
