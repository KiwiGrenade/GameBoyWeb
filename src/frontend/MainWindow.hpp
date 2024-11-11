#pragma once

#include <QMainWindow>
#include <memory>

#include "GameBoy.hpp"
#include "Cartridge.hpp"
#include "QtRenderer.hpp"
/*#include "Disassembler.hpp"*/

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
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionPause_triggered();

private:
    void closeEvent(QCloseEvent* event);
    Ui::MainWindow *ui;
    std::shared_ptr<GameBoy> gameBoy_;
    std::shared_ptr<Cartridge> cartridge_;
    std::shared_ptr<QtRenderer> renderer_;
    /*std::shared_ptr<Disassembler> disassembler_;*/
};
