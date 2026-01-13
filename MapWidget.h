#pragma once

#include <QWidget>
#include <optional>

#include "Point.h"
#include "GraphMap.h"
#include "Path.h"

class MapWidget final : public QWidget {
    Q_OBJECT
public:
    explicit MapWidget(QWidget* parent = nullptr);

    void setMap(const Map* map);
    void setStart(std::optional<Point> s);
    void setGoal(std::optional<Point> g);
    void setPath(std::optional<Path> p);

signals:
    void cellClicked(Point p);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* ev) override;

private:
    const Map* map_ = nullptr;
    std::optional<Point> start_;
    std::optional<Point> goal_;
    std::optional<Path> path_;

    QRect gridRect() const;
    std::optional<Point> cellFromPos(const QPoint& pos) const;
};
