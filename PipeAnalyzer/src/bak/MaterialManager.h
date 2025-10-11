#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <QObject>
#include <QMap>
#include <QVariant>

class PipeType {
public:
    QString name;
    double roughness;
    QString description;
};

class PipeFitting {
public:
    QString name;
    double resistanceCoef;
    QString description;
};

class InsulationMaterial {
public:
    QString name;
    QString conductivityEq1;
    QString conductivityEq2;
    QString conductivityEq3;
    double density;
    QString description;

    double calculateConductivity(double tmK) const;
};

class OuterProtection {
public:
    QString name;
    double emissivity;
    QString description;
};

class MaterialManager : public QObject
{
    Q_OBJECT

public:
    MaterialManager(QObject *parent = nullptr);

    // 材料访问
    QMap<QString, InsulationMaterial> getInsulationMaterials() const { //return insulationMaterials;
        qDebug() << "MaterialManager::getInsulationMaterials() 开始";

        try {
            qDebug() << "步骤1: 检查 insulationMaterials 映射...";
            qDebug() << "insulationMaterials 大小:" << insulationMaterials.size();

            qDebug() << "步骤2: 返回 insulationMaterials...";
            return insulationMaterials;

        } catch (const std::exception& e) {
            qCritical() << "MaterialManager::getInsulationMaterials 中捕获异常:" << e.what();
            throw;
        } catch (...) {
            qCritical() << "MaterialManager::getInsulationMaterials 中捕获未知异常";
            throw;
        }

        qDebug() << "MaterialManager::getInsulationMaterials() 完成";
    }
    QMap<QString, OuterProtection> getProtectionMaterials() const { //return protectionMaterials;
        qDebug() << "MaterialManager::getProtectionMaterials() 开始";
        qDebug() << "protectionMaterials 大小:" << protectionMaterials.size();
        return protectionMaterials;
    }
    QMap<QString, PipeFitting> getPipeFittings() const { return pipeFittings; }
    QMap<QString, PipeType> getPipeTypes() const { //return pipeTypes;
        qDebug() << "MaterialManager::getPipeTypes() 开始";
        qDebug() << "pipeTypes 大小:" << pipeTypes.size();
        return pipeTypes;
    }

    // 材料操作
    void addInsulationMaterial(const QString& name, const QString& eq1,
                               const QString& eq2, const QString& eq3,
                               double density, const QString& description);
    void addProtectionMaterial(const QString& name, double emissivity,
                               const QString& description);
    void addPipeFitting(const QString& name, double resistanceCoef,
                        const QString& description);
    void addPipeType(const QString& name, double roughness,
                     const QString& description);

    bool saveMaterialsToFile();
    bool loadMaterialsFromFile();
    void createDefaultMaterialFiles();

private:
    QMap<QString, InsulationMaterial> insulationMaterials;
    QMap<QString, OuterProtection> protectionMaterials;
    QMap<QString, PipeFitting> pipeFittings;
    QMap<QString, PipeType> pipeTypes;
};

#endif // MATERIALMANAGER_H
