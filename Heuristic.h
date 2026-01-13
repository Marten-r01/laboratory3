#pragma once
#include "Point.h"
#include <math.h>

struct ManhattanHeuristic {
    static constexpr int min_step_cost = 10;

    static int h_result(Point cur, Point finish) noexcept {
        return (std::abs(finish.x - cur.x) + std::abs(finish.y - cur.y)) * min_step_cost;
    }
};
