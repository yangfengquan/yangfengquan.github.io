#ifndef REGISTERFILEASSOCIATION_H
#define REGISTERFILEASSOCIATION_H
#ifdef Q_OS_WIN
#include <windows.h>
#include <QString>
#include <QCoreApplication>
#include <QSettings>
void registerFileAssociation()
{
    QString appPath = QCoreApplication::applicationFilePath().replace('/', '\\');
    QString ext = ".tang";
    QString keyName = "TangYuanFile";

    // 创建文件类型关联
    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes\\" + ext, QSettings::NativeFormat);
    settings.setValue("Default", keyName);

    QSettings settings2("HKEY_CURRENT_USER\\Software\\Classes\\" + keyName, QSettings::NativeFormat);
    settings2.setValue("Default", "My Application Document");

    QSettings settings3("HKEY_CURRENT_USER\\Software\\Classes\\" + keyName + "\\shell\\open\\command", QSettings::NativeFormat);
    settings3.setValue("Default", "\"" + appPath + "\" \"%1\"");
}
#endif
#endif // REGISTERFILEASSOCIATION_H
