#include "properties.h"
extern "C" {
    #include "CoolPropLib.h"
}
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>

Properties::Properties(QWidget *parent)
    : QWidget{parent}
{
    initializePropertiesMap();
}

void Properties::setupUi(QWidget *parent)
{
    QWidget *mainWidget = new QWidget(parent);
    mainWidget->setGeometry(QRect(10, 10, 930, 380));

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    QGroupBox *componentGroup = new QGroupBox("选择组分", mainWidget);
    QGridLayout *componentLayout = new QGridLayout(componentGroup);

    int row = 0;

    // Search area
    m_componentSearchEdit = new QLineEdit(this);
    m_componentSearchEdit->setPlaceholderText("输入物质名称、化学式或CAS号...");
    connect(m_componentSearchEdit, &QLineEdit::textChanged,
            this, &Properties::onComponentSearchTextChanged);

    componentLayout->addWidget(m_componentSearchEdit, row, 0);
    componentLayout->addWidget(new QLabel("已选组分:", this), row, 2);

    ++row;

    // Available components table
    m_componentTable = new QTableWidget(this);
    m_componentTable->setColumnCount(3);
    m_componentTable->setHorizontalHeaderLabels({"名称", "化学式", "CAS号"});
    m_componentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_componentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_componentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_componentTable->setMaximumHeight(200);
    //m_componentTable->setColumnWidth(0, 100);
    //m_componentTable->setColumnWidth(1, 100);
    //m_componentTable->setColumnWidth(2, 100);

    componentLayout->addWidget(m_componentTable, row, 0);

    // Buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    m_addButton = new QPushButton("添加 >>", this);
    m_removeButton = new QPushButton("<< 删除", this);

    connect(m_addButton, &QPushButton::clicked, this, &Properties::onAddComponentClicked);
    connect(m_removeButton, &QPushButton::clicked, this, &Properties::onRemoveComponentClicked);

    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_removeButton);
    buttonLayout->addStretch();

    componentLayout->addLayout(buttonLayout, row, 1);

    // Selected components table
    m_selectedComponentTable = new QTableWidget(this);
    m_selectedComponentTable->setColumnCount(2);
    m_selectedComponentTable->setHorizontalHeaderLabels({"名称", "摩尔分数"});
    m_selectedComponentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_selectedComponentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_selectedComponentTable->setMaximumHeight(200);
    // m_selectedComponentTable->setColumnWidth(0, 200);
    // m_selectedComponentTable->setColumnWidth(1, 200);

    componentLayout->addWidget(m_selectedComponentTable, row, 2);

    mainLayout->addWidget(componentGroup);

    QGroupBox *parameterGroup = new QGroupBox("计算参数", this);
    QGridLayout *paramLayout = new QGridLayout(parameterGroup);

    // Parameter 1
    QLabel *param1Label = new QLabel("参数 1:", this);
    m_param1Combo = new QComboBox(this);
    m_param1Combo->addItems({"压力", "温度", "干度"});
    m_param1ValueEdit = new QLineEdit(this);
    m_param1UnitLabel = new QLabel("MPa", this);

    connect(m_param1Combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Properties::onParameter1Changed);

    paramLayout->addWidget(param1Label, 0, 0);
    paramLayout->addWidget(m_param1Combo, 0, 1);
    paramLayout->addWidget(m_param1ValueEdit, 0, 2);
    paramLayout->addWidget(m_param1UnitLabel, 0, 3);

    // Parameter 2
    QLabel *param2Label = new QLabel("参数 2:", this);
    m_param2Combo = new QComboBox(this);
    m_param2Combo->addItems({"温度", "干度"});
    m_param2ValueEdit = new QLineEdit(this);
    m_param2UnitLabel = new QLabel("°C", this);

    connect(m_param2Combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Properties::onParameter2Changed);

    paramLayout->addWidget(param2Label, 1, 0);
    paramLayout->addWidget(m_param2Combo, 1, 1);
    paramLayout->addWidget(m_param2ValueEdit, 1, 2);
    paramLayout->addWidget(m_param2UnitLabel, 1, 3);

    mainLayout->addWidget(parameterGroup);

    populateComponentList();
}

