#ifndef CHAPTERS_H
#define CHAPTERS_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPropertyAnimation>

class Chapters : public QWidget
{
    Q_OBJECT
public:
    explicit Chapters(QWidget *parent = nullptr);
    ~Chapters();

private slots:
    void onReadyClicked();

private:
    void setupUI();
    void setupStyles();
    bool eventFilter(QObject *obj, QEvent *event) override;
    void showChapterDescription(int chapter);
    bool isSelected = false;
    void toggleChapterSelection();

    // Core UI elements
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QPushButton *readyButton;

    // Chapter containers
    QWidget *outerContainer1, *outerContainer2, *outerContainer3, *outerContainer4, *outerContainer5;
    QWidget *container1, *container2, *container3, *container4, *container5;
    QLabel *description1;
    QLabel *checkmark1;

    // Animation handling
    QPropertyAnimation *currentAnimation = nullptr;

    // Tracking
    QList<QLabel*> chapterLabels;
    QVector<bool> chapterUnlocked;
};

#endif // CHAPTERS_H
