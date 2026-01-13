#include "EditorWindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QFrame>
#include <QString>

#include "MapWidget.h"

namespace {
QString cardButtonStyle() {
    return R"(
        QPushButton {
            background: white;
            border: 1px solid #dcdcdc;
            border-radius: 12px;
            padding: 0;
            text-align: left;
        }
        QPushButton:checked {
            background: #fff4b8;
        }
    )";
}

QPushButton* makeTerrainButton(QWidget* parent, const QString& title, const QString& subtitle, const QString& colorHex) {
    auto* btn = new QPushButton(parent);
    btn->setCheckable(true);
    btn->setMinimumHeight(92);
    btn->setStyleSheet(cardButtonStyle());

    auto* frame = new QFrame(btn);
    frame->setStyleSheet("background: transparent;");
    auto* lay = new QHBoxLayout(frame);
    lay->setContentsMargins(12, 12, 12, 12);
    lay->setSpacing(12);

    auto* swatch = new QFrame(frame);
    swatch->setFixedSize(64, 64);
    swatch->setStyleSheet(QString("background:%1; border: 1px solid black;").arg(colorHex));

    auto* textLay = new QVBoxLayout;
    auto* t1 = new QLabel(title, frame);
    t1->setStyleSheet("font-weight: 800; font-size: 16px;");
    auto* t2 = new QLabel(subtitle, frame);
    t2->setStyleSheet("font-size: 14px;");
    textLay->addWidget(t1);
    textLay->addWidget(t2);
    textLay->addStretch(1);

    lay->addWidget(swatch);
    lay->addLayout(textLay);

    auto* outer = new QHBoxLayout(btn);
    outer->setContentsMargins(0,0,0,0);
    outer->addWidget(frame);

    return btn;
}

QString smallButtonStyle() {
    return R"(
        QPushButton {
            background: white;
            border: 1px solid #dcdcdc;
            border-radius: 10px;
            padding: 10px;
            font-size: 15px;
            font-weight: 700;
        }
        QPushButton:checked {
            background: #e6ecff;
        }
        QPushButton:hover { background: #f6f6f6; }
    )";
}
}

EditorWindow::EditorWindow(Map map, QWidget* parent)
    : QWidget(parent), map_(std::move(map))
{
    setWindowTitle("A* — карта");
    setMinimumSize(1200, 800);

    auto* root = new QHBoxLayout(this);
    root->setContentsMargins(10, 10, 10, 10);
    root->setSpacing(10);

    // Left: grid widget
    mapWidget_ = new MapWidget(this);
    mapWidget_->setMap(&map_);
    root->addWidget(mapWidget_, 1);
    connect(mapWidget_, &MapWidget::cellClicked, this, &EditorWindow::onCellClicked);

    // Right: panel
    auto* panel = new QFrame(this);
    panel->setFixedWidth(340);
    panel->setStyleSheet("background:#efefef; border-radius:12px;");

    auto* panelLay = new QVBoxLayout(panel);
    panelLay->setContentsMargins(12, 12, 12, 12);
    panelLay->setSpacing(12);

    terrainGroup_ = new QButtonGroup(this);
    terrainGroup_->setExclusive(true);

    // 0..4 ids
    auto* bGrass = makeTerrainButton(panel, "Трава", "стоимость: 10", "#4fe36c");
    auto* bSand  = makeTerrainButton(panel, "Песок", "стоимость: 20", "#f7d074");
    auto* bWater = makeTerrainButton(panel, "Вода", "стоимость: 30", "#bfe9ff");
    auto* bDeep  = makeTerrainButton(panel, "Глубоководье", "стоимость: 60", "#2f5cff");
    auto* bForest= makeTerrainButton(panel, "Лес", "непроходимая", "#2e7d32");

    terrainGroup_->addButton(bGrass, 0);
    terrainGroup_->addButton(bSand, 1);
    terrainGroup_->addButton(bWater, 2);
    terrainGroup_->addButton(bDeep, 3);
    terrainGroup_->addButton(bForest,4);

    panelLay->addWidget(bGrass);
    panelLay->addWidget(bSand);
    panelLay->addWidget(bWater);
    panelLay->addWidget(bDeep);
    panelLay->addWidget(bForest);

    bGrass->setChecked(true);

    connect(terrainGroup_, &QButtonGroup::idClicked, this, &EditorWindow::onTerrainSelected);

    // Start/Goal buttons
    auto* row = new QHBoxLayout;
    startBtn_ = new QPushButton("Начало", panel);
    goalBtn_  = new QPushButton("Конец", panel);
    startBtn_->setCheckable(true);
    goalBtn_->setCheckable(true);
    startBtn_->setStyleSheet(smallButtonStyle());
    goalBtn_->setStyleSheet(smallButtonStyle());
    startBtn_->setMinimumHeight(44);
    goalBtn_->setMinimumHeight(44);

    row->addWidget(startBtn_);
    row->addWidget(goalBtn_);
    panelLay->addLayout(row);

    connect(startBtn_, &QPushButton::clicked, this, &EditorWindow::onSetStartMode);
    connect(goalBtn_, &QPushButton::clicked, this, &EditorWindow::onSetGoalMode);

    // Distance label
    distanceLabel_ = new QLabel(panel);
    distanceLabel_->setMinimumHeight(56);
    distanceLabel_->setStyleSheet(
        "background:white; border:1px solid #dcdcdc; border-radius:12px; padding:14px;"
        "font-size:18px; font-weight:800;"
    );
    panelLay->addWidget(distanceLabel_);
    panelLay->addStretch(1);

    root->addWidget(panel);

    updateDistanceLabelNotComputed();
}