void Properties::initializePropertiesMap()
{
    m_properties = {
        {"Phase", {"相态", ""}},
        {"P", {"压力", "MPa"}},
        {"T", {"温度", "°C"}},
        {"Q", {"干度", ""}},
        {"M", {"摩尔质量", "kg/mol"}},
        {"D", {"密度", "kg/m³"}},
        {"H", {"比焓", "kJ/kg"}},
        {"S", {"比熵", "kJ/kg·K"}},
        {"U", {"比内能", "kJ/kg"}},
        {"CVMASS", {"定压比热", "kJ/kg·K"}},
        {"CPMASS", {"定容比热", "kJ/kg·K"}},
        {"W", {"声速", "m/s"}},
        {"V", {"动力粘度", "Pa·s"}},
        {"L", {"导热系数", "W/m·K"}},
        {"Z", {"压缩因子", ""}}
    };

    m_parameterUnits = {
        {"压力", "MPa"},
        {"温度", "°C"},
        {"干度", ""}
    };
}

void Properties::populateComponentList()
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

            char formulaBuffer[256];
            get_fluid_param_string(fluid.toUtf8().constData(), "formula", formulaBuffer, sizeof(formulaBuffer));

            char casBuffer[256];
            get_fluid_param_string(fluid.toUtf8().constData(), "CAS", casBuffer, sizeof(casBuffer));

            QString formulaStr = QString::fromUtf8(formulaBuffer).remove(QRegularExpression("[_{}1]"));
            QString casStr = QString::fromUtf8(casBuffer);

            m_componentTable->setItem(i, 0, new QTableWidgetItem(fluid));
            m_componentTable->setItem(i, 1, new QTableWidgetItem(formulaStr));
            m_componentTable->setItem(i, 2, new QTableWidgetItem(casStr));
        }

        m_componentTable->resizeColumnsToContents();
    }


}

void Properties::onComponentSearchTextChanged(const QString &text)
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

void Properties::onAddComponentClicked()
{
    int currentRow = m_componentTable->currentRow();
    if (currentRow >= 0) {
        QString componentName = m_componentTable->item(currentRow, 0)->text();

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

        m_selectedComponentTable->resizeColumnsToContents();
    }
}

void Properties::onRemoveComponentClicked()
{
    int currentRow = m_selectedComponentTable->currentRow();
    if (currentRow >= 0) {
        m_selectedComponentTable->removeRow(currentRow);
    }
}

void Properties::onParameter1Changed(int index)
{
    QString param = m_param1Combo->currentText();
    m_param1UnitLabel->setText(m_parameterUnits.value(param));

    if (m_param1Combo->currentText() == m_param2Combo->currentText()) {
        QMessageBox::warning(this, "警告", "参数1和参数2不能相同");
    }
}

void Properties::onParameter2Changed(int index)
{
    QString param = m_param2Combo->currentText();
    m_param2UnitLabel->setText(m_parameterUnits.value(param));

    if (m_param1Combo->currentText() == m_param2Combo->currentText()) {
        QMessageBox::warning(this, "警告", "参数1和参数2不能相同");
    }
}

QString Properties::createMixtureString() const
{
    if (m_selectedComponentTable->rowCount() == 0) {
        QMessageBox::warning(const_cast<Properties*>(this), "错误", "请至少选择一个组分");
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
            QMessageBox::warning(const_cast<Properties*>(this), "错误",
                                 QString("组分 '%1' 的摩尔分数无效").arg(name));
            return "";
        }

        components.append(QString("%1[%2]").arg(name).arg(fraction));
        totalFraction += fraction;
    }

    if (qAbs(totalFraction - 1.0) > 1e-6) {
        QMessageBox::warning(const_cast<Properties*>(this), "错误", "摩尔分数总和必须为1");
        return "";
    }

    return components.join("&");
}

double Properties::convertToSI(const QString &parameter, double value) const
{
    if (parameter == "压力") return value * 1e6;  // MPa to Pa
    if (parameter == "温度") return value + 273.15; // °C to K
    return value;
}

