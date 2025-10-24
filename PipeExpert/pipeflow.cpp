#include "pipeflow.h"
#include "MaterialManager.h"
#include "pipeline.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QJsonArray>
#include <QJsonObject>

#include <QMessageBox>

extern "C" {
#include "CoolPropLib.h"
}

#include <QDebug>

PipeFlow::PipeFlow(QWidget *parent)
    : QWidget(parent)
    , materialManager(new MaterialManager())
{

}

PipeFlow::~PipeFlow()
{

}

void PipeFlow::setupUi(QWidget *parent)
{
    QWidget *mainWidget = new QWidget(parent);
    mainWidget->setGeometry(QRect(10, 10, 930, 380));
    //QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    QGridLayout *basicLayout = new QGridLayout(mainWidget);
    basicLayout->setColumnMinimumWidth(0,100);
    basicLayout->setColumnMinimumWidth(1,210);
    basicLayout->setColumnMinimumWidth(2,100);
    basicLayout->setColumnMinimumWidth(3,210);
    basicLayout->setColumnMinimumWidth(4,100);
    basicLayout->setColumnMinimumWidth(5,210);

    int row = 0;

    basicLayout->addWidget(new QLabel("流体"), row, 0);
    fluidCombo = new QComboBox();
    //fluidCombo->addItems({"Water", "Air", "Ammonia", "CarbonDioxide", "R134a"});
    basicLayout->addWidget(fluidCombo, row, 1);

    flowRateCombo = new QComboBox();
    flowRateCombo->addItems({"质量流量（kg/hr）", "体积流量（Nm3/hr）"});
    basicLayout->addWidget(flowRateCombo, row, 2);
    flowRateEdit = new QLineEdit();
    basicLayout->addWidget(flowRateEdit, row, 3);
    ++row;

    basicLayout->addWidget(new QLabel("压力（MPaA）"), row, 0);
    inletPressureEdit = new QLineEdit();
    basicLayout->addWidget(inletPressureEdit, row, 1);
    inletArg2Combo = new QComboBox();
    inletArg2Combo->addItems({"温度（C）", "干度（0-1）"});
    basicLayout->addWidget(inletArg2Combo, row, 2);
    inletArg2Edit = new QLineEdit();
    basicLayout->addWidget(inletArg2Edit, row, 3);
    ++row;

    basicLayout->addWidget(new QLabel("管道类型"), row, 0);
    pipeTypeCombo = new QComboBox();
    basicLayout->addWidget(pipeTypeCombo, row, 1);

    basicLayout->addWidget(new QLabel("管道长度（m）"), row, 2);
    lengthEdit = new QLineEdit();
    basicLayout->addWidget(lengthEdit, row, 3);

    basicLayout->addWidget(new QLabel("分段长度（m）"), row, 4);
    segmentLengthEdit = new QLineEdit();
    basicLayout->addWidget(segmentLengthEdit, row, 5);
    ++row;

    basicLayout->addWidget(new QLabel("管道外径（mm）"), row, 0);
    pipeOdEdit = new QLineEdit();
    basicLayout->addWidget(pipeOdEdit, row, 1);

    basicLayout->addWidget(new QLabel("管道壁厚（mm）"), row, 2);
    pipeWallThicknessEdit = new QLineEdit();
    basicLayout->addWidget(pipeWallThicknessEdit, row, 3);
    ++row;

    QGroupBox *fittingsGroup = new QGroupBox("管道元件");
    basicLayout->addWidget(fittingsGroup, row, 0, 1, 6);
    fittingsGroup->setFixedHeight(180);

    // 管道元件表格
    fittingsTable = new QTableWidget(fittingsGroup);
    fittingsTable->setColumnCount(2);
    fittingsTable->setHorizontalHeaderLabels({"元件名称",  "数量"});
    fittingsTable->setGeometry(QRect(20, 20, 400, 150));

    addFittingButton = new QPushButton("添加管件", fittingsGroup);
    addFittingButton->setGeometry(QRect(470, 20, 75, 23));

    removeFittingButton = new QPushButton("删除管件", fittingsGroup);
    removeFittingButton->setGeometry(QRect(470, 50, 75, 23));

    ++row;

    basicLayout->addWidget(new QLabel("保温材料"), row, 0);
    insulationMaterialCombo = new QComboBox();
    basicLayout->addWidget(insulationMaterialCombo, row, 1);
    basicLayout->addWidget(new QLabel("保温厚度（mm）"), row, 2);
    insulationThicknessEdit = new QLineEdit();
    basicLayout->addWidget(insulationThicknessEdit, row, 3);

    basicLayout->addWidget(new QLabel("外保护层"), row, 4);
    cladMaterialCombo = new QComboBox();
    basicLayout->addWidget(cladMaterialCombo, row, 5);
    ++row;

    basicLayout->addWidget(new QLabel("环境温度（C）"), row, 0);
    ambientTempEdit = new QLineEdit();
    basicLayout->addWidget(ambientTempEdit, row, 1);

    basicLayout->addWidget(new QLabel("风速（m/sec）"), row, 2);
    windSpeedEdit = new QLineEdit();
    basicLayout->addWidget(windSpeedEdit, row, 3);

    connect(addFittingButton, &QPushButton::clicked, this, &PipeFlow::addFitting);
    connect(removeFittingButton, &QPushButton::clicked, this, &PipeFlow::removeFitting);

    refreshMaterialCombos();

    loadFluidsToCombobox();
    pipeTypeCombo->setCurrentText("操作中基本无腐蚀的无缝钢管");
    cladMaterialCombo->setCurrentText("铝合金薄板");

}
void PipeFlow::loadFluidsToCombobox()
{

    // 获取CoolProp支持的所有流体
    char buffer[4096]; // 足够大的缓冲区
    long return_val = get_global_param_string("fluids_list", buffer, sizeof(buffer));
    //qDebug()<<buffer;
    if (return_val == 1) { // 成功
        std::string fluids_str(buffer);

        // 分割字符串，流体之间用逗号分隔
        std::vector<std::string> fluids;
        size_t start = 0;
        size_t end = fluids_str.find(',');

        while (end != std::string::npos) {
            fluids.push_back(fluids_str.substr(start, end - start));
            start = end + 1;
            end = fluids_str.find(',', start);
        }
        // 添加最后一个流体
        if (start < fluids_str.length()) {
            fluids.push_back(fluids_str.substr(start));
        }


        for (const std::string& fluid : fluids) {
            QString fluidName = QString::fromStdString(fluid).trimmed();
            if (fluidName.isEmpty()) continue;

            QString displayName;

            // 如果有中文映射则使用中文，否则使用英文原名
            if (fluidMap.contains(fluidName)) {
                displayName = fluidMap[fluidName] + " (" + fluidName + ")";
            } else {
                displayName = fluidName;
            }

            fluidCombo->addItem(displayName, fluidName);
        }

        int waterIndex = fluidCombo->findData("Water");
        fluidCombo->setCurrentIndex(waterIndex);
    }
}

