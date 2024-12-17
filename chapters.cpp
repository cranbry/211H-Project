#include "chapters.h"
#include "storyscene.h"
#include <QGridLayout>
#include <QTimer>
#include <QIcon>
#include <QPropertyAnimation>
#include <QPainter>
#include <QGraphicsOpacityEffect>

Chapters::Chapters(QWidget *parent)
    : QWidget(parent)
{
    chapterUnlocked = {true, false, false, false, false};
    setupUI();
    setupStyles();
}

void Chapters::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(40);
    mainLayout->setContentsMargins(50, 20, 50, 50);

    // Title
    titleLabel = new QLabel("M A I N   M E N U", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");
    mainLayout->addWidget(titleLabel);


    // Forest Background
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap background(":/res/background-chap.png");
    palette.setBrush(QPalette::Window, background);
    setPalette(palette);

    // Chapters Layout
    QHBoxLayout *chaptersLayout = new QHBoxLayout();
    chaptersLayout->setSpacing(20);

    // Chapter 1 (Unlocked)
    outerContainer1 = new QWidget(this);
    outerContainer1->setObjectName("chapter1Outer");
    outerContainer1->setFixedSize(200, 260);
    outerContainer1->setStyleSheet("QWidget#chapter1Outer { background-color: #353F4A; border-radius: 40px; }");


    QVBoxLayout *outerLayout1 = new QVBoxLayout(outerContainer1);
    outerLayout1->setAlignment(Qt::AlignCenter);
    outerLayout1->setContentsMargins(20, 20, 20, 20);
    outerLayout1->setSpacing(10);

    // Gray background container for the green box and description
    QWidget *contentContainer1 = new QWidget();
    QVBoxLayout *contentLayout1 = new QVBoxLayout(contentContainer1);
    contentLayout1->setSpacing(10);
    contentLayout1->setContentsMargins(0, 0, 0, 0);

    container1 = new QWidget();
    container1->setFixedSize(160, 200);
    container1->setStyleSheet("background-color: #C4FCDE; border: 3px solid #76D084; border-radius: 40px;");
    container1->setProperty("expanded", false);
    container1->installEventFilter(this);

    QVBoxLayout *content1 = new QVBoxLayout(container1);
    content1->setAlignment(Qt::AlignCenter);
    content1->setSpacing(35);

    QLabel *icon1 = new QLabel(container1);
    icon1->setPixmap(QPixmap(":/res/chap1-icon.png").scaled(140, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    icon1->setAlignment(Qt::AlignCenter);
    icon1->setStyleSheet("background: transparent; border: none;");

    QLabel *text1 = new QLabel("CHAPTER I", container1);
    text1->setAlignment(Qt::AlignCenter);
    text1->setStyleSheet("background: transparent; border: none; font-family: Mohave; color: #1D6649; font-size: 12px; font-weight: bold;");

    content1->addWidget(icon1);
    content1->addWidget(text1);

    contentLayout1->addWidget(container1);

    description1 = new QLabel();
    description1->setText("In the shadows of a world drowning in misinformation, "
                          "you emerge as a Fact Seeker. Your first mission leads you to a "
                          "small town where a mysterious herbal remedy has taken root, "
                          "promising impossible cures.");
    description1->setWordWrap(true);
    description1->setAlignment(Qt::AlignLeft);
    description1->setStyleSheet("color: white; font-family: Mohave; font-size: 12px; background: transparent; padding: 5px;");
    description1->hide();

    contentLayout1->addWidget(description1);
    contentLayout1->addStretch(); // stretches the content to the container

    outerLayout1->addWidget(contentContainer1);

    QWidget *checkmarkContainer = new QWidget();
    QHBoxLayout *checkmarkLayout = new QHBoxLayout(checkmarkContainer);
    checkmarkLayout->setAlignment(Qt::AlignCenter);
    checkmarkLayout->setContentsMargins(0, 0, 0, 0);

    checkmark1 = new QLabel();
    QPixmap checkmark(":/res/checkmark.png");
    checkmark1->setPixmap(checkmark.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    checkmark1->setFixedSize(24, 24);
    checkmark1->setStyleSheet("background: white; border-radius: 12px; padding: 4px;");
    checkmark1->setAlignment(Qt::AlignCenter);
    checkmark1->hide();
    checkmark1->setCursor(Qt::PointingHandCursor);  // cursor change, fix this tmrw. It doesn't work for some reason.
    checkmark1->installEventFilter(this);  // event filter to handle click.

    checkmarkLayout->addWidget(checkmark1);
    outerLayout1->addWidget(checkmarkContainer);
    outerLayout1->addStretch();

    // Creating the 4 other chapters
    auto createLockedChapter = [this](const QString &number, const QString &bgColor,
                                      const QString &borderColor, const QString &iconPath) {
        QWidget *outerContainer = new QWidget(this);
        outerContainer->setFixedSize(190, 250);
        outerContainer->setStyleSheet("background-color: #353F4A; border-radius: 40px;");

        QVBoxLayout *outerLayout = new QVBoxLayout(outerContainer);
        outerLayout->setAlignment(Qt::AlignCenter);
        outerLayout->setContentsMargins(10, 10, 10, 10);

        QWidget *innerContainer = new QWidget();
        innerContainer->setFixedSize(150, 200);
        innerContainer->setStyleSheet(QString("background-color: %1; border: 3px solid %2; border-radius: 40px;")
                                          .arg(bgColor, borderColor));

        QVBoxLayout *content = new QVBoxLayout(innerContainer);
        content->setAlignment(Qt::AlignCenter);
        content->setSpacing(25);

        QLabel *icon = new QLabel(innerContainer);
        icon->setPixmap(QPixmap(iconPath).scaled(140, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        icon->setAlignment(Qt::AlignCenter);
        icon->setStyleSheet("background: transparent; border: none;");

        QLabel *text = new QLabel(number, innerContainer);
        text->setAlignment(Qt::AlignCenter);
        text->setStyleSheet("background: transparent; border: none; font-family: Mohave; "
                            "color: black; font-size: 12px; font-weight: bold;");

        content->addWidget(icon);
        content->addWidget(text);

        outerLayout->addWidget(innerContainer);

        // opacity to show it locked.
        QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(outerContainer);
        opacity->setOpacity(0.7);
        outerContainer->setGraphicsEffect(opacity);

        return outerContainer;
    };

    outerContainer2 = createLockedChapter("CHAPTER II", "#CBDEFA", "#5391EF", ":/res/chap2-icon.png");
    outerContainer3 = createLockedChapter("CHAPTER III", "#E9D0C6", "#B77A72", ":/res/chap3-icon.png");
    outerContainer4 = createLockedChapter("CHAPTER IV", "#DECEE8", "#9274CE", ":/res/chap4-icon.png");
    outerContainer5 = createLockedChapter("CHAPTER V", "#C4B0BA", "#867A84", ":/res/chap5-icon.png");

    chaptersLayout->addWidget(outerContainer1);
    chaptersLayout->addWidget(outerContainer2);
    chaptersLayout->addWidget(outerContainer3);
    chaptersLayout->addWidget(outerContainer4);
    chaptersLayout->addWidget(outerContainer5);

    mainLayout->addLayout(chaptersLayout);

    // Ready Button
    readyButton = new QPushButton("I'm Ready", this);
    readyButton->setFixedSize(120, 45);
    readyButton->setObjectName("readyButton");
    connect(readyButton, &QPushButton::clicked, this, &Chapters::onReadyClicked);
    mainLayout->addWidget(readyButton, 0, Qt::AlignCenter);

    // Window setup
    QIcon windowIcon(":/res/logo.png");
    setWindowIcon(windowIcon);
    setWindowTitle("Unveiled");
    setFixedSize(1280, 750);
}

void Chapters::setupStyles()
{
    setStyleSheet(R"(
        Chapters {
            background-image: url(:/res/forest-background.png);
        }
        QLabel#titleLabel {
            color: white;
            font-size: 76px;
            font-weight: 800;
            letter-spacing: 20px;
            background: transparent;
        }
        QPushButton#readyButton {
            font-family: Raleway;
            font-weight: bold;
            background-color: #FFFFFF;
            border: 2px solid white;
            border-radius: 22px;
            color: #353F4A;
            font-size: 14px;
            padding: 8px 20px;
        }
        QPushButton#readyButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
            color: white;
            border: 2px solid #353F4A;
        }
    )");
}

bool Chapters::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == container1 && event->type() == QEvent::MouseButtonPress) {
        bool isExpanded = container1->property("expanded").toBool();

        if (currentAnimation) {
            currentAnimation->stop();
            delete currentAnimation;
        }

        QPropertyAnimation *outerAnimation = new QPropertyAnimation(outerContainer1, "minimumHeight");
        QPropertyAnimation *outerMaxAnimation = new QPropertyAnimation(outerContainer1, "maximumHeight");

        outerAnimation->setDuration(300);
        outerMaxAnimation->setDuration(300);

        if (!isExpanded) {
            outerAnimation->setStartValue(260);
            outerAnimation->setEndValue(450);
            outerMaxAnimation->setStartValue(260);
            outerMaxAnimation->setEndValue(450);

            QTimer::singleShot(50, [this]() {
                description1->show();
                checkmark1->show();
            });
        } else {
            outerAnimation->setStartValue(450);
            outerAnimation->setEndValue(260);
            outerMaxAnimation->setStartValue(450);
            outerMaxAnimation->setEndValue(260);

            description1->hide();
            checkmark1->hide();
        }

        container1->setProperty("expanded", !isExpanded);

        outerAnimation->start(QPropertyAnimation::DeleteWhenStopped);
        outerMaxAnimation->start(QPropertyAnimation::DeleteWhenStopped);

        return true;
    }
    else if (obj == checkmark1 && event->type() == QEvent::MouseButtonPress) {
        toggleChapterSelection();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void Chapters::toggleChapterSelection()
{
    isSelected = !isSelected;
    if (isSelected) {
        outerContainer1->setStyleSheet("QWidget#chapter1Outer { background-color: #353F4A; border-radius: 40px; border: 3px solid #FFFFFF; }");
    } else {
        outerContainer1->setStyleSheet("QWidget#chapter1Outer { background-color: #353F4A; border-radius: 40px; }");
    }
}

void Chapters::showChapterDescription(int chapter)
{
    // other chapter descriptions laeter
}

// to go to game screen -> delete this later after debugging

// void Chapters::onReadyClicked()
// {
//     GameWindow *gameWindow = new GameWindow();
//     gameWindow->show();
//     this->hide();
// }

void Chapters::onReadyClicked()
{
    StoryScene *scene = new StoryScene();
    scene->show();
    this->hide();
}

Chapters::~Chapters()
{
    if (currentAnimation) {
        delete currentAnimation;
    }
}
