#include "materialdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QMessageBox>
#include "MaterialManager.h"
#include <QDebug>

MaterialDialog::MaterialDialog(const QString& type, QWidget *parent)
    : QDialog(parent)
    , materialType(type)
{qDebug()<<materialType;
    setupUi();
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

    deleteButton = new QPushButton("删除", this);
    btnlayout->addWidget(deleteButton);

    btnlayout->addStretch();

    closeButton = new QPushButton("关闭", this);
    btnlayout->addWidget(closeButton);

    layout->addLayout(btnlayout);

    // 连接信号槽
    connect(addButton, &QPushButton::clicked, this, &MaterialDialog::addMaterial);
    connect(editButton, &QPushButton::clicked, this, &MaterialDialog::editMaterial);
    connect(deleteButton, &QPushButton::clicked, this, &MaterialDialog::deleteMaterial);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    materialManager = new MaterialManager();
}


QString MaterialDialog::getTitle() const
{
    if (materialType == "insulation") {
        return "保温材料";
    } else if (materialType == "protection") {
        return "外保护层";
    } else if (materialType == "fitting") {
        return "管道元件";
    } else if (materialType == "pipeType") {
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
            materialTable->setItem(row, 1, new QTableWidgetItem(QString::number(material.density)));
            materialTable->setItem(row, 2, new QTableWidgetItem(material.description));

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
    } else if (materialType == "fitting") {
        int row = 0;
        for (auto it = pipeFittings.begin(); it != pipeFittings.end(); ++it) {
            const PipeFitting &fitting = it.value();
            materialTable->insertRow(row);

            materialTable->setItem(row, 0, new QTableWidgetItem(fitting.name));
            materialTable->setItem(row, 1, new QTableWidgetItem(QString::number(fitting.resistanceCoef, 'f', 3)));
            materialTable->setItem(row, 2, new QTableWidgetItem(fitting.description));

            row++;
        }
    } else if (materialType == "pipeType") {
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
        } else if (materialType == "fitting") {
            materialManager->getPipeFittings().remove(materialName);
        } else if (materialType == "pipeType") {
            materialManager->getPipeTypes().remove(materialName);
        }

        materialManager->saveMaterialsToFile();
        refreshTable();
    }
}

// MaterialEditDialog 实现
MaterialEditDialog::MaterialEditDialog(QWidget *parent,
                                       MaterialManager *materialManager,
                                       const QString &materialType,
                                       const QString &materialName)
    : QDialog(parent)
    , materialManager(materialManager)
    , materialType(materialType)
    , materialName(materialName)
    , isEditMode(!materialName.isEmpty())
{
    setupUI();

    if (isEditMode) {
        loadMaterialData();
    }
}

MaterialEditDialog::~MaterialEditDialog()
{
}

void MaterialEditDialog::setupUI()
{
    QString title = isEditMode ? "编辑" : "添加";
    if (materialType == "insulation") {
        title += "保温材料";
    } else if (materialType == "protection") {
        title += "外保护层";
    } else if (materialType == "fitting") {
        title += "管道元件";
    } else if (materialType == "pipeType") {
        title += "管道类型";
    } else {
        title += "材料";
    }

    setWindowTitle(title);
    setFixedSize(500, 200);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 字段布局
    QGridLayout *fieldsLayout = new QGridLayout();

    // 名称字段
    fieldsLayout->addWidget(new QLabel("名称:"), 0, 0);
    nameEdit = new QLineEdit();
    fieldsLayout->addWidget(nameEdit, 0, 1);

    int currentRow = 1;

    // 根据材料类型创建特定字段
    if (materialType == "insulation") {
        createInsulationFields(fieldsLayout);
        currentRow = 8; // 保温材料有5行字段
    } else if (materialType == "protection") {
        createProtectionFields(fieldsLayout);
        currentRow = 2; // 外保护层有2行字段
    } else if (materialType == "fitting") {
        createFittingsFields(fieldsLayout);
        currentRow = 2; // 管道元件有2行字段
    } else if (materialType == "pipeType") {
        createPipeTypeFields(fieldsLayout);
        currentRow = 2; // 管道类型有2行字段
    }

    // 描述字段
    fieldsLayout->addWidget(new QLabel("描述:"), currentRow, 0);
    descriptionEdit = new QLineEdit();
    fieldsLayout->addWidget(descriptionEdit, currentRow, 1);

    mainLayout->addLayout(fieldsLayout);

    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    saveButton = new QPushButton("保存");
    cancelButton = new QPushButton("取消");

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    // 连接信号槽
    connect(saveButton, &QPushButton::clicked, this, &MaterialEditDialog::saveMaterial);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    // 设置列拉伸
    fieldsLayout->setColumnStretch(1, 1);
}

