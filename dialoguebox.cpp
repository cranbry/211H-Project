#include "dialoguebox.h"
#include <QVBoxLayout>

DialogueBox::DialogueBox(QWidget *parent) : QWidget(parent), currentCharIndex(0) {
    setupUI();
    typingTimer = new QTimer(this);
    typingTimer->setInterval(30); // Adjust typing speed here
    connect(typingTimer, &QTimer::timeout, this, &DialogueBox::updateTypingAnimation);
}

void DialogueBox::setupUI() {
    setMinimumWidth(600);
    setMinimumHeight(250);
    setMaximumHeight(300);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(5);  // Containeer spacing.
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Dialogue container with UI
    dialogueContainer = new QWidget(this);
    dialogueContainer->setStyleSheet(R"(
        QWidget {
            background-color: rgba(0, 0, 0, 0.85);
            border: 2px solid rgba(255, 255, 255, 0.8);
            border-radius: 15px;
        }
    )");

    QVBoxLayout* containerLayout = new QVBoxLayout(dialogueContainer);
    containerLayout->setSpacing(2);  // Dialogue spacing
    containerLayout->setContentsMargins(10, 5, 10, 5);

    // Showing speaker label + UI
    speakerLabel = new QLabel(dialogueContainer);
    speakerLabel->setStyleSheet(R"(
        QLabel {
            color: #FFD700;
            font-weight: bold;
            font-size: 16px;
            background: transparent;
            border: none;
            padding-left: 15px;
        }
    )");
    containerLayout->addWidget(speakerLabel);

    // Text label
    textLabel = new QLabel(dialogueContainer);
    textLabel->setWordWrap(true);
    textLabel->setStyleSheet(R"(
        QLabel {
            color: white;
            font-size: 14px;
            background: transparent;
            border: none;
            padding: 5px 15px;
        }
    )");
    containerLayout->addWidget(textLabel);

    // Choices container
    choicesContainer = new QWidget(dialogueContainer);
    choicesContainer->setStyleSheet("background: transparent; border: none;");
    QHBoxLayout* choicesLayout = new QHBoxLayout(choicesContainer);
    choicesLayout->setSpacing(20);
    containerLayout->addWidget(choicesContainer);
    choicesContainer->hide();

    mainLayout->addWidget(dialogueContainer);

    // Next button
    nextButton = new QPushButton("Next", this);
    nextButton->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            border: 2px solid rgba(255, 255, 255, 0.8);
            border-radius: 15px;
            color: white;
            padding: 8px 20px;
            font-size: 14px;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
        }
    )");
    mainLayout->addWidget(dialogueContainer, 0, Qt::AlignTop);

    mainLayout->addWidget(nextButton, 0, Qt::AlignRight);
    connect(nextButton, &QPushButton::clicked, this, &DialogueBox::onNextClicked);
}

void DialogueBox::showMessage(const QString &text, const QString &speaker) {
    // Ensure the dialogue box is visible
    this->show();  // forcing visibility

    speakerLabel->setText(speaker);
    speakerLabel->setVisible(!speaker.isEmpty());

    fullText = text;
    currentCharIndex = 0;
    currentDisplayText.clear();

    startTypingAnimation(text);


    nextButton->show();  // only for next button.
}
void DialogueBox::startTypingAnimation(const QString &text) {
    fullText = text;
    currentCharIndex = 0;
    currentDisplayText.clear();
    textLabel->setText("");
    typingTimer->start();
}

void DialogueBox::updateTypingAnimation() {
    if (currentCharIndex < fullText.length()) {
        currentDisplayText += fullText[currentCharIndex];
        textLabel->setText(currentDisplayText);
        currentCharIndex++;
    } else {
        typingTimer->stop();
    }
}

void DialogueBox::showChoices(const QVector<QPair<QString, QString>>& choices) {
    clearChoices();

    QHBoxLayout* choicesLayout = qobject_cast<QHBoxLayout*>(choicesContainer->layout());

    for (int i = 0; i < choices.size(); ++i) {
        QPushButton* choiceButton = new QPushButton(choices[i].first, choicesContainer);
        choiceButton->setStyleSheet(R"(
            QPushButton {
                background-color: transparent;
                border: 2px solid rgba(255, 255, 255, 0.8);
                border-radius: 12px;
                color: white;
                padding: 8px 15px;
                font-size: 14px;
            }
            QPushButton:hover {
                background-color: rgba(255, 255, 255, 0.1);
            }
        )");

        connect(choiceButton, &QPushButton::clicked, this, [this, i]() {
            emit choiceSelected(i);
            clearChoices();
        });

        choicesLayout->addWidget(choiceButton);
        choiceButtons.append(choiceButton);
    }

    choicesContainer->show();
    nextButton->hide();
}

void DialogueBox::clearChoices() {
    for (auto button : choiceButtons) {
        button->deleteLater();
    }
    choiceButtons.clear();
    choicesContainer->hide();
    nextButton->show();
}

void DialogueBox::setNextDialogue(const QString &text, const QString &speaker) {
    nextText = text;
    nextSpeaker = speaker;
    // Show the next button even if there's no next text
    nextButton->setVisible(true);
    this->show();
}

void DialogueBox::onNextClicked() {
    emit dialogueFinished(); // emit for event signal of dialogue ending.
    if (!nextText.isEmpty()) {
        showMessage(nextText, nextSpeaker);
        nextText.clear();
        nextSpeaker.clear();
    }
}
