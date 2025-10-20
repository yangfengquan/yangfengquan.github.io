#ifndef PIPEFLOW_H
#define PIPEFLOW_H

class QWidget;
class QComboBox;
class QLineEdit;
class  QTableWidget;
class QPushButton;

class PipeFlow
{
public:
    PipeFlow();
    void setupUi(QWidget *parentWidget);
    void run();

private:
    QComboBox *fluidCombo;
    QComboBox *flowRateCombo;
    QLineEdit *flowRateEdit;
    QLineEdit *inletPressureEdit;
    QComboBox *inletArg2Combo;
    QLineEdit *inletArg2Edit;
    QComboBox *pipeTypeCombo;
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

    QPushButton *addFittingButton;
    QPushButton *removeFittingButton;
};

#endif // PIPEFLOW_H
