#include "quiz.h"
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QScreen>

Quiz::Quiz(QWidget *parent)
    : QWidget(parent)
    , currentQuestion(0)
    , score(0)
{
    qDebug() << "Quiz constructor called";
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);  // quiz stay at top
    setAttribute(Qt::WA_DeleteOnClose);  // delte when closed

    loadQuestions();
    setupUI();
    displayQuestion();
}

// CHANGE QUESTIONS!!!!!!

void Quiz::loadQuestions() {
    questions.clear(); // Avoids duplication by deleting any previous qurestions
    questionsAnswered = 0;
    // Village Trust/Cultural Sensitivity Quiz
    questions.append({
        "When Maya expresses doubt about the traditional remedies, what's the most appropriate response?",
        {"Mock the village traditions",
         "Immediately tell everyone she doubts the doctor",
         "Listen carefully and ask gentle questions about her concerns",
         "Tell her to stop questioning authority"},
        2,  // Correct answer: Listen carefully
        "Building trust requires creating safe spaces for honest dialogue while showing respect."
    });

    questions.append({
        "You notice villagers performing a ritual that seems ineffective. What should you do?",
        {"Observe respectfully while gathering information",
         "Interrupt and correct them immediately",
         "Join in without question",
         "Laugh at their practices"},
        0,  // Correct answer: Observe respectfully
        "Understanding cultural practices requires patient observation before suggesting changes."
    });

    questions.append({
        "If a villager shares concerns about the doctor's methods, you should:",
        {"Immediately confront the doctor",
         "Ignore their concerns completely",
         "Listen privately and document their concerns",
         "Tell everyone in the village"},
        2,  // Correct answer: Listen privately
        "Building trust requires discretion and protecting those who share sensitive information."
    });

    questions.append({
        "When the doctor presents his theories, what's the best approach?",
        {"Dismiss them outright",
         "Accept everything without question",
         "Listen attentively while noting inconsistencies",
         "Leave the room immediately"},
        2,  // Correct answer: Listen attentively
        "Effective investigation requires careful attention to details while maintaining respect."
    });

    questions.append({
        "How should you respond to villagers who are skeptical of outsiders?",
        {"Demand their trust immediately",
         "Demonstrate trustworthiness through consistent actions",
         "Avoid them completely",
         "Force them to interact with you"},
        1,  // Correct answer: Demonstrate trustworthiness
        "Trust must be earned through consistent, respectful behavior over time."
    });

    questions.append({
        "When you discover concerning information about village practices, you should:",
        {"Post it on social media immediately",
         "Ignore it completely",
         "Start a village riot",
         "Gather evidence carefully while maintaining confidentiality"},
        3,  // Correct answer: Gather evidence carefully
        "Responsible investigation requires careful documentation while protecting sources."
    });

    questions.append({
        "If villagers ask about your purpose here, what's the best response?",
        {"Lie about your intentions",
         "Be honest while emphasizing your respect for their community",
         "Refuse to answer",
         "Tell them it's none of their business"},
        1,  // Correct answer: Be honest while respectful
        "Honesty builds trust, but it must be balanced with cultural sensitivity."
    });

    questions.append({
        "When observing village customs, what's most important?",
        {"Showing respect while staying alert",
         "Finding ways to mock them",
         "Blindly following everything",
         "Refusing to participate"},
        0,  // Correct answer: Showing respect while alert
        "Balance respect for traditions with careful observation of their effects."
    });

    questions.append({
        "If you notice harmful practices, your first step should be:",
        {"Call the authorities immediately",
         "Start arguing with everyone",
         "Run away from the village",
         "Understand why these practices exist"},
        3,  // Correct answer: Understand why
        "Understanding the root causes of practices helps address them effectively."
    });

    questions.append({
        "How should you handle conflicting accounts from villagers?",
        {"Choose a side immediately",
         "Document all perspectives carefully",
         "Ignore the conflicts",
         "Start arguments between villagers"},
        1,  // Correct answer: Document carefully
        "Good investigation requires documenting all perspectives without immediate judgment."
    });
}

