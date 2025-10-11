#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

// 仅包含头文件并使用extern "C"，不重复声明函数
extern "C" {
#include "CoolPropLib.h"
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCalculateClicked(); // 计算按钮点击事件

private:
    // UI组件
    QLineEdit* fluidEdit;     // 流体名称输入
    QLineEdit* pressureEdit;  // 压力输入（Pa）
    QLineEdit* temperatureEdit;// 温度输入（K）
    QPushButton* calcBtn;     // 计算按钮
    QLabel* resultLabel;      // 结果显示
    QLabel* versionLabel;     // CoolProp版本显示
};

#endif // MAINWINDOW_H
