#include <iostream>

#include <QApplication>
#include <memory>

#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Hello World! ~ GameBoyWeb by Jakub Jaśków" << std::endl;
    QApplication app(argc, argv);
    auto window = std::make_unique<MainWindow>();
    window->show();
    return app.exec();
}
