#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QKeyEvent>
#include "GameBoy.hpp"
#include "QtRenderer.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr,
                        const QString &title = "GameBoyWeb");

    private slots:
    // Update the display to show the most recent frame in the renderer.
    // This is called by the PPU on VBLANK.
    void updateDisplay();
    void on_actionLoad_triggered();
    void on_actionReload_triggered();
    void on_actionStartEmulator_triggered();
    void on_actionStopEmulator_triggered();
    void on_actionStepEmulator_triggered();
    void on_actionPauseEmulator_triggered();

    // Open file dialog to choose a ROM file
    void openRom();

    private:
    // load the cartridge at fileName onto the Gameboy.

    void keyReleaseEvent(QKeyEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void closeEvent(QCloseEvent* event);
    void loadRom(const QString &fileName);

    std::shared_ptr<GameBoy> gameBoy_;

    // Produces graphical output for the display.
    QtRenderer *renderer_ {nullptr};

    // Base title of the MainWindow (i.e. not including FPS counter)
    QString title_;

    // Title of the current ROM loaded
    QString curRom;

    // Control mapping
    Controls controls_;

};

#endif // MAINWINDOW_H
