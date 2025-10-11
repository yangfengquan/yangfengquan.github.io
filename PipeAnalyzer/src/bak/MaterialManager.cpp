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

// InsulationMaterial 方法实现
double InsulationMaterial::calculateConductivity(double tmK) const
{
    double tmC = tmK - 273.15;

    try {
        if (!conductivityEq1.isEmpty()) {
            QStringList eqParts = conductivityEq1.split(":");
            if (eqParts.size() > 1) {
                // 有条件的情况
                QString condition = eqParts[1];
                // 简单的条件解析 - 实际应用中需要更复杂的解析器
                if (condition.contains("<=")) {
                    QStringList condParts = condition.split("<=");
                    if (condParts.size() == 2) {
                        double limit = condParts[1].toDouble();
                        if (tmC <= limit) {
                            // 使用方程1
                            QString equation = eqParts[0];
                            // 简单的方程求值 - 实际应用中需要更复杂的数学表达式解析器
                            if (equation.contains("tm")) {
                                // 替换tm为实际值并计算
                                // 这里简化处理，实际需要完整的表达式求值
                                return 0.04; // 示例值
                            }
                        }
                    }
                }
            } else {
                // 无条件的情况
                QString equation = eqParts[0];
                // 简化处理
                if (equation.contains("0.0479+0.00010185")) {
                    return 0.0479 + 0.00010185 * tmC + 9.65015e-11 * pow(tmC, 3);
                } else if (equation.contains("0.0564+0.00007786")) {
                    return 0.0564 + 0.00007786 * tmC + 7.8571e-8 * pow(tmC, 2);
                }
                // 其他方程...
            }
        }

        if (!conductivityEq2.isEmpty()) {
            // 类似处理方程2
            QStringList eqParts = conductivityEq2.split(":");
            if (eqParts.size() > 1) {
                QString condition = eqParts[1];
                if (condition.contains("500<=tm<=800")) {
                    if (tmC >= 500 && tmC <= 800) {
                        QString equation = eqParts[0];
                        if (equation.contains("0.0937+1.67397E-10")) {
                            return 0.0937 + 1.67397e-10 * pow(tmC, 3);
                        }
                    }
                }
            }
        }

        if (!conductivityEq3.isEmpty()) {
            // 类似处理方程3
        }

    } catch (const std::exception& e) {
        qDebug() << "绝热材料导热系数计算错误:" << e.what();
    }

    return 0.04; // 默认值
}

// MaterialManager 实现
MaterialManager::MaterialManager(QObject *parent)
    : QObject(parent)
    , pipeTypes()
    , insulationMaterials()
    , protectionMaterials()
    , pipeFittings()
{/*
    if (!loadMaterialsFromFile()) {
        createDefaultMaterialFiles();
        loadMaterialsFromFile();
    }*/
    qDebug() << "MaterialManager 构造函数开始";

    try {
        qDebug() << "步骤1: 初始化材料数据...";
        loadMaterialsFromFile();
        qDebug() << "材料数据加载完成";

    } catch (const std::exception& e) {
        qCritical() << "MaterialManager 构造函数中捕获异常:" << e.what();
        throw;
    }

    qDebug() << "MaterialManager 构造函数完成";
}

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

void MaterialManager::addProtectionMaterial(const QString& name, double emissivity,
                                            const QString& description)
{
    OuterProtection material;
    material.name = name;
    material.emissivity = emissivity;
    material.description = description;

    protectionMaterials[name] = material;
}

void MaterialManager::addPipeFitting(const QString& name, double resistanceCoef,
                                     const QString& description)
{
    PipeFitting fitting;
    fitting.name = name;
    fitting.resistanceCoef = resistanceCoef;
    fitting.description = description;

    pipeFittings[name] = fitting;
}

void MaterialManager::addPipeType(const QString& name, double roughness,
                                  const QString& description)
{
    PipeType pipeType;
    pipeType.name = name;
    pipeType.roughness = roughness;
    pipeType.description = description;

    pipeTypes[name] = pipeType;
}

