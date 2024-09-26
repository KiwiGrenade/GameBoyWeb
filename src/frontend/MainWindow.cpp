#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <cstdint>
#include <memory>

#include <QFileDialog>

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
        gameBoy_->loadROM(fileContent);
    };

    QFileDialog::getOpenFileContent(" ROMs (*.gb)", fileContentReady);
}


void MainWindow::on_pushButton_released(){
    ui->plainTextEdit->appendPlainText(ui->lineEdit->text());
}


