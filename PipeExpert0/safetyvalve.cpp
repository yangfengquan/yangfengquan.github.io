#include "safetyvalve.h"
#include "tymodule.h"
#include "dialog/fluiddialog.h"
#include "CoolPropLib.h"
#include <QGridLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QMessageBox>

SafetyValve::SafetyValve(QWidget *parent)
    : TYModule(parent)
{
    setupUi();
}

SafetyValve::~SafetyValve()
{}

void SafetyValve::setupUi()
{
    QGridLayout *mainLayout = new QGridLayout(m_mainWidget);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    // 设置列的最小宽度，确保标签列等宽
    //mainLayout->setColumnMinimumWidth(0, 80); // 标签列
    //mainLayout->setColumnStretch(1, 1);       // 内容列可拉伸

    int row = 0;
    // 流体行
    mainLayout->addWidget(new QLabel("流体", this), row, 0);

    QPushButton *fluidButton = new QPushButton("设置流体组分", this);
    connect(fluidButton, &QPushButton::clicked, this, &SafetyValve::onfluidButtonClicked);
    mainLayout->addWidget(fluidButton, row, 1, Qt::AlignLeft);

    row++;
    // 超压工况行
    mainLayout->addWidget(new QLabel("超压工况", this), row, 0);

    // 单选框水平布局
    QHBoxLayout *radioLayout = new QHBoxLayout();
    fireConditionRadio = new QRadioButton("火灾工况", this);
    tubeRuptureConditionRadio = new QRadioButton("换热管破裂", this);
    thermalVaporizationRadio = new QRadioButton("换热设备产生蒸汽", this);
    gasInflowRadio = new QRadioButton("气体流入", this);

    radioLayout->addWidget(fireConditionRadio);
    radioLayout->addWidget(tubeRuptureConditionRadio);
    radioLayout->addWidget(thermalVaporizationRadio);
    radioLayout->addWidget(gasInflowRadio);
    radioLayout->addStretch();

    mainLayout->addLayout(radioLayout, row, 1, 1, 3);

    // 按钮组
    conditionButtonGroup = new QButtonGroup(this);
    conditionButtonGroup->addButton(fireConditionRadio, 0);
    conditionButtonGroup->addButton(tubeRuptureConditionRadio, 1);
    conditionButtonGroup->addButton(thermalVaporizationRadio, 2);
    conditionButtonGroup->addButton(gasInflowRadio, 3);

    connect(conditionButtonGroup, &QButtonGroup::buttonClicked,
            this, &SafetyValve::onConditionButtonClicked);

    ++row;
    mainLayout->addWidget(new QLabel("泄放压力（MPa）", this), row, 0);
    releasePressure = new QLineEdit(this);

    mainLayout->addWidget(new QLabel("背压（MPa）", this), row, 2);
    backPressure = new QLineEdit(this);

    mainLayout->addWidget(releasePressure, row, 1);
    mainLayout->addWidget(backPressure, row, 3);

    ++row;
    mainLayout->addWidget(new QLabel("泄放装置的泄放系数", this), row, 0);
    releaseCoefficient = new QLineEdit(this);

    mainLayout->addWidget(releaseCoefficient, row, 1);
}
void SafetyValve::onfluidButtonClicked()
{
    FluidDialog fluidDialog(this);
    if (fluidDialog.exec() == QDialog::Accepted) {
        fluid = fluidDialog.getFluidString();
    }
}

void SafetyValve::onConditionButtonClicked(QAbstractButton *button)
{
    QRadioButton *radio = qobject_cast<QRadioButton*>(button);
    if (radio) {
        QMessageBox::information(this, "选择变化",
                                 QString("选中了: %1 (ID: %2)")
                                     .arg(radio->text())
                                     .arg(conditionButtonGroup->id(button)));
        overpressureCondition = conditionButtonGroup->id(button);
    }
}
void SafetyValve::open(QVariantMap data)
{}

QVariantMap SafetyValve::save()
{}

QString SafetyValve::run()
{

}
