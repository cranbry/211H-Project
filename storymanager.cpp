#include "storymanager.h"
#include <QFile>
#include <QDebug>

StoryManager::StoryManager(QObject *parent)
    : QObject(parent)
    , currentIndex(0)
    , trustLevel(0)
{
}

bool StoryManager::loadStory(const QString &chapter)
{
    QFile file(":/story/maya-story.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open story file:" << file.fileName();
        qDebug() << "Error:" << file.errorString();
        return false;
    }
    QByteArray data = file.readAll();
    qDebug() << "File content size:" << data.size();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qDebug() << "Failed to parse story file";
        qDebug() << "JSON error:" << doc.object();
        return false;
    }
    qDebug() << "Successfully loaded story file";
    storyData = doc.object();
    currentChapter = storyData[chapter].toObject();
    loadSequence("intro_sequence");
    return true;
}

void StoryManager::loadSequence(const QString &sequenceId) {
    qDebug() << "Loading sequence:" << sequenceId;
    currentSequenceId = sequenceId;
    currentSequence = currentChapter[sequenceId].toArray();
    qDebug() << "Sequence content:" << currentSequence;
    currentIndex = 0;
    emit dialogueChanged();
}

QString StoryManager::getCurrentText() const {
    if (isCreditsMode && currentCreditsIndex < creditsSequence.size()) {
        return creditsSequence[currentCreditsIndex].toObject()["text"].toString();
    }

    // DEBUG THE FUCKING INDEX -> SHOWS INDEX OF ANIMAITON LOOK AT JSON
    if (currentSequence.isEmpty() || currentIndex >= currentSequence.size()) {
        qDebug() << "No current text available. Index:" << currentIndex;
        return QString();
    }
    return currentSequence[currentIndex].toObject()["text"].toString();
}

QString StoryManager::getCurrentSpeaker() const {
    if (isCreditsMode) {
        return ""; // No speaker for credits
    }

    // DEBUG THE FUCKING INDEX -> SHOWS INDEX OF ANIMAITON LOOK AT JSON
    if (currentSequence.isEmpty() || currentIndex >= currentSequence.size()) {
        qDebug() << "No current speaker available. Index:" << currentIndex;
        return QString();
    }
    return currentSequence[currentIndex].toObject()["speaker"].toString();
}

bool StoryManager::hasNextDialogue() const {
    if (isCreditsMode) {
        // Check if there are more things other than seperators
        for (int i = currentCreditsIndex + 1; i < creditsSequence.size(); ++i) {
            QJsonObject entry = creditsSequence[i].toObject();
            if (entry["type"].toString() == "text") {
                return true;
            }
        }
        return false;
    }

    // DEBUG THE FUCKING INDEX -> SHOWS INDEX OF ANIMAITON LOOK AT JSON
    bool hasNext = currentIndex < currentSequence.size() - 1;
    qDebug() << "Has next dialogue:" << hasNext << "CurrentIndex:" << currentIndex;
    return hasNext;
}

void StoryManager::advance() {
    if (isCreditsMode) {
        // Move to next text part of tstory
        while (currentCreditsIndex < creditsSequence.size() - 1) {
            currentCreditsIndex++;
            QJsonObject currentItem = creditsSequence[currentCreditsIndex].toObject();
            if (currentItem["type"].toString() == "text") {
                emit dialogueChanged();
                return;
            }
        }

        // Reached end of credits
        isCreditsMode = false;
        return;
    }

    // Existing implementation for other sequences
    if (hasNextDialogue()) {
        currentIndex++;
        qDebug() << "advance: Advanced to index:" << currentIndex;
        emit dialogueChanged();
    } else {
        // Handle the case where a "next" field exists
        QJsonObject currentObj = currentSequence[currentIndex].toObject();
        if (currentObj.contains("next")) {
            QString nextSequence = currentObj["next"].toString();
            qDebug() << "advance: Moving to next sequence:" << nextSequence;
            loadSequence(nextSequence);
        } else {
            qDebug() << "advance: No 'next' field found. End of sequence.";
        }
    }
}

bool StoryManager::hasChoices() const
{
    if (currentSequence.isEmpty() || currentIndex >= currentSequence.size()) {
        qDebug() << "hasChoices: Current sequence is empty or index is out of bounds.";
        return false;
    }
    QJsonObject currentObj = currentSequence[currentIndex].toObject();
    qDebug() << "hasChoices: Current JSON object:" << currentObj;

    bool hasChoices = currentObj.contains("choices");
    qDebug() << "hasChoices: Key 'choices' exists:" << hasChoices;
    return hasChoices;
}

