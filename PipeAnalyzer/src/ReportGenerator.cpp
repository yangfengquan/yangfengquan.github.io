#include "ReportGenerator.h"
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextFrame>
#include <QTextFrameFormat>
#include <QTextLength>
#include <QFont>
#include <QFontMetrics>
#include <QPrinter>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

ReportGenerator::ReportGenerator(QObject *parent)
    : QObject(parent)
    , m_includeCharts(true)
    , m_includeDetailedData(true)
    , m_maxTableRows(100)
    , m_numberFormat("%.6f")
    , m_dateFormat("yyyy-MM-dd hh:mm:ss")
{
}

bool ReportGenerator::generatePipeReport(const QVariantMap& params,
                                         const QList<QVariantMap>& segmentResults,
                                         const QVariantMap& analysisResults,
                                         QChart* chart,
                                         const QString& filename)
{
    try {
        // 创建文本文档
        QTextDocument document;
        QTextCursor cursor(&document);

        // 设置文档基本结构
        setupDocument(&document, cursor);

        // 添加报告标题
        addTitle(cursor, params["pipe_name"].toString());

        // 添加项目基本信息
        addBasicInfoTable(cursor, params);

        // 添加输入参数
        addInputParamsTable(cursor, params);

        // 添加计算结果汇总
        addResultsSummaryTable(cursor, analysisResults);

        // 添加技术分析
        addTechnicalAnalysis(cursor, analysisResults);

        // 添加结论与建议
        addConclusions(cursor, analysisResults);

        // 添加详细分段数据
        if (m_includeDetailedData) {
            addSegmentDataTable(cursor, segmentResults);
        }

        // 根据文件扩展名选择输出格式
        QFileInfo fileInfo(filename);
        QString extension = fileInfo.suffix().toLower();

        bool success = false;
        if (extension == "pdf") {
            success = saveAsPdfDocument(&document, filename);
        } else if (extension == "html" || extension == "htm") {
            success = saveAsHtmlDocument(&document, filename);
        } else {
            // 默认保存为Word兼容的格式
            success = saveAsWordDocument(&document, filename);
        }

        if (success) {
            qDebug() << "报告生成成功:" << filename;
            return true;
        } else {
            qDebug() << "报告生成失败:" << filename;
            return false;
        }

    } catch (const std::exception& e) {
        qCritical() << "生成报告时发生错误:" << e.what();
        QMessageBox::critical(nullptr, "错误", QString("生成报告失败: %1").arg(e.what()));
        return false;
    }
}