void EditorWindow::onTerrainSelected(int id) {
    tool_ = Tool::PaintTerrain;
    startBtn_->setChecked(false);
    goalBtn_->setChecked(false);

    switch (id) {
        case 0: selected_ = PointType::Grass; break;
        case 1: selected_ = PointType::Sand; break;
        case 2: selected_ = PointType::Water; break;
        case 3: selected_ = PointType::DeepWater; break;
        case 4: selected_ = PointType::Lava; break; // we show it as "Лес"
        default: selected_ = PointType::Grass; break;
    }
}

void EditorWindow::onSetStartMode() {
    tool_ = Tool::SetStart;
    goalBtn_->setChecked(false);
}

void EditorWindow::onSetGoalMode() {
    tool_ = Tool::SetGoal;
    startBtn_->setChecked(false);
}

void EditorWindow::onCellClicked(Point p) {
    if (!map_.in_bound(p)) return;

    if (tool_ == Tool::SetStart) {
        if (!passable(map_.tile(p))) return;
        start_ = p;
        mapWidget_->setStart(start_);
        startBtn_->setChecked(false);
        tool_ = Tool::PaintTerrain;
        recomputePath();
        return;
    }

    if (tool_ == Tool::SetGoal) {
        if (!passable(map_.tile(p))) return;
        goal_ = p;
        mapWidget_->setGoal(goal_);
        goalBtn_->setChecked(false);
        tool_ = Tool::PaintTerrain;
        recomputePath();
        return;
    }

    // Paint terrain
    // Do not allow making start/goal cell impassable
    if ((start_ && *start_ == p) || (goal_ && *goal_ == p)) {
        if (!passable(selected_)) {
            return;
        }
    }

    map_.setTile(p, selected_);
    recomputePath();
}

void EditorWindow::recomputePath() {
    if (!start_ || !goal_) {
        path_.reset();
        mapWidget_->setPath(std::nullopt);
        updateDistanceLabelNotComputed();
        return;
    }
    
    if (!passable(map_.tile(*start_)) || !passable(map_.tile(*goal_))) {
        path_.reset();
        mapWidget_->setPath(std::nullopt);
        updateDistanceLabelNotComputed();
        return;
    }

    auto res = astar_.find_path(map_, *start_, *goal_);
    if (!res) {
        path_.reset();
        mapWidget_->setPath(std::nullopt);
        updateDistanceLabelUnreachable();
        return;
    }

    path_ = std::move(res);
    mapWidget_->setPath(path_);
    updateDistanceLabelValue(path_->get_final_cost());
}

void EditorWindow::updateDistanceLabelNotComputed() {
    distanceLabel_->setText("Расстояние: <span style='color:red'>не посчитано</span>");
}

void EditorWindow::updateDistanceLabelUnreachable() {
    distanceLabel_->setText("Расстояние: <span style='color:red'>недостижимо</span>");
}

void EditorWindow::updateDistanceLabelValue(int cost) {
    distanceLabel_->setText(QString("Расстояние: %1").arg(cost));
}
