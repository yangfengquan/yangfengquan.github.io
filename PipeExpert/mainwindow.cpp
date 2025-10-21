#include "mainwindow.h"
#include "dialog/materialdialog.h"
#include "pipeflow.h"
#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("汤圆工具集");
    setMinimumSize(940, 600);
    setupMenu();
}

MainWindow::~MainWindow() {}

void MainWindow::setupMenu()
{
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

    QMenu *moduleMenu = menuBar()->addMenu("功能");

    QAction *pipeFlowAction = new QAction("管道阻力和绝热", this);

    moduleMenu->addAction(pipeFlowAction);

    connect(pipeFlowAction, &QAction::triggered, this, [this]() {setupUi("pipeFlow");});

    QMenu *runMenu = menuBar()->addMenu("运行");

    QAction *runAction = new QAction("运行", this);

    runMenu->addAction(runAction);

    connect(runAction, &QAction::triggered, this, &MainWindow::run);

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

    connect(pipeTypesAction, &QAction::triggered, this, [this]() { openMaterialDialog("pipeType"); });
    connect(fittingsAction, &QAction::triggered, this, [this]() { openMaterialDialog("fitting"); });
    connect(insulationAction, &QAction::triggered, this, [this]() { openMaterialDialog("insulation"); });
    connect(protectionAction, &QAction::triggered, this, [this]() { openMaterialDialog("protection"); });

    // 帮助菜单
    QMenu *helpMenu = menuBar()->addMenu("帮助");

    QAction *activationAction = new QAction("激活", this);
    QAction *aboutAction = new QAction("关于", this);

    helpMenu->addAction(activationAction);
    helpMenu->addAction(aboutAction);

    connect(activationAction, &QAction::triggered, this, &MainWindow::openActivationDialog);
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "关于", "管道分析软件\n版本：v1.0\n作者：杨奉全\nQQ群：816103114");
    });
}

void MainWindow::setupUi(const QString& module)
{
    this->module = module;
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    if(this->module == "pipeFlow")
    {
        pipeFlow = new PipeFlow();
        pipeFlow->setupUi(centralWidget);
    }

}

void MainWindow::save()
{
    if (this->module.isEmpty()){
        QMessageBox::warning(this, "警告", "请先在菜单栏中选择功能。");
        return;
    }

    QVariantMap data;

    if (this->module == "pipeFlow"){
        data = pipeFlow->save();
    }

    try {
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

void MainWindow::open()
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
                if (data["module"] == "pipeFlow") {
                    setupUi("pipeFlow");
                    pipeFlow->open(data);
                }
            }
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("读取数据失败: %1").arg(e.what()));
    }
}

void MainWindow::run()
{
    if (this->module.isEmpty()){
        QMessageBox::warning(this, "警告", "请先在菜单栏中选择功能。");
    } else if (this->module == "pipeFlow"){
        pipeFlow->run();
    }
}

void MainWindow::openMaterialDialog(const QString& type)
{
    MaterialDialog *dlg = new MaterialDialog(type, this);
    if (this->module == "pipeFlow"){
        connect(dlg, &MaterialDialog::materialDataChanged, pipeFlow, &PipeFlow::refreshMaterialCombos);
    }
    dlg->exec();
}

void MainWindow::openActivationDialog()
{

}
