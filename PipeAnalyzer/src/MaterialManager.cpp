#include "MaterialManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <cmath>
#include <QRegularExpression>

// 保温材料导热系数计算
double InsulationMaterial::calculateConductivity(double tmK) const
{
    const double tmC = tmK - 273.15;  // 转换为摄氏度

    try {
        // 处理方程1
        if (!conductivityEq1.isEmpty()) {
            const int colonIndex = conductivityEq1.indexOf(':');
            if (colonIndex != -1) {
                // 分离方程和条件
                const QString equation = conductivityEq1.left(colonIndex).trimmed();
                const QString condition = conductivityEq1.mid(colonIndex + 1).trimmed();

                if (MaterialManager::evaluateCondition(condition, tmC)) {
                    return MaterialManager::evaluateExpression(equation, tmC);
                }
            } else {
                // 无条件方程
                return MaterialManager::evaluateExpression(conductivityEq1, tmC);
            }
        }

        // 处理方程2
        if (!conductivityEq2.isEmpty()) {
            const int colonIndex = conductivityEq2.indexOf(':');
            if (colonIndex != -1) {
                const QString equation = conductivityEq2.left(colonIndex).trimmed();
                const QString condition = conductivityEq2.mid(colonIndex + 1).trimmed();

                if (MaterialManager::evaluateCondition(condition, tmC)) {
                    return MaterialManager::evaluateExpression(equation, tmC);
                }
            } else {
                return MaterialManager::evaluateExpression(conductivityEq2, tmC);
            }
        }

        // 处理方程3
        if (!conductivityEq3.isEmpty()) {
            const int colonIndex = conductivityEq3.indexOf(':');
            if (colonIndex != -1) {
                const QString equation = conductivityEq3.left(colonIndex).trimmed();
                const QString condition = conductivityEq3.mid(colonIndex + 1).trimmed();

                if (MaterialManager::evaluateCondition(condition, tmC)) {
                    return MaterialManager::evaluateExpression(equation, tmC);
                }
            } else {
                return MaterialManager::evaluateExpression(conductivityEq3, tmC);
            }
        }

    } catch (const std::exception& e) {
        qWarning() << "导热系数计算错误:" << e.what()
                   << "材料:" << name << "温度:" << tmK << "K";
    }

    return 0.04;  // 默认值
}

// 材料管理器构造函数
MaterialManager::MaterialManager(QObject *parent)
    : QObject(parent)
{
    qDebug() << "MaterialManager 初始化开始";

    try {
        // 尝试加载材料，如果失败则创建默认材料
        if (!loadMaterialsFromFile()) {
            qWarning() << "加载材料失败，创建默认材料文件";
            createDefaultMaterialFiles();
            loadMaterialsFromFile();
        }
        qDebug() << "材料初始化完成";
    } catch (const std::exception& e) {
        qCritical() << "初始化失败:" << e.what();
        throw;
    }
}

// 获取保温材料列表
QMap<QString, InsulationMaterial> MaterialManager::getInsulationMaterials() const
{
    return insulationMaterials;
}

// 获取外保护材料列表
QMap<QString, OuterProtection> MaterialManager::getProtectionMaterials() const
{
    return protectionMaterials;
}

// 获取管道配件列表
QMap<QString, PipeFitting> MaterialManager::getPipeFittings() const
{
    return pipeFittings;
}

// 获取管道类型列表
QMap<QString, PipeType> MaterialManager::getPipeTypes() const
{
    return pipeTypes;
}

// 添加保温材料
void MaterialManager::addInsulationMaterial(const QString& name, const QString& eq1,
                                            const QString& eq2, const QString& eq3,
                                            double density, const QString& description)
{
    InsulationMaterial material;
    material.name = name;
    material.conductivityEq1 = eq1;
    material.conductivityEq2 = eq2;
    material.conductivityEq3 = eq3;
    material.density = density;
    material.description = description;

    insulationMaterials[name] = material;
}

