#include "ActivationManager.h"
#include <QDebug>
#include <QProcess>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>

#ifdef Q_OS_WIN
#include <windows.h>
#include <winreg.h>
#endif

ActivationManager::ActivationManager(QObject *parent)
    : QObject(parent)
    , secretKey("Ypipe2025")
{
}

bool ActivationManager::isActivated()
{
    try {
        QVariantMap activationData = getActivationData();
        if (activationData.isEmpty() || !activationData["activated"].toBool()) {
            return false;
        }

        QString requestCode = activationData["request_code"].toString();
        QString activationCode = activationData["activation_code"].toString();

        return verifyActivation(activationCode, requestCode);

    } catch (const std::exception& e) {
        qDebug() << "检查激活状态失败:" << e.what();
        return false;
    }
}

bool ActivationManager::validateActivation(const QString& activationCode, const QString& requestCode)
{
    if (verifyActivation(activationCode, requestCode)) {
        QVariantMap activationData;
        activationData["activated"] = true;
        activationData["request_code"] = requestCode;
        activationData["activation_code"] = activationCode;

        return saveActivationData(activationData);
    }
    return false;
}

QString ActivationManager::generateRequestCode()
{
    QString hardwareInfo = getHardwareInfo();
    QByteArray hash = QCryptographicHash::hash(hardwareInfo.toUtf8(), QCryptographicHash::Sha256);
    QString hashHex = hash.toHex().toUpper();

    // 格式化为 XXXX-XXXX-XXXX-XXXX
    requestCode = QString("%1-%2-%3-%4")
                      .arg(hashHex.mid(0, 8))
                      .arg(hashHex.mid(8, 8))
                      .arg(hashHex.mid(16, 8))
                      .arg(hashHex.mid(24, 8));

    return requestCode;
}

QString ActivationManager::generateActivationCode(const QString& requestCode)
{
    QString usedRequestCode = requestCode.isEmpty() ? this->requestCode : requestCode;
    if (usedRequestCode.isEmpty()) {
        generateRequestCode();
        usedRequestCode = this->requestCode;
    }

    QString combined = usedRequestCode + "-" + secretKey;
    QByteArray hash = QCryptographicHash::hash(combined.toUtf8(), QCryptographicHash::Sha512);
    QString hashHex = hash.toHex().toUpper();

    // 格式化为 XXXX-XXXX-XXXX-XXXX-XXXX-XXXX
    return QString("%1-%2-%3-%4-%5-%6")
        .arg(hashHex.mid(0, 8))
        .arg(hashHex.mid(8, 8))
        .arg(hashHex.mid(16, 8))
        .arg(hashHex.mid(24, 8))
        .arg(hashHex.mid(32, 8))
        .arg(hashHex.mid(40, 8));
}

bool ActivationManager::verifyActivation(const QString& activationCode, const QString& requestCode)
{
    QString generatedCode = generateActivationCode(requestCode);
    return generatedCode == activationCode;
}

int ActivationManager::getTrialCount()
{
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\YPipe", QSettings::NativeFormat);
    return settings.value("Settings/TrialCount", 0).toInt();
#else
    // 在其他平台上使用文件存储
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/YPipe.ini", QSettings::IniFormat);
    return settings.value("TrialCount", 0).toInt();
#endif
}

bool ActivationManager::updateTrialCount(int count)
{
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\YPipe", QSettings::NativeFormat);
    settings.setValue("Settings/TrialCount", count);
    return settings.status() == QSettings::NoError;
#else
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/YPipe.ini", QSettings::IniFormat);
    settings.setValue("TrialCount", count);
    return settings.status() == QSettings::NoError;
#endif
}

bool ActivationManager::saveActivationData(const QVariantMap& data)
{
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\YPipe", QSettings::NativeFormat);
    QJsonDocument doc(QJsonObject::fromVariantMap(data));
    settings.setValue("Settings/ActivationData", doc.toJson(QJsonDocument::Compact));
    return settings.status() == QSettings::NoError;
#else
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/YPipe.ini", QSettings::IniFormat);
    QJsonDocument doc(QJsonObject::fromVariantMap(data));
    settings.setValue("ActivationData", doc.toJson(QJsonDocument::Compact));
    return settings.status() == QSettings::NoError;
#endif
}

QVariantMap ActivationManager::getActivationData()
{
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\YPipe", QSettings::NativeFormat);
    QByteArray jsonData = settings.value("Settings/ActivationData").toByteArray();
#else
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/YPipe.ini", QSettings::IniFormat);
    QByteArray jsonData = settings.value("ActivationData").toByteArray();
#endif

    if (!jsonData.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        return doc.object().toVariantMap();
    }

    return QVariantMap();
}

QString ActivationManager::getHardwareInfo()
{
    QStringList info;

    // 操作系统信息
    info << QString("OS: %1 %2").arg(QSysInfo::productType()).arg(QSysInfo::productVersion());

    // 主机名
    info << QString("Hostname: %1").arg(QSysInfo::machineHostName());

#ifdef Q_OS_WIN
    // Windows CPU信息
    try {
        QProcess process;
        process.start("wmic", QStringList() << "cpu" << "get" << "Name");
        if (process.waitForFinished(3000)) {
            QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
            QStringList lines = output.split('\n', Qt::SkipEmptyParts);
            if (lines.size() > 1) {
                info << QString("CPU: %1").arg(lines[1].trimmed());
            }
        }

        // 主板序列号
        process.start("wmic", QStringList() << "baseboard" << "get" << "SerialNumber");
        if (process.waitForFinished(3000)) {
            QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
            QStringList lines = output.split('\n', Qt::SkipEmptyParts);
            if (lines.size() > 1) {
                info << QString("Motherboard SN: %1").arg(lines[1].trimmed());
            }
        }
    } catch (const std::exception& e) {
        info << QString("HW Error: %1").arg(e.what());
    }
#endif

    // MAC地址 - 重命名 interface 变量
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &netInterface : interfaces) {
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack) &&
            (netInterface.flags() & QNetworkInterface::IsUp)) {
            QString mac = netInterface.hardwareAddress();
            if (!mac.isEmpty()) {
                info << QString("MAC Address: %1").arg(mac);
                break;
            }
        }
    }

    // 机器唯一ID - 修复 Qt6 兼容性
    QString machineId;
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    machineId = QSysInfo::machineUniqueId();
#else
    // 对于旧版本 Qt，使用其他方法生成机器ID
    machineId = generateFallbackMachineId();
