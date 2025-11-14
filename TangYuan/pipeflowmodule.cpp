#include "pipeflowmodule.h"
#include "dialog/fluiddialog.h"
#include "materialmanager.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QMessageBox>
#include <QDebug>

PipeFlowModule::PipeFlowModule(QWidget *parent)
    : TYModule(parent)
    , materialManager(new MaterialManager())
{
    setupUi();
}

PipeFlowModule::~PipeFlowModule()
{}

void PipeFlowModule::setupUi()
{
    QGridLayout *mainLayout = new QGridLayout(m_mainWidget);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    int row = 0;
    QPushButton *fluidButton = new QPushButton("设置流体", this);

    connect(fluidButton, &QPushButton::clicked, this, &PipeFlowModule::openFluidDialog);

    mainLayout->addWidget(fluidButton, row, 0);
    ++row;

    flowRateCombo = new QComboBox(this);
    flowRateCombo->addItems({"质量流量（kg/hr）", "体积流量（Nm3/hr）"});
    flowRateEdit = new QLineEdit(this);
    mainLayout->addWidget(new QLabel("压力（MPaA）"), row, 2);
    inletPressureEdit = new QLineEdit(this);
    inletArg2Combo = new QComboBox(this);
    inletArg2Combo->addItems({"温度（C）", "干度（0-1）"});
    inletArg2Edit = new QLineEdit(this);

    mainLayout->addWidget(flowRateCombo, row, 0);
    mainLayout->addWidget(flowRateEdit, row, 1);
    mainLayout->addWidget(inletPressureEdit, row, 3);
    mainLayout->addWidget(inletArg2Combo, row, 4);
    mainLayout->addWidget(inletArg2Edit, row, 5);
    ++row;

    mainLayout->addWidget(new QLabel("管道类型"), row, 0);
    pipeTypeCombo = new QComboBox(this);
    mainLayout->addWidget(pipeTypeCombo, row, 1);

    mainLayout->addWidget(new QLabel("管道长度（m）"), row, 2);
    lengthEdit = new QLineEdit(this);
    mainLayout->addWidget(lengthEdit, row, 3);

    mainLayout->addWidget(new QLabel("分段长度（m）"), row, 4);
    segmentLengthEdit = new QLineEdit(this);
    mainLayout->addWidget(segmentLengthEdit, row, 5);
    ++row;

    mainLayout->addWidget(new QLabel("管道外径（mm）"), row, 0);
    pipeOdEdit = new QLineEdit();
    mainLayout->addWidget(pipeOdEdit, row, 1);

    mainLayout->addWidget(new QLabel("管道壁厚（mm）"), row, 2);
    pipeWallThicknessEdit = new QLineEdit();
    mainLayout->addWidget(pipeWallThicknessEdit, row, 3);
    ++row;

    QGroupBox *fittingsGroup = new QGroupBox("管道元件");
    fittingsGroup->setFixedHeight(180);

    // 管道元件表格
    fittingsTable = new QTableWidget(fittingsGroup);
    fittingsTable->setColumnCount(2);
    fittingsTable->setHorizontalHeaderLabels({"元件名称",  "数量"});
    fittingsTable->setGeometry(QRect(20, 20, 400, 150));

    QPushButton *addFittingButton;
    QPushButton *removeFittingButton;

    addFittingButton = new QPushButton("添加管件", fittingsGroup);
    addFittingButton->setGeometry(QRect(470, 20, 75, 23));

    removeFittingButton = new QPushButton("删除管件", fittingsGroup);
    removeFittingButton->setGeometry(QRect(470, 50, 75, 23));

    connect(addFittingButton, &QPushButton::clicked, this, &PipeFlowModule::addFitting);
    connect(removeFittingButton, &QPushButton::clicked, this, &PipeFlowModule::removeFitting);

    mainLayout->addWidget(fittingsGroup, row, 0, 1, 6);
    ++row;

    mainLayout->addWidget(new QLabel("保温材料"), row, 0);
    insulationMaterialCombo = new QComboBox();
    mainLayout->addWidget(insulationMaterialCombo, row, 1);
    mainLayout->addWidget(new QLabel("保温厚度（mm）"), row, 2);
    insulationThicknessEdit = new QLineEdit();
    mainLayout->addWidget(insulationThicknessEdit, row, 3);
    mainLayout->addWidget(new QLabel("外保护层"), row, 4);
    cladMaterialCombo = new QComboBox();
    mainLayout->addWidget(cladMaterialCombo, row, 5);
    ++row;

    mainLayout->addWidget(new QLabel("环境温度（C）"), row, 0);
    ambientTempEdit = new QLineEdit();
    mainLayout->addWidget(ambientTempEdit, row, 1);

    mainLayout->addWidget(new QLabel("风速（m/sec）"), row, 2);
    windSpeedEdit = new QLineEdit();
    mainLayout->addWidget(windSpeedEdit, row, 3);

    populateMaterialCombos();

    pipeTypeCombo->setCurrentText("操作中基本无腐蚀的无缝钢管");
    cladMaterialCombo->setCurrentText("铝合金薄板");
}

