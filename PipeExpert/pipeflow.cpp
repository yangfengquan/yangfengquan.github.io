#include "pipeflow.h"
#include "MaterialManager.h"
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
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
PipeFlow::PipeFlow(QWidget *parent)
    : QWidget(parent)
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
    fluidCombo->addItems({"Water", "Air", "Ammonia", "CarbonDioxide", "R134a"});
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
    QComboBox *pipeTypeCombo = new QComboBox();
    pipeTypeCombo->addItems({"无缝黄铜、铜及铅管","操作中基本无腐蚀的无缝钢管"});
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
    protectionMaterialCombo = new QComboBox();
    basicLayout->addWidget(protectionMaterialCombo, row, 5);
    ++row;

    basicLayout->addWidget(new QLabel("环境温度（C）"), row, 0);
    ambientTempEdit = new QLineEdit();
    basicLayout->addWidget(ambientTempEdit, row, 1);

    basicLayout->addWidget(new QLabel("风速（m/sec）"), row, 2);
    windSpeedEdit = new QLineEdit();
    basicLayout->addWidget(windSpeedEdit, row, 3);

    connect(addFittingButton, &QPushButton::clicked, this, &PipeFlow::addFitting);
    connect(removeFittingButton, &QPushButton::clicked, this, &PipeFlow::removeFitting);
}

void PipeFlow::addFitting()
{
    // 获取所有可用的管道元件
    auto fittings = materialManager->getPipeFittings();
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
        fittingsTable->setItem(i, 1, new QTableWidgetItem(QString::number(pipeFitting.resistanceCoef, 'f', 3)));
        fittingsTable->setItem(i, 2, new QTableWidgetItem(QString::number(fitting.second)));
    }
}

void PipeFlow::run()
{
    qDebug()<<"pipeflow run";
}

void PipeFlow::save()
{
    try {
        QVariantMap data;
        data["fluid"] = fluidCombo->currentText();
        data["massFlow"] = flowRateEdit->text();
        data["inletPressure"] = inletPressureEdit->text();
        data["inletArg2Combo"] = inletArg2Combo->currentText();
        data["inletArg2Edit"] = inletArg2Edit->text();
        data["pipeType"] = pipeTypeCombo->currentText();
        data["length"] = lengthEdit->text();
        data["segment_length"] = segmentLengthEdit->text();
        data["pipeOd"] = pipeOdEdit->text();
        data["pipeWallThickness"] = pipeWallThicknessEdit->text();
        data["insulationMaterial"] = insulationMaterialCombo->currentText();
        data["insulationThickness"] = insulationThicknessEdit->text();
        data["protectionMaterial"] = protectionMaterialCombo->currentText();
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
        data["fittings_data"] = fittingsArray;

        QString filename = QFileDialog::getSaveFileName(
            this, "保存数据", "", "Pipe文件 (*.pipe);;所有文件 (*.*)");

        if (!filename.isEmpty()) {
            QFile file(filename);
            if (file.open(QIODevice::WriteOnly)) {
                QJsonDocument doc(QJsonObject::fromVariantMap(data));
                file.write(doc.toJson());
                file.close();
                QMessageBox::information(this, "成功", QString("数据已保存到: %1").arg(filename));
            }
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("保存数据失败: %1").arg(e.what()));
    }
}
