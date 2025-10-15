#include "mainwindow.h"
#include "MaterialManager.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    MaterialManager m;
}

MainWindow::~MainWindow() {}
