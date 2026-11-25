#ifndef MOVE_RESULT_H
#define MOVE_RESULT_H


enum class MoveStatus {
    Accepted,
    Pending,
    Occupied,
    Suicide,
    Ko,
    OutOfBounds,
    NetworkError,
};

struct MoveResult {
    MoveStatus status;
};

#endif
