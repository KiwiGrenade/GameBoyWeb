#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GameBoy.hpp"
#include "QtRenderer.hpp"

#include <QMainWindow>

class QAction;
class QMenu;
class QImage;
class QLabel;
class QTimer;

struct Preferences {
    bool antialiasing;
};

struct Controls {
    Qt::Key a{Qt::Key_D},
            b{Qt::Key_S},
            up{Qt::Key_Up},
            down{Qt::Key_Down},
            left{Qt::Key_Left},
            right{Qt::Key_Right},
            select{Qt::Key_Shift},
            start{Qt::Key_Return},
            turbo{Qt::Key_Space};
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr,
                        const QString &title = "Gameboy");

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private
    slots:
    // Update the display to show the most recent frame in the renderer.
    // This is called by the PPU on VBLANK.
    void updateDisplay();

    // Open file dialog to choose a ROM file
    void openRom();

    void pause();

    // options menu slots
    void toggleAntiAlias(bool);

private:
    // load the cartridge at fileName onto the Gameboy.
    void loadRom(const QString &fileName);

    // Create a new menu in the toolbar.
    QMenu *createMenu(const QString &name);

    // Add a button to a menu that calls slot when clicked.
    QAction *createSingleAction(const QString &name,
                                QMenu *menu,
                                void (MainWindow::*slot)());

    // Add a toggle-able button to a menu that toggles *slot when clicked.
    QAction *createCheckableAction(const QString &name,
                                   QMenu *menu,
                                   void (MainWindow::*slot)(bool),
                                   bool initallyChecked = false);

    // Create all the actions performed by the buttons in the toolbar.
    void createActions();

    std::shared_ptr <GameBoy> gameBoy_;

    // Display for the Gameboy. Displays the contents output by the renderer.
    QLabel *display_;

    // Produces graphical output for the display.
    QtRenderer *renderer_{nullptr};

    // Base title of the MainWindow (i.e. not including FPS counter)
    QString title_;

    // Saved preferences for options
    Preferences prefs_;

    // Control mapping
    Controls controls_;
};

#endif // MAINWINDOW_H
