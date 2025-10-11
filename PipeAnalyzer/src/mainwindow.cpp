#include "MainWindow.h"
#include "MaterialManager.h"
#include "FluidAnalyzer.h"
#include "ActivationManager.h"
#include "MaterialDialog.h"
#include "ActivationDialog.h"

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
#include <QChartView>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QString>

//using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, materialManager(new MaterialManager(this))
    //, fluidAnalyzer(new FluidAnalyzer(this))
    //, activationManager(new ActivationManager(this))
    , materialManager(nullptr)
    , fluidAnalyzer(nullptr)
    , activationManager(nullptr)
    , mainTabWidget(nullptr)
    , pipeNameEdit(nullptr)
    , fluidCombo(nullptr)
    , pipeTypeCombo(nullptr)
    , inletPressureEdit(nullptr)
    , inletTemperatureEdit(nullptr)
    , inletQualityEdit(nullptr)
    , massFlowEdit(nullptr)
    , pipeLengthEdit(nullptr)
    , pipeOdEdit(nullptr)
    , pipeWallThicknessEdit(nullptr)
    , segmentLengthEdit(nullptr)
    , insulationMaterialCombo(nullptr)  // 确保初始化为 nullptr
    , insulationThicknessEdit(nullptr)
    , protectionMaterialCombo(nullptr)  // 确保初始化为 nullptr
    , ambientTempEdit(nullptr)
    , windSpeedEdit(nullptr)
    , fittingsTable(nullptr)
    , resultsTabWidget(nullptr)
    , resultsText(nullptr)
    , segmentTable(nullptr)
    , chartView(nullptr)
{
    //fluidAnalyzer = nullptr;
    // 检查软件激活状态
    setupUI();
    //checkSoftwareStatus();
}

MainWindow::~MainWindow()
{
}

void MainWindow::checkSoftwareStatus()
{setupUI();
    if (activationManager->isActivated()) {
//setupUI();
        return;
    }

    int trialCount = activationManager->getTrialCount();
    if (trialCount < 5) {
        activationManager->updateTrialCount(trialCount + 1);
        if (trialCount > 2) {
            QMessageBox::information(this, "试用提示",
                                     QString("这是您的第 %1 次试用，还剩 %2 次试用机会。\n"
                                             "试用结束后，添加QQ群：816103114，免费获取激活码。")
                                         .arg(trialCount + 1).arg(4 - trialCount));
        }
        setupUI();
    } else {
        showActivationInterface();
    }
}

void MainWindow::showActivationInterface()
{
    ActivationDialog dialog(this, activationManager);
    if (dialog.exec() == QDialog::Accepted) {
        setupUI();
    } else {
        QApplication::quit();
    }
}

void MainWindow::setupUI()
{
    /*
    // 设置窗口属性
    setWindowTitle("管道分析软件");
    setMinimumSize(1200, 800);

    // 创建菜单栏
    createMenu();

    // 创建中心部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 创建标签页
    mainTabWidget = new QTabWidget(this);
    mainLayout->addWidget(mainTabWidget);

    // 设置各标签页
    setupInputTab();
    setupFittingsTab();
    setupResultsTab();

    // 状态栏
    statusBar()->showMessage("就绪");
*/
    qDebug() << "setupUI() 开始";

    try {
        // 设置窗口属性
        qDebug() << "设置窗口属性...";
        setWindowTitle("管道分析软件");
        setMinimumSize(1200, 800);

        // 创建菜单栏
        qDebug() << "创建菜单栏...";
        createMenu();

        // 创建中心部件
        qDebug() << "创建中心部件...";
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        // 主布局
        qDebug() << "创建主布局...";
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        // 创建标签页
        qDebug() << "创建标签页...";
        mainTabWidget = new QTabWidget(this);
        mainLayout->addWidget(mainTabWidget);

        // 设置各标签页
        qDebug() << "设置输入标签页...";
        setupInputTab();
        qDebug() << "设置元件标签页...";
        setupFittingsTab();
        qDebug() << "设置结果标签页...";
        setupResultsTab();

        // 状态栏
        qDebug() << "设置状态栏...";
        statusBar()->showMessage("就绪");

    } catch (const std::exception& e) {
        qCritical() << "setupUI 中发生异常: " << e.what();
        throw;
    } catch (...) {
        qCritical() << "setupUI 中发生未知异常";
        throw;
    }

    qDebug() << "setupUI() 完成";
}

