#include <QtWidgets>
#include "MainWindow.hpp"

#include <memory>

MainWindow::MainWindow(QWidget *parent,
                       const QString &title)
        : QMainWindow{parent},
          gameBoy_{std::make_unique<GameBoy>()},
          display_{new QLabel(this)},
          renderer_{new QtRenderer(160, 144, this)},
          title_{title} {
    setCentralWidget(display_);
    setWindowTitle(title_);

    display_->setScaledContents(true);
    display_->setMinimumSize(160, 144);

    gameBoy_->setRenderer(renderer_);

    connect(renderer_, SIGNAL(showScreen()), this, SLOT(updateDisplay()));
    createActions();
}


void MainWindow::loadRom() {
    auto fileContentReady = [this](const QString &fileName, const QByteArray &fileContent) {
        if (fileName.isEmpty()) {
            Utils::error("File is empty!");
        }
        gameBoy_->reset();
        std::shared_ptr <Cartridge> car = std::make_shared<Cartridge>(fileContent);
        gameBoy_->loadCartridge(car);
        gameBoy_->runConcurrently();
    };

    QFileDialog::getOpenFileContent(" ROMs (*.gb)", fileContentReady);
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
}

void MainWindow::pause() {
    gameBoy_->pause();
}

void MainWindow::toggleAntiAlias(bool b) {
    prefs_.antialiasing = b;
}

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
    QAction *openAct = createSingleAction(tr("&Open"), fileMenu, &MainWindow::loadRom);
    openAct->setShortcuts(QKeySequence::Open);

    QAction *pauseAct = createSingleAction(tr("&Pause"), fileMenu, &MainWindow::pause);
    pauseAct->setShortcut(QKeySequence::Paste);

    // Options menu
    QMenu *optionsMenu = createMenu(tr("&Options"));
    createCheckableAction(tr("Anti-aliasing"),
                          optionsMenu,
                          &MainWindow::toggleAntiAlias);
}
