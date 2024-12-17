#ifndef CREDITSCENE_H
#define CREDITSCENE_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

class CreditScene : public QWidget {
    Q_OBJECT
public:
    explicit CreditScene(QWidget* parent = nullptr);
    void showCreditScene();
    void setupUI();

private:
    QLabel* creditLabel;
    QPropertyAnimation* scrollAnimation;
    void loadCreditsFromJson();
};

#endif // CREDITSCENE_H
