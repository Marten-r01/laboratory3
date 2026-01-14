#include "MapWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>

#include "Terrain.h"

namespace {
QColor baseColor(PointType t) {
    switch (t) {
        case PointType::Grass:     return QColor("#4fe36c");
        case PointType::Sand:      return QColor("#f7d074");
        case PointType::Water:     return QColor("#bfe9ff");
        case PointType::DeepWater: return QColor("#2f5cff");
        case PointType::Lava:      return QColor("#2e7d32");
        default:                   return QColor("#aaaaaa");
    }
}

void drawWaves(QPainter& p, const QRect& r, const QColor& c) {
    QPen pen(c.darker(140), 2);
    pen.setCapStyle(Qt::RoundCap);
    p.setPen(pen);

    const int pad = r.width() / 6;
    const int left = r.left() + pad;
    const int right = r.right() - pad;

    auto waveLine = [&](int y) {
        QPainterPath path;
        const int mid = (left + right) / 2;
        path.moveTo(left, y);
        path.quadTo(mid, y - r.height()/8, right, y);
        p.drawPath(path);
    };

    const int y1 = r.top() + r.height() * 3 / 8;
    const int y2 = r.top() + r.height() * 5 / 8;
    waveLine(y1);
    waveLine(y2);
}

void drawDunes(QPainter& p, const QRect& r) {
    QPen pen(QColor("#c9a24a"), 2);
    p.save();
    pen.setCapStyle(Qt::RoundCap);
    p.setPen(pen);

    auto arc = [&](int y) {
        QPainterPath path;
        const int left = r.left() + r.width()/6;
        const int right = r.right() - r.width()/6;
        const int mid = (left + right) / 2;
        path.moveTo(left, y);
        path.quadTo(mid, y - r.height()/6, right, y);
        p.drawPath(path);
        p.restore();
    };

    arc(r.top() + r.height()*4/10);
    arc(r.top() + r.height()*6/10);
}

void drawTree(QPainter& p, const QRect& r) {
    // Simple stylized tree
    p.save();
    p.setPen(Qt::NoPen);

    QRect trunk(r.left() + r.width()*45/100, r.top() + r.height()*55/100,
                r.width()*10/100, r.height()*30/100);
    p.setBrush(QColor("#7b4a1f"));
    p.drawRect(trunk);

    p.setBrush(QColor("#1b5e20"));
    QPolygon canopy;
    canopy << QPoint(r.center().x(), r.top() + r.height()*18/100)
           << QPoint(r.left() + r.width()*25/100, r.top() + r.height()*55/100)
           << QPoint(r.right() - r.width()*25/100, r.top() + r.height()*55/100);
    p.drawPolygon(canopy);

    QPolygon canopy2;
    canopy2 << QPoint(r.center().x(), r.top() + r.height()*32/100)
            << QPoint(r.left() + r.width()*18/100, r.top() + r.height()*65/100)
            << QPoint(r.right() - r.width()*18/100, r.top() + r.height()*65/100);
    p.drawPolygon(canopy2);
    p.restore();
}

void drawFlag(QPainter& p, const QRect& cellRect, const QColor& flagColor) {
    p.save();
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#333333"));
    // pole
    QRect pole(cellRect.left() + cellRect.width()*40/100,
               cellRect.top() + cellRect.height()*20/100,
               cellRect.width()*6/100,
               cellRect.height()*60/100);
    p.drawRect(pole);

    p.setBrush(flagColor);
    QPolygon tri;
    tri << QPoint(pole.right(), pole.top())
        << QPoint(cellRect.left() + cellRect.width()*80/100, pole.top() + cellRect.height()*12/100)
        << QPoint(pole.right(), pole.top() + cellRect.height()*24/100);
    p.drawPolygon(tri);
    p.restore();
}
}

MapWidget::MapWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(700, 700);
    setMouseTracking(true);
}

void MapWidget::setMap(const Map* map) {
    map_ = map;
    update();
}

void MapWidget::setStart(std::optional<Point> s) {
    start_ = s;
    update();
}

void MapWidget::setGoal(std::optional<Point> g) {
    goal_ = g;
    update();
}

void MapWidget::setPath(std::optional<Path> p) {
    path_ = std::move(p);
    update();
}

QRect MapWidget::gridRect() const {
    if (!map_) return {};
    const int W = map_->Width();
    const int H = map_->Height();

    const int cell = std::min(width() / W, height() / H);
    const int gw = cell * W;
    const int gh = cell * H;
    const int x0 = (width() - gw) / 2;
    const int y0 = (height() - gh) / 2;
    return QRect(x0, y0, gw, gh);
}

std::optional<Point> MapWidget::cellFromPos(const QPoint& pos) const {
    if (!map_) return std::nullopt;

    const int W = map_->Width();
    const int H = map_->Height();

    const QRect r = gridRect();
    if (!r.contains(pos)) return std::nullopt;

    const int cell = r.width() / W;
    const int x = (pos.x() - r.x()) / cell;
    const int y = (pos.y() - r.y()) / cell;

    if (x < 0 || x >= W || y < 0 || y >= H) return std::nullopt;
    return Point{x, y};
}

void MapWidget::mousePressEvent(QMouseEvent* ev) {
    if (ev->button() != Qt::LeftButton) return;
    if (const auto p = cellFromPos(ev->pos())) {
        emit cellClicked(*p);
    }
}

void MapWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    p.fillRect(rect(), QColor("#e9e9e9"));

    if (!map_) return;

    const int W = map_->Width();
    const int H = map_->Height();

    const QRect r = gridRect();
    const int cell = r.width() / W;

    // Precompute for fast membership check
    const auto* pathVec = path_ ? &path_->get_path() : nullptr;

    auto onPath = [&](const Point& pt) -> bool {
        if (!pathVec) return false;
        return std::find(pathVec->begin(), pathVec->end(), pt) != pathVec->end();
    };

    QPen gridPen(QColor("#0a2a6b"));
    gridPen.setWidth(1);

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            const Point pt{x, y};
            const QRect cr(r.left() + x * cell, r.top() + y * cell, cell, cell);

            const PointType t = map_->tile(pt);
            p.fillRect(cr, baseColor(t));

            // Patterns
            if (t == PointType::Water || t == PointType::DeepWater) {
                drawWaves(p, cr, baseColor(t));
            } else if (t == PointType::Sand) {
                drawDunes(p, cr);
            } else if (t == PointType::Lava) {
                drawTree(p, cr);
            }

            // Path overlay
            if (onPath(pt)) {
               p.fillRect(cr, QColor(255, 255, 255, 70));
            }

            p.setPen(gridPen);
            p.drawRect(cr);

            // Flags must be above overlay
            if (start_ == pt ) {
                drawFlag(p, cr, QColor("#13a113"));
            }
            if (goal_ == pt) {
                drawFlag(p, cr, QColor("#d12020"));
            }
        }
    }
}