void MainWindow::createMenu()
{
    // 文件菜单
    QMenu *fileMenu = menuBar()->addMenu("文件");

    QAction *saveAction = new QAction("保存数据", this);
    QAction *loadAction = new QAction("读取数据", this);
    QAction *exitAction = new QAction("退出", this);

    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    connect(saveAction, &QAction::triggered, this, &MainWindow::saveData);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadData);
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);

    // 材料管理菜单
    QMenu *materialMenu = menuBar()->addMenu("材料管理");

    QAction *pipeTypesAction = new QAction("管道类型管理", this);
    QAction *fittingsAction = new QAction("管道元件管理", this);
    QAction *insulationAction = new QAction("保温材料管理", this);
    QAction *protectionAction = new QAction("外保护层管理", this);

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

    connect(activationAction, &QAction::triggered, this, &MainWindow::showActivationInterface);
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "关于", "管道分析软件\n版本：v1.0\n作者：杨奉全\nQQ群：816103114");
    });
}

void MainWindow::setupInputTab()
{
    /*
    QWidget *inputTab = new QWidget();
    mainTabWidget->addTab(inputTab, "输入参数");

    QVBoxLayout *layout = new QVBoxLayout(inputTab);

    // 工况名称
    QGroupBox *basicGroup = new QGroupBox("基本参数");
    QGridLayout *basicLayout = new QGridLayout(basicGroup);

    basicLayout->addWidget(new QLabel("管道名称:"), 0, 0);
    pipeNameEdit = new QLineEdit("过热蒸汽管道");
    basicLayout->addWidget(pipeNameEdit, 0, 1);

    basicLayout->addWidget(new QLabel("流体:"), 1, 0);
    fluidCombo = new QComboBox();
    fluidCombo->addItems({"Water", "Air", "Ammonia", "CarbonDioxide", "R134a"});
    basicLayout->addWidget(fluidCombo, 1, 1);

    basicLayout->addWidget(new QLabel("管道类型:"), 2, 0);
    pipeTypeCombo = new QComboBox();
    refreshMaterialCombos();
    basicLayout->addWidget(pipeTypeCombo, 2, 1);

    layout->addWidget(basicGroup);

    // 入口参数
    QGroupBox *inletGroup = new QGroupBox("入口参数");
    QGridLayout *inletLayout = new QGridLayout(inletGroup);

    inletLayout->addWidget(new QLabel("压力(MPa, 绝对压力):"), 0, 0);
    inletPressureEdit = new QLineEdit("1.0");
    inletLayout->addWidget(inletPressureEdit, 0, 1);

    inletLayout->addWidget(new QLabel("温度(°C):"), 0, 2);
    inletTemperatureEdit = new QLineEdit("300");
    inletLayout->addWidget(inletTemperatureEdit, 0, 3);

    inletLayout->addWidget(new QLabel("干度:"), 1, 0);
    inletQualityEdit = new QLineEdit();
    inletLayout->addWidget(inletQualityEdit, 1, 1);
    inletLayout->addWidget(new QLabel("(可选，如指定则忽略温度)"), 1, 2, 1, 2);

    inletLayout->addWidget(new QLabel("质量流量(kg/h):"), 2, 0);
    massFlowEdit = new QLineEdit("10000");
    inletLayout->addWidget(massFlowEdit, 2, 1);

    layout->addWidget(inletGroup);

    // 管道参数
    QGroupBox *pipeGroup = new QGroupBox("管道参数");
    QGridLayout *pipeLayout = new QGridLayout(pipeGroup);

    pipeLayout->addWidget(new QLabel("管道长度(m):"), 0, 0);
    pipeLengthEdit = new QLineEdit("500");
    pipeLayout->addWidget(pipeLengthEdit, 0, 1);

    pipeLayout->addWidget(new QLabel("管道外径(mm):"), 0, 2);
    pipeOdEdit = new QLineEdit("168");
    pipeLayout->addWidget(pipeOdEdit, 0, 3);

    pipeLayout->addWidget(new QLabel("分段长度(m):"), 1, 0);
    segmentLengthEdit = new QLineEdit("50");
    pipeLayout->addWidget(segmentLengthEdit, 1, 1);

    pipeLayout->addWidget(new QLabel("管道壁厚(mm):"), 1, 2);
    pipeWallThicknessEdit = new QLineEdit("9");
    pipeLayout->addWidget(pipeWallThicknessEdit, 1, 3);

    layout->addWidget(pipeGroup);

    // 保温参数
    QGroupBox *insulationGroup = new QGroupBox("保温参数");
    QGridLayout *insulationLayout = new QGridLayout(insulationGroup);

    insulationLayout->addWidget(new QLabel("保温材料:"), 0, 0);
    insulationMaterialCombo = new QComboBox();
    insulationLayout->addWidget(insulationMaterialCombo, 0, 1);

    insulationLayout->addWidget(new QLabel("保温厚度(mm):"), 0, 2);
    insulationThicknessEdit = new QLineEdit("50");
    insulationLayout->addWidget(insulationThicknessEdit, 0, 3);

    insulationLayout->addWidget(new QLabel("外保护层:"), 1, 0);
    protectionMaterialCombo = new QComboBox();
    insulationLayout->addWidget(protectionMaterialCombo, 1, 1);

    layout->addWidget(insulationGroup);

    // 环境参数
    QGroupBox *envGroup = new QGroupBox("环境参数");
    QGridLayout *envLayout = new QGridLayout(envGroup);

    envLayout->addWidget(new QLabel("环境温度(°C):"), 0, 0);
    ambientTempEdit = new QLineEdit("20");
    envLayout->addWidget(ambientTempEdit, 0, 1);

    envLayout->addWidget(new QLabel("风速(m/s):"), 0, 2);
    windSpeedEdit = new QLineEdit("3.0");
    envLayout->addWidget(windSpeedEdit, 0, 3);

    layout->addWidget(envGroup);

    // 按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *analyzeButton = new QPushButton("开始分析");
    QPushButton *reportButton = new QPushButton("生成报告");
    QPushButton *clearButton = new QPushButton("清空输入");

    buttonLayout->addWidget(analyzeButton);
    buttonLayout->addWidget(reportButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    connect(analyzeButton, &QPushButton::clicked, this, &MainWindow::analyzePipe);
    connect(reportButton, &QPushButton::clicked, this, &MainWindow::generateReport);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearInput);
*/
    qDebug() << "setupInputTab() 开始";

    try {
        qDebug() << "步骤1: 创建输入标签页控件...";
        QWidget *inputTab = new QWidget();
        qDebug() << "输入标签页控件创建完成";

        qDebug() << "步骤2: 添加标签页到主标签控件...";
        mainTabWidget->addTab(inputTab, "输入参数");
        qDebug() << "标签页添加完成";

        qDebug() << "步骤3: 创建主布局...";
        QVBoxLayout *layout = new QVBoxLayout(inputTab);
        qDebug() << "主布局创建完成";

        // 工况名称
        qDebug() << "步骤4: 创建基本参数组...";
        QGroupBox *basicGroup = new QGroupBox("基本参数");
        qDebug() << "基本参数组创建完成";

        qDebug() << "步骤5: 创建基本参数布局...";
        QGridLayout *basicLayout = new QGridLayout(basicGroup);
        qDebug() << "基本参数布局创建完成";

        qDebug() << "步骤6: 添加管道名称标签...";
        basicLayout->addWidget(new QLabel("管道名称:"), 0, 0);
        qDebug() << "管道名称标签添加完成";

        qDebug() << "步骤7: 创建管道名称编辑框...";
        pipeNameEdit = new QLineEdit("过热蒸汽管道");
        basicLayout->addWidget(pipeNameEdit, 0, 1);
        qDebug() << "管道名称编辑框创建完成";

        qDebug() << "步骤8: 添加流体标签...";
        basicLayout->addWidget(new QLabel("流体:"), 1, 0);
        qDebug() << "流体标签添加完成";

        qDebug() << "步骤9: 创建流体下拉框...";
        fluidCombo = new QComboBox();
        fluidCombo->addItems({"Water", "Air", "Ammonia", "CarbonDioxide", "R134a"});
        basicLayout->addWidget(fluidCombo, 1, 1);
        qDebug() << "流体下拉框创建完成";

        qDebug() << "步骤10: 添加管道类型标签...";
        basicLayout->addWidget(new QLabel("管道类型:"), 2, 0);
        qDebug() << "管道类型标签添加完成";

        qDebug() << "步骤11: 创建管道类型下拉框...";
        pipeTypeCombo = new QComboBox();
        basicLayout->addWidget(pipeTypeCombo, 2, 1);
        qDebug() << "管道类型下拉框创建完成";

        // 继续创建其他组件...

        // 保温参数
        qDebug() << "创建保温参数组...";
        QGroupBox *insulationGroup = new QGroupBox("保温参数");
        QGridLayout *insulationLayout = new QGridLayout(insulationGroup);

        qDebug() << "添加保温材料标签...";
        insulationLayout->addWidget(new QLabel("保温材料:"), 0, 0);
        qDebug() << "保温材料标签添加完成";

        qDebug() << "创建保温材料下拉框...";
        insulationMaterialCombo = new QComboBox();  // 确保在这里创建
        insulationLayout->addWidget(insulationMaterialCombo, 0, 1);
        qDebug() << "保温材料下拉框创建完成";

        qDebug() << "添加保温厚度标签...";
        insulationLayout->addWidget(new QLabel("保温厚度(mm):"), 0, 2);
        insulationThicknessEdit = new QLineEdit("50");
        insulationLayout->addWidget(insulationThicknessEdit, 0, 3);
        qDebug() << "保温厚度相关组件创建完成";

        qDebug() << "添加外保护层标签...";
        insulationLayout->addWidget(new QLabel("外保护层:"), 1, 0);
        qDebug() << "外保护层标签添加完成";

        qDebug() << "创建外保护层下拉框...";
        protectionMaterialCombo = new QComboBox();  // 确保在这里创建
        insulationLayout->addWidget(protectionMaterialCombo, 1, 1);
        qDebug() << "外保护层下拉框创建完成";

        layout->addWidget(insulationGroup);
        qDebug() << "保温参数组添加完成";

        qDebug() << "步骤12: 将基本参数组添加到布局...";
        layout->addWidget(basicGroup);
        qDebug() << "基本参数组添加完成";

        // 继续添加其他组件的调试信息...
        // 入口参数组
        qDebug() << "步骤13: 创建入口参数组...";
        QGroupBox *inletGroup = new QGroupBox("入口参数");
        // ... 继续添加每个组件的调试信息
        // 在所有下拉框创建完成后，再调用 refreshMaterialCombos()
        qDebug() << "所有下拉框创建完成，调用 refreshMaterialCombos()...";
        refreshMaterialCombos();
        qDebug() << "refreshMaterialCombos() 调用完成";
    } catch (const std::exception& e) {
        qCritical() << "setupInputTab 中捕获异常:" << e.what();
        throw;
    } catch (...) {
        qCritical() << "setupInputTab 中捕获未知异常";
        throw;
    }

    qDebug() << "setupInputTab() 完成";
}

