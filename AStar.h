#pragma once
//холоунайт
#include <vector>
#include <optional>
#include <queue>
#include <stdexcept>
#include <limits>
#include <ranges>
#include <algorithm>

#include"Point.h"
#include"Path.h"
#include"GraphMap.h"
#include"Heuristic.h"

class AStar {
private:
    ManhattanHeuristic h{};

    bool is_valid_req(const Map &map,const Point &start,const Point &goal) const{
        return map.in_bound(start) && map.in_bound(goal) && map.is_passable(start) && map.is_passable(goal);
    }

    std::vector<Point> reconstruct_path(const Map& map,const std::vector<int>& cameFrom,int startId,int goalId) const {
        std::vector<Point> nodes;
        int current = goalId;
        while (current != -1) {
            nodes.push_back(map.id_to_cor(current));
            if (current == startId) break;
            current = cameFrom[current];
        }

        if (nodes.empty() || map.cor_to_id(nodes.back()) != startId) {
            nodes.clear();
            return nodes;
        }

        std::reverse(nodes.begin(),nodes.end());
        return nodes;
    }

public:
    std::optional<Path> find_path(const Map& map,Point start, Point goal) const {
        if (!is_valid_req(map,start,goal)) return std::nullopt;
        const int N = map.Size();
        constexpr int INF = std::numeric_limits<int>::max() / 4;

        const int startId = map.cor_to_id(start);
        const int goalId  = map.cor_to_id(goal);

        std::vector<int> gScore(N, INF);
        std::vector<int> cameFrom(N, -1);
        std::vector<unsigned char> closed(N, 0);

        gScore[startId] = 0;

        struct Node { int id; int g; int f; };

        struct Cmp {
            bool operator()(const Node& a, const Node& b) const noexcept {
                return a.f > b.f; // min f on top
            }
        };

        std::priority_queue<Node, std::vector<Node>, Cmp> open;

        const int h0 = h.h_result(start, goal); // или h_.h_result(start, goal) — если ты так оставишь
        open.push(Node{ startId, 0, h0 });

        std::vector<Point> neigh;
        neigh.reserve(4);

        while (!open.empty()) {
            const Node cur = open.top();
            open.pop();

            if (cur.g != gScore[cur.id]) continue; // stale
            if (closed[cur.id]) continue;

            if (cur.id == goalId) {
                auto nodes = reconstruct_path(map, cameFrom, startId, goalId);
                if (nodes.empty()) return std::nullopt; // защита, если цепочка сломана
                return Path(std::move(nodes), gScore[goalId]);
            }

            closed[cur.id] = 1;

            const Point cur_p = map.id_to_cor(cur.id);

            neigh.clear();
            map.neighbors(cur_p, neigh);

            for (const Point& next_point : neigh) {
                int next_pointId = map.cor_to_id(next_point);
                if (closed[next_pointId]) continue;
                int step = map.weight(cur_p,next_point);
                int tentative_g = gScore[cur.id] + step;
                if (tentative_g < gScore[next_pointId]) {
                    gScore[next_pointId] = tentative_g;
                    cameFrom[next_pointId] = cur.id;
                    int f = tentative_g + h.h_result(next_point, goal);
                    open.push(Node{next_pointId, tentative_g, f});
                }
            }
        }

        return std::nullopt;


    }
};