bool ReportGenerator::generateTextReport(const QVariantMap& params,
                                         const QList<QVariantMap>& segmentResults,
                                         const QVariantMap& analysisResults,
                                         const QString& filename)
{
    try {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false;
        }

        QTextStream stream(&file);
        //stream.setCodec("UTF-8");

        // 报告标题
        stream << "管道分析报告\n";
        stream << "=" << QString("=").repeated(50) << "\n\n";

        // 基本信息
        stream << "一、项目基本信息\n";
        stream << QString(20,QChar('-')) << "\n";
        stream << "项目名称: " << params["pipe_name"].toString() << "\n";
        stream << "分析日期: " << QDateTime::currentDateTime().toString(m_dateFormat) << "\n";
        stream << "流体介质: " << params["fluid"].toString() << "\n";
        stream << "管道总长度: " << params["pipe_length_m"].toString() << " m\n";
        stream << "管道外径: " << params["pipe_od_m"].toDouble() * 1000 << " mm\n";
        stream << "管道壁厚: " << params["pipe_wall_thickness_m"].toDouble() * 1000 << " mm\n\n";

        // 输入参数
        stream << "二、输入参数\n";
        stream << QString(20,QChar('-')) << "\n";
        stream << "入口压力: " << formatPressure(params["inlet_pressure_pa"].toDouble()) << " MPa\n";
        stream << "入口温度: " << formatTemperature(params["inlet_temperature_k"].toDouble()) << " °C\n";
        stream << "质量流量: " << params["mass_flow_kg_s"].toDouble() * 3600 << " kg/h\n";
        stream << "保温材料: " << params["insulation_material"].toString() << "\n";
        stream << "保温厚度: " << params["insulation_thickness_m"].toDouble() * 1000 << " mm\n";
        stream << "外保护层: " << params["protection_material"].toString() << "\n";
        stream << "环境温度: " << formatTemperature(params["ambient_temperature_k"].toDouble()) << " °C\n";
        stream << "风速: " << params["wind_speed_m_s"].toString() << " m/s\n\n";

        // 计算结果汇总
        stream << "三、计算结果汇总\n";
        stream << QString(20,QChar('-')) << "\n";
        stream << "出口压力: " << formatPressure(analysisResults["outlet_pressure_pa"].toDouble()) << " MPa\n";
        stream << "出口温度: " << formatTemperature(analysisResults["outlet_temperature_k"].toDouble()) << " °C\n";
        stream << "出口干度: " << QString::number(analysisResults["outlet_quality"].toDouble(), 'f', 4) << "\n";
        stream << "总压力降: " << formatPressure(analysisResults["total_pressure_drop_pa"].toDouble()) << " MPa\n";
        stream << "单位压降: " << QString::number(analysisResults["pressure_drop_pa_m"].toDouble() / 1000, 'f', 1) << " kPa/km\n";
        stream << "压降比例: " << QString::number(analysisResults["pressure_ratio"].toDouble() * 100, 'f', 2) << "%\n";
        stream << "总热损失: " << QString::number(analysisResults["total_heat_loss_w"].toDouble() / 1000, 'f', 1) << " kW\n";
        stream << "平均单位热损失: " << QString::number(analysisResults["avg_heat_loss_per_m_w"].toDouble(), 'f', 1) << " W/m\n";
        stream << "最大表面温度: " << formatTemperature(analysisResults["max_surface_temp_k"].toDouble()) << " °C\n";
        stream << "入口流速: " << QString::number(analysisResults["inlet_velocity_m_s"].toDouble(), 'f', 2) << " m/s\n";
        stream << "出口流速: " << QString::number(analysisResults["outlet_velocity_m_s"].toDouble(), 'f', 2) << " m/s\n\n";

        // 技术分析
        stream << "四、技术分析\n";
        stream << QString(20,QChar('-')) << "\n";

        double pressureRatio = analysisResults["pressure_ratio"].toDouble();
        double avgHeatLoss = analysisResults["avg_heat_loss_per_m_w"].toDouble();
        double maxSurfaceTemp = analysisResults["max_surface_temp_k"].toDouble() - 273.15;

        stream << "1. 压力损失分析:\n";
        stream << "   总压力损失为 " << formatPressure(analysisResults["total_pressure_drop_pa"].toDouble()) << " MPa，";
        if (pressureRatio < 0.05) {
            stream << "在可接受范围内。\n";
        } else {
            stream << "偏高，建议优化管道设计。\n";
        }

        stream << "2. 热损失分析:\n";
        stream << "   平均单位长度热损失为 " << QString::number(avgHeatLoss, 'f', 1) << " W/m，";
        if (avgHeatLoss < 50) {
            stream << "保温效果良好。\n";
        } else {
            stream << "热损失较大，建议增加保温厚度。\n";
        }

        stream << "3. 安全性能评价:\n";
        stream << "   最大表面温度为 " << QString::number(maxSurfaceTemp, 'f', 1) << " °C，";
        if (maxSurfaceTemp < 60) {
            stream << "符合安全要求。\n";
        } else {
            stream << "表面温度较高，需采取防护措施。\n";
        }
        stream << "\n";

        // 分段数据（仅显示部分）
        if (m_includeDetailedData && !segmentResults.isEmpty()) {
            stream << "五、分段计算结果（部分）\n";
            stream << QString(20,QChar('-')) << "\n";

            // 表头
            stream << QString("%1 %2 %3 %4 %5 %6 %7 %8 %9\n")
                          .arg("段", 4)
                          .arg("距离(m)", 8)
                          .arg("压力(MPa)", 12)
                          .arg("温度(°C)", 10)
                          .arg("干度", 8)
                          .arg("流速(m/s)", 10)
                          .arg("热损失(W/m)", 12)
                          .arg("热损失(W/m²)", 14)
                          .arg("表面温度(°C)", 12);

            stream << QString(100,QChar('-')) << "\n";

            // 数据行（每10行显示一次）
            for (int i = 0; i < segmentResults.size(); ++i) {
                if (i % 10 == 0 || i == segmentResults.size() - 1) {
                    const QVariantMap& row = segmentResults[i];
                    stream << QString("%1 %2 %3 %4 %5 %6 %7 %8 %9\n")
                                  .arg(row["segment"].toInt(), 4)
                                  .arg(QString::number(row["distance_m"].toDouble(), 'f', 0), 8)
                                  .arg(formatPressure(row["pressure_pa"].toDouble()), 12)
                                  .arg(formatTemperature(row["temperature_k"].toDouble()), 10)
                                  .arg(QString::number(row["quality"].toDouble(), 'f', 4), 8)
                                  .arg(QString::number(row["velocity_m_s"].toDouble(), 'f', 2), 10)
                                  .arg(QString::number(row["heat_loss_per_m_w"].toDouble(), 'f', 1), 12)
                                  .arg(QString::number(row["heat_loss_per_area_w"].toDouble(), 'f', 1), 14)
                                  .arg(formatTemperature(row["surface_temp_k"].toDouble()), 12);
                }
            }
        }

        file.close();
        return true;

    } catch (const std::exception& e) {
        qCritical() << "生成文本报告时发生错误:" << e.what();
        return false;
    }
}

