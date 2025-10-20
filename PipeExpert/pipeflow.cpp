#include "pipeflow.h"
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
#include <QDebug>
PipeFlow::PipeFlow() {}

void PipeFlow::setupUi(QWidget *parentWidget)
{
    QWidget *mainWidget = new QWidget(parentWidget);
    mainWidget->setGeometry(QRect(10, 10, 930, 360));
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

}

void PipeFlow::run()
{
    qDebug()<<"pipeflow run";
}
