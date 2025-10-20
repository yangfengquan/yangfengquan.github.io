#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->comboBox->addItems({"无缝黄铜、铜及铅管","操作中基本无腐蚀的无缝钢管"});
    Dialog d(this);// = new Dialog(this);
    d.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