void MainWindow::setupFittingsTab()
{
    QWidget *fittingsTab = new QWidget();
    mainTabWidget->addTab(fittingsTab, "管道元件");

    QVBoxLayout *layout = new QVBoxLayout(fittingsTab);

    // 管道元件表格
    fittingsTable = new QTableWidget(this);
    fittingsTable->setColumnCount(3);
    fittingsTable->setHorizontalHeaderLabels({"元件名称", "阻力系数", "数量"});
    fittingsTable->horizontalHeader()->setStretchLastSection(true);

    layout->addWidget(new QLabel("管道元件配置:"));
    layout->addWidget(fittingsTable);

    // 按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("添加元件");
    QPushButton *removeButton = new QPushButton("删除元件");

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addFitting);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeFitting);

    // 初始化管道元件数据
    fittingsData.clear();
}

void MainWindow::setupResultsTab()
{
    QWidget *resultsTab = new QWidget();
    mainTabWidget->addTab(resultsTab, "分析结果");

    QVBoxLayout *layout = new QVBoxLayout(resultsTab);

    // 结果标签页
    resultsTabWidget = new QTabWidget(this);
    layout->addWidget(resultsTabWidget);

    // 文字分析结果页
    QWidget *textTab = new QWidget();
    resultsTabWidget->addTab(textTab, "文字分析");

    QVBoxLayout *textLayout = new QVBoxLayout(textTab);
    resultsText = new QTextEdit();
    resultsText->setReadOnly(true);
    textLayout->addWidget(resultsText);

    // 分段数据表格页
    QWidget *tableTab = new QWidget();
    resultsTabWidget->addTab(tableTab, "分段数据");

    QVBoxLayout *tableLayout = new QVBoxLayout(tableTab);
    segmentTable = new QTableWidget(this);
    segmentTable->setColumnCount(9);
    segmentTable->setHorizontalHeaderLabels({
        "段", "距离(m)", "压力(MPa)", "温度(°C)", "干度",
        "流速(m/s)", "单位长度热损失(W/m)", "单位面积热损失(W/m²)", "表面温度(°C)"
    });
    tableLayout->addWidget(segmentTable);

    // 分析图表页
    QWidget *chartTab = new QWidget();
    resultsTabWidget->addTab(chartTab, "分析图表");

    QVBoxLayout *chartLayout = new QVBoxLayout(chartTab);
    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    chartLayout->addWidget(chartView);
}

