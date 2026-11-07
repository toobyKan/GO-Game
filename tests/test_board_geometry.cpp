#include "board_geometry.hpp"

#include <iostream>
#include <string>

namespace {

int failures = 0;

void expect(bool condition, const std::string& description) {
    if (!condition) {
        ++failures;
        std::cerr << "FAIL: " << description << '\n';
    }
}

void expectGridPoint(
    const BoardGeometry& geometry,
    int pixel_x,
    int pixel_y,
    int expected_x,
    int expected_y,
    const std::string& description
) {
    int grid_x = -1;
    int grid_y = -1;

    expect(
        geometry.screenToGrid(pixel_x, pixel_y, grid_x, grid_y),
        description + " is inside the board"
    );
    expect(grid_x == expected_x && grid_y == expected_y,
           description + " maps to the expected intersection");
}

void test_square_window() {
    BoardGeometry geometry(19, 800, 800);

    expect(geometry.getBoardStartX() == 30,
           "square board has a horizontal margin");
    expect(geometry.getBoardStartY() == 30,
           "square board has a vertical margin");
    expect(geometry.getBoardEndX() == 769,
           "square board ends at the expected x coordinate");
    expect(geometry.getBoardEndY() == 769,
           "square board ends at the expected y coordinate");

    expectGridPoint(
        geometry,
        geometry.gridToPixelX(0),
        geometry.gridToPixelY(0),
        0,
        0,
        "top-left intersection"
    );

    expectGridPoint(
        geometry,
        geometry.gridToPixelX(18),
        geometry.gridToPixelY(18),
        18,
        18,
        "bottom-right intersection"
    );
}

void test_wide_window() {
    BoardGeometry geometry(19, 1200, 800);

    expect(geometry.getBoardStartX() == 230,
           "wide window centers the board horizontally");
    expect(geometry.getBoardStartY() == 30,
           "wide window preserves the vertical margin");
    expect(geometry.getBoardEndX() == 969,
           "wide window preserves a square board width");
    expect(geometry.getBoardEndY() == 769,
           "wide window uses the available board height");
}

void test_tall_window() {
    BoardGeometry geometry(19, 800, 1200);

    expect(geometry.getBoardStartX() == 30,
           "tall window preserves the horizontal margin");
    expect(geometry.getBoardStartY() == 230,
           "tall window centers the board vertically");
    expect(geometry.getBoardEndX() == 769,
           "tall window uses the available board width");
    expect(geometry.getBoardEndY() == 969,
           "tall window preserves a square board height");
}

void test_resize_updates_geometry() {
    BoardGeometry geometry(19, 800, 800);
    const int originalEnd = geometry.getBoardEndX();

    geometry.resize(1200, 800);

    expect(geometry.getBoardStartX() == 230,
           "resize updates the horizontal origin");
    expect(geometry.getBoardEndX() != originalEnd,
           "resize updates the board dimensions");
}

void test_outside_clicks_are_rejected() {
    BoardGeometry geometry(19, 800, 800);
    int grid_x = -1;
    int grid_y = -1;

    expect(!geometry.screenToGrid(0, 0, grid_x, grid_y),
           "click outside the board is rejected");
    expect(!geometry.screenToGrid(799, 799, grid_x, grid_y),
           "click in the lower-right margin is rejected");
}

} // namespace

int main() {
    test_square_window();
    test_wide_window();
    test_tall_window();
    test_resize_updates_geometry();
    test_outside_clicks_are_rejected();

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << "All geometry tests passed\n";
    return 0;
}
