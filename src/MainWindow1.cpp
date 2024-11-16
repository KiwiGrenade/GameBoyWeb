#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <chrono>
#include <memory>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow {parent}
    , ui(new Ui::MainWindow)
    , gameBoy_ {std::make_shared<GameBoy>()},
      display_ {new QLabel(this)},
      renderer_ {new QtRenderer(160, 144, this)}
{
    ui->setupUi(this);

    gameBoy_->setRenderer(renderer_);

    connect(renderer_, SIGNAL(showScreen()), this, SLOT(updateDisplay()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_triggered() {
    auto fileContentReady = [this](const QString& fileName, const QByteArray& fileContent) {
        if(fileName.isEmpty())
        {
            //TODO: Replace this with error() from utils.hpp
            std::cout << "No file was selected! Exiting!" << std::endl;
            exit(1);
        }
        gameBoy_->reset();
        gameBoy_->loadCartridge(std::make_shared<Cartridge>(fileContent));
    };

    QFileDialog::getOpenFileContent(" ROMs (*.ch8)", fileContentReady);
}

void MainWindow::on_actionReload_triggered() {

}

void MainWindow::on_actionStartEmulator_triggered() {
    gameBoy_->start();
}

void MainWindow::on_actionStopEmulator_triggered() {
    gameBoy_->stop();
}

void MainWindow::on_actionStepEmulator_triggered() {

}

void MainWindow::on_actionPauseEmulator_triggered() {

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
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

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
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

void MainWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event)

    gameBoy_->stop();
    gameBoy_->wait();
}

void MainWindow::updateDisplay()
{
    QImage img {renderer_->getImage()};
    display_->setPixmap(QPixmap::fromImage(img).scaled(
                           centralWidget()->width(),
                           centralWidget()->height(),
                           Qt::IgnoreAspectRatio,
                           Qt::SmoothTransformation));
}