bool MaterialManager::saveMaterialsToFile()
{
    try {
        // 保存保温材料
        QJsonObject insulationData;
        for (auto it = insulationMaterials.begin(); it != insulationMaterials.end(); ++it) {
            const InsulationMaterial& material = it.value();
            QJsonObject materialObj;
            materialObj["conductivity_eq1"] = material.conductivityEq1;
            materialObj["conductivity_eq2"] = material.conductivityEq2;
            materialObj["conductivity_eq3"] = material.conductivityEq3;
            materialObj["density"] = material.density;
            materialObj["description"] = material.description;

            insulationData[it.key()] = materialObj;
        }

        QFile insulationFile("insulation_materials.json");
        if (insulationFile.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(insulationData);
            insulationFile.write(doc.toJson());
            insulationFile.close();
        } else {
            qWarning() << "无法打开保温材料文件进行写入";
            return false;
        }

        // 保存外保护层材料
        QJsonObject protectionData;
        for (auto it = protectionMaterials.begin(); it != protectionMaterials.end(); ++it) {
            const OuterProtection& material = it.value();
            QJsonObject materialObj;
            materialObj["emissivity"] = material.emissivity;
            materialObj["description"] = material.description;

            protectionData[it.key()] = materialObj;
        }

        QFile protectionFile("protection_materials.json");
        if (protectionFile.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(protectionData);
            protectionFile.write(doc.toJson());
            protectionFile.close();
        } else {
            qWarning() << "无法打开外保护层材料文件进行写入";
            return false;
        }

        // 保存管道元件
        QJsonObject fittingsData;
        for (auto it = pipeFittings.begin(); it != pipeFittings.end(); ++it) {
            const PipeFitting& fitting = it.value();
            QJsonObject fittingObj;
            fittingObj["resistance_coef"] = fitting.resistanceCoef;
            fittingObj["description"] = fitting.description;

            fittingsData[it.key()] = fittingObj;
        }

        QFile fittingsFile("pipe_fittings.json");
        if (fittingsFile.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(fittingsData);
            fittingsFile.write(doc.toJson());
            fittingsFile.close();
        } else {
            qWarning() << "无法打开管道元件文件进行写入";
            return false;
        }

        // 保存管道类型
        QJsonObject pipeTypesData;
        for (auto it = pipeTypes.begin(); it != pipeTypes.end(); ++it) {
            const PipeType& pipeType = it.value();
            QJsonObject pipeTypeObj;
            pipeTypeObj["roughness"] = pipeType.roughness;
            pipeTypeObj["description"] = pipeType.description;

            pipeTypesData[it.key()] = pipeTypeObj;
        }

        QFile pipeTypesFile("pipe_types.json");
        if (pipeTypesFile.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(pipeTypesData);
            pipeTypesFile.write(doc.toJson());
            pipeTypesFile.close();
        } else {
            qWarning() << "无法打开管道类型文件进行写入";
            return false;
        }

        qDebug() << "材料文件保存成功";
        return true;

    } catch (const std::exception& e) {
        qCritical() << "保存材料文件失败:" << e.what();
        QMessageBox::critical(nullptr, "错误", QString("保存材料文件失败: %1").arg(e.what()));
        return false;
    }
}

