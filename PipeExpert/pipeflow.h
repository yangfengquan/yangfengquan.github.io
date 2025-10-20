#ifndef PIPEFLOW_H
#define PIPEFLOW_H

#include <QWidget>
#include <QPair>
#include <QList>
class QWidget;
class QComboBox;
class QLineEdit;
class  QTableWidget;
class QPushButton;
class QString;
class MaterialManager;

class PipeFlow : public QWidget
{
public:
    PipeFlow(QWidget *parent = nullptr);
    ~PipeFlow();
    void setupUi(QWidget *parent);
    void run();
    void save();

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

    QList<QPair<QString, int>> fittingsData;

    QPushButton *addFittingButton;
    QPushButton *removeFittingButton;
    MaterialManager *materialManager;

    void loadFittingsToTable();

private slots:
    void addFitting();
    void removeFitting();
};

#endif // PIPEFLOW_H
