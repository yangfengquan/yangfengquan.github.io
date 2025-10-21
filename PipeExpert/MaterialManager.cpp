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
#include "ConditionParser.h"
#include "FormulaParser.h"
// 保温材料导热系数计算
double InsulationMaterial::calculateConductivity(double tm) const
{
    const double tm_C = tm - 273.15;  // 转换为摄氏度

    try {
        // 处理方程1
        if (!conductivityEq1.isEmpty()) {
            if(!range1.isEmpty()){
                ConditionParser condition(range1.toStdString(), "tm");
                if (condition.evaluate(tm_C)) {
                    FormulaParser formula(conductivityEq1.toStdString(), "tm");
                    return formula.evaluate(tm_C);
                }
            } else {
                // 无条件方程
                FormulaParser formula(conductivityEq1.toStdString(), "tm");
                return formula.evaluate(tm_C);
            }
        }

        // 处理方程2
        if (!conductivityEq2.isEmpty()) {
            if(!range2.isEmpty()){
                ConditionParser condition(range2.toStdString(), "tm");
                if (condition.evaluate(tm_C)) {
                    FormulaParser formula(conductivityEq2.toStdString(), "tm");
                    return formula.evaluate(tm_C);
                }
            } else {
                // 无条件方程
                FormulaParser formula(conductivityEq2.toStdString(), "tm");
                return formula.evaluate(tm_C);
            }
        }

        // 处理方程3
        if (!conductivityEq3.isEmpty()) {
            if(!range3.isEmpty()){
                ConditionParser condition(range3.toStdString(), "tm");
                if (condition.evaluate(tm_C)) {
                    FormulaParser formula(conductivityEq3.toStdString(), "tm");
                    return formula.evaluate(tm_C);
                }
            } else {
                // 无条件方程
                FormulaParser formula(conductivityEq3.toStdString(), "tm");
                return formula.evaluate(tm_C);
            }
        }

    } catch (const std::exception& e) {
        qWarning() << "导热系数计算错误:" << e.what()
                   << "材料:" << name << "温度:" << tm << "K";
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
            initDefaultMaterials();
            saveMaterialsToFile();
            //createDefaultMaterialFiles();
            //loadMaterialsFromFile();
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
                                            const QString& range1, const QString& range2,
                                            const QString& range3, const QString& density,
                                            const QString& description)
{
    InsulationMaterial material;
    material.name = name;
    material.conductivityEq1 = eq1;
    material.conductivityEq2 = eq2;
    material.conductivityEq3 = eq3;
    material.range1 = range1;
    material.range2 = range2;
    material.range3 = range3;
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

// 删除保温材料
void MaterialManager::removeInsulationMaterial(const QString& name)
{
    insulationMaterials.remove(name);
}

// 删除外保护层
void MaterialManager::removeProtectionMaterial(const QString& name)
{
    protectionMaterials.remove(name);
}

// 删除管道元件
void MaterialManager::removePipeFitting(const QString& name)
{
    pipeFittings.remove(name);
}

// 删除管道类型
void MaterialManager::removePipeType(const QString& name)
{
    pipeTypes.remove(name);
}

// 保存材料到文件
bool MaterialManager::saveMaterialsToFile()
{
    try {
        // 确保材料目录存在
        const QString dataDir = QApplication::applicationDirPath() + "/data";
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
            obj["range1"] = material.range1;
            obj["range2"] = material.range2;
            obj["range3"] = material.range3;
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
                obj["range1"].toString(),
                obj["range2"].toString(),
                obj["range3"].toString(),
                obj["density"].toString(),
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

void MaterialManager::initDefaultMaterials()
{
    addInsulationMaterial(
        "硅酸钙制品-I型-170",
        "0.0479+0.00010185*tm+9.65015e-11*tm^3",
        "",
        "",
        "tm<800",
        "",
        "",
        "170",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸钙制品-II型-170",
        "0.0479+0.00010185*tm+9.65015e-11*tm^3",
        "",
        "",
        "tm<800",
        "",
        "",
        "170",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸钙制品-I型-220",
        "0.0564+0.00007786*tm+7.8571e-8*tm^2",
        "0.0937+1.67397E-10*tm^3",
        "",
        "tm<500",
        "500<=tm<=800",
        "",
        "220",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸钙制品-II型-220",
        "0.0564+0.00007786*tm+7.8571e-8*tm^2",
        "0.0937+1.67397E-10*tm^3",
        "",
        "tm<500",
        "500<=tm<=800",
        "",
        "220",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "复合硅酸盐制品-涂料-180-200",
        "0.065+0.00017*(tm-70)",
        "",
        "",
        "",
        "",
        "",
        "180-200",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "复合硅酸盐制品-毡-60-80",
        "0.043+0.00015*(tm-70)",
        "",
        "",
        "",
        "",
        "",
        "60-80",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "复合硅酸盐制品-毡-81-130",
        "0.044+0.00015*(tm-70)",
        "",
        "",
        "",
        "",
        "",
        "81-130",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "复合硅酸盐制品-管壳-80-180",
        "0.048",
        "",
        "",
        "",
        "",
        "",
        "80-180",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "岩棉制品-毡-60-100",
        "0.0337+0.000151*tm",
        "0.0395+4.71e-5*tm+5.03e-7*tm^2",
        "",
        "-20<=tm<=100",
        "100<tm<=600",
        "",
        "80",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "岩棉制品-缝毡-80-130",
        "0.0337+0.000128*tm",
        "0.0407+2.52e-5*tm+3.34e-7*tm^2",
        "",
        "-20<=tm<=100",
        "100<tm<=600",
        "",
        "80-130",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "岩棉制品-板-60-100",
        "0.0337+0.000151*tm",
        "0.0395+4.71e-5*tm+5.03e-7*tm^2",
        "",
        "-20<=tm<=100",
        "100<tm<=600",
        "",
        "60-100",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "岩棉制品-板-101-160",
        "0.0337+0.000128*tm",
        "0.0407+2.52e-5*tm+3.34e-7*tm^2",
        "",
        "-20<=tm<=100",
        "100<tm<=600",
        "",
        "101-160",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "岩棉制品-管壳-100-150",
        "0.0314+0.000174*tm",
        "0.0384+7.13e-5*tm+3.51e-7*tm^2",
        "",
        "-20<=tm<=100",
        "100<tm<=600",
        "",
        "100-150",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-毯-24-40",
        "0.046+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "24-40",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-毯-41-120",
        "0.041+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "41-120",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-板-24",
        "0.047+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "24",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-板-32",
        "0.044+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "32",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-板-40",
        "0.042+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "40",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-板-48",
        "0.041+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "48",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-板-64",
        "0.040+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "64",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-毡-24",
        "0.046+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "24",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-毡-32",
        "0.046+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "32",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-毡-40",
        "0.046+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "40",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-毡-48",
        "0.041+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "48",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "玻璃棉制品-管壳-48",
        "0.041+0.00017*tm",
        "",
        "",
        "-20<=tm<=220",
        "",
        "",
        "48",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸铝棉及其制品-1#毯-96",
        "0.044+0.0002*(tm-70)",
        "0.11+0.00036*(tm-400)",
        "",
        "tm<=400",
        "tm>400",
        "",
        "96",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸铝棉及其制品-1#毯-128",
        "0.044+0.0002*(tm-70)",
        "0.11+0.00036*(tm-400)",
        "",
        "tm<=400",
        "tm>400",
        "",
        "128",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸铝棉及其制品-2#毯-96",
        "0.044+0.0002*(tm-70)",
        "0.11+0.00036*(tm-400)",
        "",
        "tm<=400",
        "tm>400",
        "",
        "96",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸铝棉及其制品-2#毯-128",
        "0.044+0.0002*(tm-70)",
        "0.11+0.00036*(tm-400)",
        "",
        "tm<=400",
        "tm>400",
        "",
        "128",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸铝棉及其制品-1#毡-200",
        "0.044+0.0002*(tm-70)",
        "0.11+0.00036*(tm-400)",
        "",
        "tm<=400",
        "tm>400",
        "",
        "≤200",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸铝棉及其制品-2#毡-200",
        "0.044+0.0002*(tm-70)",
        "0.11+0.00036*(tm-400)",
        "",
        "tm<=400",
        "tm>400",
        "",
        "≤200",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸铝棉及其制品-板-220",
        "0.044+0.0002*(tm-70)",
        "0.11+0.00036*(tm-400)",
        "",
        "tm<=400",
        "tm>400",
        "",
        "≤220",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸铝棉及其制品-管壳-220",
        "0.044+0.0002*(tm-70)",
        "0.11+0.00036*(tm-400)",
        "",
        "tm<=400",
        "tm>400",
        "",
        "≤220",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸铝棉及其制品-树脂结合毡-128",
        "0.044+0.0002*(tm-70)",
        "",
        "",
        "",
        "",
        "",
        "128",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸镁纤维毯-100",
        "0.0397-2.741e-6*tm+4.526e-7*tm^2",
        "",
        "",
        "70<=tm<=500",
        "",
        "",
        "100±10",
        "引自GB/50264-2013 附录A 表A.0.1"
        );
    addInsulationMaterial(
        "硅酸镁纤维毯-130",
        "0.0397-2.741e-6*tm+4.526e-7*tm^2",
        "",
        "",
        "70<=tm<=500",
        "",
        "",
        "130±10",
        "引自GB/50264-2013 附录A 表A.0.1"
        );

    addProtectionMaterial("铝合金薄板", 0.3, "引自GB/50264-2013 表5.8.9");
    addProtectionMaterial("不锈钢薄板", 0.4, "引自GB/50264-2013 表5.8.9");
    addProtectionMaterial("有光泽的镀钵薄钢板", 0.27, "引自GB/50264-2013 表5.8.9");
    addProtectionMaterial("已氧化的镀钵薄钢板", 0.32, "引自GB/50264-2013 表5.8.9");
    addProtectionMaterial("纤维织物", 0.8, "引自GB/50264-2013 表5.8.9");
    addProtectionMaterial("水泥砂浆", 0.69, "引自GB/50264-2013 表5.8.9");
    addProtectionMaterial("铝粉漆", 0.41, "引自GB/50264-2013 表5.8.9");
    addProtectionMaterial("黑漆(有光泽)", 0.88, "引自GB/50264-2013 表5.8.9");
    addProtectionMaterial("黑漆(无光泽)", 0.96, "引自GB/50264-2013 表5.8.9");
    addProtectionMaterial("油漆", 0.9, "引自GB/50264-2013 表5.8.9");

    addPipeFitting("45°标准弯头", 0.35, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("90°标准弯头", 0.75, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("90°斜接弯头", 1.3, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("180°标准弯头", 1.5, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("等径三通(流出)", 1.2, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("等径三通(流入)", 1.8, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("截止阀(全开)", 6.0, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("角阀(全开)", 3.0, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("闸阀(全开)", 0.17, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("旋塞阀(全开)", 0.05, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("蝶阀(全开)", 0.24, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("旋启式止回阀", 2.0, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("升降式止回阀", 10.0, "引自SH/3035-2018 表6.2.5");
    addPipeFitting("底阀", 15.0, "引自SH/3035-2018 表6.2.5");
    addPipeType("无缝黄铜、铜及铅管", 0.00001, "引自SH/3035-2018 表6.2.4");
    addPipeType("操作中基本无腐蚀的无缝钢管", 0.0001, "引自SH/3035-2018 表6.2.4");
    addPipeType("操作中有轻度腐蚀的无缝钢管",  0.0002, "引自SH/3035-2018 表6.2.4");
    addPipeType("操作中有显著腐蚀的无缝钢管", 0.0005, "引自SH/3035-2018 表6.2.4");
    addPipeType("铜板卷管", 0.00033, "引自SH/3035-2018 表6.2.4");
    addPipeType("铸铁管", 0.00085, "引自SH/3035-2018 表6.2.4");
    addPipeType("干净的玻璃管",  0.00001, "引自SH/3035-2018 表6.2.4");
}

/*
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
            {"conductivity_eq1", "0.0479+0.00010185*tm+9.65015e-11*tm^3"},
            {"conductivity_eq2", ""},
            {"conductivity_eq3", ""},
            {"range1", "tm<800"},
            {"range2", ""},
            {"range3", ""},
            {"density", 170},
            {"description", "引自GB/50264-2013 附录A 表A.0.1"}
        });

        defaultInsulation["硅酸钙制品-I型-220"] = QJsonObject({
            {"conductivity_eq1", "0.0564+0.00007786*tm+7.8571e-8*tm^2"},
            {"conductivity_eq2", "0.0937+1.67397E-10*tm^3"},
            {"conductivity_eq3", ""},
            {"range1", "tm<500"},
            {"range2", "500<=tm<=800"},
            {"range3", ""},
            {"density", 220},
            {"description", "引自GB/50264-2013 附录A 表A.0.1"}
        });

        defaultInsulation["岩棉制品-毡-60-100"] = QJsonObject({
            {"conductivity_eq1", "0.0337+0.000151*tm"},
            {"conductivity_eq2", "0.0395+4.71e-5*tm+5.03e-7*tm^2"},
            {"conductivity_eq3", ""},
            {"range1", "-20<=tm<=100"},
            {"range2", "100<tm<=600"},
            {"range3", ""},
            {"density", 80},
            {"description", "引自GB/50264-2013 附录A 表A.0.1"}
        });

        QFile file_insulation_materials(getMaterialFilePath("insulation_materials.json"));
        // Open the file for writing
        if (file_insulation_materials.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file_insulation_materials.write(QJsonDocument(defaultInsulation).toJson(QJsonDocument::Indented));
            file_insulation_materials.close();
        } else {
            qDebug() << "Could not open file for writing:" << file_insulation_materials.errorString();
        }

        // 创建默认外保护层材料
        QJsonObject defaultProtection;
        defaultProtection["铝合金薄板"] = QJsonObject({{"emissivity", 0.3}, {"description", "引自GB/50264-2013 表5.8.9"}});
        defaultProtection["不锈钢薄板"] = QJsonObject({{"emissivity", 0.4}, {"description", "引自GB/50264-2013 表5.8.9"}});
        defaultProtection["有光泽的镀钵薄钢板"] = QJsonObject({{"emissivity", 0.27}, {"description", "引自GB/50264-2013 表5.8.9"}});
        defaultProtection["油漆"] = QJsonObject({{"emissivity", 0.90}, {"description", "引自GB/50264-2013 表5.8.9"}});

        QFile file_protection_materials(getMaterialFilePath("protection_materials.json"));
        if (file_protection_materials.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // Write your data here
            file_protection_materials.write(QJsonDocument(defaultProtection).toJson(QJsonDocument::Indented));
            file_protection_materials.close(); // Don't forget to close the file
        } else {
            qDebug() << "Could not open file for writing:" << file_protection_materials.errorString();
        }

        // 创建默认管道元件
        QJsonObject defaultFittings;
        defaultFittings["45°标准弯头"] = QJsonObject({{"resistance_coef", 0.35}, {"description", "引自SH/3035-2018 表6.2.5"}});
        defaultFittings["90°标准弯头"] = QJsonObject({{"resistance_coef", 0.75}, {"description", "引自SH/3035-2018 表6.2.5"}});
        defaultFittings["90°斜接弯头"] = QJsonObject({{"resistance_coef", 1.3}, {"description", "引自SH/3035-2018 表6.2.5"}});
        defaultFittings["等径三通(流出)"] = QJsonObject({{"resistance_coef", 1.2}, {"description", "引自SH/3035-2018 表6.2.5"}});
        defaultFittings["截止阀(全开)"] = QJsonObject({{"resistance_coef", 6.0}, {"description", "引自SH/3035-2018 表6.2.5"}});
        defaultFittings["闸阀(全开)"] = QJsonObject({{"resistance_coef", 0.17}, {"description", "引自SH/3035-2018 表6.2.5"}});

        QFile file_pipe_fittings(getMaterialFilePath("pipe_fittings.json"));
        if (file_pipe_fittings.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // Write your data here
            file_pipe_fittings.write(QJsonDocument(defaultFittings).toJson(QJsonDocument::Indented));
            file_pipe_fittings.close(); // Don't forget to close the file
        } else {
            qDebug() << "Could not open file for writing:" << file_pipe_fittings.errorString();
        }

        // 创建默认管道类型
        QJsonObject defaultPipeTypes;
        defaultPipeTypes["无缝黄铜、铜及铅管"] = QJsonObject({{"roughness", 0.00001}, {"description", "引自SH/3035-2018 表6.2.4"}});
        defaultPipeTypes["操作中基本无腐蚀的无缝钢管"] = QJsonObject({{"roughness", 0.0001}, {"description", "引自SH/3035-2018 表6.2.4"}});
        defaultPipeTypes["操作中有轻度腐蚀的无缝钢管"] = QJsonObject({{"roughness", 0.0002}, {"description", "引自SH/3035-2018 表6.2.4"}});
        defaultPipeTypes["操作中有显著腐蚀的无缝钢管"] = QJsonObject({{"roughness", 0.0005}, {"description", "引自SH/3035-2018 表6.2.4"}});
        defaultPipeTypes["铸铁管"] = QJsonObject({{"roughness", 0.00085}, {"description", "引自SH/3035-2018 表6.2.4"}});

        QFile file_pipe_types(getMaterialFilePath("pipe_types.json"));
        if (file_pipe_types.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // Write your data here
            file_pipe_types.write(QJsonDocument(defaultPipeTypes).toJson(QJsonDocument::Indented));
            file_pipe_types.close(); // Don't forget to close the file
        } else {
            qDebug() << "Could not open file for writing:" << file_pipe_types.errorString();
        }

        qDebug() << "默认材料文件创建成功";

    } catch (const std::exception& e) {
        qCritical() << "创建默认材料文件失败:" << e.what();
    }
}
*/
// 获取材料文件路径
QString MaterialManager::getMaterialFilePath(const QString& fileName)
{
    return QApplication::applicationDirPath() + "/data/" + fileName;
}
