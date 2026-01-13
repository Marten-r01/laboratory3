#pragma once

#include <QWidget>

class QPushButton;

class MapSelectWindow final : public QWidget {
    Q_OBJECT
public:
    explicit MapSelectWindow(QWidget* parent = nullptr);

private slots:
    void openMap1();
    void openMap2();

private:
    QPushButton* map1Btn_ = nullptr;
    QPushButton* map2Btn_ = nullptr;
};
