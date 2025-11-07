#ifndef SAFETYVALVE_H
#define SAFETYVALVE_H
#include "tymodule.h"
#include <QWidget>
#include <QDialog>

class QPushButton;
class QLineEdit;
class QRadioButton;
class QButtonGroup;
class QAbstractButton;

class SafetyValve : public TYModule
{
    Q_OBJECT
public:
    explicit SafetyValve(QWidget *parent = nullptr);
    ~SafetyValve();
    void open(QVariantMap data);
    QVariantMap save();
    QString run();

private:
    QString fluid;
    int overpressureCondition;
    QRadioButton *fireConditionRadio;
    QRadioButton *tubeRuptureConditionRadio;
    QRadioButton *thermalVaporizationRadio;
    QRadioButton *gasInflowRadio;
    QButtonGroup *conditionButtonGroup;
    QLineEdit *releasePressure;
    QLineEdit *backPressure;
    QLineEdit *releaseCoefficient;
    void setupUi();

private slots:
    void onfluidButtonClicked();
    void onConditionButtonClicked(QAbstractButton *button);
};

class FireConditionDialog : public QDialog
{
    Q_OBJECT
public:
    FireConditionDialog(QWidget *parent = nullptr);
    ~FireConditionDialog();

private:
    QLineEdit *heatingArea;
    QLineEdit *f;
};

#endif // SAFETYVALVE_H