void MainWindow::analyzePipe()
{
    try {
        // 获取管道类型的粗糙度
        QString pipeTypeName = pipeTypeCombo->currentText();
        double roughness = 2e-5; // 默认值

        // 获取输入参数
        QVariantMap pipeParams;
        pipeParams["pipe_name"] = pipeNameEdit->text();
        pipeParams["fluid"] = fluidCombo->currentText();
        pipeParams["pipe_type"] = pipeTypeName;
        pipeParams["inlet_pressure_pa"] = inletPressureEdit->text().toDouble() * 1e6;
        pipeParams["inlet_temperature_k"] = inletTemperatureEdit->text().toDouble() + 273.15;
        pipeParams["mass_flow_kg_s"] = massFlowEdit->text().toDouble() / 3600;
        pipeParams["pipe_length_m"] = pipeLengthEdit->text().toDouble();
        pipeParams["pipe_od_m"] = pipeOdEdit->text().toDouble() / 1000;
        pipeParams["pipe_wall_thickness_m"] = pipeWallThicknessEdit->text().toDouble() / 1000;
        pipeParams["insulation_thickness_m"] = insulationThicknessEdit->text().toDouble() / 1000;
        pipeParams["insulation_material"] = insulationMaterialCombo->currentText();
        pipeParams["protection_material"] = protectionMaterialCombo->currentText();
        pipeParams["ambient_temperature_k"] = ambientTempEdit->text().toDouble() + 273.15;
        pipeParams["wind_speed_m_s"] = windSpeedEdit->text().toDouble();
        pipeParams["segment_length_m"] = segmentLengthEdit->text().toDouble();

        // 如果指定了干度，则使用干度
        QString qualityText = inletQualityEdit->text();
        if (!qualityText.isEmpty()) {
            pipeParams["inlet_quality"] = qualityText.toDouble();
        }

        // 转换管道元件数据
        QList<QVariant> fittingsList;
        for (const auto& fitting : fittingsData) {
            QVariantMap fittingMap;
            fittingMap["name"] = fitting.first;
            fittingMap["count"] = fitting.second;
            fittingsList.append(fittingMap);
        }
        pipeParams["fittings_data"] = fittingsList;

        // 执行分析
        FluidAnalyzer::AnalysisResult result = fluidAnalyzer->analyzePipe(pipeParams);

        // 存储结果
        currentResults = result.segmentResults;
        currentAnalysis = result.summary;

        // 显示结果
        displayResults(currentAnalysis);
        displaySegmentTable(currentResults);

        if (result.chart) {
            chartView->setChart(result.chart);
        }

        QMessageBox::information(this, "成功", "管道分析完成！");

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("分析过程中发生错误: %1").arg(e.what()));
    }
}