// 添加外保护材料
void MaterialManager::addProtectionMaterial(const QString& name, double emissivity,
                                            const QString& description)
{
    OuterProtection material;
    material.name = name;
    material.emissivity = emissivity;
    material.description = description;

    protectionMaterials[name] = material;
}

// 添加管道配件
void MaterialManager::addPipeFitting(const QString& name, double resistanceCoef,
                                     const QString& description)
{
    PipeFitting fitting;
    fitting.name = name;
    fitting.resistanceCoef = resistanceCoef;
    fitting.description = description;

    pipeFittings[name] = fitting;
}

// 添加管道类型
void MaterialManager::addPipeType(const QString& name, double roughness,
                                  const QString& description)
{
    PipeType pipeType;
    pipeType.name = name;
    pipeType.roughness = roughness;
    pipeType.description = description;

    pipeTypes[name] = pipeType;
}

// 保存材料到文件
bool MaterialManager::saveMaterialsToFile()
{
    try {
        // 确保材料目录存在
        const QString dataDir = QApplication::applicationDirPath() + "/materials";
        if (!QDir().mkpath(dataDir)) {
            qWarning() << "无法创建材料目录:" << dataDir;
            return false;
        }

        // 保存保温材料
        QFile insulationFile(getMaterialFilePath("insulation_materials.json"));
        if (!insulationFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "无法打开保温材料文件:" << insulationFile.errorString();
            return false;
        }

        QJsonObject insulationData;
        for (const auto& key : insulationMaterials.keys()) {
            const auto& material = insulationMaterials[key];
            QJsonObject obj;
            obj["conductivity_eq1"] = material.conductivityEq1;
            obj["conductivity_eq2"] = material.conductivityEq2;
            obj["conductivity_eq3"] = material.conductivityEq3;
            obj["density"] = material.density;
            obj["description"] = material.description;
            insulationData[key] = obj;
        }
        insulationFile.write(QJsonDocument(insulationData).toJson(QJsonDocument::Indented));
        insulationFile.close();

        // 保存外保护材料
        QFile protectionFile(getMaterialFilePath("protection_materials.json"));
        if (!protectionFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "无法打开外保护材料文件:" << protectionFile.errorString();
            return false;
        }

        QJsonObject protectionData;
        for (const auto& key : protectionMaterials.keys()) {
            const auto& material = protectionMaterials[key];
            QJsonObject obj;
            obj["emissivity"] = material.emissivity;
            obj["description"] = material.description;
            protectionData[key] = obj;
        }
        protectionFile.write(QJsonDocument(protectionData).toJson(QJsonDocument::Indented));
        protectionFile.close();

        // 保存管道配件
        QFile fittingsFile(getMaterialFilePath("pipe_fittings.json"));
        if (!fittingsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "无法打开管道配件文件:" << fittingsFile.errorString();
            return false;
        }

        QJsonObject fittingsData;
        for (const auto& key : pipeFittings.keys()) {
            const auto& fitting = pipeFittings[key];
            QJsonObject obj;
            obj["resistance_coef"] = fitting.resistanceCoef;
            obj["description"] = fitting.description;
            fittingsData[key] = obj;
        }
        fittingsFile.write(QJsonDocument(fittingsData).toJson(QJsonDocument::Indented));
        fittingsFile.close();

        // 保存管道类型
        QFile pipeTypesFile(getMaterialFilePath("pipe_types.json"));
        if (!pipeTypesFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "无法打开管道类型文件:" << pipeTypesFile.errorString();
            return false;
        }

        QJsonObject pipeTypesData;
        for (const auto& key : pipeTypes.keys()) {
            const auto& pipeType = pipeTypes[key];
            QJsonObject obj;
            obj["roughness"] = pipeType.roughness;
            obj["description"] = pipeType.description;
            pipeTypesData[key] = obj;
        }
        pipeTypesFile.write(QJsonDocument(pipeTypesData).toJson(QJsonDocument::Indented));
        pipeTypesFile.close();

        qDebug() << "材料保存成功";
        return true;

    } catch (const std::exception& e) {
        qCritical() << "保存材料失败:" << e.what();
        QMessageBox::critical(nullptr, "错误", QString("保存材料失败: %1").arg(e.what()));
        return false;
    }
}

