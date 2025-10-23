#include "pipeline.h"
extern "C" {
    #include "CoolPropLib.h"
}
#include "string.h"
#include <string>
Pipeline::Pipeline()
    : head(nullptr)
{}

Pipeline::Pipeline(const char *fluid,
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
                   double inletQuality
                   )
    : fluid(fluid)
    , inletPressure(inletPressure)
    , inletTemperature(inletTemperature)
    , pipeOd(pipeOd)
    , pipeWallThickness(pipeWallThickness)
    , length(length)
    , insulationThickness(insulationThickness)
    , pipeTypeName(pipeTypeName)
    , insulationMaterialName(insulationMaterialName)
    , cladMaterialName(cladMaterialName)
    , ambientTemperature(ambientTemperature)
    , windSpeed(windSpeed)
    , fittingsData(fittingsData)
    , segmentLength(segmentLength)
    , inletQuality(inletQuality)
    , head(nullptr)
{


}
Pipeline::~Pipeline()
{
    SegmentParameters* current = head;
    while(current != nullptr) {
        SegmentParameters* next = current->next;
        delete current;  // 如果用 new 分配
        // 或者 free(current);  // 如果用 malloc 分配
        current = next;
    }
}
void Pipeline::volumetricFlowRate(double flowRate)
{
    double density = PropsSI("D", "P", 101325, "T", 273.15, fluid);
    massFlow = flowRate * density;
}

void Pipeline::massFlowRate(double flowRate)
{
    massFlow = flowRate;
}

Pipeline::SegmentParameters* Pipeline::getHead()
{
    double quality = -1;
    if (inletQuality == -1) {
        head->enthalpy = PropsSI("H", "P", inletPressure, "T", inletTemperature, fluid);
        quality = PropsSI("H", "P", inletPressure, "T", inletTemperature, fluid);
    } else {
        quality = inletQuality;
        head->enthalpy = PropsSI("H", "P", inletPressure, "Q", quality, fluid);
    }

    char phase_str[256];
    PhaseSI("P", inletPressure, "H", head->enthalpy, fluid, phase_str, sizeof(phase_str));

    if (quality == -1) {
        if(strcmp(phase_str, "gas") == 0 || strcmp(phase_str, "supercritical_gas") == 0) {
            head->vaporFlow = massFlow;
        } else {
            head->liquidFlow = massFlow;
        }
    } else {
        head->vaporFlow = massFlow * quality;
        head->liquidFlow = massFlow * (1 - quality);
    }

    return head;
}
