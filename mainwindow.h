#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "gameboard.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleGameOver(bool won);
    void resetGame();

private:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    GameBoard *gameBoard;
};

#endif // MAINWINDOW_H
