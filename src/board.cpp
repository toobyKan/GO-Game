#include "../includes/board.hpp"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

Board::Board(int size, QWidget *parent)
    : QWidget(parent), boardSize(size), currentPlayer(Color::Black) {
    // Initialize the grid with empty positions
    grid.resize(boardSize);
    for (int i = 0; i < boardSize; ++i) {
        grid[i].resize(boardSize, Color::None);
    }

    setMinimumSize(600, 600);  // Set minimum size for the board
}

void Board::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    int w = width();
    int h = height();

    int cellSize = qMin(w, h) / boardSize;  // Calculate the size of each cell

    // Draw the grid
    painter.setPen(Qt::black);
    for (int i = 0; i < boardSize; ++i) {
        // Draw horizontal lines
        painter.drawLine(cellSize / 2, i * cellSize + cellSize / 2, boardSize * cellSize - cellSize / 2, i * cellSize + cellSize / 2);
        // Draw vertical lines
        painter.drawLine(i * cellSize + cellSize / 2, cellSize / 2, i * cellSize + cellSize / 2, boardSize * cellSize - cellSize / 2);
    }

    // Draw the stones
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            if (grid[row][col] != Color::None) {
                if (grid[row][col] == Color::Black) {
                    painter.setBrush(Qt::black);
                } else {
                    painter.setBrush(Qt::white);
                }
                // Draw the stone as a circle
                int x = col * cellSize + cellSize / 2;
                int y = row * cellSize + cellSize / 2;
                painter.drawEllipse(QPoint(x, y), cellSize / 2 - 2, cellSize / 2 - 2);
            }
        }
    }
}

void Board::mousePressEvent(QMouseEvent *event) {
    QPoint clickPos = event->pos();
    QPoint gridPos = getGridCoordinates(clickPos);

    int row = gridPos.y();
    int col = gridPos.x();

    // Make sure the clicked position is within bounds and the cell is empty
    if (row >= 0 && row < boardSize && col >= 0 && col < boardSize && grid[row][col] == Color::None) {
        placeStone(row, col);  // Place the stone
        update();  // Trigger a repaint

        switchPlayer();  // Switch turns
    }
}

QPoint Board::getGridCoordinates(QPoint clickPos) {
    int w = width();
    int h = height();
    int cellSize = qMin(w, h) / boardSize;

    int col = clickPos.x() / cellSize;
    int row = clickPos.y() / cellSize;

    return QPoint(col, row);
}

void Board::placeStone(int row, int col) {
    grid[row][col] = currentPlayer;  // Place the stone for the current player
    emit stonePlaced(row, col, currentPlayer);  // Signal for stone placement
}

void Board::switchPlayer() {
    currentPlayer = (currentPlayer == Color::Black) ? Color::White : Color::Black;
}
