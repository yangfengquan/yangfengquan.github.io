#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class PipeFlowModule;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString currentModule;
    QString filename;
    PipeFlowModule *pipeFlow;
    void setupMenu();
    void setupUi(const QString& module = QString());

private slots:
    void save();
    void saveAs();
    void run();
    void open(const QString& filepath = QString());
    void reportFile(QString& content);
    void openMaterialDialog(const QString& type);
    void openActivationDialog();
};
#endif // MAINWINDOW_H
