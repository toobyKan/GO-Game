#include "board_geometry.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

int failures = 0;

void expect(bool condition, const std::string& description) {
    if (!condition) {
        ++failures;
        std::cerr << "FAIL: " << description << '\n';
    }
}

void test_round_trip_mapping() {
    BoardGeometry geometry(19, 1200, 800);

    for (int x = 0; x < 19; ++x) {
        for (int y = 0; y < 19; ++y) {
            int mappedX = -1;
            int mappedY = -1;

            const bool mapped = geometry.screenToGrid(
                geometry.gridToPixelX(x),
                geometry.gridToPixelY(y),
                mappedX,
                mappedY
            );

            expect(mapped,
                   "rendered intersection maps back to the board");
            expect(mappedX == x && mappedY == y,
                   "rendered intersection maps to its original coordinates");
        }
    }
}

void test_single_point_board() {
    BoardGeometry geometry(1, 200, 100);
    int gridX = -1;
    int gridY = -1;

    expect(geometry.screenToGrid(
               geometry.gridToPixelX(0),
               geometry.gridToPixelY(0),
               gridX,
               gridY
           ),
           "single-point board accepts its only intersection");
    expect(gridX == 0 && gridY == 0,
           "single-point board maps to coordinate zero");
}

void test_invalid_board_size() {
    bool threw = false;

    try {
        BoardGeometry geometry(0, 800, 800);
        (void)geometry;
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    expect(threw,
           "zero-sized geometry is rejected");
}

void test_resize_preserves_round_trip_mapping() {
    BoardGeometry geometry(9, 800, 800);
    geometry.resize(1400, 700);

    int gridX = -1;
    int gridY = -1;

    const bool mapped = geometry.screenToGrid(
        geometry.gridToPixelX(8),
        geometry.gridToPixelY(4),
        gridX,
        gridY
    );

    expect(mapped,
           "resized geometry accepts rendered intersections");
    expect(gridX == 8 && gridY == 4,
           "resized geometry preserves coordinate mapping");
}

} // namespace

int main() {
    test_round_trip_mapping();
    test_single_point_board();
    test_invalid_board_size();
    test_resize_preserves_round_trip_mapping();

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << "All additional geometry tests passed\n";
    return 0;
}
