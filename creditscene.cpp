#include "creditscene.h"
#include <QDebug>

CreditScene::CreditScene(QWidget* parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    setWindowTitle("Credits");
    setupUI();
}

void CreditScene::setupUI() {
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);

    creditLabel = new QLabel(this);
    creditLabel->setWordWrap(true);
    creditLabel->setStyleSheet(R"(
        QLabel {
            color: white;
            font-size: 18px;
            font-family: Arial;
            background-color: transparent;
        }
    )");
    creditLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    creditLabel->setMinimumSize(400, 600);

    layout->addWidget(creditLabel);

    scrollAnimation = new QPropertyAnimation(creditLabel, "pos");
    scrollAnimation->setDuration( 28000); // less -> increses speed of scroll
    scrollAnimation->setEasingCurve(QEasingCurve::Linear);

    connect(scrollAnimation, &QPropertyAnimation::finished, this, &CreditScene::close);

    QIcon windowIcon(":/res/logo.png");
    setWindowIcon(windowIcon);

    setWindowTitle("Unveiled");
    setStyleSheet("background-color: black;");
    setFixedSize(500, 600);
}

void CreditScene::loadCreditsFromJson() {
    QFile file(":/story/credits.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open credits.json";
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray creditsArray = jsonObj["credits"].toArray();

    QString creditsText;
    for (const QJsonValue &value : creditsArray) {
        QJsonObject creditItem = value.toObject();

        // to handle different types of text -> look at credits,json for reference
        if (creditItem.contains("text")) {
            QString text = creditItem["text"].toString();
            if (text == "CREATED BY" || text == "THE END") {
                // headers are in yellow
                creditsText += QString("<h2 style='color: #FFD700; text-align: center; margin: 20px 0;'>%1</h2>").arg(text);
            } else {
                // names are in white
                creditsText += QString("<p style='color: #FFFFFF; text-align: center; margin: 10px 0;'>%1</p>").arg(text);
            }
            continue;
        }

        // to handle different types of format -> look at credits,json for reference
        QString type = creditItem["type"].toString();
        if (type == "text") {
            QString content = creditItem["content"].toString();
            QString style = creditItem["style"].toString();

            if (style == "header") {
                creditsText += QString("<h2 style='color: #FFD700; text-align: center; margin: 20px 0;'>%1</h2>").arg(content);
            } else if (style == "name") {
                creditsText += QString("<p style='color: #FFFFFF; text-align: center; margin: 10px 0;'>%1</p>").arg(content);
            } else if (style == "subtitle") {
                creditsText += QString("<p style='color: #A0A0A0; text-align: center; font-style: italic; margin: 10px 0;'>%1</p>").arg(content);
            }
        } else if (type == "separator") {
            creditsText += "<div style='height: 30px;'></div>"; // seperator text to not go over the writing.
        }
    }

    // padding at the bottom so the end shows due to small window issue.
    creditsText += "<div style='height: 100px;'></div>";

    creditLabel->setText(creditsText);
}

void CreditScene::showCreditScene() {
    loadCreditsFromJson();

    creditLabel->adjustSize();

    // second fix to show ending.
    int startY = height();
    int contentHeight = creditLabel->height();
    int endY = -(contentHeight - height() + 100); // so it doesn't go too high

    scrollAnimation->setStartValue(QPoint(20, startY));
    scrollAnimation->setEndValue(QPoint(20, endY));

    show();
    scrollAnimation->start(QPropertyAnimation::DeleteWhenStopped);
}
