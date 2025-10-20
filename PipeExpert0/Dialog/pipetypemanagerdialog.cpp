#include "pipetypemanagerdialog.h"
#include "ui_pipetypemanagerdialog.h"
#include "MaterialManager.h"

PipeTypeManagerDialog::PipeTypeManagerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PipeTypeManagerDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,120);
    ui->tableWidget->setColumnWidth(2,180);

    MaterialManager m;
    QMap<QString, PipeType> pipeTypes = m.getPipeTypes();
    int row = 0;
    for (auto it = pipeTypes.begin(); it != pipeTypes.end(); ++it) {
        const PipeType &pipeType = it.value();
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(pipeType.name));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(pipeType.roughness, 'f', 6)));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(pipeType.description));

        row++;
    }

    connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::close);
}

PipeTypeManagerDialog::~PipeTypeManagerDialog()
{
    delete ui;
}

