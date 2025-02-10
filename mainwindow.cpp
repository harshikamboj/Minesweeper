#include "mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);
    gameBoard = new GameBoard(this);

    // Add cells to grid
    for (int x = 0; x < 30; ++x) {
        for (int y = 0; y < 16; ++y) {
            gridLayout->addWidget(gameBoard->cells[x][y], y, x);
        }
    }

    setCentralWidget(centralWidget);
    connect(gameBoard, &GameBoard::gameOver, this, &MainWindow::handleGameOver);
}

MainWindow::~MainWindow() {
    delete gameBoard;
    delete gridLayout;
    delete centralWidget;
}

void MainWindow::handleGameOver(bool won) {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Game Over", won ? "You won! Restart?" : "You lost! Restart?",
        QMessageBox::Yes | QMessageBox::No
        );
    if (reply == QMessageBox::Yes) resetGame();
}

void MainWindow::resetGame() {
    gameBoard->resetGame();
}