QVector<QPair<QString, QString>> StoryManager::getCurrentChoices() const
{
    QVector<QPair<QString, QString>> choices;
    if (!hasChoices())
        return choices;
    QJsonArray choicesArray = currentSequence[currentIndex].toObject()["choices"].toArray();
    for (const QJsonValue &choice : choicesArray) {
        QJsonObject choiceObj = choice.toObject();
        choices.append({choiceObj["text"].toString(), choiceObj["next"].toString()});
    }
    qDebug() << "Retrieved" << choices.size() << "choices";
    return choices;
}

QString StoryManager::getCurrentSequence() const {
    return currentSequenceId;
}

QString StoryManager::getCurrentNextSequence() const {
    if (currentSequence.isEmpty() || currentIndex >= currentSequence.size()) {
        qDebug() << "getCurrentNextSequence: Sequence empty or index out of range";
        return QString();
    }

    QJsonObject currentObj = currentSequence[currentIndex].toObject();
    QString nextSeq = currentObj["next"].toString();
    qDebug() << "getCurrentNextSequence: Current index:" << currentIndex;
    qDebug() << "getCurrentNextSequence: Next sequence:" << nextSeq;
    return nextSeq;
}

int StoryManager::getCurrentIndex() const {
    return currentIndex;
}

void StoryManager::makeChoice(int choiceIndex) {
    qDebug() << "Making choice:" << choiceIndex;  // Debug

    if (!hasChoices() || choiceIndex < 0) {
        qDebug() << "Invalid choice index or no choices available";
        return;
    }

    QJsonArray choices = currentSequence[currentIndex].toObject()["choices"].toArray();
    if (choiceIndex >= choices.size()) {
        qDebug() << "Choice index out of range";
        return;
    }

    QJsonObject choice = choices[choiceIndex].toObject();
    QString nextSequence = choice["next"].toString();
    qDebug() << "Next sequence will be:" << nextSequence;  // Debug

    if (!nextSequence.isEmpty()) {
        loadSequence(nextSequence);
    }
}

void StoryManager::loadCredits() {
    try {
        QFile file(":/res/credits.json");
        if (!file.open(QIODevice::ReadOnly)) {
            throw std::runtime_error(("Could not open credits.json: " + file.errorString()).toStdString());
        }

        QByteArray data = file.readAll();
        if (data.isEmpty()) {
            throw std::runtime_error("Credits JSON file is empty");
        }

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            throw std::runtime_error(("JSON parse error: " + parseError.errorString()).toStdString());
        }

        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("credits")) {
            throw std::runtime_error("JSON does not contain 'credits' key");
        }

        creditsSequence = jsonObj["credits"].toArray();
        if (creditsSequence.isEmpty()) {
            throw std::runtime_error("Credits array is empty");
        }

        // Debug print the credits
        qDebug() << "Credits loaded. Number of entries:" << creditsSequence.size();
        for (int i = 0; i < creditsSequence.size(); ++i) {
            QJsonObject entry = creditsSequence[i].toObject();
            qDebug() << "Credit" << i
                     << "Type:" << (entry.contains("type") ? entry["type"].toString() : "N/A")
                     << "Content:" << (entry.contains("content") ? entry["content"].toString() : "N/A");
        }

        // Set credits mode
        isCreditsMode = true;
        currentCreditsIndex = 0;
        emit dialogueChanged();
    }
    catch (const std::exception& e) {
        qCritical() << "Error loading credits:" << e.what();

        // Fallback to a default credits sequence
        QJsonArray defaultCredits;
        defaultCredits.append(QJsonObject{
            {"type", "text"},
            {"content", "Credits Loading Failed"},
            {"style", "header"}
        });
        creditsSequence = defaultCredits;

        isCreditsMode = true;
        currentCreditsIndex = 0;
        emit dialogueChanged();
    }
    catch (...) {
        qCritical() << "Error occurred while loading credits";

        // Fallback to a minimal default credits sequence
        QJsonArray defaultCredits;
        defaultCredits.append(QJsonObject{
            {"type", "text"},
            {"content", "Credits"},
            {"style", "header"}
        });
        creditsSequence = defaultCredits;

        isCreditsMode = true;
        currentCreditsIndex = 0;
        emit dialogueChanged();
    }
}