void Quiz::loadCovidMisconceptionsQuestions() {
    questions.clear();
    questionsAnswered = 0;

    questions.append({
        "Why do properly fitted masks help prevent virus spread?",
        {"They trap evil spirits",
         "They block most respiratory droplets that carry the virus",
         "They create a magical barrier",
         "They please the village elders"},
        1,  // Correct answer: Block droplets
        "Masks work by physically blocking respiratory droplets that contain viral particles."
    });

    questions.append({
        "What makes social distancing effective against virus spread?",
        {"It confuses evil spirits",
         "It prevents cursed air from reaching you",
         "It keeps you beyond the typical range of respiratory droplets",
         "It aligns with magical symbols"},
        2,  // Correct answer: Beyond droplet range
        "Distance reduces exposure to respiratory droplets that can carry the virus."
    });

    questions.append({
        "How does hand washing help prevent virus transmission?",
        {"It physically removes and kills viral particles",
         "It washes away bad luck",
         "It cleanses your spiritual energy",
         "It strengthens magical protection"},
        0,  // Correct answer: Removes viral particles
        "Soap and water physically remove and destroy virus particles on hands."
    });

    questions.append({
        "What makes indoor gatherings risky during outbreaks?",
        {"Limited ventilation allows virus particles to accumulate",
         "Evil spirits gather indoors",
         "The curse is stronger inside",
         "Indoor symbols are weaker"},
        0,  // Correct answer: Limited ventilation
        "Poor ventilation indoor allows viral particles to build up in the air."
    });

    questions.append({
        "Why do some people get more severe symptoms than others?",
        {"They're cursed more strongly",
         "Their protective herbs are weak",
         "Random chance only",
         "Factors like age and health conditions affect severity"},
        3,  // Correct answer: Health factors
        "Individual health factors and conditions influence disease severity."
    });

    questions.append({
        "What makes vaccines effective against viruses?",
        {"They contain blessed water",
         "They train your immune system to recognize and fight the virus",
         "They ward off evil spirits",
         "They enhance magical protection"},
        1,  // Correct answer: Train immune system
        "Vaccines work by teaching your body's natural defenses to recognize specific threats."
    });

    questions.append({
        "Why do virus mutations occur?",
        {"Natural random changes in viral genetic material",
         "Evil spirits evolve",
         "Curses getting stronger",
         "Magical interference"},
        0,  // Correct answer: Natural changes
        "Viruses naturally mutate as they reproduce, leading to new variants."
    });

    questions.append({
        "What makes isolation effective when someone is sick?",
        {"It concentrates healing energy",
         "It prevents infected people from spreading droplets to others",
         "It strengthens protective spells",
         "It weakens the curse"},
        1,  // Correct answer: Prevents spread
        "Isolation physically prevents sick people from exposing others to the virus."
    });

    questions.append({
        "Why are some traditional remedies ineffective against viruses?",
        {"The spirits reject them",
         "Modern medicine blocks them",
         "They don't target the actual viral infection",
         "The moon phase is wrong"},
        2,  // Correct answer: Don't target virus
        "Effective treatments must specifically target the virus or support the body's immune response."
    });

    questions.append({
        "What makes testing important during an outbreak?",
        {"It identifies who has the virus so they can isolate",
         "It measures curse strength",
         "It strengthens immunity spells",
         "It wards off evil"},
        0,  // Correct answer: Identifies cases
        "Testing helps identify and isolate cases to prevent further spread."
    });
}

