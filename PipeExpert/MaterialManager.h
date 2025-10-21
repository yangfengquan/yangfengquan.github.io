#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>

// 管道类型类
class PipeType {
public:
    QString name;           // 名称
    double roughness;       // 粗糙度(m)
    QString description;    // 描述
};

// 管道配件类
class PipeFitting {
public:
    QString name;               // 名称
    double resistanceCoef;      // 阻力系数
    QString description;        // 描述
};

// 保温材料类
class InsulationMaterial {
public:
    QString name;               // 名称
    QString conductivityEq1;    // 导热系数方程1
    QString conductivityEq2;    // 导热系数方程2
    QString conductivityEq3;    // 导热系数方程3
    QString range1;            // 导热系数方程1 变量取值范围
    QString range2;            // 导热系数方程2 变量取值范围
    QString range3;            // 导热系数方程3 变量取值范围
    QString density;             // 密度(kg/m³)
    QString description;        // 描述

    // 计算导热系数(W/(m·K))
    // 参数: tm_K - 平均温度(K)
    double calculateConductivity(double tm_K) const;
};

// 外保护材料类
class OuterProtection {
public:
    QString name;           // 名称
    double emissivity;      // 发射率
    QString description;    // 描述
};

// 材料管理类
class MaterialManager : public QObject
{
    Q_OBJECT

public:
    explicit MaterialManager(QObject *parent = nullptr);

    // 获取材料列表
    QMap<QString, InsulationMaterial> getInsulationMaterials() const;
    QMap<QString, OuterProtection> getProtectionMaterials() const;
    QMap<QString, PipeFitting> getPipeFittings() const;
    QMap<QString, PipeType> getPipeTypes() const;

    // 添加材料
    void addInsulationMaterial(const QString& name, const QString& eq1,
                               const QString& eq2, const QString& eq3,
                               const QString& range1, const QString& range2,
                               const QString& range3, const QString& density,
                                const QString& description);
    void addProtectionMaterial(const QString& name, double emissivity,
                               const QString& description);
    void addPipeFitting(const QString& name, double resistanceCoef,
                        const QString& description);
    void addPipeType(const QString& name, double roughness,
                     const QString& description);

    void removeInsulationMaterial(const QString& name);
    void removeProtectionMaterial(const QString& name);
    void removePipeFitting(const QString& name);
    void removePipeType(const QString& name);

    // 文件操作
    bool saveMaterialsToFile();
    bool loadMaterialsFromFile();
    void initDefaultMaterials();
    //void createDefaultMaterialFiles();

    // 获取材料路径
    static QString getMaterialFilePath(const QString& fileName);

public:
    QMap<QString, InsulationMaterial> insulationMaterials;  // 保温材料
    QMap<QString, OuterProtection> protectionMaterials;     // 外保护材料
    QMap<QString, PipeFitting> pipeFittings;                // 管道配件
    QMap<QString, PipeType> pipeTypes;                      // 管道类型
};

#endif // MATERIALMANAGER_H