void MainWindow::generateReport()
{
    if (currentResults.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先进行管道分析");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(
        this, "保存分析报告", "", "Word文档 (*.docx);;所有文件 (*.*)");

    if (!filename.isEmpty()) {
        try {
            // 获取输入参数
            QVariantMap pipeParams;
            pipeParams["pipe_name"] = pipeNameEdit->text();
            pipeParams["fluid"] = fluidCombo->currentText();
            pipeParams["inlet_pressure_pa"] = inletPressureEdit->text().toDouble() * 1e6;
            pipeParams["inlet_temperature_k"] = inletTemperatureEdit->text().toDouble() + 273.15;
            pipeParams["mass_flow_kg_s"] = massFlowEdit->text().toDouble() / 3600;
            pipeParams["pipe_length_m"] = pipeLengthEdit->text().toDouble();
            pipeParams["pipe_od_m"] = pipeOdEdit->text().toDouble() / 1000;
            pipeParams["pipe_wall_thickness_m"] = pipeWallThicknessEdit->text().toDouble() / 1000;
            pipeParams["insulation_thickness_m"] = insulationThicknessEdit->text().toDouble() / 1000;
            pipeParams["insulation_material"] = insulationMaterialCombo->currentText();
            pipeParams["protection_material"] = protectionMaterialCombo->currentText();
            pipeParams["ambient_temperature_k"] = ambientTempEdit->text().toDouble() + 273.15;
            pipeParams["wind_speed_m_s"] = windSpeedEdit->text().toDouble();
            pipeParams["segment_length_m"] = segmentLengthEdit->text().toDouble();

            FluidAnalyzer::AnalysisResult result;
            result.segmentResults = currentResults;
            result.summary = currentAnalysis;

            bool success = fluidAnalyzer->generateReport(pipeParams, result, filename);

            if (success) {
                QMessageBox::information(this, "成功", QString("报告已保存至: %1").arg(filename));
            } else {
                QMessageBox::critical(this, "错误", "生成报告失败");
            }

        } catch (const std::exception& e) {
            QMessageBox::critical(this, "错误", QString("生成报告时发生错误: %1").arg(e.what()));
        }
    }
}

