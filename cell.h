#ifndef CELL_H
#define CELL_H

#include <QPushButton>
#include <QMouseEvent>

class Cell : public QPushButton {
    Q_OBJECT

public:
    explicit Cell(int x, int y, QWidget *parent = nullptr);
    int x, y; // Grid position
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    bool isQuestion = false;
    int adjacentMines = 0;

    void reset();
    void updateAppearance();

signals:
    void leftClicked(int x, int y);
    void rightClicked(int x, int y);

protected:
    void mousePressEvent(QMouseEvent *e) override;
};

#endif // CELL_H
