#include "MapSelectWindow.h"

#include <QPushButton>
#include <QVBoxLayout>

#include "EditorWindow.h"
#include "MapFactory.h"

namespace {
QString buttonStyle() {
    return R"(
        QPushButton {
            background: white;
            border: 1px solid #dcdcdc;
            border-radius: 12px;
            padding: 14px;
            font-size: 18px;
            font-weight: 700;
        }
        QPushButton:hover { background: #f6f6f6; }
        QPushButton:pressed { background: #efefef; }
    )";
}
}

MapSelectWindow::MapSelectWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Выбор карты");
    setMinimumSize(360, 220);

    map1Btn_ = new QPushButton("Карта 1", this);
    map2Btn_ = new QPushButton("Карта 2", this);

    map1Btn_->setStyleSheet(buttonStyle());
    map2Btn_->setStyleSheet(buttonStyle());

    map1Btn_->setMinimumHeight(64);
    map2Btn_->setMinimumHeight(64);

    auto* lay = new QVBoxLayout(this);
    lay->setContentsMargins(24, 24, 24, 24);
    lay->setSpacing(16);
    lay->addStretch(1);
    lay->addWidget(map1Btn_);
    lay->addWidget(map2Btn_);
    lay->addStretch(1);

    connect(map1Btn_, &QPushButton::clicked, this, &MapSelectWindow::openMap1);
    connect(map2Btn_, &QPushButton::clicked, this, &MapSelectWindow::openMap2);
}

void MapSelectWindow::openMap1() {
    auto* w = new EditorWindow(make_map_1());
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
    close();
}

void MapSelectWindow::openMap2() {
    auto* w = new EditorWindow(make_map_2());
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
    close();
}