void MainWindow::saveData()
{
    try {
        QVariantMap data;
        data["pipe_name"] = pipeNameEdit->text();
        data["fluid"] = fluidCombo->currentText();
        data["pipe_type"] = pipeTypeCombo->currentText();
        data["inlet_pressure"] = inletPressureEdit->text();
        data["inlet_temperature"] = inletTemperatureEdit->text();
        data["inlet_quality"] = inletQualityEdit->text();
        data["mass_flow"] = massFlowEdit->text();
        data["pipe_length"] = pipeLengthEdit->text();
        data["pipe_od"] = pipeOdEdit->text();
        data["pipe_wall_thickness"] = pipeWallThicknessEdit->text();
        data["segment_length"] = segmentLengthEdit->text();
        data["insulation_material"] = insulationMaterialCombo->currentText();
        data["insulation_thickness"] = insulationThicknessEdit->text();
        data["protection_material"] = protectionMaterialCombo->currentText();
        data["ambient_temperature"] = ambientTempEdit->text();
        data["wind_speed"] = windSpeedEdit->text();

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

void MainWindow::loadData()
{
    try {
        QString filename = QFileDialog::getOpenFileName(
            this, "读取数据", "", "Pipe文件 (*.pipe);;所有文件 (*.*)");

        if (!filename.isEmpty()) {
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly)) {
                QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                QVariantMap data = doc.object().toVariantMap();

                // 恢复数据到界面
                pipeNameEdit->setText(data["pipe_name"].toString());
                fluidCombo->setCurrentText(data["fluid"].toString());
                pipeTypeCombo->setCurrentText(data["pipe_type"].toString());
                inletPressureEdit->setText(data["inlet_pressure"].toString());
                inletTemperatureEdit->setText(data["inlet_temperature"].toString());
                inletQualityEdit->setText(data["inlet_quality"].toString());
                massFlowEdit->setText(data["mass_flow"].toString());
                pipeLengthEdit->setText(data["pipe_length"].toString());
                pipeOdEdit->setText(data["pipe_od"].toString());
                pipeWallThicknessEdit->setText(data["pipe_wall_thickness"].toString());
                segmentLengthEdit->setText(data["segment_length"].toString());
                insulationMaterialCombo->setCurrentText(data["insulation_material"].toString());
                insulationThicknessEdit->setText(data["insulation_thickness"].toString());
                protectionMaterialCombo->setCurrentText(data["protection_material"].toString());
                ambientTempEdit->setText(data["ambient_temperature"].toString());
                windSpeedEdit->setText(data["wind_speed"].toString());

                // 恢复管道元件数据
                fittingsData.clear();
                QJsonArray fittingsArray = data["fittings_data"].toJsonArray();
                for (const QJsonValue& value : fittingsArray) {
                    QJsonObject obj = value.toObject();
                    fittingsData.append(qMakePair(obj["name"].toString(), obj["count"].toInt()));
                }
                loadFittingsToTable();

                QMessageBox::information(this, "成功", QString("数据已从 %1 加载").arg(filename));
            }
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("读取数据失败: %1").arg(e.what()));
    }
}

void MainWindow::openMaterialDialog(const QString& type)
{
    MaterialDialog dialog(this, materialManager, type);
    dialog.exec();
    refreshMaterialCombos();
}

void MainWindow::addFitting()
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

void MainWindow::removeFitting()
{
    int row = fittingsTable->currentRow();
    if (row >= 0 && row < fittingsData.size()) {
        fittingsData.removeAt(row);
        loadFittingsToTable();
    } else {
        QMessageBox::warning(this, "警告", "请选择要删除的管道元件");
    }
}

