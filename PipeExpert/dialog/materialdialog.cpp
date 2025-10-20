#include "materialdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QMap>
#include <QMessageBox>
#include "MaterialManager.h"
#include <QDebug>

MaterialDialog::MaterialDialog(const QString& type, QWidget *parent)
    : QDialog(parent)
    , materialType(type)
{


    refreshTable();
}

MaterialDialog::~MaterialDialog()
{

}

void MaterialDialog::setupUi()
{
    setWindowTitle(getTitle() + "管理");
    setMinimumSize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);

    materialTable = new QTableWidget(this);
    materialTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    materialTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    materialTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // 设置列
    QStringList columns = getColumns();
    materialTable->setColumnCount(columns.size());
    materialTable->setHorizontalHeaderLabels(columns);
    layout->addWidget(materialTable);

    QHBoxLayout *btnlayout = new QHBoxLayout();

    addButton = new QPushButton("添加", this);
    btnlayout->addWidget(addButton);

    editButton = new QPushButton("编辑", this);
    btnlayout->addWidget(editButton);

    removeButton = new QPushButton("删除", this);
    btnlayout->addWidget(removeButton);

    btnlayout->addStretch();

    closeButton = new QPushButton("关闭", this);
    btnlayout->addWidget(closeButton);

    layout->addLayout(btnlayout);

    // 连接信号槽
    connect(addButton, &QPushButton::clicked, this, &MaterialDialog::addMaterial);
    connect(editButton, &QPushButton::clicked, this, &MaterialDialog::editMaterial);
    connect(deleteButton, &QPushButton::clicked, this, &MaterialDialog::deleteMaterial);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}


QString MaterialDialog::getTitle() const
{
    if (materialType == "insulation") {
        return "保温材料";
    } else if (materialType == "protection") {
        return "外保护层";
    } else if (materialType == "fittings") {
        return "管道元件";
    } else if (materialType == "pipe_types") {
        return "管道类型";
    } else {
        return "材料";
    }
}

QStringList MaterialDialog::getColumns() const
{
    if (materialType == "insulation") {
        //return {"名称", "导热系数方程一", "导热系数方程二", "导热系数方程三", "密度(kg/m³)", "备注"};
        return {"名称","密度(kg/m³)", "备注"};
    } else if (materialType == "protection") {
        return {"名称", "黑度", "备注"};
    } else if (materialType == "fitting") {
        return {"名称", "阻力系数", "备注"};
    } else if (materialType == "pipeType") {
        return {"名称", "粗糙度(m)", "备注"};
    } else {
        return {"名称", "备注"};
    }
}

void MaterialDialog::refreshTable()
{
    materialTable->setRowCount(0);

    QMap<QString, InsulationMaterial> insulationMaterials;
    QMap<QString, OuterProtection> protectionMaterials;
    QMap<QString, PipeFitting> pipeFittings;
    QMap<QString, PipeType> pipeTypes;

    if (materialType == "insulation") {
        insulationMaterials = materialManager->getInsulationMaterials();
    } else if (materialType == "protection") {
        protectionMaterials = materialManager->getProtectionMaterials();
    } else if (materialType == "fitting") {
        pipeFittings = materialManager->getPipeFittings();
    } else if (materialType == "pipeType") {
        pipeTypes = materialManager->getPipeTypes();
    }

    if (materialType == "insulation") {
        int row = 0;
        for (auto it = insulationMaterials.begin(); it != insulationMaterials.end(); ++it) {
            const InsulationMaterial &material = it.value();
            materialTable->insertRow(row);

            materialTable->setItem(row, 0, new QTableWidgetItem(material.name));
            //materialTable->setItem(row, 1, new QTableWidgetItem(material.conductivityEq1));
            //materialTable->setItem(row, 2, new QTableWidgetItem(material.conductivityEq2));
            //materialTable->setItem(row, 3, new QTableWidgetItem(material.conductivityEq3));
            materialTable->setItem(row, 4, new QTableWidgetItem(QString::number(material.density)));
            materialTable->setItem(row, 5, new QTableWidgetItem(material.description));

            row++;
        }
    } else if (materialType == "protection") {
        int row = 0;
        for (auto it = protectionMaterials.begin(); it != protectionMaterials.end(); ++it) {
            const OuterProtection &material = it.value();
            materialTable->insertRow(row);

            materialTable->setItem(row, 0, new QTableWidgetItem(material.name));
            materialTable->setItem(row, 1, new QTableWidgetItem(QString::number(material.emissivity, 'f', 3)));
            materialTable->setItem(row, 2, new QTableWidgetItem(material.description));

            row++;
        }
    } else if (materialType == "fittings") {
        int row = 0;
        for (auto it = pipeFittings.begin(); it != pipeFittings.end(); ++it) {
            const PipeFitting &fitting = it.value();
            materialTable->insertRow(row);

            materialTable->setItem(row, 0, new QTableWidgetItem(fitting.name));
            materialTable->setItem(row, 1, new QTableWidgetItem(QString::number(fitting.resistanceCoef, 'f', 3)));
            materialTable->setItem(row, 2, new QTableWidgetItem(fitting.description));

            row++;
        }
    } else if (materialType == "pipe_types") {
        int row = 0;
        for (auto it = pipeTypes.begin(); it != pipeTypes.end(); ++it) {
            const PipeType &pipeType = it.value();
            materialTable->insertRow(row);

            materialTable->setItem(row, 0, new QTableWidgetItem(pipeType.name));
            materialTable->setItem(row, 1, new QTableWidgetItem(QString::number(pipeType.roughness, 'f', 6)));
            materialTable->setItem(row, 2, new QTableWidgetItem(pipeType.description));

            row++;
        }
    }

    materialTable->resizeColumnsToContents();
}

void MaterialDialog::addMaterial()
{
    MaterialEditDialog dialog(this, materialManager, materialType);
    if (dialog.exec() == QDialog::Accepted) {
        refreshTable();
    }
}

void MaterialDialog::editMaterial()
{
    QList<QTableWidgetItem*> selectedItems = materialTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要编辑的材料");
        return;
    }

    int row = selectedItems.first()->row();
    QString materialName = materialTable->item(row, 0)->text();

    MaterialEditDialog dialog(this, materialManager, materialType, materialName);
    if (dialog.exec() == QDialog::Accepted) {
        refreshTable();
    }
}

void MaterialDialog::deleteMaterial()
{
    QList<QTableWidgetItem*> selectedItems = materialTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要删除的材料");
        return;
    }

    int row = selectedItems.first()->row();
    QString materialName = materialTable->item(row, 0)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认",
                                  QString("确定要删除材料 '%1' 吗？").arg(materialName),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (materialType == "insulation") {
            materialManager->getInsulationMaterials().remove(materialName);
        } else if (materialType == "protection") {
            materialManager->getProtectionMaterials().remove(materialName);
        } else if (materialType == "fittings") {
            materialManager->getPipeFittings().remove(materialName);
        } else if (materialType == "pipe_types") {
            materialManager->getPipeTypes().remove(materialName);
        }

        materialManager->saveMaterialsToFile();
        refreshTable();
    }
}
