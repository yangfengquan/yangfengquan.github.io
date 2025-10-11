#ifndef ACTIVATIONMANAGER_H
#define ACTIVATIONMANAGER_H

#include <QObject>
#include <QString>
#include <QVariantMap>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class ActivationManager : public QObject
{
    Q_OBJECT

public:
    ActivationManager(QObject *parent = nullptr);

    bool isActivated();
    bool validateActivation(const QString& activationCode, const QString& requestCode);
    QString generateRequestCode();
    QString generateActivationCode(const QString& requestCode);

    int getTrialCount();
    bool updateTrialCount(int count);
    bool saveActivationData(const QVariantMap& data);
    QVariantMap getActivationData();
    bool verifyActivation(const QString& activationCode, const QString& requestCode);

private:
    QString secretKey;
    QString hardwareInfo;
    QString requestCode;

#ifdef Q_OS_WIN
    QString getRegistryKey(const QString& path, HKEY root = HKEY_CURRENT_USER);
    bool setRegistryValue(const QString& path, const QString& valueName,
                          const QVariant& value, HKEY root = HKEY_CURRENT_USER);
    QVariant getRegistryValue(const QString& path, const QString& valueName,
                              HKEY root = HKEY_CURRENT_USER);
#endif

    QString getHardwareInfo();
    QString hashString(const QString& input, const QString& algorithm = "SHA256");
    QString generateFallbackMachineId();
};

#endif // ACTIVATIONMANAGER_H