void PipeFlow::refreshMaterialCombos()
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

void PipeFlow::addFitting()
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
    dialog.setFixedSize(300, 200);

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
        QString fittingName = fittingCombo->currentText();
        int count = countSpin->value();

        fittingsData.append(qMakePair(fittingName, count));
        loadFittingsToTable();
        dialog.accept();
    });

    dialog.exec();
}

void PipeFlow::removeFitting()
{
    int row = fittingsTable->currentRow();
    if (row >= 0 && row < fittingsData.size()) {
        fittingsData.removeAt(row);
        loadFittingsToTable();
    } else {
        QMessageBox::warning(this, "警告", "请选择要删除的管道元件");
    }
}

void PipeFlow::loadFittingsToTable()
{
    fittingsTable->setRowCount(fittingsData.size());

    for (int i = 0; i < fittingsData.size(); ++i) {
        const auto& fitting = fittingsData[i];
        auto pipeFitting = materialManager->getPipeFittings().value(fitting.first);

        fittingsTable->setItem(i, 0, new QTableWidgetItem(fitting.first));
        //fittingsTable->setItem(i, 1, new QTableWidgetItem(QString::number(pipeFitting.resistanceCoef, 'f', 3)));
        fittingsTable->setItem(i, 1, new QTableWidgetItem(QString::number(fitting.second)));
    }
}

