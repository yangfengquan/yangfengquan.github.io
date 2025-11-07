#include "FluidAnalyzer.h"

extern "C" {
#include "CoolPropLib.h"
}
#include <cmath>
#include <algorithm>
//#include <map>
#include <string>
#include <cstring>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QCoreApplication>
// 使用标准库函数
using std::exp;
using std::log;
using std::pow;
using std::sqrt;

FluidAnalyzer::FluidAnalyzer()
    :materialManager(new MaterialManager())
{
}

FluidAnalyzer::~FluidAnalyzer()
{
}


double FluidAnalyzer::frictionFactor(double Re, double roughness, double diameter)
{
    if (Re <= 0) {
        return 0.015;
    }

    if (Re < 2000) {
        return 64.0 / Re;
    }

    if (Re < 4000) {
        double f_lam = 64.0 / 2000;
        double f_turb = swameeJain(4000, roughness, diameter);
        double weight = (Re - 2000) / 2000;
        return (1 - weight) * f_lam + weight * f_turb;
    }

    return swameeJain(Re, roughness, diameter);
}

double FluidAnalyzer::swameeJain(double Re, double roughness, double diameter)
{
    double eD = roughness / diameter;
    double f = 0.25 / pow(log10(eD/3.7 + 5.74/pow(Re, 0.9)), 2);
    return f;
}

std::map<std::string, double> FluidAnalyzer::pressureDropCalculation(
    double massFlow, double density,
    double viscosity, /*PaS*/
    double diameter, double length,
    double roughness, double fittingsResistance)
{
    double area = M_PI * pow(diameter/2, 2);

    double velocity = massFlow / (density * area);
    double Re = (density * velocity * diameter) / viscosity;
    double f = frictionFactor(Re, roughness, diameter);

    // 沿程阻力损失
    double frictionPressureDrop = f * (length / diameter) * (density * pow(velocity, 2)) / 2;

    // 局部阻力损失
    double fittingsPressureDrop = fittingsResistance * (density * pow(velocity, 2)) / 2;

    double totalPressureDrop = frictionPressureDrop + fittingsPressureDrop;

    std::map<std::string, double> results;

    results["velocity"] = velocity;
    results["reynolds"] = Re;
    results["frictionFactor"] = f;
    results["pressureDrop"] = totalPressureDrop;
    results["frictionPressureDrop"] = frictionPressureDrop;
    results["fittingsPressureDrop"] = fittingsPressureDrop;

    return results;
}

