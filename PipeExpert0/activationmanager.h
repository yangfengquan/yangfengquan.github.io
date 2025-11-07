// activationmanager.h
#ifndef ACTIVATIONMANAGER_H
#define ACTIVATIONMANAGER_H

#include <QObject>
#include <QString>
#include <QVariantMap>

class ActivationManager : public QObject
{
    Q_OBJECT

public:
    explicit ActivationManager(QObject *parent = nullptr);

    // 配置
    void setRegistryPath(const QString &path);
    void setSoftName(const QString &name);

    // 激活状态管理
    bool isActivated();
    bool validateActivation(const QString &activationCode, const QString &requestCode);

    // 代码生成
    QString generateRequestCode();
    QString generateActivationCode(const QString &requestCode = QString());

    // 试用管理
    int getTrialCount();
    bool incrementTrialCount();

private:
    // 验证
    bool verifyActivation(const QString &activationCode, const QString &requestCode);

    // 硬件信息
    QString getHardwareInfo();
    QString getCpuInfo();
    QString getMotherboardInfo();
    QString getDiskInfo();

    // 注册表操作
    bool writeRegistry(const QString &key, const QVariant &value);
    QVariant readRegistry(const QString &key, const QVariant &defaultValue = QVariant());
    QString getFullRegistryPath() const;

    // 工具函数
    QString formatCode(const QString &code, int groupSize = 4);
    QString hashString(const QString &input);

private:
    const QString m_secretKey = "TY2025";
    QString m_softName = "tangyuan";
    QString m_registryBase = "HKEY_CURRENT_USER\\Software";
    QString m_hardwareInfo;
    QString m_requestCode;
};

#endif // ACTIVATIONMANAGER_H
