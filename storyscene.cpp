#include "storyscene.h"
#include "creditscene.h"
#include "quiz.h"
#include <QGraphicsOpacityEffect>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QApplication>
#include <QScreen>
#include <QMediaPlayer>
#include <QAudioOutput>

StoryScene::StoryScene(QWidget *parent)
    : QWidget(parent)
{
    storyManager = new StoryManager(this);
    storyManager->loadStory("chapter1");

    // Connect quiz triggers
    connect(storyManager, &StoryManager::startQuiz,
            this, &StoryScene::showQuiz);
    connect(storyManager, &StoryManager::trustLevelChanged,
            this, &StoryScene::updateTrustMeter);
    setupScene();
    showMaya();
}

void StoryScene::setupScene()
{
    // Create scene and view
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    // Set up the view with layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(view);

    // Set view properties
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setAlignment(Qt::AlignCenter);
    view->setFixedSize(1060, 610);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Create and set background
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap(":/res/forest-background.png").scaled(1060, 610, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    background->setZValue(-1);  // Make sure background is behind everything
    scene->addItem(background);

    // Set scene size to match view
    scene->setSceneRect(0, 0, 1060, 610);

    // Create and position player
    player = new Player();
    scene->addItem(player);
    player->setPos(-50, 458);

    // Fade effect setup
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);
    view->setGraphicsEffect(opacity);

    QPropertyAnimation *fadeIn = new QPropertyAnimation(opacity, "opacity");
    fadeIn->setDuration(2000);
    fadeIn->setStartValue(0);
    fadeIn->setEndValue(1);
    connect(fadeIn, &QPropertyAnimation::finished, this, &StoryScene::startAutoWalk);
    fadeIn->start(QPropertyAnimation::DeleteWhenStopped);

    setWindowIcon(QIcon(":/res/logo.png"));
    setWindowTitle("Unveiled");
    setFixedSize(1058, 605);
}

void StoryScene::showNextMessage() {
    if (!storyManager) return;

    QString currentSequence = storyManager->getCurrentSequence();
    qDebug() << "Current sequence in showNextMessage:" << currentSequence;

    // quiz trigger
    if (currentSequence == "quiz_start" && !storyManager->hasNextDialogue()) {
        qDebug() << "Triggering quiz from quiz_start sequence";
        showVillageQuiz();
        return;
    }

    if (!dialogueBox->isVisible()) {
        dialogueBox->show();
    }

    if (storyManager->hasChoices()) {
        QString currentText = storyManager->getCurrentText();
        QString currentSpeaker = storyManager->getCurrentSpeaker();
        dialogueBox->showMessage(currentText, currentSpeaker);
        showChoices();
    } else {
        QString currentText = storyManager->getCurrentText();
        QString currentSpeaker = storyManager->getCurrentSpeaker();
        dialogueBox->showMessage(currentText, currentSpeaker);

        if (storyManager->hasNextDialogue()) {
            storyManager->advance();
            dialogueBox->setNextDialogue(storyManager->getCurrentText(),
                                         storyManager->getCurrentSpeaker());
        } else {
            QString nextSequence = storyManager->getCurrentNextSequence();
            if (nextSequence == "game_over") {
                dialogueBox->showMessage(currentText, currentSpeaker);
                QTimer::singleShot(2000, [this]() {
                    dialogueBox->hide();
                });
            } else if (!nextSequence.isEmpty()) {
                storyManager->loadSequence(nextSequence);
                dialogueBox->setNextDialogue("", "");
            }
        }
    }
}

void StoryScene::startAutoWalk()
{
    walkAnimation = new QPropertyAnimation(player, "x");
    walkAnimation->setDuration(5000);
    walkAnimation->setStartValue(player->x());
    walkAnimation->setEndValue(400);
    player->setMovingRight(true);
    walkAnimation->start();

    connect(walkAnimation, &QPropertyAnimation::finished, [this]() {
        player->setMovingRight(false);

        dialogueBox = new DialogueBox(view);
        // Position the dialogue box at the bottom center of the view
        dialogueBox->move((view->width() - dialogueBox->width()) / 2,
                          view->height() - dialogueBox->height() - 50);

        // Connect DialogueBox finished signal to our showNextMessage function
        connect(dialogueBox, &DialogueBox::dialogueFinished,
                this, &StoryScene::showNextMessage);

        // Show first message
        showNextMessage();
    });
}


