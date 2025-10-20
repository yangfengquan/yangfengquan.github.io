#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MaterialManager.h"
#include "Dialog/pipetypemanagerdialog.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMaterialCombo();
    //connect(ui->saveAction, &QAction::triggered, this, &MainWindow::save);;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QMessageBox::warning(nullptr,"save","save");
}

void MainWindow::initMaterialCombo()
{
    MaterialManager m;
    ui->PipeTypeComboBox->clear();
    ui->PipeTypeComboBox->addItems(m.getPipeTypes().keys());
    ui->insultNameComboBox->clear();
    ui->insultNameComboBox->addItems(m.getInsulationMaterials().keys());
    ui->cladNameComboBox->clear();
    ui->cladNameComboBox->addItems(m.getProtectionMaterials().keys());
}

void MainWindow::on_actionPipe_triggered()
{
    PipeTypeManagerDialog dlg(this);
    dlg.exec();
}

