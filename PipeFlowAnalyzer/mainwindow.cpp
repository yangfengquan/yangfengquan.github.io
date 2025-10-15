#include "mainwindow.h"
#include "FluidAnalyzer.h"
#include "report.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //MaterialManager m;
    FluidAnalyzer analyer;
    std::map<std::string, int> f = {
                                    {"45°标准弯头",2},
        {"90°标准弯头",2}
    };
    FluidAnalyzer::AnalysisResult res = analyer.analyzePipe("Water", 10000/3600.0, 1e6, 300+273.15, 500, 0.168, 0.009, 0.05, "操作中有轻度腐蚀的无缝钢管",
                        "硅酸钙制品-I型-170", "铝合金薄板", 20+273.15, 3, 50, f);

    qDebug()<<res.outletPressure/1e6<<res.outletTemperature-273.15;
    ResultFormatter formatter;
    std::string outputFile = "pipeline_analysis_output.txt";

    if (formatter.formatToFile(res, outputFile)) {
        std::cout << "结果已成功输出到文件: " << outputFile << std::endl;
        formatter.openWithNotepad(outputFile);
    } else {
        std::cerr << "无法创建输出文件" << std::endl;
    }
}

MainWindow::~MainWindow() {}