void StoryScene::showMaya() {
    // Create new character
    mayaNPC = new MayaNPC();

    // Load and set up sprite sheet
    QPixmap spriteSheet(":/res/maya-walk.png");
    QVector<QPixmap> sprites;
    for (int i = 0; i < 8; i++) {
        sprites.push_back(spriteSheet.copy(i * 100, 0, 100, 150));
    }

    // Set up initial Maya state
    mayaNPC->setPixmap(sprites[0]);
    mayaNPC->setPos(900, 374);
    mayaNPC->setScale(1.45);
    mayaNPC->setAcceptedMouseButtons(Qt::NoButton);  // Initially not clickable
    scene->addItem(mayaNPC);

    // Connect click signal
    connect(mayaNPC, &MayaNPC::mayaClicked, this, &StoryScene::startMayaInteraction);

    // Add fade-in effect
    QGraphicsOpacityEffect* mayaOpacity = new QGraphicsOpacityEffect();
    mayaNPC->setGraphicsEffect(mayaOpacity);

    QPropertyAnimation* fadeIn = new QPropertyAnimation(mayaOpacity, "opacity");
    fadeIn->setDuration(1000);
    fadeIn->setStartValue(0);
    fadeIn->setEndValue(1);
    fadeIn->start(QPropertyAnimation::DeleteWhenStopped);

    // Set up single walking animation
    mayaWalkAnimation = new QPropertyAnimation(this);
    mayaWalkAnimation->setTargetObject(mayaNPC);
    mayaWalkAnimation->setPropertyName("x");
    mayaWalkAnimation->setDuration(8000);
    mayaWalkAnimation->setStartValue(1000);    // Start position on the right
    mayaWalkAnimation->setEndValue(800);      // Final position
    mayaWalkAnimation->setEasingCurve(QEasingCurve::InOutQuad);  // Smooth movement

    connect(mayaWalkAnimation, &QPropertyAnimation::valueChanged,
            this, &StoryScene::updateMayaPosition);

    // Make Maya clickable after walking
    connect(mayaWalkAnimation, &QPropertyAnimation::finished, [this]() {
        mayaNPC->setAcceptedMouseButtons(Qt::LeftButton);
    });

    mayaWalkAnimation->start();
}

void StoryScene::showChoices() {
    auto choices = storyManager->getCurrentChoices();
    if (choices.isEmpty()) return;

    // Debug output
    qDebug() << "Showing choices:" << choices.size() << "choices available";

    disconnect(dialogueBox, &DialogueBox::choiceSelected, nullptr, nullptr);

    connect(dialogueBox, &DialogueBox::choiceSelected,
            this, [this](int index) {
                qDebug() << "Choice selected:" << index;
                storyManager->makeChoice(index);
                showNextMessage();
            });

    dialogueBox->showChoices(choices);
}


void StoryScene::updateMayaPosition(const QVariant& value) {
    mayaNPC->setPos(value.toReal(), mayaNPC->y());
}

void StoryScene::startMayaInteraction() {
    if (!dialogueBox->isVisible()) {
        storyManager->loadSequence("maya_intro");
        showNextMessage();
    }
}
// trust level not done.
void StoryScene::updateTrustMeter(int newLevel) {
    qDebug() << "Trust level changed to:" << newLevel;
}

void StoryScene::handleChoice(int index) {
    storyManager->makeChoice(index);
    showNextMessage();
}

void StoryScene::showQuiz(const QString& quizType) {
    enablePlayerMovement(false);
    dialogueBox->hide();

    Quiz* quiz = new Quiz(this);
    quiz->loadQuestions();

    connect(quiz, &Quiz::quizFinished, this, [this](int score) {
        Quiz* quiz = qobject_cast<Quiz*>(sender());
        if (quiz) {
            quiz->deleteLater();
        }

        dialogueBox->show();
        dialogueBox->move((view->width() - dialogueBox->width()) / 2,
                          view->height() - dialogueBox->height() - 50);

        storyManager->loadSequence("hide_test");
        showNextMessage();
    });

    quiz->move((width() - quiz->width()) / 2,
               (height() - quiz->height()) / 2);
    quiz->show();
}

