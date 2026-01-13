#pragma once

struct Point {
    int x{};
    int y{};

    Point(int x_, int y_):x{x_},y{y_} {}

    Point () : x(0), y(0){}

    bool operator==(const Point &other) const {
        return this->x == other.x && this->y == other.y;
    }
};
