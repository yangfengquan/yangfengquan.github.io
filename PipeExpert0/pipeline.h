#ifndef PIPEFLOWCALCULATOR_H
#define PIPEFLOWCALCULATOR_H

#include <string>
#include <map>

class InsulationMaterial;
class QString;

class Pipeline
{
public:
    Pipeline();
    Pipeline(const char *fluid,
             double inletPressure,
             double inletTemperature,
             double pipeOd,
             double pipeWallThickness,
             double length,
             double insulationThickness,
             std::string pipeTypeName,
             std::string insulationMaterialName,
             std::string cladMaterialName,
             double ambientTemperature,
             double windSpeed,
             std::map<std::string, int> fittingsData,
             double segmentLength,
             double inletQuality = -1
            );
    ~Pipeline();
    void massFlowRate(double flowRate);
    void volumetricFlowRate(double flowRate);
    void calculate();
    QString getReport();

private:
    const char *fluid;
    double massFlow;
    double inletPressure;
    double inletTemperature;
    double pipeOd;
    double pipeId;
    double pipeWallThickness;
    double length;
    double insulationThickness;
    std::string pipeTypeName;
    std::string insulationMaterialName;
    std::string cladMaterialName;
    double ambientTemperature;
    double windSpeed;
    std::map<std::string, int> fittingsData;
    double segmentLength;
    double inletQuality;

    struct SegmentParameters
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
        struct SegmentParameters* prev;
        struct SegmentParameters* next;
    } *phead;

    // 入口参数
    SegmentParameters* getHead();

    // 出口参数
    SegmentParameters* getLast();

    // 摩擦系数
    double _frictionFactor(double reynolds, double roughness);

    // 定义科尔布鲁克-怀特方程的内部函数
    // 该方程描述了过渡区和湍流状态下摩擦因子与雷诺数、相对粗糙度的关系
    double colebrook_equation(double friction_factor, double reynolds, double relative_roughness);

    void pressureDrop(
        double length,
        double roughness,
        double fittingsResistance,
        double density,
        double viscosity,
        double *frictionFactor,
        double *frictionPressureDrop,
        double *fittingsPressureDrop
        );
    double convectiveHeatTransferCoeff(double surfaceTemperature);
    double radiationHeatTransferCoeffi(double surfaceTemperature, double emissivity);
    double surfaceThermalResistance(double surfaceTemperature,double emissivity);
    void heatLoss(
        double fluidTemperature,
        double length,
        double emissivity,
        const InsulationMaterial& insulationMaterial,
        double* Q,
        double* surfaceTemperature
        );
    Pipeline::SegmentParameters* segment(
        SegmentParameters* pPrev,
        double length,
        double roughness,
        double emissivity,
        double fittingsResistance,
        const InsulationMaterial& insulationMaterial
        );
};

#endif // PIPEFLOWCALCULATOR_H