void StoryScene::showVillageQuiz() {
    Quiz* quiz = new Quiz(nullptr);
    quiz->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    quiz->loadQuestions();

    connect(quiz, &Quiz::quizFinished, this, [this, quiz](int score) {
        quiz->deleteLater();
        show();

        if (!dialogueBox) {
            dialogueBox = new DialogueBox(view);
            dialogueBox->move((view->width() - dialogueBox->width()) / 2,
                              view->height() - dialogueBox->height() - 50);
            connect(dialogueBox, &DialogueBox::dialogueFinished,
                    this, &StoryScene::showNextMessage);
        }

        double percentage = (score / 10.0) * 100;

        if (percentage >= 80) {
            // Show pass message
            storyManager->loadSequence("quiz_pass");
            dialogueBox->show();

            // After quiz_pass dialogue finishes, start transition
            connect(dialogueBox, &DialogueBox::dialogueFinished, this, [this]() {
                dialogueBox->hide();
                startVillageTransition();
                // Disconnect this after it runs
                disconnect(dialogueBox, &DialogueBox::dialogueFinished, this, nullptr);
            });

            showNextMessage();
        } else {
            // Show fail message and game over
            storyManager->loadSequence("quiz_fail");
            dialogueBox->show();
            showNextMessage();
        }
    });

    quiz->show();
    hide();
}

void StoryScene::enablePlayerMovement(bool enable) {
    if (enable) {
        player->setCanMove(true);
        player->setFocus();
    } else {
        player->setCanMove(false);
        player->stopMovement();
        player->clearFocus();
    }
}

void StoryScene::onQuizFinished(int score) {
    // Get the quiz that sent the signal
    Quiz* quiz = qobject_cast<Quiz*>(sender());
    if (quiz) {
        quiz->deleteLater();
    }

    dialogueBox->show();

    // Change Maya's dialogue based on quiz performance
    if (score > 1) { // Good performance
        storyManager->loadSequence("quiz_success");
    } else {
        storyManager->loadSequence("quiz_needs_improvement");
    }
}

void StoryScene::handleQuizCompletion(int score) {
    const int PASSING_SCORE = 70;
    if (score >= PASSING_SCORE) {
        storyManager->loadSequence("quiz_success");
    } else {
        storyManager->loadSequence("quiz_needs_review");
    }
}

void StoryScene::startVillageTransition() {
    // Disable normal player controls during transition
    enablePlayerMovement(false);

    // Calculate the end position
    qreal endX = scene->width() + 100;

    // Create exit animations
    playerExitAnimation = new QPropertyAnimation(player, "pos");
    playerExitAnimation->setDuration(4000);
    playerExitAnimation->setStartValue(player->pos());
    playerExitAnimation->setEndValue(QPointF(endX, player->y()));

    mayaExitAnimation = new QPropertyAnimation(mayaNPC, "pos");
    mayaExitAnimation->setDuration(4000);
    mayaExitAnimation->setStartValue(mayaNPC->pos());
    mayaExitAnimation->setEndValue(QPointF(endX, mayaNPC->y()));

    // Start walking animation
    player->setMovingRight(true);

    // Connect animation progression to update player position
    connect(playerExitAnimation, &QPropertyAnimation::valueChanged,
            this, [this](const QVariant &value) {
                QPointF pos = value.toPointF();
                player->setPos(pos);
            });

    // When animation finishes
    connect(playerExitAnimation, &QPropertyAnimation::finished, this, [this]() {
        player->setMovingRight(false);
        switchToVillageBackground();
    });

    // Start both animations
    playerExitAnimation->start(QPropertyAnimation::DeleteWhenStopped);
    mayaExitAnimation->start(QPropertyAnimation::DeleteWhenStopped);
}

