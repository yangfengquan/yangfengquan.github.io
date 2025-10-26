#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "activationmanager.h"

class PipeFlow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupMenu();
    void setupUi(const QString& module);
    QString m_currentModule;
    PipeFlow* pipeFlow;
    ActivationManager* activationManager;

private slots:
    void save();
    void open();
    void run();
    void reportFile(QString& content);
    void openMaterialDialog(const QString& type);
    void openActivationDialog();
    void checkSoftwareStatus();
};
#endif // MAINWINDOW_H