QString Properties::calculateProperties()
{
    QString mixture = createMixtureString();
    if (mixture.isEmpty()) return "";

    // Validate parameters
    QString param1 = m_param1Combo->currentText();
    QString param2 = m_param2Combo->currentText();

    if (param1.isEmpty() || param2.isEmpty()) {
        QMessageBox::warning(this, "错误", "请选择参数1和参数2");
        return "";
    }

    bool ok1, ok2;
    double value1 = m_param1ValueEdit->text().toDouble(&ok1);
    double value2 = m_param2ValueEdit->text().toDouble(&ok2);

    if (!ok1 || !ok2) {
        QMessageBox::warning(this, "错误", "请输入有效的参数值");
        return "";
    }

    // Convert to SI units
    double siValue1 = convertToSI(param1, value1);
    double siValue2 = convertToSI(param2, value2);

    // Map parameter names to CoolProp names
    QMap<QString, QString> paramMap = {
        {"压力", "P"}, {"温度", "T"}, {"干度", "Q"}
    };

    QString cpParam1 = paramMap.value(param1);
    QString cpParam2 = paramMap.value(param2);

    // 计算属性名称的最大显示宽度
    int maxNameWidth = 0;
    for (auto it = m_properties.begin(); it != m_properties.end(); ++it) {
        QString name = it.value().first;
        int width = calculateDisplayWidth(name);
        if (width > maxNameWidth) {
            maxNameWidth = width;
        }
    }

    maxNameWidth += 4;

    // Build result string
    QString result;
    result += "============================================\n";
    result += "          物质物性计算结果\n";
    result += "============================================\n\n";

    result += QString("物质: %1\n").arg(mixture);
    result += QString("计算条件:\n");
    result += QString("  %1 = %2 %3\n").arg(param1).arg(value1).arg(m_parameterUnits[param1]);
    result += QString("  %1 = %2 %3\n\n").arg(param2).arg(value2).arg(m_parameterUnits[param2]);

    result += "物性参数:\n";
    result += "--------------------------------------------\n";

    // Calculate and format each property
    for (auto it = m_properties.begin(); it != m_properties.end(); ++it) {
        QString property = it.key();
        QString name = it.value().first;
        QString unit = it.value().second;

        // 使用辅助函数对齐文本
        QString alignedName = alignMixedText(name, maxNameWidth);

        try {
            double calculatedValue = 0.0;
            char phase_str[256];
            if (property == "Phase") {
                PhaseSI(cpParam1.toUtf8().constData(), siValue1,
                        cpParam2.toUtf8().constData(), siValue2,
                        mixture.toUtf8().constData(), phase_str,
                        sizeof(phase_str));
                result += formatPropertyLine(name, QString::fromUtf8(phase_str), "", maxNameWidth) + "\n";
            } else {
                calculatedValue = PropsSI(property.toUtf8().constData(),
                                          cpParam1.toUtf8().constData(), siValue1,
                                          cpParam2.toUtf8().constData(), siValue2,
                                          mixture.toUtf8().constData());

                // Convert back to display units
                if (property == "P") calculatedValue /= 1e6;  // Pa to MPa
                else if (property == "T") calculatedValue -= 273.15; // K to °C
                else if (property == "H" || property == "S" || property == "U" ||
                         property == "CPMASS" || property == "CVMASS") {
                    calculatedValue /= 1000.0; // J to kJ
                }

                // 格式化数值
                QString formattedValue;
                if (qIsInf(calculatedValue)) {
                    formattedValue = "inf";
                } else if (qIsNaN(calculatedValue)) {
                    formattedValue = "NaN";
                } else if (qAbs(calculatedValue) < 0.001 || qAbs(calculatedValue) > 10000) {
                    formattedValue = QString::number(calculatedValue, 'e', 6);
                } else {
                    formattedValue = QString::number(calculatedValue, 'f', 6);
                }

                result += formatPropertyLine(name, formattedValue, unit, maxNameWidth) + "\n";
            }

        } catch (const std::exception &e) {
            result += formatPropertyLine(name, "计算错误", "", maxNameWidth) + "\n";
        }
    }

    return result;
}

QString Properties::run()
{
    return calculateProperties();
}

// 计算字符串在宋体下的显示宽度
int Properties::calculateDisplayWidth(const QString &text) const
{
    int width = 0;
    for (const QChar &ch : text) {
        // 在宋体下，中文字符宽度约为英文字符的2倍
        // 这里使用更精确的宽度计算
        if (ch.unicode() >= 0x4E00 && ch.unicode() <= 0x9FFF) {
            width += 2; // 中文字符
        } else {
            width += 1; // 英文字符、数字和符号
        }
    }
    return width;
}

// 对齐混合文本，专门针对宋体优化
QString Properties::alignMixedText(const QString &text, int targetWidth) const
{
    int currentWidth = calculateDisplayWidth(text);
    if (currentWidth >= targetWidth) {
        return text;
    }

    // 在宋体下，使用更精确的对齐方法
    // 计算需要添加的空格数量
    int spaceCount = targetWidth - currentWidth;

    // 使用 arg 方法设置宽度，确保在宋体下对齐
    return QString("%1%2").arg(text).arg("", spaceCount);
}

// 格式化属性行，确保在宋体下对齐
QString Properties::formatPropertyLine(const QString &name, const QString &value, const QString &unit, int maxNameWidth) const
{
    QString alignedName = alignMixedText(name, maxNameWidth);

    if (unit.isEmpty()) {
        return QString("  %1: %2").arg(alignedName).arg(value, 20);
    } else {
        return QString("  %1: %2 %3").arg(alignedName).arg(value, 20).arg(unit);
    }
}
