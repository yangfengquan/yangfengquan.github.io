#ifndef FLUIDANALYZER_H
#define FLUIDANALYZER_H


#include "MaterialManager.h"

class FluidAnalyzer
{

public:
    FluidAnalyzer();
    ~FluidAnalyzer();

    struct AnalysisResult {
        QList<QVariantMap> segmentResults;
        QVariantMap summary;
        //QChart* chart;
    };

    AnalysisResult analyzePipe(const QVariantMap& params);
    bool generateReport(const QVariantMap& params, const AnalysisResult& result,
                        const QString& filename);

private:
    MaterialManager *materialManager;

    double frictionFactor(double Re, double roughness, double diameter);
    double swameeJain(double Re, double roughness, double diameter);
    std::map<std::string, double> pressureDropCalculation(double massFlow, double density,
                                                          double viscosity, /*PaS*/
                                                          double diameter, double length,
                                                          double roughness, double fittingsResistance = 0);
    std::map<std::string, double> heatLossCalculation(double fluidTemp, double ambientTemp,
                                                      double pipeOd, double insulationThickness,
                                                      const InsulationMaterial& insulationMaterial,
                                                      const OuterProtection& protectionMaterial,
                                                      double length, double windSpeed);
    std::map<std::string, double> analyzePipeSegment(
        double massFlow, double inletP,
        double inletT, double inletDensity,
        double inletViscosity, double inletEnthalpy,
        double pipeOd, double wallThickness, double length,
        const PipeType& pipeType, double insulationThickness,
        const InsulationMaterial& insulationMaterial,
        const OuterProtection& protectionMaterial,
        double ambientTemp, double fittingsResistance,
        double windSpeed, const char* fluid)

    double calculateExternalHeatTransfer(double surfaceTemp, double ambientTemp,
                                         double windSpeed, double emissivity,
                                         double pipeOd, double dOuter);
    double calculateConvectionCoeff(double windSpeed, double surfaceTemp = -1,
                                    double ambientTemp = -1, double pipeOd = -1,
                                    double dOuter = -1);
    double calculateRadiationCoeff(double surfaceTemp, double ambientTemp,
                                   double emissivity);

    QVariantMap comprehensiveResultsAnalysis(const QList<QVariantMap>& results,
                                             const QVariantMap& inletProps,
                                             double totalLength, double massFlow,
                                             const QString& pipeName, const QString& fluid,
                                             double fittingsResistance, double inletVelocity,
                                             double roughness);
};

#endif // FLUIDANALYZER_H
