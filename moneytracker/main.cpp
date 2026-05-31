#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Money Tracker");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("MyApp");

    MainWindow window;
    window.show();

    return app.exec();
}