void Quiz::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);

    // Question display
    questionLabel = new QLabel(this);
    questionLabel->setWordWrap(true);
    questionLabel->setStyleSheet("font-size: 16px; color: black;");
    mainLayout->addWidget(questionLabel);

    // Answer buttons
    for (int i = 0; i < 4; ++i) {
        QPushButton *button = new QPushButton(this);
        button->setFixedWidth(300);
        button->setStyleSheet(R"(
            QPushButton {
                background-color: #353F4A;
                border: 2px solid white;
                border-radius: 15px;
                color: white;
                padding: 10px;
                text-align: left;
            }
            QPushButton:hover {
                background-color: rgba(255, 255, 255, 0.1);
                border: 2px solid #353F4A;
                color: #353F4A;
            }
        )");
        connect(button, &QPushButton::clicked, this, [this, i]() { checkAnswer(i); });
        answerButtons.append(button);
        mainLayout->addWidget(button);
    }

    // Feedback label
    feedbackLabel = new QLabel(this);
    feedbackLabel->setWordWrap(true);
    feedbackLabel->setStyleSheet("color: #353F4A; font-style: italic; font-family: Mohave");
    feedbackLabel->hide();
    mainLayout->addWidget(feedbackLabel);

    // Score display
    scoreLabel = new QLabel("Score: 0", this);
    scoreLabel->setStyleSheet("color: #353F4A; font-size: 15px; font-family: Mohave");
    mainLayout->addWidget(scoreLabel);

    // Navigation buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    nextButton = new QPushButton("Next", this);
    nextButton->setStyleSheet(R"(
        QPushButton {
            background-color: #353F4A;
            border: 2px solid white;
            border-radius: 15px;
            color: white;
            padding: 10px 20px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
            border: 2px solid #353F4A;
            color: #353F4A;
        }
    )");
    nextButton->hide();
    connect(nextButton, &QPushButton::clicked, this, &Quiz::nextQuestion);
    buttonLayout->addWidget(nextButton);

    retryButton = new QPushButton("Retry", this);
    retryButton->setStyleSheet(nextButton->styleSheet());
    retryButton->hide();
    connect(retryButton, &QPushButton::clicked, this, &Quiz::retryQuiz);
    buttonLayout->addWidget(retryButton);

    mainLayout->addLayout(buttonLayout);

    setWindowIcon(QIcon(":/res/logo.png"));
    setWindowTitle("Unveiled");
}


void Quiz::displayQuestion()
{
    if (currentQuestion < questions.size()) {
        QuizQuestion current = questions[currentQuestion];
        questionLabel->setText(current.question);

        for (int i = 0; i < answerButtons.size(); ++i) {
            answerButtons[i]->setText(current.choices[i]);
            answerButtons[i]->setEnabled(true);
        }

        feedbackLabel->hide();
        nextButton->hide();
    }
}

void Quiz::checkAnswer(int choice) {
    QuizQuestion current = questions[currentQuestion];

    // Disable answer buttons
    for (auto button : answerButtons) {
        button->setEnabled(false);
    }

    // Check if answer is correct and update score
    if (choice == current.correctAnswer) {
        score++;
        feedbackLabel->setText("Correct! " + current.explanation);
        feedbackLabel->setStyleSheet("color: #4ED5AE;"); // Green for correct
    } else {
        feedbackLabel->setText("Incorrect. " + current.explanation);
        feedbackLabel->setStyleSheet("color: #FF6B6B;"); // Red for incorrect
    }

    feedbackLabel->show();
    updateScore();

    questionsAnswered++;

    // Check if quiz is complete
    if (questionsAnswered >= TOTAL_QUESTIONS) {
        // Hide next button and show results
        nextButton->hide();
        retryButton->show();
        emit quizFinished(score);
    } else if (currentQuestion < questions.size() - 1) {
        nextButton->show();
    }
}

void Quiz::nextQuestion()
{
    currentQuestion++;
    displayQuestion();
}

void Quiz::retryQuiz()
{
    currentQuestion = 0;
    score = 0;
    updateScore();
    displayQuestion();
    retryButton->hide();
}

void Quiz::updateScore()
{
    scoreLabel->setText(QString("Score: %1/%2").arg(score).arg(questions.size()));
}

Quiz::~Quiz()
{
}
