#ifndef BOARD_GEOMETRY_H
#define BOARD_GEOMETRY_H

class BoardGeometry {
public:
    BoardGeometry(
        int board_size,
        int window_width,
        int window_height
    );

    void resize(int window_width, int window_height);

    int gridToPixelX(int grid_x) const;
    int gridToPixelY(int grid_y) const;

    int getBoardStartX() const;
    int getBoardStartY() const;
    int getBoardEndX() const;
    int getBoardEndY() const;

    int getStoneRadius() const;

    bool screenToGrid(
        int mouse_x,
        int mouse_y,
        int& grid_x,
        int& grid_y
    ) const;

private:
    int coordinateToPixel(int coordinate) const;

    int board_size_;
    int window_width_;
    int window_height_;
    int board_pixels_;

    int origin_x_;
    int origin_y_;

    double spacing_;
};

#endif