bool MaterialManager::loadMaterialsFromFile()
{
    try {
        // 清空现有数据
        insulationMaterials.clear();
        protectionMaterials.clear();
        pipeFittings.clear();
        pipeTypes.clear();

        // 加载保温材料
        QFile insulationFile("insulation_materials.json");
        if (insulationFile.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(insulationFile.readAll());
            QJsonObject insulationData = doc.object();

            for (auto it = insulationData.begin(); it != insulationData.end(); ++it) {
                QJsonObject materialObj = it.value().toObject();
                addInsulationMaterial(
                    it.key(),
                    materialObj["conductivity_eq1"].toString(),
                    materialObj["conductivity_eq2"].toString(),
                    materialObj["conductivity_eq3"].toString(),
                    materialObj["density"].toDouble(),
                    materialObj["description"].toString()
                    );
            }
            insulationFile.close();
        } else {
            qWarning() << "无法打开保温材料文件，将创建默认文件";
            return false;
        }

        // 加载外保护层材料
        QFile protectionFile("protection_materials.json");
        if (protectionFile.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(protectionFile.readAll());
            QJsonObject protectionData = doc.object();

            for (auto it = protectionData.begin(); it != protectionData.end(); ++it) {
                QJsonObject materialObj = it.value().toObject();
                addProtectionMaterial(
                    it.key(),
                    materialObj["emissivity"].toDouble(),
                    materialObj["description"].toString()
                    );
            }
            protectionFile.close();
        } else {
            qWarning() << "无法打开外保护层材料文件，将创建默认文件";
            return false;
        }

        // 加载管道元件
        QFile fittingsFile("pipe_fittings.json");
        if (fittingsFile.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(fittingsFile.readAll());
            QJsonObject fittingsData = doc.object();

            for (auto it = fittingsData.begin(); it != fittingsData.end(); ++it) {
                QJsonObject fittingObj = it.value().toObject();
                addPipeFitting(
                    it.key(),
                    fittingObj["resistance_coef"].toDouble(),
                    fittingObj["description"].toString()
                    );
            }
            fittingsFile.close();
        } else {
            qWarning() << "无法打开管道元件文件，将创建默认文件";
            return false;
        }

        // 加载管道类型
        QFile pipeTypesFile("pipe_types.json");
        if (pipeTypesFile.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(pipeTypesFile.readAll());
            QJsonObject pipeTypesData = doc.object();

            for (auto it = pipeTypesData.begin(); it != pipeTypesData.end(); ++it) {
                QJsonObject pipeTypeObj = it.value().toObject();
                addPipeType(
                    it.key(),
                    pipeTypeObj["roughness"].toDouble(),
                    pipeTypeObj["description"].toString()
                    );
            }
            pipeTypesFile.close();
        } else {
            qWarning() << "无法打开管道类型文件，将创建默认文件";
            return false;
        }

        qDebug() << "材料文件加载成功";
        return true;

    } catch (const std::exception& e) {
        qCritical() << "加载材料文件失败:" << e.what();
        return false;
    }
}