void StoryScene::switchToVillageBackground() {
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);
    view->setGraphicsEffect(opacity);

    QPropertyAnimation *fadeOut = new QPropertyAnimation(opacity, "opacity");
    fadeOut->setDuration(1000);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);

    connect(fadeOut, &QPropertyAnimation::finished, this, [this, opacity]() {
        // Change background
        QGraphicsPixmapItem *newBackground = new QGraphicsPixmapItem(
            QPixmap(":/res/village-path.png").scaled(1060, 610,
                                                     Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        newBackground->setZValue(-1);
        scene->addItem(newBackground);

        // Reset character positions
        player->setPos(50, 380);
        mayaNPC->setPos(80, 350);

        // Create entrance animations
        QPropertyAnimation *playerWalkIn = new QPropertyAnimation(player, "pos");
        playerWalkIn->setDuration(3000);
        playerWalkIn->setStartValue(player->pos());
        playerWalkIn->setEndValue(QPointF(300, player->y()));

        QPropertyAnimation *mayaWalkIn = new QPropertyAnimation(mayaNPC, "pos");
        mayaWalkIn->setDuration(3000);
        mayaWalkIn->setStartValue(mayaNPC->pos());
        mayaWalkIn->setEndValue(QPointF(400, mayaNPC->y()));

        // Start walk animations
        player->setMovingRight(true);

        // When walking animation finishes
        connect(playerWalkIn, &QPropertyAnimation::finished, this, [this]() {
            player->setMovingRight(false);

            // Show enter_village dialogue
            storyManager->loadSequence("enter_village");
            dialogueBox->show();
            showNextMessage();

            // Monitor dialogue changes for transition
            connect(dialogueBox, &DialogueBox::dialogueFinished, this, [this]() {
                QString nextSeq = storyManager->getCurrentNextSequence();
                if (nextSeq == "meet_doctor") {
                    dialogueBox->hide();
                    QTimer::singleShot(1000, this, &StoryScene::transitionToVillageInterior);
                    disconnect(dialogueBox, &DialogueBox::dialogueFinished, this, nullptr);
                } else {
                    showNextMessage();
                }
            });
        });

        // Fade back in
        QPropertyAnimation *fadeIn = new QPropertyAnimation(opacity, "opacity");
        fadeIn->setDuration(1000);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);

        fadeIn->start(QPropertyAnimation::DeleteWhenStopped);
        playerWalkIn->start(QPropertyAnimation::DeleteWhenStopped);
        mayaWalkIn->start(QPropertyAnimation::DeleteWhenStopped);
    });

    fadeOut->start(QPropertyAnimation::DeleteWhenStopped);
}

void StoryScene::transitionToVillageInterior() {
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);
    view->setGraphicsEffect(opacity);

    QPropertyAnimation *fadeOut = new QPropertyAnimation(opacity, "opacity");
    fadeOut->setDuration(1000);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);

    connect(fadeOut, &QPropertyAnimation::finished, this, [this, opacity]() {
        // Change to village interior background
        QGraphicsPixmapItem *newBackground = new QGraphicsPixmapItem(
            QPixmap(":/res/village.png").scaled(1060, 610,
                                                Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        newBackground->setZValue(-1);
        scene->addItem(newBackground);

        // Create and position the doctor NPC if not already created
        if (!doctorNPC) {
            doctorNPC = new DoctorNPC();
            doctorNPC->setPos(900, 374);
            doctorNPC->setScale(1.45);
            scene->addItem(doctorNPC);
        }

        // Position characters for entrance
        player->setPos(50, 370);
        mayaNPC->setPos(880, 300);
        doctorNPC->setPos(900, 380);

        // Create entrance animations
        QPropertyAnimation *playerWalkIn = new QPropertyAnimation(player, "pos");
        playerWalkIn->setDuration(3000);
        playerWalkIn->setStartValue(player->pos());
        playerWalkIn->setEndValue(QPointF(300, player->y()));

        QPropertyAnimation *mayaWalkIn = new QPropertyAnimation(mayaNPC, "pos");
        mayaWalkIn->setDuration(3000);
        mayaWalkIn->setStartValue(mayaNPC->pos());
        mayaWalkIn->setEndValue(QPointF(400, mayaNPC->y()));

        QPropertyAnimation *doctorWalkIn = new QPropertyAnimation(doctorNPC, "pos");
        doctorWalkIn->setDuration(3000);
        doctorWalkIn->setStartValue(doctorNPC->pos());
        doctorWalkIn->setEndValue(QPointF(700, doctorNPC->y()));

        // Start animations
        player->setMovingRight(true);

        // Replace the existing connection with the updated code
        connect(playerWalkIn, &QPropertyAnimation::finished, this, [this]() {
            player->setMovingRight(false);
            storyManager->loadSequence("meet_doctor");
            dialogueBox->show();
            showNextMessage();

            // Add connection to handle dialogue progression
            connect(dialogueBox, &DialogueBox::dialogueFinished, this, [this]() {
                if (storyManager->hasChoices()) {
                    showChoices();
                } else if (storyManager->hasNextDialogue()) {
                    showNextMessage();
                } else {
                    QString nextSequence = storyManager->getCurrentNextSequence();
                    if (!nextSequence.isEmpty()) {
                        storyManager->loadSequence(nextSequence);
                        showNextMessage();
                    } else {
                        // Show the COVID-19 misconceptions quiz
                        showCovidQuiz();
                    }
                }
            });
        });

        // Fade back in
        QPropertyAnimation *fadeIn = new QPropertyAnimation(opacity, "opacity");
        fadeIn->setDuration(1000);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);

        fadeIn->start(QPropertyAnimation::DeleteWhenStopped);
        playerWalkIn->start(QPropertyAnimation::DeleteWhenStopped);
        mayaWalkIn->start(QPropertyAnimation::DeleteWhenStopped);
        doctorWalkIn->start(QPropertyAnimation::DeleteWhenStopped);
    });

    fadeOut->start(QPropertyAnimation::DeleteWhenStopped);
}

