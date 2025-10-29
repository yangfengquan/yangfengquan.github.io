#include "mainwindow.h"
#include "registerFileAssociation.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/app_icon.ico"));

    registerFileAssociation(); // 注册关联文件

    QApplication::setApplicationName("TangYuan");
    QApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("简单的文本编辑器");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "要打开的文件");

    parser.process(a);

    const QStringList args = parser.positionalArguments();
    QString filePath;
    if (!args.isEmpty()) {
        filePath = args.at(0);
    }

    MainWindow w(filePath);
    w.show();
    return a.exec();
}