QString PipeFlow::run()
{
    QByteArray byteArr = fluidCombo->currentData().toString().toUtf8();
    //const char* fluid = fluidCombo->currentText().toUtf8().constData();
    const char* fluid = byteArr.constData();
    QString flowRateType = flowRateCombo->currentText();
    double flowRate = flowRateEdit->text().toDouble() / 3600;
    double inletPressure = inletPressureEdit->text().toDouble() * 1e6;
    QString inletArg2Para = inletArg2Combo->currentText();
    double inletArg2 = inletArg2Edit->text().toDouble();
    std::string pipeTypeName = pipeTypeCombo->currentText().toUtf8().constData();
    double length = lengthEdit->text().toDouble();
    double segmentLength = segmentLengthEdit->text().toDouble();
    double pipeOd = pipeOdEdit->text().toDouble() / 1000;
    double pipeWallThickness = pipeWallThicknessEdit->text().toDouble() / 1000;
    std::string insulationMaterialName = insulationMaterialCombo->currentText().toUtf8().toStdString();
    double insulationThickness = insulationThicknessEdit->text().toDouble() / 1000;
    std::string cladMaterialName = cladMaterialCombo->currentText().toUtf8().toStdString();
    double ambientTemperature = ambientTempEdit->text().toDouble() + 273.15;
    double windSpeed = windSpeedEdit->text().toDouble();
    double inletTemperature, inletQuality;
    if (inletArg2Para == "温度（C）") {
        inletTemperature = inletArg2 + 273.15;
        inletQuality = -1;
    } else {
        inletQuality = inletArg2;
    }

    std::map<std::string, int> stdfittingsData;

    for (const auto& fitting : fittingsData) {
        std::string name = fitting.first.toStdString();
        int num = fitting.second;
        stdfittingsData[name] = num;
    }
    qDebug()<<"f1";
    Pipeline *pipeline = new Pipeline(
        fluid,
        inletPressure,
        inletTemperature,
        pipeOd,
        pipeWallThickness,
        length,
        insulationThickness,
        pipeTypeName,
        insulationMaterialName,
        cladMaterialName,
        ambientTemperature,
        windSpeed,
        stdfittingsData,
        segmentLength,
        inletQuality
        );
qDebug()<<"f2";
    if (flowRateType == "质量流量（kg/hr）") {
        pipeline->massFlowRate(flowRate);
    } else {
        pipeline->volumetricFlowRate(flowRate);
    }
qDebug()<<"f3";
    pipeline->calculate();
qDebug()<<"f4";
    return pipeline->getReport();
qDebug()<<"f5";
}

QVariantMap PipeFlow::save()
{
    QVariantMap data;
    data["module"] = "pipeFlow";
    data["fluid"] = fluidCombo->currentText();
    data["flowRateType"] = flowRateCombo->currentText();
    data["flowRate"] = flowRateEdit->text();
    data["inletPressure"] = inletPressureEdit->text();
    data["inletArg2Combo"] = inletArg2Combo->currentText();
    data["inletArg2Edit"] = inletArg2Edit->text();
    data["pipeType"] = pipeTypeCombo->currentText();
    data["length"] = lengthEdit->text();
    data["segmentLength"] = segmentLengthEdit->text();
    data["pipeOd"] = pipeOdEdit->text();
    data["pipeWallThickness"] = pipeWallThicknessEdit->text();
    data["insulationMaterial"] = insulationMaterialCombo->currentText();
    data["insulationThickness"] = insulationThicknessEdit->text();
    data["cladMaterial"] = cladMaterialCombo->currentText();
    data["ambientTemperature"] = ambientTempEdit->text();
    data["windSpeed"] = windSpeedEdit->text();

    // 保存管道元件数据
    QJsonArray fittingsArray;
    for (const auto& fitting : fittingsData) {
        QJsonObject fittingObj;
        fittingObj["name"] = fitting.first;
        fittingObj["count"] = fitting.second;
        fittingsArray.append(fittingObj);
    }
    data["fittingsData"] = fittingsArray;

    return data;
}

void PipeFlow::open(QVariantMap data)
{

    // 恢复数据到界面
    fluidCombo->setCurrentText(data["fluid"].toString());
    flowRateCombo->setCurrentText(data["flowRateType"].toString());
    flowRateEdit->setText(data["flowRate"].toString());
    inletPressureEdit->setText(data["inletPressure"].toString());
    inletArg2Combo->setCurrentText(data["inletArg2Combo"].toString());
    inletArg2Edit->setText(data["inletArg2Edit"].toString());
    pipeTypeCombo->setCurrentText(data["pipeType"].toString());

    lengthEdit->setText(data["length"].toString());
    pipeOdEdit->setText(data["pipeOd"].toString());
    pipeWallThicknessEdit->setText(data["pipeWallThickness"].toString());
    segmentLengthEdit->setText(data["segmentLength"].toString());

    insulationMaterialCombo->setCurrentText(data["insulationMaterial"].toString());
    insulationThicknessEdit->setText(data["insulationThickness"].toString());
    cladMaterialCombo->setCurrentText(data["cladMaterial"].toString());
    ambientTempEdit->setText(data["ambientTemperature"].toString());
    windSpeedEdit->setText(data["windSpeed"].toString());

    // 恢复管道元件数据
    fittingsData.clear();
    QJsonArray fittingsArray = data["fittingsData"].toJsonArray();
    for (const QJsonValue& value : fittingsArray) {
        QJsonObject obj = value.toObject();
        fittingsData.append(qMakePair(obj["name"].toString(), obj["count"].toInt()));
    }
    loadFittingsToTable();
}
