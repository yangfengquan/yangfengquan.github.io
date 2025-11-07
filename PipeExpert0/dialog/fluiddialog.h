#ifndef FLUIDDIALOG_H
#define FLUIDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTableWidget>

class FluidDialog : public QDialog
{
    Q_OBJECT

public:
    FluidDialog(QWidget *parent = nullptr);
    QString getFluidString() const;

private:
    QLineEdit *m_componentSearchEdit;
    QTableWidget *m_componentTable;
    QTableWidget *m_selectedComponentTable;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;

    void populateComponentList();

private slots:
    void onComponentSearchTextChanged(const QString &text);
    void onAddComponentClicked();
    void onRemoveComponentClicked();
};

#endif // FLUIDDIALOG_H
