#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "utils.hpp"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString str = "kurwa";
    ui->plainTextEdit->appendPlainText(str);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_triggered() {
    auto fileContentReady = [this](const QString& fileName, const QByteArray& fileContent) {
        if(fileName.isEmpty())
        {
            error("No file was selected! Exiting!");
        }
    };

    QFileDialog::getOpenFileContent(" ROMs (*.ch8)", fileContentReady);
}


void MainWindow::on_pushButton_released(){
    ui->plainTextEdit->appendPlainText(ui->lineEdit->text());
}


