#ifndef QUIZ_H
#define QUIZ_H

#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVector>

struct QuizQuestion {
    QString question;
    QVector<QString> choices;
    int correctAnswer;
    QString explanation;
};

class Quiz : public QWidget {
    Q_OBJECT

public:
    void loadQuestions();
    void loadCovidMisconceptionsQuestions();
    explicit Quiz(QWidget *parent = nullptr);
    ~Quiz();

signals:
    void quizFinished(int score);

private slots:
    void checkAnswer(int choice);
    void nextQuestion();
    void retryQuiz();

private:
    void setupUI();
    void displayQuestion();
    void updateScore();

    QVector<QuizQuestion> questions;
    int currentQuestion;
    int score;
    static const int TOTAL_QUESTIONS = 10;
    int questionsAnswered = 0;

    QLabel *questionLabel;
    QVector<QPushButton*> answerButtons;
    QPushButton *nextButton;
    QPushButton *retryButton;
    QLabel *feedbackLabel;
    QLabel *scoreLabel;
};

#endif // QUIZ_H