void MaterialManager::createDefaultMaterialFiles()
{
    try {
        // 创建默认保温材料
        QJsonObject defaultInsulation;

        // 添加默认保温材料数据
        defaultInsulation["硅酸钙制品-I型-170"] = QJsonObject({
            {"conductivity_eq1", "0.0479+0.00010185*tm+9.65015e-11*tm**3:(tm<800)"},
            {"conductivity_eq2", ""},
            {"conductivity_eq3", ""},
            {"density", 170},
            {"description", "引自GB/50264-2013 附录A 表A.0.1"}
        });

        defaultInsulation["硅酸钙制品-II型-170"] = QJsonObject({
            {"conductivity_eq1", "0.0479+0.00010185*tm+9.65015e-11*tm**3:(tm<800)"},
            {"conductivity_eq2", ""},
            {"conductivity_eq3", ""},
            {"density", 170},
            {"description", "引自GB/50264-2013 附录A 表A.0.1"}
        });

        defaultInsulation["硅酸钙制品-I型-220"] = QJsonObject({
            {"conductivity_eq1", "0.0564+0.00007786*tm+7.8571e-8*tm**2:(tm<500)"},
            {"conductivity_eq2", "0.0937+1.67397E-10*tm**3:(500<=tm<=800)"},
            {"conductivity_eq3", ""},
            {"density", 220},
            {"description", "引自GB/50264-2013 附录A 表A.0.1"}
        });

        // 添加更多默认材料...
        defaultInsulation["岩棉制品-毡-60-100"] = QJsonObject({
            {"conductivity_eq1", "0.0337+0.000151*tm:(-20<=tm<=100)"},
            {"conductivity_eq2", "0.0395+4.71e-5*tm+5.03e-7*tm**2:(100<tm<=600)"},
            {"conductivity_eq3", ""},
            {"density", 80},
            {"description", "引自GB/50264-2013 附录A 表A.0.1"}
        });

        QFile insulationFile("insulation_materials.json");
        if (insulationFile.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(defaultInsulation);
            insulationFile.write(doc.toJson());
            insulationFile.close();
        }

        // 创建默认外保护层材料
        QJsonObject defaultProtection;
        defaultProtection["铝合金薄板"] = QJsonObject({
            {"emissivity", 0.3},
            {"description", "引自GB/50264-2013 表5.8.9"}
        });

        defaultProtection["不锈钢薄板"] = QJsonObject({
            {"emissivity", 0.4},
            {"description", "引自GB/50264-2013 表5.8.9"}
        });

        defaultProtection["有光泽的镀钵薄钢板"] = QJsonObject({
            {"emissivity", 0.27},
            {"description", "引自GB/50264-2013 表5.8.9"}
        });

        defaultProtection["油漆"] = QJsonObject({
            {"emissivity", 0.90},
            {"description", "引自GB/50264-2013 表5.8.9"}
        });

        QFile protectionFile("protection_materials.json");
        if (protectionFile.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(defaultProtection);
            protectionFile.write(doc.toJson());
            protectionFile.close();
        }

        // 创建默认管道元件
        QJsonObject defaultFittings;
        defaultFittings["45°标准弯头"] = QJsonObject({
            {"resistance_coef", 0.35},
            {"description", "引自SH/3035-2018 表6.2.5"}
        });

        defaultFittings["90°标准弯头"] = QJsonObject({
            {"resistance_coef", 0.75},
            {"description", "引自SH/3035-2018 表6.2.5"}
        });

        defaultFittings["90°斜接弯头"] = QJsonObject({
            {"resistance_coef", 1.3},
            {"description", "引自SH/3035-2018 表6.2.5"}
        });

        defaultFittings["等径三通(流出)"] = QJsonObject({
            {"resistance_coef", 1.2},
            {"description", "引自SH/3035-2018 表6.2.5"}
        });

        defaultFittings["截止阀(全开)"] = QJsonObject({
            {"resistance_coef", 6.0},
            {"description", "引自SH/3035-2018 表6.2.5"}
        });

        defaultFittings["闸阀(全开)"] = QJsonObject({
            {"resistance_coef", 0.17},
            {"description", "引自SH/3035-2018 表6.2.5"}
        });

        QFile fittingsFile("pipe_fittings.json");
        if (fittingsFile.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(defaultFittings);
            fittingsFile.write(doc.toJson());
            fittingsFile.close();
        }

        // 创建默认管道类型
        QJsonObject defaultPipeTypes;
        defaultPipeTypes["无缝黄铜、铜及铅管"] = QJsonObject({
            {"roughness", 0.00001},
            {"description", "引自SH/3035-2018 表6.2.4 推荐值0.000005-0.00001"}
        });

        defaultPipeTypes["操作中基本无腐蚀的无缝钢管"] = QJsonObject({
            {"roughness", 0.0001},
            {"description", "引自SH/3035-2018 表6.2.4 推荐值0.00005-0.0001"}
        });

        defaultPipeTypes["操作中有轻度腐蚀的无缝钢管"] = QJsonObject({
            {"roughness", 0.0002},
            {"description", "引自SH/3035-2018 表6.2.4 推荐值0.0001-0.0002"}
        });

        defaultPipeTypes["操作中有显著腐蚀的无缝钢管"] = QJsonObject({
            {"roughness", 0.0005},
            {"description", "引自SH/3035-2018 表6.2.4 推荐值0.0002-0.0005"}
        });

        defaultPipeTypes["铸铁管"] = QJsonObject({
            {"roughness", 0.00085},
            {"description", "引自SH/3035-2018 表6.2.4 推荐值0.0005-0.00085"}
        });

        QFile pipeTypesFile("pipe_types.json");
        if (pipeTypesFile.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(defaultPipeTypes);
            pipeTypesFile.write(doc.toJson());
            pipeTypesFile.close();
        }

        qDebug() << "默认材料文件创建成功";

    } catch (const std::exception& e) {
        qCritical() << "创建默认材料文件失败:" << e.what();
    }
}
