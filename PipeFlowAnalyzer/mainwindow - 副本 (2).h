#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void createMenu();

    QComboBox *fluidCombo;
    QComboBox *pipeTypeCombo;
    QComboBox *flowRateCombo;
    QLineEdit *flowRateEdit;
    QLineEdit *inletPressureEdit;
    QComboBox *inletArg2Combo;
    QLineEdit *inletArg2Edit;
    QLineEdit *lengthEdit;
    QLineEdit *pipeOdEdit;
    QLineEdit *pipeWallThicknessEdit;
    QLineEdit *segmentLengthEdit;
    QTableWidget *fittingsTable;
    QComboBox *insulationMaterialCombo;
    QLineEdit *insulationThicknessEdit;
    QComboBox *protectionMaterialCombo;
    QLineEdit *ambientTempEdit;
    QLineEdit *windSpeedEdit;

private slots:
    void save();
    void open();
    void openMaterialDialog(const QString& type);
    void openActivationDialog();
};
#endif // MAINWINDOW_H