// 从文件加载材料
bool MaterialManager::loadMaterialsFromFile()
{
    try {
        // 清空现有数据
        insulationMaterials.clear();
        protectionMaterials.clear();
        pipeFittings.clear();
        pipeTypes.clear();

        // 加载保温材料
        QFile insulationFile(getMaterialFilePath("insulation_materials.json"));
        if (!insulationFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "无法打开保温材料文件:" << insulationFile.errorString();
            return false;
        }

        QJsonObject insulationData = QJsonDocument::fromJson(insulationFile.readAll()).object();
        insulationFile.close();

        for (const auto& key : insulationData.keys()) {
            const auto& obj = insulationData[key].toObject();
            addInsulationMaterial(
                key,
                obj["conductivity_eq1"].toString(),
                obj["conductivity_eq2"].toString(),
                obj["conductivity_eq3"].toString(),
                obj["density"].toDouble(),
                obj["description"].toString()
                );
        }

        // 加载外保护材料
        QFile protectionFile(getMaterialFilePath("protection_materials.json"));
        if (!protectionFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "无法打开外保护材料文件:" << protectionFile.errorString();
            return false;
        }

        QJsonObject protectionData = QJsonDocument::fromJson(protectionFile.readAll()).object();
        protectionFile.close();

        for (const auto& key : protectionData.keys()) {
            const auto& obj = protectionData[key].toObject();
            addProtectionMaterial(
                key,
                obj["emissivity"].toDouble(),
                obj["description"].toString()
                );
        }

        // 加载管道配件
        QFile fittingsFile(getMaterialFilePath("pipe_fittings.json"));
        if (!fittingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "无法打开管道配件文件:" << fittingsFile.errorString();
            return false;
        }

        QJsonObject fittingsData = QJsonDocument::fromJson(fittingsFile.readAll()).object();
        fittingsFile.close();

        for (const auto& key : fittingsData.keys()) {
            const auto& obj = fittingsData[key].toObject();
            addPipeFitting(
                key,
                obj["resistance_coef"].toDouble(),
                obj["description"].toString()
                );
        }

        // 加载管道类型
        QFile pipeTypesFile(getMaterialFilePath("pipe_types.json"));
        if (!pipeTypesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "无法打开管道类型文件:" << pipeTypesFile.errorString();
            return false;
        }

        QJsonObject pipeTypesData = QJsonDocument::fromJson(pipeTypesFile.readAll()).object();
        pipeTypesFile.close();

        for (const auto& key : pipeTypesData.keys()) {
            const auto& obj = pipeTypesData[key].toObject();
            addPipeType(
                key,
                obj["roughness"].toDouble(),
                obj["description"].toString()
                );
        }

        qDebug() << "材料加载成功";
        return true;

    } catch (const std::exception& e) {
        qCritical() << "加载材料失败:" << e.what();
        return false;
    }
}