void ReportGenerator::setupDocument(QTextDocument* document, QTextCursor& cursor)
{
    // 设置文档页边距
    QTextFrame* rootFrame = document->rootFrame();
    QTextFrameFormat frameFormat = rootFrame->frameFormat();
    frameFormat.setMargin(50); // 50像素页边距
    rootFrame->setFrameFormat(frameFormat);

    // 设置默认字体
    QFont defaultFont("宋体", 10);
    document->setDefaultFont(defaultFont);
}

void ReportGenerator::addTitle(QTextCursor& cursor, const QString& pipeName)
{
    QTextCharFormat titleFormat = createTitleFormat();
    cursor.setCharFormat(titleFormat);

    cursor.insertText("管道系统分析报告\n");

    QTextBlockFormat centerFormat;
    centerFormat.setAlignment(Qt::AlignCenter);
    cursor.setBlockFormat(centerFormat);

    QTextCharFormat subtitleFormat = createHeadingFormat();
    cursor.setCharFormat(subtitleFormat);
    cursor.insertText(QString("管道名称: %1\n").arg(pipeName));

    // 添加分析日期
    cursor.setCharFormat(createBodyFormat());
    cursor.insertText(QString("分析日期: %1\n\n").arg(QDateTime::currentDateTime().toString(m_dateFormat)));

    // 恢复左对齐
    QTextBlockFormat leftFormat;
    leftFormat.setAlignment(Qt::AlignLeft);
    cursor.setBlockFormat(leftFormat);
}

