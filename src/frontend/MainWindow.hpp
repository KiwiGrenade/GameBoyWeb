#pragma once

#include <QMainWindow>
#include <memory>

#include "GameBoy.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionLoad_triggered();
    void on_pushButton_released();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<GameBoy> gameBoy_;
};
