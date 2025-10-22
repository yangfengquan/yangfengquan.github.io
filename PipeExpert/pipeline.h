#ifndef PIPEFLOWCALCULATOR_H
#define PIPEFLOWCALCULATOR_H

#include <string>
#include <map>

class Pipeline
{
public:
    Pipeline(const char *fluid,
             double massFlow,
             double inletPressure,
             double inletTemperature,
             double length,
             double pipeOd,
             double pipeWallThickness,
             double insulationThickness,
             std::string pipeTypeName,
             std::string insulationMaterialName,
             std::string cladMaterialName,
             double ambientTemperature,
             double windSpeed,
             double segmentLength,
             std::map<std::string, int> fittingsData,
             double inletQuality = -1
            );

    const char *fluid;

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
    double pipeWallThickness;
    std::string pipeTypeName;
    double roughness;
    double totalFittingsResistance;
    std::string insulationMaterialName;
    double insulationThickness;
    std::string cladMaterialName;
    double emissivity;
    double ambientTemperature;
    double windSpeed;

    std::map<std::string, int> fittingsData;

    double segmentLength;
    struct segmentParameters
    {
        double pressure;
        double temperature;
        double enthalpy;
        double vaporFlow;
        double liquidFlow;
        double surfaceTemperature;
        double frictionFactor;
        double frictionPressureDrop;
        double fittingsPressureDrop;
        segmentParameters* next;
    };


};

#endif // PIPEFLOWCALCULATOR_H
