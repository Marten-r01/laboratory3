#pragma once

enum class PointType {
    Grass,
    Sand,
    Water,
    DeepWater,
    Lava
};
constexpr bool passable(PointType type) {
    return type!= PointType::Lava;
}
constexpr int step_cost(PointType type) {
    switch (type) {
            case PointType::Grass: return 10;
            case PointType::Sand: return 20;
            case PointType::Water: return 30;
            case PointType::DeepWater: return 60;
            case PointType::Lava: return 0;
            default: return 0;
    }
}

constexpr PointType point_type_from_char(char c) {
    switch (c) {
            case 'G': return PointType::Grass;
            case 'S': return PointType::Sand;
            case 'W': return PointType::Water;
            case 'D': return PointType::DeepWater;
            case 'L': return PointType::Lava;
            default: return PointType::Grass;
    }
}