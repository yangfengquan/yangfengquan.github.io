#include "mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    // 设置窗口属性
    setWindowTitle("管道分析软件");
    setMinimumSize(800, 600);

    // 创建菜单栏
    createMenu();

    // 创建中心部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 主布局 - 设置左上对齐，固定边距和间距
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    int row = 0;
    // 基础参数布局 - 固定控件大小和间距
    QGridLayout *basicLayout = new QGridLayout();
    basicLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    basicLayout->setHorizontalSpacing(15);
    basicLayout->setVerticalSpacing(15);
    basicLayout->setColumnMinimumWidth(0, 100);  // 第一列最小宽度
    basicLayout->setColumnMinimumWidth(1, 200);  // 第二列最小宽度
    basicLayout->setColumnMinimumWidth(2, 120);  // 第三列最小宽度
    basicLayout->setColumnMinimumWidth(3, 120);  // 第四列最小宽度

    // 管道类型
    QLabel *pipeTypeLabel = new QLabel("管道类型");
    pipeTypeLabel->setFixedHeight(25);
    basicLayout->addWidget(pipeTypeLabel, row, 0);
    pipeTypeCombo = new QComboBox();
    pipeTypeCombo->setFixedSize(200, 25);
    basicLayout->addWidget(pipeTypeCombo, row, 1);
    ++row;

    // 流体
    QLabel *fluidLabel = new QLabel("流    体");
    fluidLabel->setFixedHeight(25);
    basicLayout->addWidget(fluidLabel, row, 0);
    fluidCombo = new QComboBox();
    fluidCombo->addItems({"Water", "Air", "Ammonia", "CarbonDioxide", "R134a"});
    fluidCombo->setFixedSize(200, 25);
    basicLayout->addWidget(fluidCombo, row, 1);
    ++row;

    // 流量设置
    flowRateCombo = new QComboBox();
    flowRateCombo->addItems({"质量流量（kg/hr）", "体积流量（Nm3/hr）"});
    flowRateCombo->setFixedSize(200, 25);
    basicLayout->addWidget(flowRateCombo, row, 0);
    flowRateEdit = new QLineEdit();
    flowRateEdit->setFixedSize(200, 25);
    basicLayout->addWidget(flowRateEdit, row, 1);
    ++row;

    // 压力设置
    QLabel *pressureLabel = new QLabel("压力（MPaA）");
    pressureLabel->setFixedHeight(25);
    basicLayout->addWidget(pressureLabel, row, 0);
    inletPressureEdit = new QLineEdit();
    inletPressureEdit->setFixedSize(200, 25);
    basicLayout->addWidget(inletPressureEdit, row, 1);

    inletArg2Combo = new QComboBox();
    inletArg2Combo->addItems({"温度（C）", "干度（0-1）"});
    inletArg2Combo->setFixedSize(120, 25);
    basicLayout->addWidget(inletArg2Combo, row, 2);
    inletArg2Edit = new QLineEdit();
    inletArg2Edit->setFixedSize(120, 25);
    basicLayout->addWidget(inletArg2Edit, row, 3);
    ++row;

    // 管道长度和外径
    QLabel *lengthLabel = new QLabel("管道长度（m）");
    lengthLabel->setFixedHeight(25);
    basicLayout->addWidget(lengthLabel, row, 0);
    lengthEdit = new QLineEdit();
    lengthEdit->setFixedSize(200, 25);
    basicLayout->addWidget(lengthEdit, row, 1);

    QLabel *odLabel = new QLabel("管道外径（mm）");
    odLabel->setFixedHeight(25);
    basicLayout->addWidget(odLabel, row, 2);
    pipeOdEdit = new QLineEdit();
    pipeOdEdit->setFixedSize(200, 25);
    basicLayout->addWidget(pipeOdEdit, row, 3);
    ++row;

    // 分段长度和壁厚
    QLabel *segmentLabel = new QLabel("分段长度（m）");
    segmentLabel->setFixedHeight(25);
    basicLayout->addWidget(segmentLabel, row, 0);
    segmentLengthEdit = new QLineEdit();
    segmentLengthEdit->setFixedSize(200, 25);
    basicLayout->addWidget(segmentLengthEdit, row, 1);

    QLabel *thicknessLabel = new QLabel("管道壁厚（mm）");
    thicknessLabel->setFixedHeight(25);
    basicLayout->addWidget(thicknessLabel, row, 2);
    pipeWallThicknessEdit = new QLineEdit();
    pipeWallThicknessEdit->setFixedSize(200, 25);
    basicLayout->addWidget(pipeWallThicknessEdit, row, 3);

    mainLayout->addLayout(basicLayout);

    // 管道元件组 - 固定大小避免内部控件重叠
    QGroupBox *fittingsGroup = new QGroupBox("管道元件");
    fittingsGroup->setMinimumSize(740, 220);  // 使用最小尺寸而非固定尺寸
    QHBoxLayout *fittingslayout = new QHBoxLayout(fittingsGroup);
    fittingslayout->setContentsMargins(10, 10, 10, 10);
    fittingslayout->setSpacing(15);

    // 管道元件表格
    fittingsTable = new QTableWidget();
    fittingsTable->setColumnCount(2);
    fittingsTable->setHorizontalHeaderLabels({"元件名称",  "数量"});
    fittingsTable->horizontalHeader()->setStretchLastSection(true);
    fittingsTable->setFixedHeight(180);  // 固定高度
    fittingsTable->setMinimumWidth(580);  // 最小宽度
    fittingslayout->addWidget(fittingsTable);

    // 按钮布局
    QVBoxLayout *fittingsBtnlayout = new QVBoxLayout();
    fittingsBtnlayout->setAlignment(Qt::AlignTop);
    fittingsBtnlayout->setSpacing(10);

    QPushButton *addButton = new QPushButton("添加元件");
    addButton->setFixedSize(120, 30);
    QPushButton *removeButton = new QPushButton("删除元件");
    removeButton->setFixedSize(120, 30);

    fittingsBtnlayout->addWidget(addButton);
    fittingsBtnlayout->addWidget(removeButton);
    fittingslayout->addLayout(fittingsBtnlayout);

    mainLayout->addWidget(fittingsGroup);

    // 其他参数布局
    QGridLayout *otherlayout = new QGridLayout();
    otherlayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    otherlayout->setHorizontalSpacing(15);
    otherlayout->setVerticalSpacing(15);
    otherlayout->setColumnMinimumWidth(0, 100);
    otherlayout->setColumnMinimumWidth(1, 200);
    otherlayout->setColumnMinimumWidth(2, 120);
    otherlayout->setColumnMinimumWidth(3, 120);

    row = 0;
    // 保温材料
    QLabel *insulationLabel = new QLabel("保温材料");
    insulationLabel->setFixedHeight(25);
    otherlayout->addWidget(insulationLabel, row, 0);
    insulationMaterialCombo = new QComboBox();
    insulationMaterialCombo->setFixedSize(200, 25);
    otherlayout->addWidget(insulationMaterialCombo, row, 1);

    QLabel *insulationThickLabel = new QLabel("保温厚度（mm）");
    insulationThickLabel->setFixedHeight(25);
    otherlayout->addWidget(insulationThickLabel, row, 2);
    insulationThicknessEdit = new QLineEdit();
    insulationThicknessEdit->setFixedSize(120, 25);
    otherlayout->addWidget(insulationThicknessEdit, row, 3);
    ++row;

    // 外保护层
    QLabel *protectionLabel = new QLabel("外保护层");
    protectionLabel->setFixedHeight(25);
    otherlayout->addWidget(protectionLabel, row, 0);
    protectionMaterialCombo = new QComboBox();
    protectionMaterialCombo->setFixedSize(200, 25);
    otherlayout->addWidget(protectionMaterialCombo, row, 1);
    ++row;

    // 环境参数
    QLabel *ambientLabel = new QLabel("环境温度（C）");
    ambientLabel->setFixedHeight(25);
    otherlayout->addWidget(ambientLabel, row, 0);
    ambientTempEdit = new QLineEdit();
    ambientTempEdit->setFixedSize(200, 25);
    otherlayout->addWidget(ambientTempEdit, row, 1);

    QLabel *windLabel = new QLabel("风速（m/sec）");
    windLabel->setFixedHeight(25);
    otherlayout->addWidget(windLabel, row, 2);
    windSpeedEdit = new QLineEdit();
    windSpeedEdit->setFixedSize(200, 25);
    otherlayout->addWidget(windSpeedEdit, row, 3);

    mainLayout->addLayout(otherlayout);

    // 添加伸缩项，避免底部控件被挤压
    mainLayout->addStretch();

    statusBar()->showMessage("就绪");
}