std::map<std::string, double> FluidAnalyzer::heatLossCalculation(
    double fluidTemperature, double ambientTemperature,
    double pipeOd, double insulationThickness,
    const InsulationMaterial& insulationMaterial,
    const OuterProtection& protectionMaterial,
    double length, double windSpeed)
{
    qDebug()<<"insulationThickness"<<insulationThickness<<"ambientTemperature"<<ambientTemperature;
    /*
    if (insulationThickness <= 0) {
        // 无保温情况
        double d_outer = pipeOd;
        double h_combined = calculateExternalHeatTransfer(
            fluidTemperature, ambientTemperature, windSpeed, protectionMaterial.emissivity, pipeOd, d_outer);

        double surface_area = M_PI * pipeOd * length;
        double Q_total = h_combined * surface_area * (fluidTemperature - ambientTemperature);
        double Q_per_m = Q_total / length;
        double surfaceTemperature = ambientTemperature + 0.7 * (fluidTemperature - ambientTemperature);

        // 计算单位外表面积热损失
        double surface_area_per_m = M_PI * d_outer;
        double Q_per_area = (surface_area_per_m > 0) ? Q_per_m / surface_area_per_m : 0.0;

        std::map<std::string, double> results;
        results["totalHeatLoss"] = Q_total;
        results["heatLossPerM"] = Q_per_m;
        results["heatLossPerArea"] = Q_per_area;
        results["overallHeatTransferCoeff"] = h_combined;
        results["convectionCoeff"] = h_combined * 0.8;
        results["surfaceTemperature"] = surfaceTemperature;
        results["insulationConductivity"] = 0.0;

        return results;
    }
*/
    // 有保温情况 - 迭代计算
    double d_outer = pipeOd + 2 * insulationThickness;

    // 初始假设表面温度
    double surfaceTemperature = ambientTemperature + 0.3 * (fluidTemperature - ambientTemperature);
    double insulation = 0.04; // 默认值
    double h_external = 0.0;
    double Q_per_m = 0.0;

    for (int iteration = 0; iteration < 20; ++iteration) {
        // 计算保温层平均温度
        double tm = (fluidTemperature + surfaceTemperature) / 2;

        // 计算保温材料导热系数
        insulation = insulationMaterial.calculateConductivity(tm);

        // 计算保温层热阻
        double R_insulation = log(d_outer / pipeOd) / (2 * M_PI * insulation);

        // 计算外部换热系数
        h_external = calculateExternalHeatTransfer(
            surfaceTemperature, ambientTemperature, windSpeed, protectionMaterial.emissivity, pipeOd, d_outer);

        // 计算外部热阻
        double R_external = 1 / (h_external * M_PI * d_outer);

        // 总热阻
        double R_total = R_insulation + R_external;

        // 单位长度热损失
        Q_per_m = (fluidTemperature - ambientTemperature) / R_total;

        // 更新表面温度
        double surfaceTemperature_new_k = ambientTemperature + Q_per_m * R_external;

        // 收敛检查
        if (fabs(surfaceTemperature_new_k - surfaceTemperature) < 0.1) {
            surfaceTemperature = surfaceTemperature_new_k;
            break;
        }

        surfaceTemperature = surfaceTemperature_new_k;
    }

    // 总热损失
    double Q_total = Q_per_m * length;

    // 计算对流换热系数
    double h_convection = calculateConvectionCoeff(windSpeed, surfaceTemperature, ambientTemperature, pipeOd, d_outer);

    // 计算单位外表面积热损失
    double surface_area_per_m = M_PI * d_outer;
    double Q_per_area = (surface_area_per_m > 0) ? Q_per_m / surface_area_per_m : 0.0;

    std::map<std::string, double> results;
    results["totalHeatLoss"] = Q_total;
    results["heatLossPerM"] = Q_per_m;
    results["heatLossPerArea"] = Q_per_area;
    results["overallHeatTransferCoeff"] = h_external;
    results["convectionCoeff"] = h_convection;
    results["surfaceTemperature"] = surfaceTemperature;
    results["insulationConductivity"] = insulation;

    return results;
}

double FluidAnalyzer::calculateExternalHeatTransfer(double surfaceTemperature, double ambientTemperature,
                                                    double windSpeed, double emissivity,
                                                    double pipeOd, double dOuter)
{
    // 对流换热系数
    double h_conv = calculateConvectionCoeff(windSpeed, surfaceTemperature, ambientTemperature, pipeOd, dOuter);

    // 辐射换热系数
    double h_rad = calculateRadiationCoeff(surfaceTemperature, ambientTemperature, emissivity);

    return h_conv + h_rad;
}

double FluidAnalyzer::calculateConvectionCoeff(double windSpeed, double surfaceTemp,
                                               double ambientTemperature, double pipeOd, double dOuter)
{
    double h_conv = 0.0;

    if (windSpeed == 0) {
        // 无风时自然对流
        double T_avg = (surfaceTemp + ambientTemperature) / 2;
        h_conv = 26.4 / sqrt(297 - 273.15 + 0.5 * T_avg) * pow((surfaceTemp - ambientTemperature) / pipeOd, 0.25);
    } else if (windSpeed * dOuter <= 0.8) {
        // 低风速情况
        h_conv = 0.08 / dOuter + 4.2 * pow(windSpeed, 0.618) / pow(dOuter, 0.382);
    } else {
        // 强制对流
        h_conv = 4.53 * pow(windSpeed, 0.805) / pow(pipeOd, 0.195);
    }

    return std::max(1.0, h_conv); // 确保最小值
}

