#pragma once

#include <QWidget>
#include <optional>

#include "GraphMap.h"
#include "Point.h"
#include "Path.h"
#include "AStar.h"
#include "Terrain.h"

class QLabel;
class QPushButton;
class QButtonGroup;
class MapWidget;

class EditorWindow final : public QWidget {
    Q_OBJECT
public:
    explicit EditorWindow(Map map, QWidget* parent = nullptr);

private slots:
    void onCellClicked(Point p);
    void onTerrainSelected(int id);
    void onSetStartMode();
    void onSetGoalMode();

private:
    enum class Tool { PaintTerrain, SetStart, SetGoal };

    void recomputePath();
    void updateDistanceLabelNotComputed();
    void updateDistanceLabelUnreachable();
    void updateDistanceLabelValue(int cost);

    Map map_;
    AStar astar_;

    Tool tool_ = Tool::PaintTerrain;
    PointType selected_ = PointType::Grass;

    std::optional<Point> start_;
    std::optional<Point> goal_;
    std::optional<Path> path_;

    MapWidget* mapWidget_ = nullptr;
    QLabel* distanceLabel_ = nullptr;

    QButtonGroup* terrainGroup_ = nullptr;
    QPushButton* startBtn_ = nullptr;
    QPushButton* goalBtn_ = nullptr;
};
