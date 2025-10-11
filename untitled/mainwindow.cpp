#include "mainwindow.h"
#include <QDoubleValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 1. 窗口基础配置
    setWindowTitle("CoolProp Qt Demo");
    setFixedSize(450, 350);

    // 2. 中心部件与布局（垂直布局）
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    setCentralWidget(centralWidget);

    // 3. 显示CoolProp版本（与Visual Studio测试一致，调用get_global_param_string）
    versionLabel = new QLabel(this);
    char versionBuf[200];
    get_global_param_string("version", versionBuf, sizeof(versionBuf));
    versionLabel->setText(QString("CoolProp Version: %1").arg(versionBuf));
    mainLayout->addWidget(versionLabel);

    // 4. 流体名称输入行
    QHBoxLayout* fluidLayout = new QHBoxLayout();
    fluidLayout->addWidget(new QLabel("流体名称:"));
    fluidEdit = new QLineEdit("Water", this); // 默认水
    fluidLayout->addWidget(fluidEdit);
    mainLayout->addLayout(fluidLayout);

    // 5. 压力输入行（仅允许数字，默认1atm=101325Pa）
    QHBoxLayout* pressureLayout = new QHBoxLayout();
    pressureLayout->addWidget(new QLabel("压力 (Pa):"));
    pressureEdit = new QLineEdit("101325", this);
    pressureEdit->setValidator(new QDoubleValidator(0, 1e9, 2, this)); // 范围0~1e9Pa，2位小数
    pressureLayout->addWidget(pressureEdit);
    mainLayout->addLayout(pressureLayout);

    // 6. 温度输入行（默认300K）
    QHBoxLayout* tempLayout = new QHBoxLayout();
    tempLayout->addWidget(new QLabel("温度 (K):"));
    temperatureEdit = new QLineEdit("300", this);
    temperatureEdit->setValidator(new QDoubleValidator(200, 1000, 2, this)); // 范围200~1000K
    tempLayout->addWidget(temperatureEdit);
    mainLayout->addLayout(tempLayout);

    // 7. 计算按钮
    calcBtn = new QPushButton("计算密度 (kg/m³)", this);
    connect(calcBtn, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
    mainLayout->addWidget(calcBtn);

    // 8. 结果显示
    resultLabel = new QLabel("结果将显示在这里", this);
    mainLayout->addWidget(resultLabel);
}

MainWindow::~MainWindow()
{
    // Qt自动释放UI组件，无需手动delete
}

// 计算按钮点击事件：调用PropsSI计算密度（与Visual Studio测试一致）
void MainWindow::onCalculateClicked()
{
    // 1. 获取输入值并验证
    QString fluid = fluidEdit->text().trimmed();
    bool okP, okT;
    double pressure = pressureEdit->text().toDouble(&okP);
    double temperature = temperatureEdit->text().toDouble(&okT);

    if (fluid.isEmpty() || !okP || !okT) {
        QMessageBox::warning(this, "输入错误", "请填写有效的流体名称、压力和温度！");
        return;
    }

    // 2. 调用CoolProp的PropsSI计算密度（D=密度，P=压力，T=温度）
    double density = PropsSI("D", "P", pressure, "T", temperature, fluid.toUtf8().constData());

    // 3. 显示结果（保留6位小数）
    resultLabel->setText(QString("计算结果：%1 kg/m³").arg(density, 0, 'f', 6));
}