// 创建默认材料文件
void MaterialManager::createDefaultMaterialFiles()
{
    try {
        // 创建材料目录
        const QString dataDir = QApplication::applicationDirPath() + "/materials";
        QDir().mkpath(dataDir);

        // 创建默认保温材料
        QJsonObject defaultInsulation;
        defaultInsulation["硅酸钙制品-I型-170"] = QJsonObject({
            {"conductivity_eq1", "0.0479+0.00010185*tm+9.65015e-11*tm^3:tm<800"},
            {"conductivity_eq2", ""},
            {"conductivity_eq3", ""},
            {"density", 170},
            {"description", "引自GB/50264-2013 附录A 表A.0.1"}
        });

        defaultInsulation["硅酸钙制品-I型-220"] = QJsonObject({
            {"conductivity_eq1", "0.0564+0.00007786*tm+7.8571e-8*tm^2:tm<500"},
            {"conductivity_eq2", "0.0937+1.67397E-10*tm^3:500<=tm<=800"},
            {"conductivity_eq3", ""},
            {"density", 220},
            {"description", "引自GB/50264-2013 附录A 表A.0.1"}
        });

        defaultInsulation["岩棉制品-毡-60-100"] = QJsonObject({
            {"conductivity_eq1", "0.0337+0.000151*tm:-20<=tm<=100"},
            {"conductivity_eq2", "0.0395+4.71e-5*tm+5.03e-7*tm^2:100<tm<=600"},
            {"conductivity_eq3", ""},
            {"density", 80},
            {"description", "引自GB/50264-2013 附录A 表A.0.1"}
        });

        QFile(getMaterialFilePath("insulation_materials.json")).write(
            QJsonDocument(defaultInsulation).toJson(QJsonDocument::Indented)
            );

        // 创建默认外保护层材料
        QJsonObject defaultProtection;
        defaultProtection["铝合金薄板"] = QJsonObject({{"emissivity", 0.3}, {"description", "引自GB/50264-2013 表5.8.9"}});
        defaultProtection["不锈钢薄板"] = QJsonObject({{"emissivity", 0.4}, {"description", "引自GB/50264-2013 表5.8.9"}});
        defaultProtection["有光泽的镀钵薄钢板"] = QJsonObject({{"emissivity", 0.27}, {"description", "引自GB/50264-2013 表5.8.9"}});
        defaultProtection["油漆"] = QJsonObject({{"emissivity", 0.90}, {"description", "引自GB/50264-2013 表5.8.9"}});

        QFile(getMaterialFilePath("protection_materials.json")).write(
            QJsonDocument(defaultProtection).toJson(QJsonDocument::Indented)
            );

        // 创建默认管道元件
        QJsonObject defaultFittings;
        defaultFittings["45°标准弯头"] = QJsonObject({{"resistance_coef", 0.35}, {"description", "引自SH/3035-2018 表6.2.5"}});
        defaultFittings["90°标准弯头"] = QJsonObject({{"resistance_coef", 0.75}, {"description", "引自SH/3035-2018 表6.2.5"}});
        defaultFittings["90°斜接弯头"] = QJsonObject({{"resistance_coef", 1.3}, {"description", "引自SH/3035-2018 表6.2.5"}});
        defaultFittings["等径三通(流出)"] = QJsonObject({{"resistance_coef", 1.2}, {"description", "引自SH/3035-2018 表6.2.5"}});
        defaultFittings["截止阀(全开)"] = QJsonObject({{"resistance_coef", 6.0}, {"description", "引自SH/3035-2018 表6.2.5"}});
        defaultFittings["闸阀(全开)"] = QJsonObject({{"resistance_coef", 0.17}, {"description", "引自SH/3035-2018 表6.2.5"}});

        QFile(getMaterialFilePath("pipe_fittings.json")).write(
            QJsonDocument(defaultFittings).toJson(QJsonDocument::Indented)
            );

        // 创建默认管道类型
        QJsonObject defaultPipeTypes;
        defaultPipeTypes["无缝黄铜、铜及铅管"] = QJsonObject({{"roughness", 0.00001}, {"description", "引自SH/3035-2018 表6.2.4"}});
        defaultPipeTypes["操作中基本无腐蚀的无缝钢管"] = QJsonObject({{"roughness", 0.0001}, {"description", "引自SH/3035-2018 表6.2.4"}});
        defaultPipeTypes["操作中有轻度腐蚀的无缝钢管"] = QJsonObject({{"roughness", 0.0002}, {"description", "引自SH/3035-2018 表6.2.4"}});
        defaultPipeTypes["操作中有显著腐蚀的无缝钢管"] = QJsonObject({{"roughness", 0.0005}, {"description", "引自SH/3035-2018 表6.2.4"}});
        defaultPipeTypes["铸铁管"] = QJsonObject({{"roughness", 0.00085}, {"description", "引自SH/3035-2018 表6.2.4"}});

        QFile(getMaterialFilePath("pipe_types.json")).write(
            QJsonDocument(defaultPipeTypes).toJson(QJsonDocument::Indented)
            );

        qDebug() << "默认材料文件创建成功";

    } catch (const std::exception& e) {
        qCritical() << "创建默认材料文件失败:" << e.what();
    }
}