void MainWindow::createMenu()
{
    // 文件菜单
    QMenu *fileMenu = menuBar()->addMenu("文件");

    QAction *saveAction = new QAction("保存", this);
    QAction *loadAction = new QAction("打开", this);
    QAction *exitAction = new QAction("退出", this);

    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    connect(saveAction, &QAction::triggered, this, &MainWindow::save);
    connect(loadAction, &QAction::triggered, this, &MainWindow::open);
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);

    // 材料管理菜单
    QMenu *materialMenu = menuBar()->addMenu("材料管理");

    QAction *pipeTypesAction = new QAction("管道类型", this);
    QAction *fittingsAction = new QAction("管道元件", this);
    QAction *insulationAction = new QAction("保温材料", this);
    QAction *protectionAction = new QAction("外保护层", this);

    materialMenu->addAction(pipeTypesAction);
    materialMenu->addAction(fittingsAction);
    materialMenu->addAction(insulationAction);
    materialMenu->addAction(protectionAction);

    connect(pipeTypesAction, &QAction::triggered, [this]() { openMaterialDialog("pipe_types"); });
    connect(fittingsAction, &QAction::triggered, [this]() { openMaterialDialog("fittings"); });
    connect(insulationAction, &QAction::triggered, [this]() { openMaterialDialog("insulation"); });
    connect(protectionAction, &QAction::triggered, [this]() { openMaterialDialog("protection"); });

    // 帮助菜单
    QMenu *helpMenu = menuBar()->addMenu("帮助");

    QAction *activationAction = new QAction("激活", this);
    QAction *aboutAction = new QAction("关于", this);

    helpMenu->addAction(activationAction);
    helpMenu->addAction(aboutAction);

    connect(activationAction, &QAction::triggered, this, &MainWindow::openActivationDialog);
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "关于", "管道分析软件\n版本：v1.0\n作者：杨奉全\nQQ群：816103114");
    });
}

void MainWindow::save()
{

}

void MainWindow::open()
{

}

void MainWindow::openMaterialDialog(const QString& type)
{

}

void MainWindow::openActivationDialog()
{

}
