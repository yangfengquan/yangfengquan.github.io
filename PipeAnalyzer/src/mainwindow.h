#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QChartView>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QTableWidget;
class QTextEdit;
class QChartView;
QT_END_NAMESPACE

class MaterialManager;
class FluidAnalyzer;
class ActivationManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void analyzePipe();
    void generateReport();
    void saveData();
    void loadData();
    void openMaterialDialog(const QString& type);
    void addFitting();
    void removeFitting();
    void refreshMaterialCombos();
    void displayResults(const QVariantMap& analysisResults);
    void displaySegmentTable(const QList<QVariantMap>& results);


private:
    void setupUI();
    void createMenu();
    void setupInputTab();
    void setupFittingsTab();
    void setupResultsTab();
    void checkSoftwareStatus();
    void showActivationInterface();
    void clearInput();
    void loadFittingsToTable();
    // UI组件
    QTabWidget *mainTabWidget;
    QTabWidget *resultsTabWidget;

    // 输入控件
    QLineEdit *pipeNameEdit;
    QComboBox *fluidCombo;
    QComboBox *pipeTypeCombo;
    QLineEdit *inletPressureEdit;
    QLineEdit *inletTemperatureEdit;
    QLineEdit *inletQualityEdit;
    QLineEdit *massFlowEdit;
    QLineEdit *pipeLengthEdit;
    QLineEdit *pipeOdEdit;
    QLineEdit *pipeWallThicknessEdit;
    QLineEdit *segmentLengthEdit;
    QComboBox *insulationMaterialCombo;
    QLineEdit *insulationThicknessEdit;
    QComboBox *protectionMaterialCombo;
    QLineEdit *ambientTempEdit;
    QLineEdit *windSpeedEdit;

    // 结果控件
    QTextEdit *resultsText;
    QTableWidget *segmentTable;
    QChartView *chartView;

    // 管道元件
    QTableWidget *fittingsTable;
    QList<QPair<QString, int>> fittingsData;

    // 管理器
    MaterialManager *materialManager;
    FluidAnalyzer *fluidAnalyzer;
    ActivationManager *activationManager;

    QList<QVariantMap> currentResults;
    QVariantMap currentAnalysis;
};

#endif // MAINWINDOW_H
