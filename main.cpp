#include <QApplication>
#include "MapSelectWindow.h"

static void setupQtPluginPath()
{
    QCoreApplication::addLibraryPath("D:\\Qt\\6.10.1\\mingw_64\\plugins");
}

int main(int argc, char** argv) {
    setupQtPluginPath();
    QApplication app(argc, argv);
    QFont f = app.font();
    f.setPointSize(10);
    app.setFont(f);

    MapSelectWindow w;
    w.show();

    return app.exec();
}
