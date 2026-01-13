#pragma once

#include "Terrain.h"
#include "Point.h"

#include <vector>
#include <stdexcept>
#include <array>

class Map {
private:
    static constexpr int width = 20;
    static constexpr int height = 20;
    static constexpr int size = width * height;
    std::array<PointType, size> points{};

public:
    Map() {
    };

    constexpr int Width() const noexcept { return width; }
    constexpr int Height() const noexcept { return height; }
    constexpr int Size() const noexcept { return size; }


    bool in_bound(Point p) const {
        return (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height);
    }

    PointType tile(Point p) const {
        if (!in_bound(p)) {
            throw std::out_of_range("point out of map");
        }
        return points[cor_to_id(p)];
    }

    void setTile(Point p, PointType t) {
        if (!in_bound(p)) {
            throw std::out_of_range("point out of map");
        }
        points[cor_to_id(p)] = t;
    }

    Point id_to_cor(int id) const {
        Point p;
        p.x = id % width;
        p.y = id / width;
        return p;
    }

    int cor_to_id(Point p) const {
        return (p.x + p.y * width);
    }

    void neighbors(Point p, std::vector<Point> &neighbors) const {
        neighbors.clear();
        Point p1(p.x - 1, p.y);
        if (in_bound(p1) && is_passable(p1)) {
            neighbors.push_back(p1);
        }
        Point p2(p.x + 1, p.y);
        if (in_bound(p2) && is_passable(p2)) {
            neighbors.push_back(p2);
        }
        Point p3(p.x, p.y - 1);
        if (in_bound(p3) && is_passable(p3)) {
            neighbors.push_back(p3);
        }
        Point p4(p.x, p.y + 1);
        if (in_bound(p4) && is_passable(p4)) {
            neighbors.push_back(p4);
        }
    }

    bool is_passable(Point v) const {
        if (!in_bound(v)) {
            throw std::out_of_range("point out of map");
        }
        return passable(tile(v));
    }

    int weight(Point u, Point v) const {
        if (!in_bound(u) || !in_bound(v)) {
            throw std::out_of_range("point out of map");
        }
        return step_cost(tile(v));
    }

    static constexpr int min_step_cost() noexcept {
        return 10;
    }
};