void ReportGenerator::addBasicInfoTable(QTextCursor& cursor, const QVariantMap& params)
{
    cursor.insertText("一、项目基本信息\n");
    cursor.setCharFormat(createHeadingFormat());

    QTextTableFormat tableFormat = createTableFormat();
    QTextTable* table = cursor.insertTable(6, 2, tableFormat);

    // 表头格式
    QTextCharFormat headerFormat;
    headerFormat.setFontWeight(QFont::Bold);
    headerFormat.setFontPointSize(10);

    // 数据格式
    QTextCharFormat dataFormat = createBodyFormat();

    // 填充表格数据
    QStringList headers = {"项目", "数值"};
    for (int col = 0; col < 2; ++col) {
        QTextTableCell cell = table->cellAt(0, col);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(headerFormat);
        cellCursor.insertText(headers[col]);
    }

    QList<QPair<QString, QString>> basicData = {
        {"项目名称", params["pipe_name"].toString()},
        {"分析日期", QDateTime::currentDateTime().toString(m_dateFormat)},
        {"流体介质", params["fluid"].toString()},
        {"管道总长度", QString("%1 m").arg(params["pipe_length_m"].toString())},
        {"管道外径", QString("%1 mm").arg(params["pipe_od_m"].toDouble() * 1000)},
        {"管道壁厚", QString("%1 mm").arg(params["pipe_wall_thickness_m"].toDouble() * 1000)}
    };

    for (int row = 1; row <= 6; ++row) {
        for (int col = 0; col < 2; ++col) {
            QTextTableCell cell = table->cellAt(row, col);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.setCharFormat(dataFormat);

            if (col == 0) {
                cellCursor.insertText(basicData[row-1].first);
            } else {
                cellCursor.insertText(basicData[row-1].second);
            }
        }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.insertText("\n");
}

void ReportGenerator::addInputParamsTable(QTextCursor& cursor, const QVariantMap& params)
{
    cursor.insertText("二、输入参数\n");
    cursor.setCharFormat(createHeadingFormat());

    QTextTableFormat tableFormat = createTableFormat();
    QTextTable* table = cursor.insertTable(8, 2, tableFormat);

    QTextCharFormat headerFormat;
    headerFormat.setFontWeight(QFont::Bold);
    headerFormat.setFontPointSize(10);

    QTextCharFormat dataFormat = createBodyFormat();

    // 表头
    QStringList headers = {"参数", "数值"};
    for (int col = 0; col < 2; ++col) {
        QTextTableCell cell = table->cellAt(0, col);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(headerFormat);
        cellCursor.insertText(headers[col]);
    }

    QList<QPair<QString, QString>> inputData = {
        {"入口压力", formatPressure(params["inlet_pressure_pa"].toDouble()) + " MPa"},
        {"入口温度", formatTemperature(params["inlet_temperature_k"].toDouble()) + " °C"},
        {"质量流量", QString("%1 kg/h").arg(params["mass_flow_kg_s"].toDouble() * 3600, 0, 'f', 1)},
        {"保温材料", params["insulation_material"].toString()},
        {"保温厚度", QString("%1 mm").arg(params["insulation_thickness_m"].toDouble() * 1000)},
        {"外保护层", params["protection_material"].toString()},
        {"环境温度", formatTemperature(params["ambient_temperature_k"].toDouble()) + " °C"},
        {"风速", QString("%1 m/s").arg(params["wind_speed_m_s"].toString())}
    };

    for (int row = 1; row <= 8; ++row) {
        for (int col = 0; col < 2; ++col) {
            QTextTableCell cell = table->cellAt(row, col);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.setCharFormat(dataFormat);

            if (col == 0) {
                cellCursor.insertText(inputData[row-1].first);
            } else {
                cellCursor.insertText(inputData[row-1].second);
            }
        }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.insertText("\n");
}

void ReportGenerator::addResultsSummaryTable(QTextCursor& cursor, const QVariantMap& analysisResults)
{
    cursor.insertText("三、计算结果汇总\n");
    cursor.setCharFormat(createHeadingFormat());

    QTextTableFormat tableFormat = createTableFormat();
    QTextTable* table = cursor.insertTable(11, 2, tableFormat);

    QTextCharFormat headerFormat;
    headerFormat.setFontWeight(QFont::Bold);
    headerFormat.setFontPointSize(10);

    QTextCharFormat dataFormat = createBodyFormat();

    // 表头
    QStringList headers = {"计算结果", "数值"};
    for (int col = 0; col < 2; ++col) {
        QTextTableCell cell = table->cellAt(0, col);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(headerFormat);
        cellCursor.insertText(headers[col]);
    }

    QList<QPair<QString, QString>> resultData = {
        {"出口压力", formatPressure(analysisResults["outlet_pressure_pa"].toDouble()) + " MPa"},
        {"出口温度", formatTemperature(analysisResults["outlet_temperature_k"].toDouble()) + " °C"},
        {"出口干度", QString::number(analysisResults["outlet_quality"].toDouble(), 'f', 4)},
        {"总压力降", formatPressure(analysisResults["total_pressure_drop_pa"].toDouble()) + " MPa"},
        {"单位压降", QString::number(analysisResults["pressure_drop_pa_m"].toDouble() / 1000, 'f', 1) + " kPa/km"},
        {"压降比例", QString::number(analysisResults["pressure_ratio"].toDouble() * 100, 'f', 2) + "%"},
        {"总热损失", QString::number(analysisResults["total_heat_loss_w"].toDouble() / 1000, 'f', 1) + " kW"},
        {"平均单位热损失", QString::number(analysisResults["avg_heat_loss_per_m_w"].toDouble(), 'f', 1) + " W/m"},
        {"最大表面温度", formatTemperature(analysisResults["max_surface_temp_k"].toDouble()) + " °C"},
        {"入口流速", QString::number(analysisResults["inlet_velocity_m_s"].toDouble(), 'f', 2) + " m/s"},
        {"出口流速", QString::number(analysisResults["outlet_velocity_m_s"].toDouble(), 'f', 2) + " m/s"}
    };

    for (int row = 1; row <= 11; ++row) {
        for (int col = 0; col < 2; ++col) {
            QTextTableCell cell = table->cellAt(row, col);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.setCharFormat(dataFormat);

            if (col == 0) {
                cellCursor.insertText(resultData[row-1].first);
            } else {
                cellCursor.insertText(resultData[row-1].second);
            }
        }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.insertText("\n");
}

void ReportGenerator::addSegmentDataTable(QTextCursor& cursor, const QList<QVariantMap>& segmentResults)
{
    if (segmentResults.isEmpty()) {
        return;
    }

    cursor.insertText("四、详细分段计算结果\n");
    cursor.setCharFormat(createHeadingFormat());

    // 限制表格行数
    int displayRows = qMin(segmentResults.size(), m_maxTableRows);

    QTextTableFormat tableFormat = createTableFormat();
    tableFormat.setColumnWidthConstraints({
        QTextLength(QTextLength::PercentageLength, 5),   // 段
        QTextLength(QTextLength::PercentageLength, 8),   // 距离
        QTextLength(QTextLength::PercentageLength, 12),  // 压力
        QTextLength(QTextLength::PercentageLength, 10),  // 温度
        QTextLength(QTextLength::PercentageLength, 8),   // 干度
        QTextLength(QTextLength::PercentageLength, 10),  // 流速
        QTextLength(QTextLength::PercentageLength, 12),  // 热损失(W/m)
        QTextLength(QTextLength::PercentageLength, 14),  // 热损失(W/m²)
        QTextLength(QTextLength::PercentageLength, 12)   // 表面温度
    });

    QTextTable* table = cursor.insertTable(displayRows + 1, 9, tableFormat);

    QTextCharFormat headerFormat;
    headerFormat.setFontWeight(QFont::Bold);
    headerFormat.setFontPointSize(9);

    QTextCharFormat dataFormat = createBodyFormat();
    dataFormat.setFontPointSize(8);

    // 表头
    QStringList headers = {
        "段", "距离(m)", "压力(MPa)", "温度(°C)", "干度",
        "流速(m/s)", "热损失(W/m)", "热损失(W/m²)", "表面温度(°C)"
    };

    for (int col = 0; col < 9; ++col) {
        QTextTableCell cell = table->cellAt(0, col);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(headerFormat);
        cellCursor.insertText(headers[col]);
    }

    // 数据行（每10行显示一次）
    int tableRow = 1;
    for (int i = 0; i < segmentResults.size(); ++i) {
        if (i % 10 == 0 || i == segmentResults.size() - 1) {
            if (tableRow > displayRows) break;

            const QVariantMap& row = segmentResults[i];

            QStringList rowData = {
                QString::number(row["segment"].toInt()),
                QString::number(row["distance_m"].toDouble(), 'f', 0),
                formatPressure(row["pressure_pa"].toDouble()),
                formatTemperature(row["temperature_k"].toDouble()),
                QString::number(row["quality"].toDouble(), 'f', 4),
                QString::number(row["velocity_m_s"].toDouble(), 'f', 2),
                QString::number(row["heat_loss_per_m_w"].toDouble(), 'f', 1),
                QString::number(row["heat_loss_per_area_w"].toDouble(), 'f', 1),
                formatTemperature(row["surface_temp_k"].toDouble())
            };

            for (int col = 0; col < 9; ++col) {
                QTextTableCell cell = table->cellAt(tableRow, col);
                QTextCursor cellCursor = cell.firstCursorPosition();
                cellCursor.setCharFormat(dataFormat);
                cellCursor.insertText(rowData[col]);
            }

            tableRow++;
        }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.insertText("\n");
}

void ReportGenerator::addTechnicalAnalysis(QTextCursor& cursor, const QVariantMap& analysisResults)
{
    cursor.insertText("五、技术分析\n");
    cursor.setCharFormat(createHeadingFormat());

    QTextCharFormat bodyFormat = createBodyFormat();
    cursor.setCharFormat(bodyFormat);

    double pressureRatio = analysisResults["pressure_ratio"].toDouble();
    double totalPressureDrop = analysisResults["total_pressure_drop_pa"].toDouble() / 1e6;
    double frictionDrop = analysisResults["total_friction_drop_pa"].toDouble() / 1e6;
    double fittingsDrop = analysisResults["total_fittings_drop_pa"].toDouble() / 1e6;
    double frictionRatio = frictionDrop / totalPressureDrop;
    double fittingsRatio = fittingsDrop / totalPressureDrop;

    double totalHeatLoss = analysisResults["total_heat_loss_w"].toDouble() / 1000;
    double avgHeatLoss = analysisResults["avg_heat_loss_per_m_w"].toDouble();
    double avgHeatLossArea = analysisResults["avg_heat_loss_per_area_w"].toDouble();

    double maxSurfaceTemp = analysisResults["max_surface_temp_k"].toDouble() - 273.15;
    double avgSurfaceTemp = analysisResults["avg_surface_temp_k"].toDouble() - 273.15;

    double inletQuality = analysisResults["inlet_quality"].toDouble();
    double outletQuality = analysisResults["outlet_quality"].toDouble();

    QString analysisText;

    // 压力损失分析
    analysisText += "1. 压力损失分析:\n";
    analysisText += QString("   总压力损失为 %1 MPa，其中沿程阻力损失占 %2%，局部阻力损失占 %3%。\n")
                        .arg(totalPressureDrop, 0, 'f', 4)
                        .arg(frictionRatio * 100, 0, 'f', 1)
                        .arg(fittingsRatio * 100, 0, 'f', 1);

    // 热损失分析
    analysisText += "2. 热损失分析:\n";
    analysisText += QString("   管道总热损失为 %1 kW，平均单位长度热损失为 %2 W/m，平均单位外表面积热损失为 %3 W/m²。\n")
                        .arg(totalHeatLoss, 0, 'f', 1)
                        .arg(avgHeatLoss, 0, 'f', 1)
                        .arg(avgHeatLossArea, 0, 'f', 1);

    // 流动特性分析
    analysisText += "3. 流动特性分析:\n";
    analysisText += QString("   入口流速为 %1 m/s，出口流速为 %2 m/s，流动状态为 %3，平均摩擦系数为 %4。\n")
                        .arg(analysisResults["inlet_velocity_m_s"].toDouble(), 0, 'f', 2)
                        .arg(analysisResults["outlet_velocity_m_s"].toDouble(), 0, 'f', 2)
                        .arg(analysisResults["avg_reynolds"].toDouble() > 4000 ? "湍流" : "层流")
                        .arg(analysisResults["avg_friction"].toDouble(), 0, 'f', 5);

    // 温度特性分析
    analysisText += "4. 温度特性分析:\n";
    analysisText += QString("   最大外表面温度为 %1 °C，平均外表面温度为 %2 °C。\n")
                        .arg(maxSurfaceTemp, 0, 'f', 1)
                        .arg(avgSurfaceTemp, 0, 'f', 1);

    // 相变分析
    analysisText += "5. 相变分析:\n";
    QString phaseChange;
    if (outletQuality > inletQuality) {
        phaseChange = "发生气化";
    } else if (outletQuality < inletQuality) {
        phaseChange = "发生凝结";
    } else {
        phaseChange = "未发生明显相变";
    }
    analysisText += QString("   入口干度为 %1，出口干度为 %2，%3。\n")
                        .arg(inletQuality, 0, 'f', 4)
                        .arg(outletQuality, 0, 'f', 4)
                        .arg(phaseChange);

    cursor.insertText(analysisText);
    cursor.insertText("\n");
}

void ReportGenerator::addConclusions(QTextCursor& cursor, const QVariantMap& analysisResults)
{
    cursor.insertText("六、结论与建议\n");
    cursor.setCharFormat(createHeadingFormat());

    QTextCharFormat bodyFormat = createBodyFormat();
    cursor.setCharFormat(bodyFormat);

    double pressureRatio = analysisResults["pressure_ratio"].toDouble();
    double avgHeatLoss = analysisResults["avg_heat_loss_per_m_w"].toDouble();
    double maxSurfaceTemp = analysisResults["max_surface_temp_k"].toDouble() - 273.15;

    QString conclusionsText;

    // 系统性能评价
    conclusionsText += "1. 系统性能评价:\n";
    conclusionsText += QString("   管道系统压降比例为 %1，%2。\n")
                           .arg(pressureRatio * 100, 0, 'f', 2)
                           .arg(pressureRatio < 0.05 ? "在可接受范围内" : "偏高，建议优化管道设计");

    // 保温效果评价
    conclusionsText += "2. 保温效果评价:\n";
    conclusionsText += QString("   单位长度热损失为 %1 W/m，%2。\n")
                           .arg(avgHeatLoss, 0, 'f', 1)
                           .arg(avgHeatLoss < 50 ? "保温效果良好" : "热损失较大，建议增加保温厚度");

    // 安全性能评价
    conclusionsText += "3. 安全性能评价:\n";
    conclusionsText += QString("   最大表面温度为 %1 °C，%2。\n")
                           .arg(maxSurfaceTemp, 0, 'f', 1)
                           .arg(maxSurfaceTemp < 60 ? "符合安全要求" : "表面温度较高，需采取防护措施");

    cursor.insertText(conclusionsText);
    cursor.insertText("\n");
}

void ReportGenerator::addChartToDocument(QTextCursor& cursor, QChart* chart, QTextDocument* document)
{
    if (!chart) {
        return;
    }

    // 这里应该实现将图表插入文档的功能
    // 由于Qt的QTextDocument不支持直接插入QChart，需要将图表渲染为图片再插入
    // 这个功能比较复杂，暂时留空

    qDebug() << "图表插入功能暂未实现";
}

QTextCharFormat ReportGenerator::createTitleFormat()
{
    QTextCharFormat format;
    format.setFont(QFont("黑体", 16, QFont::Bold));
    format.setForeground(Qt::black);
    return format;
}

QTextCharFormat ReportGenerator::createHeadingFormat()
{
    QTextCharFormat format;
    format.setFont(QFont("黑体", 12, QFont::Bold));
    format.setForeground(Qt::darkBlue);
    return format;
}

QTextCharFormat ReportGenerator::createBodyFormat()
{
    QTextCharFormat format;
    format.setFont(QFont("宋体", 10));
    format.setForeground(Qt::black);
    return format;
}

QTextTableFormat ReportGenerator::createTableFormat()
{
    QTextTableFormat format;
    format.setHeaderRowCount(1);
    format.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    format.setBorder(1);
    format.setBorderBrush(Qt::black);
    format.setCellSpacing(2);
    format.setCellPadding(4);
    format.setAlignment(Qt::AlignLeft);
    return format;
}

QString ReportGenerator::formatValue(double value, int precision, const QString& unit)
{
    QString formatted = QString::number(value, 'f', precision);
    if (!unit.isEmpty()) {
        formatted += " " + unit;
    }
    return formatted;
}

QString ReportGenerator::formatPressure(double pressurePa)
{
    return QString::number(pressurePa / 1e6, 'f', 6);
}

QString ReportGenerator::formatTemperature(double temperatureK)
{
    return QString::number(temperatureK - 273.15, 'f', 2);
}

bool ReportGenerator::saveAsWordDocument(QTextDocument* document, const QString& filename)
{
    // Qt本身不支持直接保存为.docx格式
    // 这里保存为HTML格式，Word可以打开HTML文件
    return saveAsHtmlDocument(document, filename);
}

bool ReportGenerator::saveAsPdfDocument(QTextDocument* document, const QString& filename)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document->print(&printer);
    return QFile::exists(filename);
}

bool ReportGenerator::saveAsHtmlDocument(QTextDocument* document, const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    //stream.setCodec("UTF-8");

    // 生成HTML头部
    stream << "<!DOCTYPE html>\n";
    stream << "<html>\n";
    stream << "<head>\n";
    stream << "<meta charset=\"UTF-8\">\n";
    stream << "<title>管道分析报告</title>\n";
    stream << "<style>\n";
    stream << "body { font-family: SimSun, serif; margin: 50px; }\n";
    stream << "h1 { text-align: center; color: #000080; }\n";
    stream << "h2 { color: #000080; border-bottom: 1px solid #ccc; padding-bottom: 5px; }\n";
    stream << "table { border-collapse: collapse; width: 100%; margin: 10px 0; }\n";
    stream << "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n";
    stream << "th { background-color: #f2f2f2; font-weight: bold; }\n";
    stream << "</style>\n";
    stream << "</head>\n";
    stream << "<body>\n";

    // 输出HTML内容
    stream << document->toHtml();

    stream << "</body>\n";
    stream << "</html>\n";

    file.close();
    return true;
}
