#include "activationmanager.h"
#include <QDebug>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QProcess>
#include <QDateTime>

ActivationManager::ActivationManager(QObject *parent)
    : QObject(parent)
{
}

void ActivationManager::setRegistryPath(const QString &path)
{
    m_registryBase = path;
}

void ActivationManager::setSoftName(const QString &name)
{
    m_softName = name;
}

QString ActivationManager::getFullRegistryPath() const
{
    return QString("%1\\%2").arg(m_registryBase, m_softName);
}

bool ActivationManager::isActivated()
{
    try {
        QVariant activationData = readRegistry("Activation/Data");
        if (activationData.isNull()) {
            return false;
        }

        QJsonDocument doc = QJsonDocument::fromJson(activationData.toByteArray());
        QVariantMap data = doc.object().toVariantMap();

        if (!data.value("activated", false).toBool()) {
            return false;
        }

        QString requestCode = data.value("request_code").toString();
        QString activationCode = data.value("activation_code").toString();

        return verifyActivation(activationCode, requestCode);

    } catch (const std::exception &e) {
        qWarning() << "检查激活状态失败:" << e.what();
        return false;
    }
}

bool ActivationManager::validateActivation(const QString &activationCode, const QString &requestCode)
{
    if (verifyActivation(activationCode, requestCode)) {
        QVariantMap activationData = {
            {"activated", true},
            {"request_code", requestCode},
            {"activation_code", activationCode},
            {"activate_date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")}
        };

        QJsonDocument doc(QJsonObject::fromVariantMap(activationData));
        return writeRegistry("Activation/Data", doc.toJson(QJsonDocument::Compact));
    }
    return false;
}

QString ActivationManager::generateRequestCode()
{
    m_hardwareInfo = getHardwareInfo();
    QString hashHex = hashString(m_hardwareInfo);

    // 格式化为 XXXX-XXXX-XXXX-XXXX
    m_requestCode = formatCode(hashHex.left(16), 4);
    return m_requestCode;
}

QString ActivationManager::generateActivationCode(const QString &requestCode)
{
    QString usedRequestCode = requestCode.isEmpty() ? m_requestCode : requestCode;
    if (usedRequestCode.isEmpty()) {
        usedRequestCode = generateRequestCode();
    }

    QString combined = usedRequestCode + "-" + m_secretKey;
    QString hashHex = hashString(combined);

    // 格式化为 XXXX-XXXX-XXXX-XXXX-XXXX-XXXX
    return formatCode(hashHex.left(24), 4);
}

bool ActivationManager::verifyActivation(const QString &activationCode, const QString &requestCode)
{
    return generateActivationCode(requestCode) == activationCode;
}

int ActivationManager::getTrialCount()
{
    return readRegistry("Trial/Count", 0).toInt();
}

bool ActivationManager::incrementTrialCount()
{
    int currentCount = getTrialCount();
    return writeRegistry("Trial/Count", currentCount + 1);
}

QString ActivationManager::getHardwareInfo()
{
    QStringList components;

    // 系统信息
    components << QSysInfo::machineHostName();
    components << QSysInfo::productType();
    components << QSysInfo::productVersion();

    // 硬件信息
    QString cpuInfo = getCpuInfo();
    QString motherboardInfo = getMotherboardInfo();
    QString diskInfo = getDiskInfo();

    if (!cpuInfo.isEmpty()) components << cpuInfo;
    if (!motherboardInfo.isEmpty()) components << motherboardInfo;
    if (!diskInfo.isEmpty()) components << diskInfo;

    return components.join("|");
}

QString ActivationManager::getCpuInfo()
{
    QProcess process;
    process.start("wmic", QStringList() << "cpu" << "get" << "ProcessorId" << "/value");

    if (process.waitForFinished(2000)) {
        QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
        QStringList lines = output.split('\n', Qt::SkipEmptyParts);

        for (const QString &line : lines) {
            if (line.startsWith("ProcessorId=")) {
                return line.trimmed();
            }
        }
    }
    return "CPU_Unknown";
}

QString ActivationManager::getMotherboardInfo()
{
    QProcess process;
    process.start("wmic", QStringList() << "baseboard" << "get" << "SerialNumber" << "/value");

    if (process.waitForFinished(2000)) {
        QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
        QStringList lines = output.split('\n', Qt::SkipEmptyParts);

        for (const QString &line : lines) {
            if (line.startsWith("SerialNumber=")) {
                QString serial = line.mid(13).trimmed();
                return serial.isEmpty() ? "MB_Default" : serial;
            }
        }
    }
    return "MB_Unknown";
}

QString ActivationManager::getDiskInfo()
{
    QProcess process;
    process.start("wmic", QStringList() << "diskdrive" << "where" << "Index=0" << "get" << "SerialNumber" << "/value");

    if (process.waitForFinished(2000)) {
        QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
        QStringList lines = output.split('\n', Qt::SkipEmptyParts);

        for (const QString &line : lines) {
            if (line.startsWith("SerialNumber=")) {
                return line.trimmed();
            }
        }
    }
    return "DISK_Unknown";
}

bool ActivationManager::writeRegistry(const QString &key, const QVariant &value)
{
    QString fullPath = getFullRegistryPath();
    QSettings settings(fullPath, QSettings::NativeFormat);
    settings.setValue(key, value);
    return settings.status() == QSettings::NoError;
}

QVariant ActivationManager::readRegistry(const QString &key, const QVariant &defaultValue)
{
    QString fullPath = getFullRegistryPath();
    QSettings settings(fullPath, QSettings::NativeFormat);
    return settings.value(key, defaultValue);
}

QString ActivationManager::formatCode(const QString &code, int groupSize)
{
    QString cleanCode = code;
    cleanCode.remove('-');

    QStringList groups;
    for (int i = 0; i < cleanCode.length(); i += groupSize) {
        groups << cleanCode.mid(i, groupSize);
    }

    return groups.join('-');
}

QString ActivationManager::hashString(const QString &input)
{
    QByteArray hash = QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Sha256);
    return hash.toHex().toUpper();
}