void MaterialEditDialog::createInsulationFields(QGridLayout *layout)
{
    int row = 1;
    layout->addWidget(new QLabel("导热系数方程一:"), row, 0);
    conductivityEq1Edit = new QLineEdit();
    layout->addWidget(conductivityEq1Edit, row, 1);

    layout->addWidget(new QLabel("tm取值范围:"), row, 2);
    range1Edit = new QLineEdit();
    layout->addWidget(range1Edit, row, 3);

    ++row;

    layout->addWidget(new QLabel("导热系数方程二:"), row, 0);
    conductivityEq2Edit = new QLineEdit();
    layout->addWidget(conductivityEq2Edit, row, 1);

    layout->addWidget(new QLabel("tm取值范围:"), row, 2);
    range2Edit = new QLineEdit();
    layout->addWidget(range2Edit, row, 3);

    ++row;

    layout->addWidget(new QLabel("导热系数方程三:"), row, 0);
    conductivityEq3Edit = new QLineEdit();
    layout->addWidget(conductivityEq3Edit, row, 1);

    layout->addWidget(new QLabel("tm取值范围:"), row, 2);
    range3Edit = new QLineEdit();
    layout->addWidget(range3Edit, row, 3);

    ++row;

    layout->addWidget(new QLabel("密度(kg/m³):"), row, 0);
    densityEdit = new QLineEdit("100");
    layout->addWidget(densityEdit, row, 1);
}

void MaterialEditDialog::createProtectionFields(QGridLayout *layout)
{
    layout->addWidget(new QLabel("黑度:"), 1, 0);
    emissivityEdit = new QLineEdit("0.3");
    layout->addWidget(emissivityEdit, 1, 1);
}

void MaterialEditDialog::createFittingsFields(QGridLayout *layout)
{
    layout->addWidget(new QLabel("阻力系数:"), 1, 0);
    resistanceEdit = new QLineEdit("0.3");
    layout->addWidget(resistanceEdit, 1, 1);
}

void MaterialEditDialog::createPipeTypeFields(QGridLayout *layout)
{
    layout->addWidget(new QLabel("粗糙度(m):"), 1, 0);
    roughnessEdit = new QLineEdit("0.0002");
    layout->addWidget(roughnessEdit, 1, 1);
}

void MaterialEditDialog::loadMaterialData()
{
    if (materialType == "insulation") {
        auto materials = materialManager->getInsulationMaterials();
        if (materials.contains(materialName)) {
            InsulationMaterial material = materials[materialName];
            nameEdit->setText(material.name);
            conductivityEq1Edit->setText(material.conductivityEq1);
            conductivityEq2Edit->setText(material.conductivityEq2);
            conductivityEq3Edit->setText(material.conductivityEq3);
            densityEdit->setText(QString::number(material.density));
            descriptionEdit->setText(material.description);
        }
    } else if (materialType == "protection") {
        auto materials = materialManager->getProtectionMaterials();
        if (materials.contains(materialName)) {
            OuterProtection material = materials[materialName];
            nameEdit->setText(material.name);
            emissivityEdit->setText(QString::number(material.emissivity));
            descriptionEdit->setText(material.description);
        }
    } else if (materialType == "fitting") {
        auto fittings = materialManager->getPipeFittings();
        if (fittings.contains(materialName)) {
            PipeFitting fitting = fittings[materialName];
            nameEdit->setText(fitting.name);
            resistanceEdit->setText(QString::number(fitting.resistanceCoef));
            descriptionEdit->setText(fitting.description);
        }
    } else if (materialType == "pipeType") {
        auto pipeTypes = materialManager->getPipeTypes();
        if (pipeTypes.contains(materialName)) {
            PipeType pipeType = pipeTypes[materialName];
            nameEdit->setText(pipeType.name);
            roughnessEdit->setText(QString::number(pipeType.roughness));
            descriptionEdit->setText(pipeType.description);
        }
    }
}