double FluidAnalyzer::calculateRadiationCoeff(double surfaceTemp, double ambientTemperature,
                                              double emissivity)
{
    double sigma = 5.669; // Stefan-Boltzmann常数
    double T_diff = surfaceTemp - ambientTemperature;

    if (T_diff <= 0) {
        return 0.0;
    }

    double h_rad = sigma * emissivity / T_diff *
                   (pow(surfaceTemp/100, 4) - pow(ambientTemperature/100, 4));

    return std::max(0.0, h_rad);
}

std::map<std::string, double> FluidAnalyzer::analyzePipeSegment(
    double massFlow, double inletPressure,
    double inletTemperature, double inletDensity,
    double inletViscosity, double inletEnthalpy,
    double pipeOd, double wallThickness, double length,
    const PipeType& pipeType, double insulationThickness,
    const InsulationMaterial& insulationMaterial,
    const OuterProtection& protectionMaterial,
    double ambientTemperature, double fittingsResistance,
    double windSpeed, const char* fluid)
{
    // 1. 压力降计算
    std::map<std::string, double> dpResults = pressureDropCalculation(
        massFlow, inletDensity, inletViscosity, pipeOd - 2*wallThickness, length,
        pipeType.roughness, fittingsResistance
        );

    double outletPressure = inletPressure - dpResults["pressureDrop"];

    // 2. 热损失计算
    std::map<std::string, double> heatLossResults;
    double outletEnthalpy;
    double outletTemperature = 0, outletTemperature_new = 0.9 * inletTemperature;
    while (abs(outletTemperature - outletTemperature_new) > 0.01) {
        outletTemperature = outletTemperature_new;
        double avgTemperature = 0.5 * (inletTemperature + outletTemperature);

        heatLossResults = heatLossCalculation(
            avgTemperature, ambientTemperature, pipeOd,
            insulationThickness, insulationMaterial, protectionMaterial, length, windSpeed
        );

        double delta_h = (massFlow > 0) ? heatLossResults["totalHeatLoss"] / massFlow : 0;
        outletEnthalpy = inletEnthalpy - delta_h;


        outletTemperature_new = PropsSI("T", "P", outletPressure, "H", outletEnthalpy, fluid);
    }

    double outletQuality = PropsSI("Q", "P", outletPressure, "H", outletEnthalpy, fluid);

    // 5. 计算气相和液相流量
    char phase_str[256];
    double vaporFlow = 0, liquidFlow = 0;
    PhaseSI("P", outletPressure, "H", outletEnthalpy, fluid, phase_str, sizeof(phase_str));
    if (outletQuality == -1) {
        if(strcmp(phase_str, "gas") == 0 || strcmp(phase_str, "supercritical_gas") == 0) {
            vaporFlow = massFlow;
        } else {
            liquidFlow = massFlow;
        }
    } else {
        vaporFlow = massFlow * outletQuality;
        liquidFlow = massFlow * (1 - outletQuality);
    }

    std::map<std::string, double> results;
    // 组装结果
    results["velocity"] = dpResults["velocity"];//inlet
    results["reynolds"] = dpResults["reynolds"];
    results["frictionFactor"] = dpResults["frictionFactor"];
    results["pressureDrop"] = dpResults["pressureDrop"];
    results["frictionPressureDrop"] = dpResults["frictionPressureDrop"];
    results["fittingsPressureDrop"] = dpResults["fittingsPressureDrop"];
    results["totalHeatLoss"] = heatLossResults["totalHeatLoss"];
    results["heatLossPerM"] = heatLossResults["heatLossPerM"];
    results["heatLossPerArea"] = heatLossResults["heatLossPerArea"];
    results["overallHeatTransferCoeff"] = heatLossResults["overallHeatTransferCoeff"];
    results["convectionCoeff"] = heatLossResults["convectionCoeff"];
    results["surfaceTemperature"] = heatLossResults["surfaceTemperature"];
    results["insulationConductivity"] = heatLossResults["insulationConductivity"];
    results["vaporFlow"] = vaporFlow;
    results["liquidFlow"] = liquidFlow;
    results["outletPressure"] = outletPressure;
    results["outletTemperature"] = outletTemperature;
    results["outletEnthalpy"] = outletEnthalpy;
    results["outletQuality"] = outletQuality;

    return results;
}