void StoryScene::showCovidQuiz() {
    // Create and configure quiz
    Quiz* covidQuiz = new Quiz(nullptr);
    covidQuiz->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    covidQuiz->loadCovidMisconceptionsQuestions();

    // Handle quiz completion
    connect(covidQuiz, &Quiz::quizFinished, this, [this, covidQuiz](int score) {
        qDebug() << "Quiz finished with score:" << score;

        // Move to a QTimer to ensure proper cleanup
        QTimer::singleShot(0, this, [this, score, covidQuiz]() {
            handleCovidQuizResults(score, covidQuiz);
        });
    });

    // Position and show quiz
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - covidQuiz->width()) / 2;
    int y = (screenGeometry.height() - covidQuiz->height()) / 2;
    covidQuiz->move(x, y);
    covidQuiz->show();
    hide();  // Hide the main scene
}

void StoryScene::handleCovidQuizResults(int score, Quiz* quiz) {
    // Safe cleanup of quiz
    if (quiz) {
        quiz->close();
        quiz->deleteLater();
    }

    if (score >= 4) {
        qDebug() << "Showing credits scene";

        CreditScene* creditScene = new CreditScene(nullptr);
        creditScene->setAttribute(Qt::WA_DeleteOnClose);  // Auto cleanup

        // Position credits scene
        QRect screenGeometry = QApplication::primaryScreen()->geometry();
        int x = (screenGeometry.width() - creditScene->width()) / 2;
        int y = (screenGeometry.height() - creditScene->height()) / 2;
        creditScene->move(x, y);

        // Connect destruction to game closure
        connect(creditScene, &QWidget::destroyed, this, [this]() {
            qDebug() << "Credits finished, closing game";
            QTimer::singleShot(500, this, &QWidget::close);
        });

        creditScene->showCreditScene();
        hide();  // Keep main window hidden

    } else {
        show();
        dialogueBox->show();
        storyManager->loadSequence("quiz_needs_review");
        showNextMessage();
    }
}
void StoryScene::handleDoctorIntroduction() {
    // Here we could add any special effects or animations for the doctor's appearance
    // For now, just continue with the dialogue
    storyManager->loadSequence("meet_doctor");
    showNextMessage();
}


void StoryScene::onDialogueChanged()
{
    // Handle any special effects or state changes when dialogue changes
}

void StoryScene::onChoicesAvailable(const QVector<QPair<QString, QString>> &choices)
{
    // We'll implement choice buttons here later
}