#endif

    if (!machineId.isEmpty()) {
        info << QString("Machine ID: %1").arg(machineId);
    }

    hardwareInfo = info.join("|");
    return hardwareInfo;
}

// 添加备用机器ID生成函数
QString ActivationManager::generateFallbackMachineId()
{
    QStringList components;

    // 使用主机名
    components << QSysInfo::machineHostName();

    // 使用产品类型和版本
    components << QSysInfo::productType();
    components << QSysInfo::productVersion();

    // 使用第一个非回环MAC地址 - 同样重命名 interface 变量
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &netInterface : interfaces) {
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack) &&
            (netInterface.flags() & QNetworkInterface::IsUp)) {
            QString mac = netInterface.hardwareAddress();
            if (!mac.isEmpty()) {
                components << mac;
                break;
            }
        }
    }

    // 生成哈希作为机器ID
    QString combined = components.join("|");
    QByteArray hash = QCryptographicHash::hash(combined.toUtf8(), QCryptographicHash::Sha256);
    return hash.toHex().left(32).toUpper();
}

QString ActivationManager::hashString(const QString& input, const QString& algorithm)
{
    QCryptographicHash::Algorithm hashAlgorithm = QCryptographicHash::Sha256;

    if (algorithm.toUpper() == "SHA512") {
        hashAlgorithm = QCryptographicHash::Sha512;
    } else if (algorithm.toUpper() == "MD5") {
        hashAlgorithm = QCryptographicHash::Md5;
    }

    QByteArray hash = QCryptographicHash::hash(input.toUtf8(), hashAlgorithm);
    return hash.toHex().toUpper();
}

#ifdef Q_OS_WIN
// Windows注册表辅助函数
QString ActivationManager::getRegistryKey(const QString& path, HKEY root)
{
    HKEY hKey;
    LONG result = RegOpenKeyExW(root,
                                reinterpret_cast<const wchar_t*>(path.utf16()),
                                0, KEY_READ, &hKey);

    if (result != ERROR_SUCCESS) {
        return QString();
    }

    wchar_t value[1024];
    DWORD valueLength = sizeof(value);
    result = RegQueryValueExW(hKey, L"", NULL, NULL,
                              reinterpret_cast<LPBYTE>(value), &valueLength);

    RegCloseKey(hKey);

    if (result == ERROR_SUCCESS) {
        return QString::fromWCharArray(value);
    }

    return QString();
}

bool ActivationManager::setRegistryValue(const QString& path, const QString& valueName,
                                         const QVariant& value, HKEY root)
{
    HKEY hKey;
    LONG result = RegCreateKeyExW(root,
                                  reinterpret_cast<const wchar_t*>(path.utf16()),
                                  0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);

    if (result != ERROR_SUCCESS) {
        return false;
    }

    bool success = false;

    switch (value.type()) {
    case QVariant::String: {
        std::wstring wstr = value.toString().toStdWString();
        result = RegSetValueExW(hKey,
                                reinterpret_cast<const wchar_t*>(valueName.utf16()),
                                0, REG_SZ,
                                reinterpret_cast<const BYTE*>(wstr.c_str()),
                                (wstr.length() + 1) * sizeof(wchar_t));
        success = (result == ERROR_SUCCESS);
        break;
    }
    case QVariant::Int: {
        DWORD dwValue = value.toInt();
        result = RegSetValueExW(hKey,
                                reinterpret_cast<const wchar_t*>(valueName.utf16()),
                                0, REG_DWORD,
                                reinterpret_cast<const BYTE*>(&dwValue),
                                sizeof(DWORD));
        success = (result == ERROR_SUCCESS);
        break;
    }
    default:
        success = false;
        break;
    }

    RegCloseKey(hKey);
    return success;
}

QVariant ActivationManager::getRegistryValue(const QString& path, const QString& valueName,
                                             HKEY root)
{
    HKEY hKey;
    LONG result = RegOpenKeyExW(root,
                                reinterpret_cast<const wchar_t*>(path.utf16()),
                                0, KEY_READ, &hKey);

    if (result != ERROR_SUCCESS) {
        return QVariant();
    }

    DWORD type;
    BYTE data[1024];
    DWORD dataSize = sizeof(data);

    result = RegQueryValueExW(hKey,
                              reinterpret_cast<const wchar_t*>(valueName.utf16()),
                              NULL, &type, data, &dataSize);

    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS) {
        return QVariant();
    }

    switch (type) {
    case REG_SZ:
        return QString::fromWCharArray(reinterpret_cast<wchar_t*>(data));
    case REG_DWORD:
        return QVariant(static_cast<uint>(*reinterpret_cast<DWORD*>(data)));
    default:
        return QVariant();
    }
}
#endif
