#ifndef FLUIDANALYZER_H
#define FLUIDANALYZER_H


#include "MaterialManager.h"

class FluidAnalyzer
{

public:
    FluidAnalyzer();
    ~FluidAnalyzer();

    //struct AnalysisResult {
        //QList<QVariantMap> segmentResults;
        //QVariantMap summary;
        //QChart* chart;
    //};

    struct AnalysisResult
    {
        double massFlow;

        double inletPressure;
        double inletTemperature;
        double inletEnthalpy;
        double inletQuality;
        double inletVelocity;
        double inletDensity;
        double inletVaporFlow;
        double inletLiquidFlow;
        double inletFrictionFactor;

        double outletPressure;
        double outletTemperature;
        double outletEnthalpy;
        double outletQuality;
        double outletVelocity;
        double outletDensity;
        double outletVaporFlow;
        double outletLiquidFlow;
        double outletFrictionFactor;

        double totalPressureDrop;
        double totalFrictionPressureDrop;
        double totalFittingsPressureDrop;
        //double accelerationPressureDrop;//未考虑动压变化
        double maxVelocity;
        //double minVelocity;
        double avgVelocity;

        double TemperatureDrop;
        double EnthalpyDrop;
        double totalHeatLoss;
        double avgHeatLossPerM;
        double avgHeatLossPerArea;
        double maxSurfaceTemperature;
        //double minSurfaceTemperature;
        double avgSurfaceTemperature;

        double pipeOd;
        double pipeId;
        double length;
        std::string pipeTypeName;
        double roughness;
        double totalFittingsResistance;
        std::string insulationMaterialName;
        double insulationThickness;
        std::string protectionMaterialName;
        double emissivity;
        double ambientTemperature;
        double windSpeed;

        std::vector<std::map<std::string, double>> segmentResults;
    };

    AnalysisResult analyzePipe(const char *fluid, double massFlow,
                               double inletPressure, double inletTemperature,
                               double length, double pipeOd,
                               double pipeWallThickness, double insulationThickness,
                               std::string pipeTypeName, std::string insulationMaterialName,
                               std::string protectionMaterialName, double ambientTemperature,
                               double windSpeed, double segmentLength,
                               std::map<std::string, int> fittingsData,
                               double inletQuality = -1);

    AnalysisResult analyzePipe_volumeFlow(const char *fluid, double volumeFlow,
                       double inletPressure, double inletTemperature,
                       double length, double pipeOd,
                       double pipeWallThickness, double insulationThickness,
                       std::string pipeTypeName, std::string insulationMaterialName,
                       std::string protectionMaterialName, double ambientTemperature,
                       double windSpeed, double segmentLength,
                       std::map<std::string, int> fittingsData,
                       double inletQuality = -1);

private:
    MaterialManager *materialManager;

    double frictionFactor(double Re, double roughness, double diameter);
    double swameeJain(double Re, double roughness, double diameter);
    std::map<std::string, double> pressureDropCalculation(double massFlow, double density,
                                                          double viscosity, /*PaS*/
                                                          double diameter, double length,
                                                          double roughness, double fittingsResistance = 0);
    std::map<std::string, double> heatLossCalculation(double fluidTemperature, double ambientTemperature,
                                                      double pipeOd, double insulationThickness,
                                                      const InsulationMaterial& insulationMaterial,
                                                      const OuterProtection& protectionMaterial,
                                                      double length, double windSpeed);
    std::map<std::string, double> analyzePipeSegment(
        double massFlow, double inletPressure,
        double inletTemperature, double inletDensity,
        double inletViscosity, double inletEnthalpy,
        double pipeOd, double wallThickness, double length,
        const PipeType& pipeType, double insulationThickness,
        const InsulationMaterial& insulationMaterial,
        const OuterProtection& protectionMaterial,
        double ambientTemperature, double fittingsResistance,
        double windSpeed, const char* fluid);

    double calculateExternalHeatTransfer(double surfaceTemp, double ambientTemp,
                                         double windSpeed, double emissivity,
                                         double pipeOd, double dOuter);
    double calculateConvectionCoeff(double windSpeed, double surfaceTemp = -1,
                                    double ambientTemp = -1, double pipeOd = -1,
                                    double dOuter = -1);
    double calculateRadiationCoeff(double surfaceTemp, double ambientTemp,
                                   double emissivity);
    bool generateReport(const AnalysisResult& result, const std::string fileName);
};

#endif // FLUIDANALYZER_H