FluidAnalyzer::AnalysisResult FluidAnalyzer::analyzePipe(
    const char *fluid, double massFlow,
    double inletPressure, double inletTemperature,
    double length, double pipeOd,
    double pipeWallThickness, double insulationThickness,
    std::string pipeTypeName, std::string insulationMaterialName,
    std::string protectionMaterialName, double ambientTemperature,
    double windSpeed, double segmentLength,
    std::map<std::string, int> fittingsData,
    double inletQuality)
{
    // qDebug()<<fluid<<"f:"<<massFlow<<"p"<<inletPressure<<"t"<<inletTemperature<<"l"<<length<<"od"<<pipeOd<<"w"<<pipeWallThickness
    //          <<"insulationThickness"<<insulationThickness<<"pname"<<pipeTypeName<<"iname"<<insulationMaterialName
    //          <<"prname"<<protectionMaterialName<<"at"<<ambientTemperature<<"ws"<<windSpeed;

    AnalysisResult result;
    try {    
        // 获取材料
        PipeType pipeType;
        if (materialManager->getPipeTypes().contains(pipeTypeName.c_str())) {
            pipeType = materialManager->getPipeTypes()[pipeTypeName.c_str()];
        } else {
            pipeType.name = "默认管道";
            pipeType.roughness = 2e-5;
            pipeType.description = "默认管道类型";
        }

        InsulationMaterial insulationMaterial;
        if (materialManager->getInsulationMaterials().contains(insulationMaterialName.c_str())) {
            insulationMaterial = materialManager->getInsulationMaterials()[insulationMaterialName.c_str()];
        } else {
            insulationMaterial.name = "默认保温";
            insulationMaterial.density = "100";
            insulationMaterial.description = "默认保温材料";
        }

        OuterProtection protectionMaterial;
        if (materialManager->getProtectionMaterials().contains(protectionMaterialName.c_str())) {
            protectionMaterial = materialManager->getProtectionMaterials()[protectionMaterialName.c_str()];
        } else {
            protectionMaterial.name = "默认保护层";
            protectionMaterial.emissivity = 0.3;
            protectionMaterial.description = "默认保护层材料";
        }

        // 计算总局部阻力系数
        double totalFittingsResistance = 0;
        for (std::map<std::string, int>::iterator it = fittingsData.begin(); it != fittingsData.end(); ++it) {
            //std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
            if (materialManager->getPipeFittings().contains(it->first.c_str())) {
                PipeFitting pipe_fitting = materialManager->getPipeFittings()[it->first.c_str()];
                totalFittingsResistance += pipe_fitting.resistanceCoef * it->second;
            }
        }

        // 获取入口条件
        //QVariantMap inlet_props;
        double inletEnthalpy, inletDensity, inletViscosity;
        char phase_str[256];
        if (inletQuality != -1) {
            inletTemperature = PropsSI("T", "P", inletPressure, "Q", inletQuality, fluid);
            inletEnthalpy = PropsSI("H", "P", inletPressure, "Q", inletQuality, fluid);
            inletDensity = PropsSI("D", "P", inletPressure, "Q", inletQuality, fluid);
            inletViscosity = PropsSI("V", "P", inletPressure, "Q", inletQuality, fluid);
            PhaseSI("P", inletPressure, "Q", inletQuality, fluid, phase_str, sizeof(phase_str));
        } else {
            //inletQuality = PropsSI("Q", "P", inletPressure, "T", inletTemperature, fluid);
            inletEnthalpy = PropsSI("H", "P", inletPressure, "T", inletTemperature, fluid);
            inletDensity = PropsSI("D", "P", inletPressure, "T", inletTemperature, fluid);
            inletViscosity = PropsSI("V", "P", inletPressure, "T", inletTemperature, fluid);
            PhaseSI("P", inletPressure, "T", inletTemperature, fluid, phase_str, sizeof(phase_str));
        }

        // 计算入口流速
        double area = M_PI * pow((pipeOd - 2 * pipeWallThickness) / 2, 2);
        double inletVelocity = massFlow / (inletDensity * area);

        //QVariantMap current_props = inlet_props;
        //QList<QVariantMap> segment_results;
        std::vector<std::map<std::string, double>> segmentResults;
        segmentLength = segmentLength ? segmentLength : length;
        int num_segments = std::max(1, (int)ceil(length / segmentLength));
        qDebug()<<"segmentLength"<<segmentLength<<"num_segments"<<num_segments;
        double segmentFittingsResistance = totalFittingsResistance / num_segments;
        double currentPressure = inletPressure;
        double currentTemperature = inletTemperature;
        double currentDensity = inletDensity;
        double currentViscosity = inletViscosity;
        double currentEnthalpy = inletEnthalpy;
        for (int i = 0; i < num_segments; ++i) {
            double seg_length = std::min(segmentLength, length - i * segmentLength);
            double distance = (i + 1) * seg_length;

            std::map<std::string, double> segmentResult = analyzePipeSegment(
                massFlow, currentPressure, currentTemperature, currentDensity, currentViscosity,
                currentEnthalpy, pipeOd, pipeWallThickness, seg_length, pipeType, insulationThickness,
                insulationMaterial, protectionMaterial, ambientTemperature, segmentFittingsResistance,
                windSpeed, fluid
                );

            segmentResult["segment"] = i + 1;
            segmentResult["distance_m"] = distance;

            segmentResults.push_back(segmentResult);
            currentPressure = segmentResult["outletPressure"];
            currentTemperature = segmentResult["outletTemperature"];
            currentEnthalpy = segmentResult["outletEnthalpy"];
            currentDensity = PropsSI("D", "P", currentPressure, "H", currentEnthalpy, fluid);
            currentViscosity = PropsSI("V", "P", currentPressure, "H", currentEnthalpy, fluid);
        }

        result.massFlow = massFlow;
        result.inletPressure = inletPressure;
        result.inletTemperature = inletTemperature;
        result.inletEnthalpy = inletEnthalpy;
        result.inletQuality = inletQuality;
        result.inletVelocity = inletVelocity;
        result.inletDensity = inletDensity;
        qDebug()<<phase_str;
        if (inletQuality == -1) {
            if(strcmp(phase_str, "gas") == 0 ||strcmp(phase_str, "supercritical_gas") == 0) {
                result.inletVaporFlow = massFlow;
            } else {
                result.inletLiquidFlow = massFlow;
            }
        } else {
            result.inletVaporFlow = massFlow * inletQuality;
            result.inletLiquidFlow = massFlow * (1 - inletQuality);
        }

        if(!segmentResults.empty()){
            result.inletFrictionFactor = segmentResults.front()["frictionFactor"];
            result.outletPressure = segmentResults.back()["outletPressure"];
            result.outletTemperature = segmentResults.back()["outletTemperature"];
            result.outletEnthalpy = segmentResults.back()["outletEnthalpy"];
            result.outletQuality = segmentResults.back()["outletQuality"];
            result.outletVaporFlow = segmentResults.back()["vaporFlow"];
            result.outletLiquidFlow = segmentResults.back()["liquidFlow"];
            result.outletFrictionFactor = segmentResults.back()["frictionFactor"];//实际是最后一段入口
            result.outletDensity = PropsSI("D", "P", result.outletPressure, "H", result.outletEnthalpy, fluid);
            result.outletVelocity = massFlow / (result.outletDensity * area);
            result.totalPressureDrop = result.inletPressure - result.outletPressure;
            result.TemperatureDrop = result.inletTemperature - result.outletTemperature;
            result.maxVelocity = 0;
            result.maxSurfaceTemperature = 0;

            double velocitySum = 0;
            double surfaceTemperatureSum = 0;

            for (auto& segmentResult: segmentResults) {
                result.totalFrictionPressureDrop += segmentResult["frictionPressureDrop"];
                result.totalFittingsPressureDrop += segmentResult["fittingsPressureDrop"];
                result.maxVelocity = std::max(result.maxVelocity, segmentResult["velocity"]);
                result.maxSurfaceTemperature = std::max(result.maxVelocity, segmentResult["surfaceTemperature"]);
                velocitySum += segmentResult["velocity"];
                surfaceTemperatureSum += segmentResult["surfaceTemperature"];
                result.totalHeatLoss += segmentResult["totalHeatLoss"];
            }

            result.avgVelocity = velocitySum / segmentResults.size();
            result.avgSurfaceTemperature = surfaceTemperatureSum / segmentResults.size();

            result.avgHeatLossPerM = result.totalHeatLoss / length;

            double surfaceArea = M_PI * (pipeOd + 2 * insulationThickness) * length;
            result.avgHeatLossPerArea = result.totalHeatLoss / surfaceArea;

            result.pipeOd = pipeOd;
            result.pipeId = pipeOd - 2 * pipeWallThickness;
            result.length = length;
            result.pipeTypeName = pipeTypeName;
            result.roughness =  pipeType.roughness;
            result.totalFittingsResistance = totalFittingsResistance;
            result.insulationMaterialName = insulationMaterialName;
            result.insulationThickness = insulationThickness;
            result.protectionMaterialName = protectionMaterialName;
            result.emissivity = protectionMaterial.emissivity;
            result.ambientTemperature = ambientTemperature;
            result.windSpeed = windSpeed;
            result.segmentResults = segmentResults;
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "错误", QString("管道分析失败: %1").arg(e.what()));
    }

    generateReport(result, "Pipe.tmp");
    return result;
}

