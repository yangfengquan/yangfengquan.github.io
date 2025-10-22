#include "pipeline.h"

#include <string>

Pipeline::Pipeline(const char *fluid,
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
                   double inletQuality
                   )
    : fluid(fluid)
    , massFlow(massFlow)
    , inletPressure(inletPressure)
    , inletTemperature(inletTemperature)
    , length(length)
    , pipeOd(pipeOd)
    , pipeWallThickness(pipeWallThickness)
    , insulationThickness(insulationThickness)
    , pipeTypeName(pipeTypeName)
    , insulationMaterialName(insulationMaterialName)
    , cladMaterialName(cladMaterialName)
    , ambientTemperature(ambientTemperature)
    , windSpeed(windSpeed)
    , segmentLength(segmentLength)
    , fittingsData(fittingsData)
    , inletQuality(inletQuality)
{}

