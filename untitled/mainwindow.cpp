#include "mainwindow.h"
extern "C" {
#include "CoolPropLib.h"
}
#include <QDebug>
#include <QMessageBox>
#include <QString>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 计算水的物性示例
    double T = 101.0 + 273.15;  // 温度 300K
    double P = 101325; // 压力 101.325 kPa

    // 计算密度
    double density = PropsSI("D", "T", T, "P", P, "Water");
    QString message = QString("水在 %1 K, %2 Pa 下的密度: %3 kg/m³")
                          .arg(T, 0, 'f', 1)
                          .arg(P, 0, 'f', 0)
                          .arg(density, 0, 'f', 4);

    QMessageBox::information(nullptr, "计算结果", message);

}

MainWindow::~MainWindow() {}