FluidAnalyzer::AnalysisResult FluidAnalyzer::analyzePipe_volumeFlow(
    const char *fluid, double volumeFlow,
    double inletPressure, double inletTemperature,
    double length, double pipeOd,
    double pipeWallThickness, double insulationThickness,
    std::string pipeTypeName, std::string insulationMaterialName,
    std::string protectionMaterialName, double ambientTemperature,
    double windSpeed, double segmentLength,
    std::map<std::string, int> fittingsData,
    double inletQuality)
{
    double nDensity;// 温度 273.15 K、压力 101 325 Pa下的密度

    nDensity = PropsSI("D", "P", 101325, "T", 273.15, fluid);


    double massFlow = volumeFlow * nDensity;

    analyzePipe(fluid, massFlow, inletPressure, inletTemperature,
                length, pipeOd, pipeWallThickness, insulationThickness,
                pipeTypeName, insulationMaterialName,
                protectionMaterialName, ambientTemperature,
                windSpeed, segmentLength,
                fittingsData, inletQuality);
}

bool FluidAnalyzer::generateReport(const FluidAnalyzer::AnalysisResult& result, const std::string fileName)
{
    // 获取程序运行目录
    QString appDir = QCoreApplication::applicationDirPath();
    QString reportFullPath = appDir + "/" + QString::fromStdString(fileName);

    // 打开文件并写入报告内容
    QFile reportFile(reportFullPath);
    if (!reportFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(nullptr, "错误", "报告文件创建失败：" + reportFile.errorString());
        return false;
    }

    // Qt 6中设置UTF-8编码
    QTextStream out(&reportFile);
    out.setEncoding(QStringConverter::Utf8);

    // 报告头部
    out << "===================================================================================\n";
    out << "                          管道压力损失与热损失分析报告\n";
    out << "===================================================================================\n";

    // 1. 操作参数（标签宽度28，数值宽度20）
    out << "操作参数\n";
    out << "-----------------------------------------------------------------------------------\n";
    out << QString("%1: %2\n").arg("质量流量（kg/hr）", -28).arg(result.massFlow * 3600, 20, 'f', 4);
    out << QString("%1: %2\n").arg("总压力损失（kPa）", -27).arg(result.totalPressureDrop / 1000, 20, 'f', 4);
    out << QString("%1: %2\n").arg("摩擦压力损失（kPa）", -26).arg(result.totalFrictionPressureDrop / 1000, 20, 'f', 4);
    out << QString("%1: %2\n").arg("管件压力损失（kPa）", -26).arg(result.totalFittingsPressureDrop / 1000, 20, 'f', 4);
    out << QString("%1: %2\n").arg("最大流速（m/sec）", -28).arg(result.maxVelocity, 20, 'f', 4);
    out << QString("%1: %2\n").arg("平均流速（m/sec）", -28).arg(result.avgVelocity, 20, 'f', 4);
    out << QString("%1: %2\n").arg("温度降（C）", -29).arg(result.TemperatureDrop, 20, 'f', 4);
    out << QString("%1: %2\n").arg("焓降（kJ/kg）", -30).arg(result.EnthalpyDrop / 1000, 20, 'f', 4);
    out << QString("%1: %2\n").arg("总热损失（kW）", -28).arg(result.totalHeatLoss / 1000, 20, 'f', 4);
    out << QString("%1: %2\n").arg("每米平均热损失（W/m）", -25).arg(result.avgHeatLossPerM, 20, 'f', 4);
    out << QString("%1: %2\n\n").arg("每平方米平均热损失（W/m2）", -23).arg(result.avgHeatLossPerArea, 20, 'f', 4);

    // 2. 进口和出口流体特性
    out << "流体特性参数\n";
    out << "-----------------------------------------------------------------------------------\n";
    out << QString("%1%2%3\n").arg("", -32).arg("进口", 20).arg("出口", 18);
    out << QString("%1%2%3\n").arg("", -34).arg("---------", 20).arg("---------", 20);
    out << QString("%1%2%3\n").arg("压力（kPa）", -30).arg(result.inletPressure / 1000, 20, 'f', 2).arg(result.outletPressure / 1000, 20, 'f', 2);
    out << QString("%1%2%3\n").arg("温度（C）", -30).arg(result.inletTemperature - 273.15, 20, 'f', 2).arg(result.outletTemperature - 273.15, 20, 'f', 2);
    out << QString("%1%2%3\n").arg("焓值（kJ/kg）", -30).arg(result.inletEnthalpy / 1000, 20, 'f', 2).arg(result.outletEnthalpy / 1000, 20, 'f', 2);
    out << QString("%1%2%3\n").arg("干度（-）", -30).arg(result.inletQuality, 20, 'f', 4).arg(result.outletQuality, 20, 'f', 4);
    out << QString("%1%2%3\n").arg("流速（m/sec）", -30).arg(result.inletVelocity, 20, 'f', 4).arg(result.outletVelocity, 20, 'f', 4);
    out << QString("%1%2%3\n").arg("密度（kg/m3）", -30).arg(result.inletDensity, 20, 'f', 4).arg(result.outletDensity, 20, 'f', 4);
    out << QString("%1%2%3\n").arg("气相流量（kg/hr）", -28).arg(result.inletVaporFlow * 3600, 20, 'f', 2).arg(result.outletVaporFlow * 3600, 20, 'f', 2);
    out << QString("%1%2%3\n").arg("液相流量（kg/hr）", -28).arg(result.inletLiquidFlow * 3600, 20, 'f', 2).arg(result.outletLiquidFlow * 3600, 20, 'f', 2);
    out << QString("%1%2%3\n\n").arg("摩擦系数（-）", -28).arg(result.inletFrictionFactor, 20, 'f', 4).arg(result.outletFrictionFactor, 20, 'f', 4);

    // 3. 其它参数
    out << "其它参数\n";
    out << "-----------------------------------------------------------------------------------\n";
    out << QString("%1: %2\n").arg("管道外径（mm）", -28).arg(result.pipeOd * 1000, 20, 'f', 2);
    out << QString("%1: %2\n").arg("管道内径（mm）", -28).arg(result.pipeId * 1000, 20, 'f', 2);
    out << QString("%1: %2\n").arg("管道长度（m）", -28).arg(result.length, 20, 'f', 2);
    out << QString("%1: %2\n").arg("管道粗糙度（mm）", -27).arg(result.roughness * 1000, 20, 'f', 4);
    out << QString("%1: %2\n").arg("管件总阻力系数", -27).arg(result.totalFittingsResistance, 20, 'f', 4);
    out << QString("%1: %2\n").arg("保温厚度（mm）", -28).arg(result.insulationThickness * 1000, 20, 'f', 2);
    out << QString("%1: %2\n").arg("保护层黑度", -29).arg(result.emissivity, 20, 'f', 4);
    out << QString("%1: %2\n").arg("环境温度（C）", -28).arg(result.ambientTemperature - 273.15, 20, 'f', 2);
    out << QString("%1: %2\n").arg("风速（m/sec）", -30).arg(result.windSpeed, 20, 'f', 2);
    out << QString("%1: %2\n").arg("管道类型", -30).arg(QString::fromStdString(result.pipeTypeName), 20);
    out << QString("%1: %2\n").arg("保温材料", -30).arg(QString::fromStdString(result.insulationMaterialName), 19);
    out << QString("%1: %2\n\n").arg("保护层材料", -29).arg(QString::fromStdString(result.protectionMaterialName), 12);



    // 4. 分段计算结果
    out << "分段计算结果\n";
    out << "-----------------------------------------------------------------------------------\n";
    out << QString("%1  %2  %3  %4  %5  %6  %7  %8\n")
               .arg("分段序号", -2)
               .arg("流速", -5)
               .arg("压力", -6)
               .arg("温度", -6)
               //.arg("焓值", -6)
               //.arg("压力损失", -4)
               //.arg("热损失", -4)
               .arg("表面温度", -4)
               .arg("保温导热系数", -4)
               .arg("气相流量", -6)
               .arg("液相流量", -6);
    out << QString("%1  %2  %3  %4  %5  %6  %7  %8\n")
               .arg("--------", -8)
               .arg("(m/s)", -7)
               .arg("(kPa)", -8)
               .arg("(C)", -8)
               //.arg("(kJ/kg)", -8)
               //.arg("(kPa)", -8)
               //.arg("(kW)", -7)
               .arg("(C)", -8)
               .arg("(W/(m·K))", -12)
               .arg("(kg/hr)", -10)
               .arg("(kg/hr)", -10);
    out << "-----------------------------------------------------------------------------------\n";

    // 写入分段数据
    for (int i = 0; i < result.segmentResults.size(); ++i)
    {
        const auto& seg = result.segmentResults[i];
        out << QString("%1  %2  %3  %4  %5  %6  %7  %8\n")
                   .arg(i + 1, -8)
                   .arg(seg.at("velocity"), -7, 'f', 2)
                   .arg(seg.at("outletPressure") / 1000, -8, 'f', 2)
                   .arg(seg.at("outletTemperature") - 273.15, -8, 'f', 2)
                   //.arg(seg.at("outletEnthalpy") / 1000, -9, 'f', 2)
                   //.arg(seg.at("pressureDrop") / 1000, -7, 'f', 2)
                   //.arg(seg.at("totalHeatLoss") / 1000, -7, 'f', 2)
                   .arg(seg.at("surfaceTemperature") - 273.15, -8, 'f', 2)
                   .arg(seg.at("insulationConductivity"), -12, 'f', 4)
                   .arg(seg.at("vaporFlow") * 3600, -10, 'f', 2)
                   .arg(seg.at("liquidFlow") * 3600, -8, 'f', 2);
    }

    // 报告尾部
    out << "===================================================================================\n";
    out << "报告生成工具：汤圆工具集 管道阻力和绝热分析\n";
    out << "作者：杨奉全，qq交流群：816103114\n";
    out << "===================================================================================\n";

    reportFile.close();

    // 调用Notepad3.exe打开报告
    QString notepad3Path = appDir + "/Notepad++.exe";
    if (!QFile::exists(notepad3Path))
    {
        QMessageBox::warning(nullptr, "警告", "未在程序目录找到Notepad++.exe，路径：\n" + notepad3Path);
        return false;
    }

    QProcess* notepadProcess = new QProcess();
    QObject::connect(notepadProcess, &QProcess::finished,
                     notepadProcess, &QObject::deleteLater);
    notepadProcess->start(notepad3Path, {reportFullPath});

    if (!notepadProcess->waitForStarted(1000))
    {
        QMessageBox::critical(nullptr, "错误", "Notepad++.exe启动失败：" + notepadProcess->errorString());
        notepadProcess->deleteLater();
        return false;
    }

    return true;
}
