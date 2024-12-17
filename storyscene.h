#ifndef STORYSCENE_H
#define STORYSCENE_H
#include "dialoguebox.h"
#include "player.h"
#include "doctornpc.h"
#include "storymanager.h"
#include "mayanpc.h"
#include "quiz.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPropertyAnimation>
#include <QMediaPlayer>
#include <QAudioOutput>

class StoryScene : public QWidget {
    Q_OBJECT
public:
    explicit StoryScene(QWidget *parent = nullptr);
    void startOpeningScene();

private slots:
    void showQuiz(const QString& quizType);
    void handleQuizCompletion(int score);
    void onQuizFinished(int score);
    void showCovidQuiz();
    void handleCovidQuizResults(int score, Quiz* quiz);
    void handleChoice(int index);
    void onDialogueChanged();
    void onChoicesAvailable(const QVector<QPair<QString, QString>> &choices);
    void updateMayaPosition(const QVariant& value);
    void startMayaInteraction();
    void showChoices();
    void updateTrustMeter(int newLevel);

private:
    enum class StoryState {
        Initial,
        VillagePath,
        VillageInterior
    };
    StoryState currentState = StoryState::Initial;
    void showVillageQuiz();
    void handleDoctorIntroduction();
    void startVillageTransition();
    QPropertyAnimation* playerExitAnimation;
    QPropertyAnimation* mayaExitAnimation;
    void transitionToVillageInterior();
    void switchToVillageBackground();
    void enablePlayerMovement(bool enable);
    MayaNPC* mayaNPC;
    QPropertyAnimation* mayaWalkAnimation;
    DialogueBox *dialogueBox;
    QGraphicsScene *scene;
    QGraphicsView *view;
    DoctorNPC* doctorNPC = nullptr;
    Player *player;
    QPropertyAnimation *walkAnimation;
    QMediaPlayer *bgMusic;
    QAudioOutput *audioOutput;
    StoryManager *storyManager;
    QVector<QString> messages;
    enum class MayaTrust { Positive, Negative };
    MayaTrust mayaTrustLevel;
    int currentMessage = 0;
    void showMaya();
    void setupScene();
    void startAutoWalk();
    void showNextMessage();
};
#endif // STORYSCENE_H
