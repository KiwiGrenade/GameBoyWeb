#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <cstdint>
#include <memory>

#include <QFileDialog>
#include <qtpreprocessorsupport.h>

#include "utils.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gameBoy_(std::make_shared<GameBoy>())
    /*, disassembler_(std::make_shared<Disassembler>())*/
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_triggered() {
    auto fileContentReady = [this](const QString& fileName, const QByteArray& fileContent) {
        if(fileName.isEmpty())
        {
            Utils::error("No file was selected! Exiting!");
        }
        cartridge_ = std::make_shared<Cartridge>(fileContent);
        gameBoy_->reset();
        gameBoy_->loadCartridge(cartridge_);
    };

    QFileDialog::getOpenFileContent(" ROMs (*.gb)", fileContentReady);
}

void MainWindow::on_pushButton_released(){
    ui->plainTextEdit->appendPlainText(ui->lineEdit->text());
}

void MainWindow::on_actionStart_triggered() {
    gameBoy_->start();
}

void MainWindow::on_actionStop_triggered() {
    gameBoy_->stop();
}

void MainWindow::on_actionPause_triggered() {
    gameBoy_->pause();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    Q_UNUSED(event);
    gameBoy_->stop();
    gameBoy_->wait();
}

