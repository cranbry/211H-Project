#include "mainwindow.h"
#include "chapters.h"
#include <QApplication>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupStyles();
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QGridLayout *gridLayout = new QGridLayout(centralWidget);
    gridLayout->setContentsMargins(50, 20, 50, 50);

    // Title without logo now
    titleLabel = new QLabel("U N V E I L E D", centralWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(titleLabel, 0, 0, 1, 3);

    // Description string first
    QString description = "Unveiled is an interactive story-driven game where players take on the role "
                          "of a Fact Seeker in a world plagued by rumors, conspiracies, and fake news. "
                          "Navigate through gripping chapters, interview characters, solve puzzles, "
                          "and make critical decisions to expose misinformation and uncover the truth. "
                          "Will you debunk the lies or let the world fall to confusion?";

    // Description and logo enxt to each other
    QHBoxLayout *descriptionLayout = new QHBoxLayout();

    descriptionLabel = new QLabel(description, centralWidget);
    descriptionLabel->setAlignment(Qt::AlignCenter);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setFixedWidth(800);
    descriptionLayout->addWidget(descriptionLabel);

    // Logo next to description
    QLabel *logoLabel = new QLabel(centralWidget);
    QPixmap logo(":/res/logo.png"); // change location to new one when I have
    logoLabel->setPixmap(logo.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    descriptionLayout->addWidget(logoLabel);

    gridLayout->addLayout(descriptionLayout, 1, 0, 1, 3);

    startButton = new QPushButton("Start", centralWidget);
    startButton->setFixedSize(120, 45);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    gridLayout->addWidget(startButton, 2, 1, Qt::AlignBottom | Qt::AlignCenter);

    gridLayout->setRowStretch(0, 1);
    gridLayout->setRowStretch(1, 2);
    gridLayout->setRowStretch(2, 3);

    // Window Size and Icon
    QIcon windowIcon(":/res/logo.png");
    setWindowIcon(windowIcon);

    setWindowTitle("Unveiled");
    resize(1060, 610);
}

void MainWindow::setupStyles()
{
    setStyleSheet(R"(
        QMainWindow {
            background-image: url(:/res/forest-background.png);
            background-position: center;
            background-repeat: no-repeat;
            background-color: #1A1A1A;
        }
        QLabel#titleLabel {
            font-family: Arial;
            font-size: 72px;
            color: white;
            letter-spacing: 15px;
            font-weight: bold;
            margin-top: 20px;
        }
        QLabel#descriptionLabel {
            font-family: Arial;
            font-size: 14px;
            color: white;
            line-height: 1.6;
        }
        QPushButton {
            background-color: transparent;
            border: 2px solid white;
            border-radius: 22px;
            font-family: Arial;
            letter-spacing: 5px;
            color: white;
            text-align: center;
            font-size: 16px;
            padding: 10px 30px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
        }
    )");

    titleLabel->setObjectName("titleLabel");
    descriptionLabel->setObjectName("descriptionLabel");
}

void MainWindow::onStartClicked()
{
    Chapters *chapters = new Chapters();
    chapters->show();
    this->hide();
}

MainWindow::~MainWindow()
{
}
