#include <QtWidgets>
#include "MainWindow.hpp"

#include <chrono>
#include <memory>
#include <sstream>

MainWindow::MainWindow(QWidget *parent,
                       const QString &title)
        : QMainWindow{parent},
          gameBoy_{std::make_shared<GameBoy>()},
          display_{new QLabel(this)},
          renderer_{new QtRenderer(160, 144, this)},
          fpsTimer_{new QTimer(this)},
          title_{title} {
    setCentralWidget(display_);
    setWindowTitle(title);

    display_->setScaledContents(true);
    display_->setMinimumSize(160, 144);

    gameBoy_->setRenderer(renderer_);

    fpsTimer_->setInterval(1000);
    fpsTimer_->callOnTimeout(this, &MainWindow::updateFps);
    fpsTimer_->start();

    connect(renderer_, SIGNAL(showScreen()), this, SLOT(updateDisplay()));
    createActions();
}


void MainWindow::loadRom(const QString &fileName) {
    auto fileContentReady = [this](const QString &fileName, const QByteArray &fileContent) {
        if (fileName.isEmpty()) {
            std::cout << "No file was selected! Exiting!" << std::endl;
            exit(1);
        }
        gameBoy_->reset();
        std::shared_ptr <Cartridge> car = std::make_shared<Cartridge>(fileContent);
        gameBoy_->loadCartridge(car);
        gameBoy_->runConcurrently();
    };

    QFileDialog::getOpenFileContent(" ROMs (*.ch8)", fileContentReady);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    auto key = event->key();
    if (key == controls_.a)
        gameBoy_->press(Joypad::Button::A);
    else if (key == controls_.b)
        gameBoy_->press(Joypad::Button::B);
    else if (key == controls_.up)
        gameBoy_->press(Joypad::Button::Up);
    else if (key == controls_.down)
        gameBoy_->press(Joypad::Button::Down);
    else if (key == controls_.left)
        gameBoy_->press(Joypad::Button::Left);
    else if (key == controls_.right)
        gameBoy_->press(Joypad::Button::Right);
    else if (key == controls_.select)
        gameBoy_->press(Joypad::Button::Select);
    else if (key == controls_.start)
        gameBoy_->press(Joypad::Button::Start);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    auto key = event->key();
    if (key == controls_.a)
        gameBoy_->release(Joypad::Button::A);
    else if (key == controls_.b)
        gameBoy_->release(Joypad::Button::B);
    else if (key == controls_.up)
        gameBoy_->release(Joypad::Button::Up);
    else if (key == controls_.down)
        gameBoy_->release(Joypad::Button::Down);
    else if (key == controls_.left)
        gameBoy_->release(Joypad::Button::Left);
    else if (key == controls_.right)
        gameBoy_->release(Joypad::Button::Right);
    else if (key == controls_.select)
        gameBoy_->release(Joypad::Button::Select);
    else if (key == controls_.start)
        gameBoy_->release(Joypad::Button::Start);
}

void MainWindow::updateDisplay() {
    QImage img{renderer_->getImage()};
    Qt::TransformationMode transformation_mode = prefs_.antialiasing
                                                 ? Qt::SmoothTransformation
                                                 : Qt::FastTransformation;
    display_->setPixmap(QPixmap::fromImage(img).scaled(
            centralWidget()->width(),
            centralWidget()->height(),
            Qt::IgnoreAspectRatio,
            transformation_mode));
    ++frames_;
}

void MainWindow::updateFps() {
    QString newTitle{title_ + " - FPS: " + QString::number(frames_)};
    setWindowTitle(newTitle);
    frames_ = 0;
}

void MainWindow::openRom() {
    auto fileContentReady = [this](const QString &fileName, const QByteArray &fileContent) {
        if (fileName.isEmpty()) {
            std::cout << "No file was selected! Exiting!" << std::endl;
            exit(1);
        }
        gameBoy_->reset();
        std::shared_ptr <Cartridge> car = std::make_shared<Cartridge>(fileContent);
        gameBoy_->loadCartridge(car);
        gameBoy_->runConcurrently();
    };

    QFileDialog::getOpenFileContent(" ROMs (*.gb)", fileContentReady);
}

void MainWindow::showDebugger() {
    /*
    if (!debuggerWindow_)
        debuggerWindow_ = new DebuggerWindow(gameBoy_, this);
    debuggerWindow_->show();
    */
}

void MainWindow::showVramViewer() {
    /*
     auto *vram_viewer = new Vram_window(gameBoy_);
     vram_viewer->show();
     */
}


void MainWindow::about() {
    QMessageBox::about(this, tr("About QtBoy"),
                       tr("A Gameboy emulator made using Qt."));
}

void MainWindow::toggleAntiAlias(bool b) {
    prefs_.antialiasing = b;
}

/*void MainWindow::toggleForceDmg(bool b)*/
/*{*/
/*    gameBoy_->set_force_dmg(b);*/
/*}*/
/**/
/**/
/*void MainWindow::toggleSound(bool b)*/
/*{*/
/*    gameBoy_->toggle_sound(b);*/
/*}*/

QMenu *MainWindow::createMenu(const QString &name) {
    return menuBar()->addMenu(name);
}

QAction *MainWindow::createSingleAction(const QString &name,
                                        QMenu *menu,
                                        void (MainWindow::*slot)()) {
    if (!menu)
        throw std::runtime_error{"Cannot add single action to menu pointing to null"};
    return menu->addAction(name, this, slot);
}

QAction *MainWindow::createCheckableAction(const QString &name,
                                           QMenu *menu,
                                           void (MainWindow::*slot)(bool),
                                           bool initiallyChecked) {
    if (!menu)
        throw std::runtime_error{"Cannot add checkable action to menu pointing to null"};
    QAction *action = new QAction(name, this);
    action->setCheckable(true);
    action->setChecked(initiallyChecked);
    connect(action, &QAction::toggled, this, slot);
    menu->addAction(action);
    return action;
}

void MainWindow::createActions() {
    // File menu
    QMenu *fileMenu = createMenu(tr("&File"));
    QAction *openAct = createSingleAction(tr("&Open"), fileMenu, &MainWindow::openRom);
    openAct->setShortcuts(QKeySequence::Open);

    // Options menu
    QMenu *optionsMenu = createMenu(tr("&Options"));
    createCheckableAction(tr("Anti-aliasing"),
                          optionsMenu,
                          &MainWindow::toggleAntiAlias);

    /*
    // Tools menu
    QMenu *toolsMenu = createMenu(tr("&Tools"));
    createSingleAction(tr("Debugger"), toolsMenu, &MainWindow::showDebugger);
    createSingleAction(tr("VRAM Viewer"), toolsMenu, &MainWindow::showVramViewer);
    */

    // Help menu
    QMenu *helpMenu = createMenu(tr("&Help"));
    createSingleAction(tr("&About"), helpMenu, &MainWindow::about);
}