bool MaterialEditDialog::validateInput()
{
    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入材料名称");
        return false;
    }

    // 检查数值字段
    if (materialType == "insulation") {
        bool ok;
        double density = densityEdit->text().toDouble(&ok);
        if (!ok || density <= 0) {
            QMessageBox::warning(this, "警告", "请输入有效的密度值");
            return false;
        }
    } else if (materialType == "protection") {
        bool ok;
        double emissivity = emissivityEdit->text().toDouble(&ok);
        if (!ok || emissivity < 0 || emissivity > 1) {
            QMessageBox::warning(this, "警告", "请输入有效的黑度值（0-1）");
            return false;
        }
    } else if (materialType == "fitting") {
        bool ok;
        double resistance = resistanceEdit->text().toDouble(&ok);
        if (!ok || resistance < 0) {
            QMessageBox::warning(this, "警告", "请输入有效的阻力系数");
            return false;
        }
    } else if (materialType == "pipeType") {
        bool ok;
        double roughness = roughnessEdit->text().toDouble(&ok);
        if (!ok || roughness <= 0) {
            QMessageBox::warning(this, "警告", "请输入有效的粗糙度值");
            return false;
        }
    }

    return true;
}

void MaterialEditDialog::saveMaterial()
{
    if (!validateInput()) {
        return;
    }

    QString name = nameEdit->text().trimmed();
    QString description = descriptionEdit->text().trimmed();

    try {
        if (materialType == "insulation") {
            QString eq1 = conductivityEq1Edit->text().trimmed();
            QString eq2 = conductivityEq2Edit->text().trimmed();
            QString eq3 = conductivityEq3Edit->text().trimmed();
            QString range1 = range1Edit->text().trimmed();
            QString range2 = range1Edit->text().trimmed();
            QString range3 = range1Edit->text().trimmed();
            double density = densityEdit->text().toDouble();

            if (isEditMode && name != materialName) {
                // 名称改变，删除旧记录
                materialManager->getInsulationMaterials().remove(materialName);
            }

            materialManager->addInsulationMaterial(name, eq1, eq2, eq3, range1, range2, range3, density, description);

        } else if (materialType == "protection") {
            double emissivity = emissivityEdit->text().toDouble();

            if (isEditMode && name != materialName) {
                materialManager->getProtectionMaterials().remove(materialName);
            }

            materialManager->addProtectionMaterial(name, emissivity, description);

        } else if (materialType == "fitting") {
            double resistance = resistanceEdit->text().toDouble();

            if (isEditMode && name != materialName) {
                materialManager->getPipeFittings().remove(materialName);
            }

            materialManager->addPipeFitting(name, resistance, description);

        } else if (materialType == "pipeType") {
            double roughness = roughnessEdit->text().toDouble();

            if (isEditMode && name != materialName) {
                materialManager->getPipeTypes().remove(materialName);
            }

            materialManager->addPipeType(name, roughness, description);
        }

        // 保存到文件
        if (materialManager->saveMaterialsToFile()) {
            QMessageBox::information(this, "成功", QString("材料 '%1' 已保存").arg(name));
            accept();
        } else {
            QMessageBox::critical(this, "错误", "保存材料失败");
        }

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "错误", QString("保存材料时发生错误: %1").arg(e.what()));
    }
}
