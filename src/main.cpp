#include <QApplication>
#include <memory>

#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    auto window = std::make_unique<MainWindow>();
    window->show();
    return app.exec();
}
