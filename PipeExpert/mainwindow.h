#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "activationmanager.h"

class PipeFlow;
class Properties;
class SafetyValve;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &filePath = QString(), QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupMenu();
    void setupUi(const QString& module = QString());
    QString m_currentModule;
    QString filename;
    PipeFlow* pipeFlow;
    Properties* props;
    SafetyValve* safetyValve;
    ActivationManager* activationManager;   

private slots:
    void save();
    void saveAs();
    void run();
    void open(const QString& filepath = QString());
    void reportFile(QString& content);
    void openMaterialDialog(const QString& type);
    void openActivationDialog();
    void checkSoftwareStatus();
};
#endif // MAINWINDOW_H
