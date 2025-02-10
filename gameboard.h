#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QObject>
#include <QVector>
#include "cell.h"

class GameBoard : public QObject {
    Q_OBJECT

public:
    explicit GameBoard(QObject *parent = nullptr);
    void initializeBoard();
    QVector<QVector<Cell*>> cells; // ⭐️ Declare ONCE as QVector<QVector<>>

public slots:
    void reveal(int x, int y);
    void toggleFlag(int x, int y);
    void resetGame();

signals:
    void gameOver(bool won);
    void updateCell(int x, int y, int adjacent, bool isRevealed);

private:
    bool minesPlaced = false;
    bool gameActive = true;
    int minesLeft = 99;

    void placeMines(int excludeX, int excludeY);
    void calculateAdjacentMines();
    void revealAdjacent(int x, int y);
    int countFlags();
};

#endif // GAMEBOARD_H
