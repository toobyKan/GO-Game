#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QVector>

enum class Color { None, Black, White };

class Board : public QWidget {
    Q_OBJECT

private:
    int boardSize;
    QVector<QVector<Color>> grid;  // 2D array representing the board
    Color currentPlayer;

public:
    explicit Board(int size = 19, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    QPoint getGridCoordinates(QPoint clickPos);
    void switchPlayer();
    void placeStone(int row, int col);

signals:
    void stonePlaced(int row, int col, Color color);
};

#endif // GO_BOARD_H
