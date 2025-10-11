#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QList>
#include <QChart>
#include <QTextTableFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
class QTextCursor;
class QTextTable;
class QTextCharFormat;
class QTextBlockFormat;
QT_END_NAMESPACE

class ReportGenerator : public QObject
{
    Q_OBJECT

public:
    explicit ReportGenerator(QObject *parent = nullptr);

    /**
     * @brief 生成管道分析报告
     * @param params 输入参数
     * @param segmentResults 分段结果数据
     * @param analysisResults 分析汇总结果
     * @param chart 分析图表（可选）
     * @param filename 输出文件名
     * @return 成功返回true，失败返回false
     */
    bool generatePipeReport(const QVariantMap& params,
                            const QList<QVariantMap>& segmentResults,
                            const QVariantMap& analysisResults,
                            QChart* chart,
                            const QString& filename);

    /**
     * @brief 生成简单的文本报告（备用方案）
     * @param params 输入参数
     * @param segmentResults 分段结果数据
     * @param analysisResults 分析汇总结果
     * @param filename 输出文件名
     * @return 成功返回true，失败返回false
     */
    bool generateTextReport(const QVariantMap& params,
                            const QList<QVariantMap>& segmentResults,
                            const QVariantMap& analysisResults,
                            const QString& filename);

private:
    /**
     * @brief 创建Word文档的基本结构
     * @param document 文本文档对象
     * @param cursor 文本光标
     */
    void setupDocument(QTextDocument* document, QTextCursor& cursor);

    /**
     * @brief 添加报告标题
     * @param cursor 文本光标
     * @param pipeName 管道名称
     */
    void addTitle(QTextCursor& cursor, const QString& pipeName);

    /**
     * @brief 添加项目基本信息表格
     * @param cursor 文本光标
     * @param params 输入参数
     */
    void addBasicInfoTable(QTextCursor& cursor, const QVariantMap& params);

    /**
     * @brief 添加输入参数表格
     * @param cursor 文本光标
     * @param params 输入参数
     */
    void addInputParamsTable(QTextCursor& cursor, const QVariantMap& params);

    /**
     * @brief 添加计算结果汇总表格
     * @param cursor 文本光标
     * @param analysisResults 分析汇总结果
     */
    void addResultsSummaryTable(QTextCursor& cursor, const QVariantMap& analysisResults);

    /**
     * @brief 添加详细分段数据表格
     * @param cursor 文本光标
     * @param segmentResults 分段结果数据
     */
    void addSegmentDataTable(QTextCursor& cursor, const QList<QVariantMap>& segmentResults);

    /**
     * @brief 添加技术分析部分
     * @param cursor 文本光标
     * @param analysisResults 分析汇总结果
     */
    void addTechnicalAnalysis(QTextCursor& cursor, const QVariantMap& analysisResults);

    /**
     * @brief 添加结论与建议部分
     * @param cursor 文本光标
     * @param analysisResults 分析汇总结果
     */
    void addConclusions(QTextCursor& cursor, const QVariantMap& analysisResults);

    /**
     * @brief 添加图表到文档
     * @param cursor 文本光标
     * @param chart 分析图表
     * @param document 文本文档
     */
    void addChartToDocument(QTextCursor& cursor, QChart* chart, QTextDocument* document);

    /**
     * @brief 创建标题格式
     * @return 文本字符格式
     */
    QTextCharFormat createTitleFormat();

    /**
     * @brief 创建章节标题格式
     * @return 文本字符格式
     */
    QTextCharFormat createHeadingFormat();

    /**
     * @brief 创建正文格式
     * @return 文本字符格式
     */
    QTextCharFormat createBodyFormat();

    /**
     * @brief 创建表格格式
     * @return 文本表格格式
     */
    QTextTableFormat createTableFormat();

    /**
     * @brief 格式化数值显示
     * @param value 数值
     * @param precision 精度
     * @param unit 单位（可选）
     * @return 格式化后的字符串
     */
    QString formatValue(double value, int precision = 2, const QString& unit = "");

    /**
     * @brief 格式化压力值（MPa）
     * @param pressurePa 压力（Pa）
     * @return 格式化后的字符串
     */
    QString formatPressure(double pressurePa);

    /**
     * @brief 格式化温度值（°C）
     * @param temperatureK 温度（K）
     * @return 格式化后的字符串
     */
    QString formatTemperature(double temperatureK);

    /**
     * @brief 保存为Word文档
     * @param document 文本文档
     * @param filename 文件名
     * @return 成功返回true，失败返回false
     */
    bool saveAsWordDocument(QTextDocument* document, const QString& filename);

    /**
     * @brief 保存为PDF文档
     * @param document 文本文档
     * @param filename 文件名
     * @return 成功返回true，失败返回false
     */
    bool saveAsPdfDocument(QTextDocument* document, const QString& filename);

    /**
     * @brief 保存为HTML文档
     * @param document 文本文档
     * @param filename 文件名
     * @return 成功返回true，失败返回false
     */
    bool saveAsHtmlDocument(QTextDocument* document, const QString& filename);

private:
    // 报告生成选项
    bool m_includeCharts;
    bool m_includeDetailedData;
    int m_maxTableRows;

    // 格式化选项
    QString m_numberFormat;
    QString m_dateFormat;
};

#endif // REPORTGENERATOR_H
