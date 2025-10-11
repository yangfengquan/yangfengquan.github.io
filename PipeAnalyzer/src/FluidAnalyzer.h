#ifndef FLUIDANALYZER_H
#define FLUIDANALYZER_H

#include <QObject>
#include <QVariant>
#include <QChart>
#include "MaterialManager.h"

class FluidAnalyzer : public QObject
{
    Q_OBJECT

public:
    FluidAnalyzer(QObject *parent = nullptr);
    ~FluidAnalyzer();

    struct AnalysisResult {
        QList<QVariantMap> segmentResults;
        QVariantMap summary;
        QChart* chart;
    };

    AnalysisResult analyzePipe(const QVariantMap& params);
    bool generateReport(const QVariantMap& params, const AnalysisResult& result,
                        const QString& filename);

    // CoolProp包装方法
    QVariantMap getFluidProperties(double pressurePa, double temperatureK = -1,
                                   double enthalpy = -1, double quality = -1,
                                   const QString& fluid = "Water");

private:
    MaterialManager *materialManager;

    double frictionFactor(double Re, double roughness, double diameter);
    double swameeJain(double Re, double roughness, double diameter);
    QVariantMap pressureDropCalculation(const QVariantMap& props, double massFlow,
                                        double diameter, double length,
                                        double roughness, double fittingsResistance = 0);
    QVariantMap heatLossCalculation(double fluidTempK, double ambientTempK,
                                    double pipeOd, double insulationThickness,
                                    const InsulationMaterial& insulationMaterial,
                                    const OuterProtection& protectionMaterial,
                                    double length, double windSpeed);
    QVariantMap analyzePipeSegment(const QVariantMap& inletProps, double massFlow,
                                   double pipeOd, double wallThickness, double length,
                                   const PipeType& pipeType, double insulationThickness,
                                   const InsulationMaterial& insulationMaterial,
                                   const OuterProtection& protectionMaterial,
                                   double ambientTempK, double fittingsResistance = 0,
                                   double windSpeed = 3.0, const QString& fluid = "Water");

    double calculateExternalHeatTransfer(double surfaceTempK, double ambientTempK,
                                         double windSpeed, double emissivity,
                                         double pipeOd, double dOuter);
    double calculateConvectionCoeff(double windSpeed, double surfaceTempK = -1,
                                    double ambientTempK = -1, double pipeOd = -1,
                                    double dOuter = -1);
    double calculateRadiationCoeff(double surfaceTempK, double ambientTempK,
                                   double emissivity);

    QVariantMap comprehensiveResultsAnalysis(const QList<QVariantMap>& results,
                                             const QVariantMap& inletProps,
                                             double totalLength, double massFlow,
                                             const QString& pipeName, const QString& fluid,
                                             double fittingsResistance, double inletVelocity,
                                             double roughness);
    QChart* plotComprehensiveResults(const QList<QVariantMap>& results,
                                     const QString& pipeName);
};

#endif // FLUIDANALYZER_H
