#pragma once

#include <QMainWindow>
#include <memory>
#include <qevent.h>

#include "GameBoy.hpp"
#include "Cartridge.hpp"
#include "QtRenderer.hpp"

class QAction;
class QMenu;
class QImage;
class QLabel;
class QTimer;

struct Controls
{
    Qt::Key a {Qt::Key_D},
            b {Qt::Key_S},
            up {Qt::Key_Up},
            down {Qt::Key_Down},
            left {Qt::Key_Left},
            right {Qt::Key_Right},
            select {Qt::Key_Shift},
            start {Qt::Key_Return};
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private slots:
    void updateDisplay();
    void openRom();

private:
    void loadRom(const QString& fileName);
    void closeEvent(QCloseEvent* event);
    Ui::MainWindow *ui;
    std::shared_ptr<GameBoy> gameBoy_;
    std::shared_ptr<Cartridge> cartridge_;
    std::shared_ptr<QtRenderer> renderer_;
    /*std::shared_ptr<Disassembler> disassembler_;*/
};