void MainWindow::refreshMaterialCombos()
{/*
    // 更新保温材料下拉列表
    insulationMaterialCombo->clear();
    insulationMaterialCombo->addItems(materialManager->getInsulationMaterials().keys());
    if (insulationMaterialCombo->count() > 0) {
        insulationMaterialCombo->setCurrentIndex(0);
    }

    // 更新外保护层下拉列表
    protectionMaterialCombo->clear();
    protectionMaterialCombo->addItems(materialManager->getProtectionMaterials().keys());
    if (protectionMaterialCombo->count() > 0) {
        protectionMaterialCombo->setCurrentIndex(0);
    }

    // 更新管道类型下拉列表
    pipeTypeCombo->clear();
    pipeTypeCombo->addItems(materialManager->getPipeTypes().keys());
    if (pipeTypeCombo->count() > 0) {
        pipeTypeCombo->setCurrentIndex(0);
    }*/
    qDebug() << "refreshMaterialCombos() 开始";

    try {
        qDebug() << "步骤1: 检查 insulationMaterialCombo 指针...";
        if (!insulationMaterialCombo) {
            qCritical() << "insulationMaterialCombo 指针为空!";
            return;
        }

        qDebug() << "步骤2: 更新保温材料下拉列表...";
        insulationMaterialCombo->clear();
        qDebug() << "保温材料下拉列表清空完成";

        qDebug() << "步骤3: 获取保温材料数据...";
        auto insulationMaterials = materialManager->getInsulationMaterials();
        qDebug() << "保温材料数据获取完成，数量:" << insulationMaterials.size();

        qDebug() << "步骤4: 添加保温材料到下拉列表...";
        insulationMaterialCombo->addItems(insulationMaterials.keys());
        qDebug() << "保温材料添加到下拉列表完成";

        if (insulationMaterialCombo->count() > 0) {
            insulationMaterialCombo->setCurrentIndex(0);
            qDebug() << "保温材料默认项设置完成";
        }

        qDebug() << "步骤5: 检查 protectionMaterialCombo 指针...";
        if (!protectionMaterialCombo) {
            qCritical() << "protectionMaterialCombo 指针为空!";
            return;
        }

        qDebug() << "步骤6: 更新外保护层下拉列表...";
        protectionMaterialCombo->clear();
        qDebug() << "外保护层下拉列表清空完成";

        qDebug() << "步骤7: 获取外保护层数据...";
        auto protectionMaterials = materialManager->getProtectionMaterials();
        qDebug() << "外保护层数据获取完成，数量:" << protectionMaterials.size();

        qDebug() << "步骤8: 添加外保护层到下拉列表...";
        protectionMaterialCombo->addItems(protectionMaterials.keys());
        qDebug() << "外保护层添加到下拉列表完成";

        if (protectionMaterialCombo->count() > 0) {
            protectionMaterialCombo->setCurrentIndex(0);
            qDebug() << "外保护层默认项设置完成";
        }

        qDebug() << "步骤9: 检查 pipeTypeCombo 指针...";
        if (!pipeTypeCombo) {
            qCritical() << "pipeTypeCombo 指针为空!";
            return;
        }

        qDebug() << "步骤10: 更新管道类型下拉列表...";
        pipeTypeCombo->clear();
        qDebug() << "管道类型下拉列表清空完成";

        qDebug() << "步骤11: 获取管道类型数据...";
        auto pipeTypes = materialManager->getPipeTypes();
        qDebug() << "管道类型数据获取完成，数量:" << pipeTypes.size();

        qDebug() << "步骤12: 添加管道类型到下拉列表...";
        pipeTypeCombo->addItems(pipeTypes.keys());
        qDebug() << "管道类型添加到下拉列表完成";

        if (pipeTypeCombo->count() > 0) {
            pipeTypeCombo->setCurrentIndex(0);
            qDebug() << "管道类型默认项设置完成";
        }

    } catch (const std::exception& e) {
        qCritical() << "refreshMaterialCombos 中捕获异常:" << e.what();
        throw;
    } catch (...) {
        qCritical() << "refreshMaterialCombos 中捕获未知异常";
        throw;
    }

    qDebug() << "refreshMaterialCombos() 完成";
}

void MainWindow::loadFittingsToTable()
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

