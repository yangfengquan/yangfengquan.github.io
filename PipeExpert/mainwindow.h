#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    QString module;
    PipeFlow *pipeFlow;

private slots:
    void save();
    void open();
    void run();
    void reportFile(QString& content);
    void openMaterialDialog(const QString& type);
    void openActivationDialog();
};
#endif // MAINWINDOW_H
