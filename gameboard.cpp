#include "gameboard.h"
#include <QRandomGenerator>
#include <QDebug>

GameBoard::GameBoard(QObject *parent) : QObject(parent) {
    initializeBoard();
}

void GameBoard::initializeBoard() {
    cells.resize(30, QVector<Cell*>(16, nullptr));
    for (int x = 0; x < 30; ++x) {
        for (int y = 0; y < 16; ++y) {
            cells[x][y] = new Cell(x, y);
            connect(cells[x][y], &Cell::leftClicked, this, &GameBoard::reveal);
            connect(cells[x][y], &Cell::rightClicked, this, &GameBoard::toggleFlag);
        }
    }
}

void GameBoard::placeMines(int excludeX, int excludeY) {
    int minesPlaced = 0;
    while (minesPlaced < 99) {
        int x = QRandomGenerator::global()->bounded(30);
        int y = QRandomGenerator::global()->bounded(16);
        if ((x == excludeX && y == excludeY) || cells[x][y]->isMine) continue;
        cells[x][y]->isMine = true;
        minesPlaced++;
    }
    calculateAdjacentMines();
}

void GameBoard::calculateAdjacentMines() {
    for (int x = 0; x < 30; ++x) {
        for (int y = 0; y < 16; ++y) {
            if (cells[x][y]->isMine) continue;
            int count = 0;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < 30 && ny >= 0 && ny < 16 && cells[nx][ny]->isMine) count++;
                }
            }
            cells[x][y]->adjacentMines = count;
        }
    }
}

void GameBoard::reveal(int x, int y) {
    Cell* cell = cells[x][y];

    if (!gameActive || cells[x][y]->isFlagged || cells[x][y]->isQuestion || cells[x][y]->isRevealed)
        return;

    // First left-click: place mines (even if flags exist)
    if (!minesPlaced) {
        placeMines(x, y); // Exclude the clicked cell
        minesPlaced = true;
        minesLeft = 99 - countFlags(); // Adjust for pre-placed flags
    }

    // Block left-clicks on flagged/question-marked cells
    if (!gameActive || cell->isFlagged || cell->isQuestion || cell->isRevealed) {
        return;
    }

    // Prevent interaction with flagged/question-marked or revealed cells
    if (!gameActive || cells[x][y]->isFlagged || cells[x][y]->isQuestion || cells[x][y]->isRevealed)
        return;

    // First click: place mines AFTER first click to ensure safety
    if (!cells[x][y]->isRevealed && minesLeft == 99) {
        placeMines(x, y);
        minesLeft = 0;
    }

    cells[x][y]->isRevealed = true;
    cells[x][y]->updateAppearance();

    if (cells[x][y]->isMine) {
        gameActive = false;
        emit gameOver(false);
        return;
    }

    if (cells[x][y]->adjacentMines == 0) revealAdjacent(x, y);

    // Check win condition
    int unrevealedSafe = 0;
    for (int x = 0; x < 30; ++x) {
        for (int y = 0; y < 16; ++y) {
            if (!cells[x][y]->isRevealed && !cells[x][y]->isMine) unrevealedSafe++;
        }
    }
    if (unrevealedSafe == 0) {
        gameActive = false;
        emit gameOver(true);
    }
}

void GameBoard::revealAdjacent(int x, int y) {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < 30 && ny >= 0 && ny < 16) {
                // Skip flagged/question-marked cells
                if (!cells[nx][ny]->isFlagged && !cells[nx][ny]->isQuestion) {
                    reveal(nx, ny);
                }
            }
        }
    }
}

void GameBoard::toggleFlag(int x, int y) {
    if (!gameActive || cells[x][y]->isRevealed) return;

    Cell* cell = cells[x][y];

    if (!cell->isFlagged && !cell->isQuestion) {
        cell->isFlagged = true;
        if (minesPlaced) minesLeft--; // Only decrement if mines exist
    } else if (cell->isFlagged) {
        cell->isFlagged = false;
        cell->isQuestion = true;
        if (minesPlaced) minesLeft++; // Only increment if mines exist
    } else if (cell->isQuestion) {
        cell->isQuestion = false;
    }

    cell->updateAppearance();
}

void GameBoard::resetGame() {
    gameActive = true;
    minesPlaced = false;
    minesLeft = 99;

    for (int x = 0; x < 30; ++x) {
        for (int y = 0; y < 16; ++y) {
            cells[x][y]->reset();
        }
    }
}

int GameBoard::countFlags() {
    int flags = 0;
    for (int x = 0; x < 30; ++x) {
        for (int y = 0; y < 16; ++y) {
            if (cells[x][y]->isFlagged) flags++;
        }
    }
    return flags;
}
