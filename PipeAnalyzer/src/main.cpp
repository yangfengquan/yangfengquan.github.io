#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "PipeAnalyzer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // 设置应用程序信息
    a.setApplicationName("管道分析软件");
    a.setApplicationVersion("1.0");
    a.setOrganizationName("YPipe");

    try {
        qDebug() << "应用程序启动...";
        MainWindow w;
        w.show();
        qDebug() << "主窗口显示成功";
        return a.exec();
    }
    catch (const std::exception& e) {
        qCritical() << "捕获到异常:" << e.what();
        QMessageBox::critical(nullptr, "启动错误",
                              QString("程序启动时发生错误:\n%1").arg(e.what()));
        return -1;
    }
    catch (...) {
        qCritical() << "捕获到未知异常";
        QMessageBox::critical(nullptr, "启动错误", "程序启动时发生未知错误");
        return -1;
    }
}
