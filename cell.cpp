#include "cell.h"
#include <QIcon>
#include <QDebug>

Cell::Cell(int x, int y, QWidget *parent)
    : QPushButton(parent), x(x), y(y)
{
    setFixedSize(30, 30);
    reset();
}

void Cell::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        emit leftClicked(x, y);
    }
    else if (e->button() == Qt::RightButton) {
        emit rightClicked(x, y);
    }
}

void Cell::reset() {
    isMine = false;
    isRevealed = false;
    isFlagged = false;
    isQuestion = false;
    adjacentMines = 0;

    // Reset visual properties
    setEnabled(true);
    setText("");
    setIcon(QIcon());
    setStyleSheet(
        "QPushButton {"
        "   background-color: #C0C0C0;"
        "   border: 2px solid;"
        "   border-top-color: #FFFFFF;"
        "   border-left-color: #FFFFFF;"
        "   border-right-color: #808080;"
        "   border-bottom-color: #808080;"
        "}"
        );
}

void Cell::updateAppearance() {
    if (isRevealed) {
        // Revealed cell styling
        setStyleSheet(
            "QPushButton {"
            "   background-color: #FFFFFF;"
            "   border: 1px solid #808080;"
            "}"
            );

        if (isMine) {
            setIcon(QIcon(":/icons/mine.png")); // Mine icon
            setText("");
        } else if (adjacentMines > 0) {
            setText(QString::number(adjacentMines));
            // Different colors for different numbers
            QColor textColor;
            switch(adjacentMines) {
            case 1: textColor = Qt::blue; break;
            case 2: textColor = Qt::darkGreen; break;
            case 3: textColor = Qt::red; break;
            case 4: textColor = Qt::darkBlue; break;
            default: textColor = Qt::black; break;
            }
            setStyleSheet(QString("color: %1").arg(textColor.name()));
        }

    } else {
        // Hidden cell states
        setEnabled(true); // Ensure cell is clickable
        setText("");

        if (isFlagged) {
            setIcon(QIcon(":/icons/flag.png"));
        } else if (isQuestion) {
            setIcon(QIcon(":/icons/question.png"));
        } else {
            setIcon(QIcon()); // No icon for unmarked cells
        }

        // Add subtle styling for question marks
        if (isQuestion) {
            setStyleSheet("QPushButton { background-color: #C0C0C0; color: #000000; }");
        }
    }
}
