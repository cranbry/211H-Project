#ifndef DIALOGUEBOX_H
#define DIALOGUEBOX_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>

class DialogueBox : public QWidget {
    Q_OBJECT
public:
    explicit DialogueBox(QWidget *parent = nullptr);
    void showMessage(const QString &text, const QString &speaker = "");
    void setNextDialogue(const QString &text, const QString &speaker = "");
    void showChoices(const QVector<QPair<QString, QString>>& choices);
    void clearChoices();

signals:
    void dialogueFinished();
    void choiceSelected(int index);

private slots:
    void onNextClicked();
    void updateTypingAnimation();

private:
    QVBoxLayout* mainLayout;
    QWidget* dialogueContainer;
    QLabel* textLabel;
    QLabel* speakerLabel;
    QPushButton* nextButton;
    QWidget* choicesContainer;
    QVector<QPushButton*> choiceButtons;

    QString fullText;
    QString currentDisplayText;
    QString nextText;
    QString nextSpeaker;
    int currentCharIndex;
    QTimer* typingTimer;

    void setupUI();
    void startTypingAnimation(const QString &text);
};
#endif // DIALOGUEBOX_H