void PipeFlowModule::openFluidDialog()
{
    FluidDialog fluidDialog(fluid, this);
    if (fluidDialog.exec() == QDialog::Accepted) {
        fluid = fluidDialog.getFluidString();
    }
}

void PipeFlowModule::addFitting()
{
    // 获取所有可用的管道元件
    QMap<QString, PipeFitting> fittings = materialManager->getPipeFittings();
    if (fittings.isEmpty()) {
        QMessageBox::warning(this, "警告", "没有可用的管道元件，请先在材料管理中添加");
        return;
    }

    // 创建选择对话框
    QDialog dialog(this);
    dialog.setWindowTitle("添加管道元件");
    dialog.setFixedSize(300, 140);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    layout->addWidget(new QLabel("选择管道元件:"));
    QComboBox *fittingCombo = new QComboBox();
    fittingCombo->addItems(fittings.keys());
    layout->addWidget(fittingCombo);

    layout->addWidget(new QLabel("数量:"));
    QSpinBox *countSpin = new QSpinBox();
    countSpin->setMinimum(1);
    countSpin->setMaximum(1000);
    countSpin->setValue(1);
    layout->addWidget(countSpin);

    QPushButton *addButton = new QPushButton("添加");
    layout->addWidget(addButton);

    connect(addButton, &QPushButton::clicked, [&]() {
        std::string fittingName = fittingCombo->currentText().toStdString();
        int count = countSpin->value();
        fittingsData.insert({fittingName, count});
        loadFittingsToTable();
        dialog.accept();
    });

    dialog.exec();
}

void PipeFlowModule::removeFitting()
{
    int currentRow = fittingsTable->currentRow();
    if (currentRow >= 0) {
        std::string name = fittingsTable->item(currentRow, 0)->text().toStdString();
        fittingsData.erase(name);
        loadFittingsToTable();
    } else {
        QMessageBox::warning(this, "警告", "请选择要删除的管道元件");
    }
}

void PipeFlowModule::loadFittingsToTable()
{
    fittingsTable->setRowCount(fittingsData.size());

    int i = 0;
    for (const auto& pair : fittingsData) {
        fittingsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(pair.first)));
        fittingsTable->setItem(i, 1, new QTableWidgetItem(QString::number(pair.second)));
        ++i;
    }
}

void PipeFlowModule::populateMaterialCombos()
{
    // 获取最新的数据
    materialManager->loadMaterialsFromFile();

    // 更新保温材料下拉列表
    insulationMaterialCombo->clear();
    insulationMaterialCombo->addItems(materialManager->getInsulationMaterials().keys());
    if (insulationMaterialCombo->count() > 0) {
        insulationMaterialCombo->setCurrentIndex(0);
    }

    // 更新外保护层下拉列表
    cladMaterialCombo->clear();
    cladMaterialCombo->addItems(materialManager->getCladMaterials().keys());
    if (cladMaterialCombo->count() > 0) {
        cladMaterialCombo->setCurrentIndex(0);
    }

    // 更新管道类型下拉列表
    pipeTypeCombo->clear();
    pipeTypeCombo->addItems(materialManager->getPipeTypes().keys());
    if (pipeTypeCombo->count() > 0) {
        pipeTypeCombo->setCurrentIndex(0);
    }
}

void PipeFlowModule::open(const std::map<std::string, std::any>& data)
{

}

std::map<std::string, std::any> PipeFlowModule::data()
{
    std::map<std::string, std::any> d;
    d["module"] = "pipeFlow";
    d["fluid"] = fluid;
    d["flowRateType"] = flowRateCombo->currentText();
    d["flowRate"] = flowRateEdit->text();
    d["inletPressure"] = inletPressureEdit->text();
    d["inletArg2Combo"] = inletArg2Combo->currentText();
    d["inletArg2Edit"] = inletArg2Edit->text();
    d["pipeType"] = pipeTypeCombo->currentText();
    d["length"] = lengthEdit->text();
    d["segmentLength"] = segmentLengthEdit->text();
    d["pipeOd"] = pipeOdEdit->text();
    d["pipeWallThickness"] = pipeWallThicknessEdit->text();
    d["insulationMaterial"] = insulationMaterialCombo->currentText();
    d["insulationThickness"] = insulationThicknessEdit->text();
    d["cladMaterial"] = cladMaterialCombo->currentText();
    d["ambientTemperature"] = ambientTempEdit->text();
    d["windSpeed"] = windSpeedEdit->text();
    d["fittingsData"] = fittingsData;

    return d;
}
QString PipeFlowModule::calculate()
{}
