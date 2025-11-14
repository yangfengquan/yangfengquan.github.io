#ifndef PIPEFLOWMODULE_H
#define PIPEFLOWMODULE_H

#include <QWidget>
#include "TYModule.h"

class QComboBox;
class QLineEdit;
class QTableWidget;
class MaterialManager;

class PipeFlowModule : public TYModule
{
    Q_OBJECT
public:
    PipeFlowModule(QWidget *parent = nullptr);
    ~PipeFlowModule();
    void open(const std::map<std::string, std::any>& data) override;
    std::map<std::string, std::any> data() override;
    QString calculate() override;

private:
    QString fluid = QString();
    std::map<std::string, int> fittingsData;
    MaterialManager *materialManager;
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
    QComboBox *cladMaterialCombo;
    QLineEdit *ambientTempEdit;
    QLineEdit *windSpeedEdit;

    void setupUi() override;
    void loadFittingsToTable();

private slots:
    void openFluidDialog();
    void addFitting();
    void removeFitting();
    void populateMaterialCombos();
};

#endif // PIPEFLOWMODULE_H
