#pragma once

#include <array>
#include <string_view>

#include "GraphMap.h"
#include "Terrain.h"
#include "Point.h"

inline Map make_map_1() {
    constexpr std::array<std::string_view, 20> rows = {
        "GGGGGGGGGGGGGGGGGGGG",
        "GGGGGGGGGGGGGGGGGGGG",
        "GGGGGGGGGGGLLLLLGGGG",
        "GGGGGGGGGGLLLLLLLLGG",
        "GGGGGGGGGSWLLLLLLLLG",
        "GGGGGGGSSWWWSLLLLLLL",
        "GGGGGSSSWWWDWWSLLLLL",
        "GGGGGSSSWWDDDWWWSSGG",
        "GGGGGSSSWDDDDDWWSSGG",
        "GGGGGGGSSWWWDWWWSGGG",
        "GGGGGGGGSSSWWWWSSGGG",
        "GGGGGGGGGGSSSSSSSSGG",
        "GGGGGGGGGGGSGSSGSGGG",
        "LGGGGGGGGGGGGGGGGGGG",
        "LLLGGGGGGGGGGGGGGSSS",
        "LLLLLGGGGGGGGGGGSSSS",
        "LLLLLLLGGGGGGGSSSSSS",
        "LLLLLLLLGGGGGSSSSSSS",
        "LLLLLLLGGGGGGSSSSSSS",
        "LLLLGGGGGGGGGGSSSSSS"
    };

    Map map;

    for (int i=0; i<map.Height(); i++) {
        for (int j=0; j<map.Width(); j++) {
            const char c = rows[i][j];
            const Point p {j, i};
            map.setTile(p,point_type_from_char(c));
        }
    }
    auto must = [&](int x, int y, PointType expected, const char* msg) {
        const auto got = map.tile(Point{x,y});
        if (got != expected) throw std::runtime_error(msg);
    };

    // Координаты из твоих строк rows (в 0-индексации)
    must(0, 0, PointType::Grass, "Expected Grass at (0,0)");
    must(9, 4, PointType::Sand,  "Expected Sand at (9,4)");      // rows[4][9] == 'S'
    must(10,4, PointType::Water, "Expected Water at (10,4)");     // rows[4][10] == 'W'
    must(11,6, PointType::DeepWater, "Expected DeepWater at (11,6)"); // rows[6][11] == 'D'


    return map;
}

inline Map make_map_2() {
    constexpr std::array<std::string_view, 20> rows = {
        "GGGGSSSSSSSSSSSSSGGGG",
        "GGGGSSSSSSSSSSSSSGGGG",
        "GGGGSSSSSSSSSSSSSGGGG",
        "GGGGGGSSSSSSSSSSGGGGG",
        "GGGGGGSSSSSSSSSSGGGGG",
        "GGGGGGGGSSSSSSGGGGGGG",
        "GGGGGGGGSSSSSSGGGGGGG",
        "GGGGGGGGGGGGGGGGGGGGG",
        "GGGGGGGGGGGGGGGGGGGGG",
        "GGGGGGGGGGGGGGGGGGGGG",
        "GGGGGGGGGGGGGGGGGGGGG",
        "GGGSSSSSSSSSSSSSSSGGG",
        "SSSWWWWWWWWWWWWWWWSSS",
        "SSWWWWWDDDDDDWWWWWWSS",
        "SWWWWWDDDDDDDDWWWWWWS",
        "LWWWWWDDDDDDDDWWWWWWL",
        "LLWWWWWDDDDDDWWWWWWLL",
        "LLLWWWWWWWWWWWWWWWLLL",
        "LLLLLLLLGGGGLLLLLLLLL",
        "LLLLLLLLLLLLLLLLLLLLL"

    };
    Map map;

    for (int i=0; i<20; i++) {
        for (int j=0; j<20; j++) {
            const char c = rows[i][j];
            const Point p {j, i};
            map.setTile(p,point_type_from_char(c));
        }
    }

    return map;
}