// 获取材料文件路径
QString MaterialManager::getMaterialFilePath(const QString& fileName)
{
    return QApplication::applicationDirPath() + "/materials/" + fileName;
}

// 解析条件表达式
bool MaterialManager::evaluateCondition(const QString& condition, double tmC)
{
    // 支持的条件格式: tm<value, tm<=value, tm>value, tm>=value, a<=tm<=b
    static QRegularExpression re(R"(^\s*tm\s*([<>]=?|=)\s*([+-]?\d+(\.\d+)?)\s*$)");
    static QRegularExpression rangeRe(R"(^\s*([+-]?\d+(\.\d+)?)\s*<=?\s*tm\s*<=?\s*([+-]?\d+(\.\d+)?)\s*$)");

    if (rangeRe.match(condition).hasMatch()) {
        const auto& match = rangeRe.match(condition);
        const double minVal = match.captured(1).toDouble();
        const double maxVal = match.captured(3).toDouble();
        const bool leftInclusive = condition.contains("<=");
        const bool rightInclusive = condition.contains("<=", condition.indexOf("tm") + 2);

        if (leftInclusive && rightInclusive) return (tmC >= minVal && tmC <= maxVal);
        if (leftInclusive && !rightInclusive) return (tmC >= minVal && tmC < maxVal);
        if (!leftInclusive && rightInclusive) return (tmC > minVal && tmC <= maxVal);
        return (tmC > minVal && tmC < maxVal);
    }

    if (re.match(condition).hasMatch()) {
        const auto& match = re.match(condition);
        const QString op = match.captured(1);
        const double val = match.captured(2).toDouble();

        if (op == "<") return tmC < val;
        if (op == "<=") return tmC <= val;
        if (op == ">") return tmC > val;
        if (op == ">=") return tmC >= val;
        if (op == "=") return qFuzzyCompare(tmC, val);
    }

    qWarning() << "无法解析条件表达式:" << condition;
    return false;
}

// 解析数学表达式
double MaterialManager::evaluateExpression(const QString& expr, double tmC)
{
    // 替换表达式中的tm变量
    QString processedExpr = expr;
    processedExpr.replace("tm", QString::number(tmC));
    processedExpr.replace("^", "**");  // 将^转换为pow函数格式

    // 简单表达式解析器（实际应用中可考虑使用更完善的表达式解析库）
    try {
        // 处理常数和运算符
        if (processedExpr.contains("+")) {
            const auto parts = processedExpr.split("+");
            return evaluateExpression(parts[0], tmC) + evaluateExpression(parts[1], tmC);
        }
        if (processedExpr.contains("-") && processedExpr.indexOf("-") > 0) {
            const auto parts = processedExpr.split("-");
            return evaluateExpression(parts[0], tmC) - evaluateExpression(parts[1], tmC);
        }
        if (processedExpr.contains("*")) {
            const auto parts = processedExpr.split("*");
            return evaluateExpression(parts[0], tmC) * evaluateExpression(parts[1], tmC);
        }
        if (processedExpr.contains("/")) {
            const auto parts = processedExpr.split("/");
            const double denominator = evaluateExpression(parts[1], tmC);
            if (qFuzzyIsNull(denominator)) throw std::runtime_error("除零错误");
            return evaluateExpression(parts[0], tmC) / denominator;
        }
        if (processedExpr.contains("**")) {
            const auto parts = processedExpr.split("**");
            return pow(evaluateExpression(parts[0], tmC), evaluateExpression(parts[1], tmC));
        }

        // 纯数字
        bool ok = false;
        const double result = processedExpr.toDouble(&ok);
        if (ok) return result;

    } catch (const std::exception& e) {
        qWarning() << "表达式计算错误:" << e.what() << "表达式:" << expr;
        throw;
    }

    throw std::runtime_error("无法解析表达式: " + expr.toStdString());
}