void MainWindow::displayResults(const QVariantMap& analysisResults)
{
    resultsText->clear();

    QString resultText;
    resultText += "管道分析结果\n";
    resultText += QString(80, QChar('=')) + "\n\n";

    resultText += "入口参数:\n";
    resultText += QString("  压力: %1 MPa\n").arg(analysisResults["inlet_pressure_pa"].toDouble() / 1e6, 0, 'f', 6);
    resultText += QString("  温度: %1 °C\n").arg(analysisResults["inlet_temperature_k"].toDouble() - 273.15, 0, 'f', 2);
    resultText += QString("  干度: %1\n").arg(analysisResults["inlet_quality"].toDouble(), 0, 'f', 4);
    resultText += QString("  流速: %1 m/s\n\n").arg(analysisResults["inlet_velocity_m_s"].toDouble(), 0, 'f', 2);

    resultText += "出口参数:\n";
    resultText += QString("  压力: %1 MPa\n").arg(analysisResults["outlet_pressure_pa"].toDouble() / 1e6, 0, 'f', 6);
    resultText += QString("  温度: %1 °C\n").arg(analysisResults["outlet_temperature_k"].toDouble() - 273.15, 0, 'f', 2);
    resultText += QString("  干度: %1\n").arg(analysisResults["outlet_quality"].toDouble(), 0, 'f', 4);
    resultText += QString("  流速: %1 m/s\n\n").arg(analysisResults["outlet_velocity_m_s"].toDouble(), 0, 'f', 2);

    resultText += "压力损失分析:\n";
    resultText += QString("  总压力降: %1 MPa\n").arg(analysisResults["total_pressure_drop_pa"].toDouble() / 1e6, 0, 'f', 4);
    resultText += QString("  单位压降: %1 kPa/km\n").arg(analysisResults["pressure_drop_pa_m"].toDouble() / 1000, 0, 'f', 1);
    resultText += QString("  压降比例: %1\n").arg(analysisResults["pressure_ratio"].toDouble(), 0, 'f', 3);
    resultText += QString("  沿程阻力: %1 MPa\n").arg(analysisResults["total_friction_drop_pa"].toDouble() / 1e6, 0, 'f', 6);
    resultText += QString("  局部阻力: %1 MPa\n").arg(analysisResults["total_fittings_drop_pa"].toDouble() / 1e6, 0, 'f', 6);
    resultText += QString("  管道粗糙度: %1 m\n\n").arg(analysisResults["pipe_roughness_m"].toDouble(), 0, 'f', 6);

    resultText += "热损失分析:\n";
    resultText += QString("  总热损失: %1 kW\n").arg(analysisResults["total_heat_loss_w"].toDouble() / 1000, 0, 'f', 1);
    resultText += QString("  平均单位热损失: %1 W/m\n").arg(analysisResults["avg_heat_loss_per_m_w"].toDouble(), 0, 'f', 1);
    resultText += QString("  平均单位面积热损失: %1 W/m²\n").arg(analysisResults["avg_heat_loss_per_area_w"].toDouble(), 0, 'f', 1);
    resultText += QString("  最大表面温度: %1 °C\n").arg(analysisResults["max_surface_temp_k"].toDouble() - 273.15, 0, 'f', 2);
    resultText += QString("  平均表面温度: %1 °C\n\n").arg(analysisResults["avg_surface_temp_k"].toDouble() - 273.15, 0, 'f', 2);

    resultText += "流动特性:\n";
    resultText += QString("  平均流速: %1 m/s\n").arg(analysisResults["avg_velocity_m_s"].toDouble(), 0, 'f', 2);
    resultText += QString("  最大流速: %1 m/s\n").arg(analysisResults["max_velocity_m_s"].toDouble(), 0, 'f', 2);
    resultText += QString("  最小流速: %1 m/s\n").arg(analysisResults["min_velocity_m_s"].toDouble(), 0, 'f', 2);
    resultText += QString("  平均雷诺数: %1\n").arg(analysisResults["avg_reynolds"].toDouble(), 0, 'f', 0);
    resultText += QString("  平均摩擦系数: %1\n").arg(analysisResults["avg_friction"].toDouble(), 0, 'f', 5);

    resultsText->setPlainText(resultText);
}

void MainWindow::displaySegmentTable(const QList<QVariantMap>& results)
{
    segmentTable->setRowCount(results.size());

    for (int i = 0; i < results.size(); ++i) {
        const auto& row = results[i];

        segmentTable->setItem(i, 0, new QTableWidgetItem(QString::number(row["segment"].toInt())));
        segmentTable->setItem(i, 1, new QTableWidgetItem(QString::number(row["distance_m"].toDouble(), 'f', 0)));
        segmentTable->setItem(i, 2, new QTableWidgetItem(QString::number(row["pressure_pa"].toDouble() / 1e6, 'f', 6)));
        segmentTable->setItem(i, 3, new QTableWidgetItem(QString::number(row["temperature_k"].toDouble() - 273.15, 'f', 2)));
        segmentTable->setItem(i, 4, new QTableWidgetItem(QString::number(row["quality"].toDouble(), 'f', 4)));
        segmentTable->setItem(i, 5, new QTableWidgetItem(QString::number(row["velocity_m_s"].toDouble(), 'f', 2)));
        segmentTable->setItem(i, 6, new QTableWidgetItem(QString::number(row["heat_loss_per_m_w"].toDouble(), 'f', 1)));
        segmentTable->setItem(i, 7, new QTableWidgetItem(QString::number(row["heat_loss_per_area_w"].toDouble(), 'f', 1)));
        segmentTable->setItem(i, 8, new QTableWidgetItem(QString::number(row["surface_temp_k"].toDouble() - 273.15, 'f', 1)));
    }
}

void MainWindow::clearInput()
{
    pipeNameEdit->clear();
    inletPressureEdit->clear();
    inletTemperatureEdit->clear();
    inletQualityEdit->clear();
    massFlowEdit->clear();
    pipeLengthEdit->clear();
    pipeOdEdit->clear();
    pipeWallThicknessEdit->clear();
    insulationThicknessEdit->clear();
    ambientTempEdit->clear();
    windSpeedEdit->clear();
    segmentLengthEdit->clear();

    // 清空管道元件
    fittingsData.clear();
    loadFittingsToTable();

    // 清空结果
    resultsText->clear();
    segmentTable->setRowCount(0);
    chartView->setChart(nullptr);

    currentResults.clear();
    currentAnalysis.clear();
}
