#pragma once

#include "Point.h"
#include <vector>
#include <utility>

struct Path {
private:

    std::vector<Point> path;
    int final_cost=0;

public:

    Path(std::vector<Point> nodes, int cost) {
        final_cost=cost;
        path = std::move(nodes);
    }

    int get_final_cost() const{
        return final_cost;
    }

    const std::vector<Point>& get_path() const{
        return path;
    }
};