#include <QApplication>
#include <QDebug>

// for testing quiz

// #include "quiz.h"

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     Quiz quiz;
//     quiz.show();
//     return a.exec();
// }

// for main game:

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
