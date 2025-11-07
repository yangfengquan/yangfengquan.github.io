#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QWidget>

class QLineEdit;
class QTableWidget;
class QPushButton;
class QComboBox;
class QLabel;
class QTextEdit;

class Properties : public QWidget
{
    Q_OBJECT
public:
    explicit Properties(QWidget *parent = nullptr);

    void setupUi(QWidget *parent);
    QString run();
    QVariantMap save();
    void open(QVariantMap data);

private:
    void initializePropertiesMap();
    void setupMainWindow();
    void setupComponentSection();
    void setupParameterSection();
    void populateComponentList();
    QString createMixtureString() const;
    double convertToSI(const QString &parameter, double value) const;
    QString calculateProperties();

    QLineEdit *m_componentSearchEdit;
    QTableWidget *m_componentTable;
    QTableWidget *m_selectedComponentTable;

    QComboBox *m_param1Combo;
    QComboBox *m_param2Combo;
    QLineEdit *m_param1ValueEdit;
    QLineEdit *m_param2ValueEdit;
    QLabel *m_param1UnitLabel;
    QLabel *m_param2UnitLabel;

    QPushButton *m_addButton;
    QPushButton *m_removeButton;

    QMap<QString, QPair<QString, QString>> m_properties;
    QMap<QString, QString> m_parameterUnits;

private slots:
    void onComponentSearchTextChanged(const QString &text);
    void onAddComponentClicked();
    void onRemoveComponentClicked();
    void onParameter1Changed(int index);
    void onParameter2Changed(int index);

    // 报告对齐
    int calculateDisplayWidth(const QString &text) const;
    QString alignMixedText(const QString &text, int targetWidth) const;
    QString formatPropertyLine(const QString &name, const QString &value, const QString &unit, int maxNameWidth) const;
};

#endif // PROPERTIES_H
