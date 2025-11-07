#include "fluiddialog.h"
#include "common.h"
extern "C" {
    #include "CoolPropLib.h"
}
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

FluidDialog::FluidDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("设置流体组分");
    QGridLayout *layout = new QGridLayout(this);
    layout->setColumnMinimumWidth(0, 500);
    layout->setColumnMinimumWidth(2, 260);

    int row = 0;

    m_componentSearchEdit = new QLineEdit(this);
    m_componentSearchEdit->setPlaceholderText("输入物质名称、化学式或CAS号...");
    connect(m_componentSearchEdit, &QLineEdit::textChanged,
            this, &FluidDialog::onComponentSearchTextChanged);

    layout->addWidget(m_componentSearchEdit, row, 0);
    layout->addWidget(new QLabel("已选组分:", this), row, 2);

    ++row;

    // Available components table
    m_componentTable = new QTableWidget(this);
    m_componentTable->setColumnCount(4);
    m_componentTable->setHorizontalHeaderLabels({"中文名称","英文名称", "化学式", "CAS号"});
    m_componentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_componentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_componentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_componentTable->setMaximumHeight(200);
    m_componentTable->setColumnWidth(0, 120);
    m_componentTable->setColumnWidth(1, 120);
    m_componentTable->setColumnWidth(2, 100);

    layout->addWidget(m_componentTable, row, 0);

    // Buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    m_addButton = new QPushButton("添加 >>", this);
    m_removeButton = new QPushButton("<< 删除", this);

    connect(m_addButton, &QPushButton::clicked, this, &FluidDialog::onAddComponentClicked);
    connect(m_removeButton, &QPushButton::clicked, this, &FluidDialog::onRemoveComponentClicked);

    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_removeButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout, row, 1);

    // Selected components table
    m_selectedComponentTable = new QTableWidget(this);
    m_selectedComponentTable->setColumnCount(2);
    m_selectedComponentTable->setHorizontalHeaderLabels({"名称", "摩尔分数"});
    m_selectedComponentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_selectedComponentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_selectedComponentTable->setMaximumHeight(200);
    m_selectedComponentTable->setColumnWidth(0, 120);
    m_selectedComponentTable->setColumnWidth(1, 120);

    layout->addWidget(m_selectedComponentTable, row, 2);

     ++row;
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                          QDialogButtonBox::Cancel);

    // 连接信号槽
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(buttonBox, row, 0, 1, 3);

    populateComponentList();
}

void FluidDialog::populateComponentList()
{
    m_componentTable->setRowCount(0);

    // 获取CoolProp支持的所有流体
    char buffer[4096]; // 足够大的缓冲区
    long return_val = get_global_param_string("fluids_list", buffer, sizeof(buffer));
    //qDebug()<<buffer;
    if (return_val == 1) { // 成功
        QString fluidsList = QString::fromUtf8(buffer);
        QStringList fluids = fluidsList.split(',');

        m_componentTable->setRowCount(fluids.size());

        for (int i = 0; i < fluids.size(); ++i) {
            const QString &fluid = fluids[i];

            QString fluidzh;

            // 如果有中文映射则使用中文，否则使用英文原名
            if (fluidMap.contains(fluid)) {
                fluidzh = fluidMap[fluid];
            } else {
                fluidzh = fluid;
            }

            char formulaBuffer[256];
            get_fluid_param_string(fluid.toUtf8().constData(), "formula", formulaBuffer, sizeof(formulaBuffer));

            char casBuffer[256];
            get_fluid_param_string(fluid.toUtf8().constData(), "CAS", casBuffer, sizeof(casBuffer));

            QString formulaStr = QString::fromUtf8(formulaBuffer).remove(QRegularExpression("[_{}1]"));
            QString casStr = QString::fromUtf8(casBuffer);

            m_componentTable->setItem(i, 0, new QTableWidgetItem(fluidzh));
            m_componentTable->setItem(i, 1, new QTableWidgetItem(fluid));
            m_componentTable->setItem(i, 2, new QTableWidgetItem(formulaStr));
            m_componentTable->setItem(i, 3, new QTableWidgetItem(casStr));
        }
    }
}

void FluidDialog::onComponentSearchTextChanged(const QString &text)
{
    if (text.isEmpty()) {
        for (int i = 0; i < m_componentTable->rowCount(); ++i) {
            m_componentTable->setRowHidden(i, false);
        }
        return;
    }

    QString searchText = text.toLower().remove(' ');

    for (int i = 0; i < m_componentTable->rowCount(); ++i) {
        QString itemText = m_componentTable->item(i, 0)->text() +
                           m_componentTable->item(i, 1)->text() +
                           m_componentTable->item(i, 2)->text();
        m_componentTable->setRowHidden(i, !itemText.toLower().contains(searchText));
    }
}

void FluidDialog::onAddComponentClicked()
{
    int currentRow = m_componentTable->currentRow();
    if (currentRow >= 0) {
        QString componentName = m_componentTable->item(currentRow, 1)->text();

        // Check if already added
        for (int i = 0; i < m_selectedComponentTable->rowCount(); ++i) {
            if (m_selectedComponentTable->item(i, 0)->text() == componentName) {
                QMessageBox::information(this, "提示", "该组分已添加");
                return;
            }
        }

        int newRow = m_selectedComponentTable->rowCount();
        m_selectedComponentTable->insertRow(newRow);
        m_selectedComponentTable->setItem(newRow, 0, new QTableWidgetItem(componentName));

        QTableWidgetItem *fractionItem = new QTableWidgetItem("1.0");
        fractionItem->setFlags(fractionItem->flags() | Qt::ItemIsEditable);
        m_selectedComponentTable->setItem(newRow, 1, fractionItem);
    }
}

void FluidDialog::onRemoveComponentClicked()
{
    int currentRow = m_selectedComponentTable->currentRow();
    if (currentRow >= 0) {
        m_selectedComponentTable->removeRow(currentRow);
    }
}

QString FluidDialog::getFluidString() const
{
    if (m_selectedComponentTable->rowCount() == 0) {
        QMessageBox::warning(const_cast<FluidDialog*>(this), "错误", "请至少选择一个组分");
        return "";
    }

    if (m_selectedComponentTable->rowCount() == 1) {
        return m_selectedComponentTable->item(0, 0)->text();
    }

    QStringList components;
    double totalFraction = 0.0;

    for (int i = 0; i < m_selectedComponentTable->rowCount(); ++i) {
        QString name = m_selectedComponentTable->item(i, 0)->text();
        bool ok;
        double fraction = m_selectedComponentTable->item(i, 1)->text().toDouble(&ok);

        if (!ok || fraction < 0) {
            QMessageBox::warning(const_cast<FluidDialog*>(this), "错误",
                                 QString("组分 '%1' 的摩尔分数无效").arg(name));
            return "";
        }

        components.append(QString("%1[%2]").arg(name).arg(fraction));
        totalFraction += fraction;
    }

    if (qAbs(totalFraction - 1.0) > 1e-6) {
        QMessageBox::warning(const_cast<FluidDialog*>(this), "错误", "摩尔分数总和必须为1");
        return "";
    }

    return components.join("&");
}
