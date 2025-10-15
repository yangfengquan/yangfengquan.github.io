#include "FluidAnalyzer.h"

extern "C" {
#include "CoolPropLib.h"
}
#include <cmath>
#include <algorithm>
#include <map>
#include <string>
#include <QString>
#include <QMessageBox>
#include <QDebug>
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
    qDebug()<<"massFlow"<<massFlow<<"density"<<density<<"viscosity"<<viscosity<<"diameter"<<diameter<<"length"<<length<<"roughness"<<roughness;
    double area = M_PI * pow(diameter/2, 2);

    double velocity = massFlow / (density * area);
    double Re = (density * velocity * diameter) / viscosity;
    double f = frictionFactor(Re, roughness, diameter);
    qDebug()<<"velocity"<<velocity<<"f"<<f;
    // 沿程阻力损失
    double frictionPressureDrop = f * (length / diameter) * (density * pow(velocity, 2)) / 2;

    // 局部阻力损失
    double fittingsPressureDrop = fittingsResistance * (density * pow(velocity, 2)) / 2;

    double totalPressureDrop = frictionPressureDrop + fittingsPressureDrop;
    qDebug()<<"totalPressureDrop"<<totalPressureDrop<<"frictionPressureDrop"<<frictionPressureDrop<<"fittingsPressureDrop"<<fittingsPressureDrop;
    std::map<std::string, double> results;

    results["velocity"] = velocity;
    results["reynolds"] = Re;
    results["friction"] = f;
    results["pressureDrop"] = totalPressureDrop;
    results["frictionPressureDrop"] = frictionPressureDrop;
    results["fittingsPressureDrop"] = fittingsPressureDrop;

    return results;
}

std::map<std::string, double> FluidAnalyzer::heatLossCalculation(
    double fluidTemp, double ambientTemperature,
    double pipeOd, double insulationThickness,
    const InsulationMaterial& insulationMaterial,
    const OuterProtection& protectionMaterial,
    double length, double windSpeed)
{

        if (insulationThickness <= 0) {
            // 无保温情况
            double d_outer = pipeOd;
            double h_combined = calculateExternalHeatTransfer(
                fluidTemp, ambientTemperature, windSpeed, protectionMaterial.emissivity, pipeOd, d_outer);

            double surface_area = M_PI * pipeOd * length;
            double Q_total = h_combined * surface_area * (fluidTemp - ambientTemperature);
            double Q_per_m = Q_total / length;
            double surfaceTemperature = ambientTemperature + 0.7 * (fluidTemp - ambientTemperature);

            // 计算单位外表面积热损失
            double surface_area_per_m = M_PI * d_outer;
            double Q_per_area = (surface_area_per_m > 0) ? Q_per_m / surface_area_per_m : 0.0;

            std::map<std::string, double> results;
            results["totalHeatLoss"] = Q_total;
            results["heatLossPerM"] = Q_per_m;
            results["heatLossPerArea"] = Q_per_area;
            results["overallHeatTransferCoeff"] = h_combined;
            results["convectionCoeff"] = h_combined * 0.8;
            results["surfaceTemperature_k"] = surfaceTemperature;
            results["insulationConductivity"] = 0.0;

            return results;
        }

        // 有保温情况 - 迭代计算
        double d_outer = pipeOd + 2 * insulationThickness;

        // 初始假设表面温度
        double surfaceTemperature = ambientTemperature + 0.3 * (fluidTemp - ambientTemperature);
        double insulation = 0.04; // 默认值
        double h_external = 0.0;
        double Q_per_m = 0.0;

        for (int iteration = 0; iteration < 20; ++iteration) {
            // 计算保温层平均温度
            double tm_k = (fluidTemp + surfaceTemperature) / 2;

            // 计算保温材料导热系数
            insulation = insulationMaterial.calculateConductivity(tm_k);

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
            Q_per_m = (fluidTemp - ambientTemperature) / R_total;

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

double FluidAnalyzer::calculateExternalHeatTransfer(double surfaceTemp, double ambientTemperature,
                                                    double windSpeed, double emissivity,
                                                    double pipeOd, double dOuter)
{
    // 对流换热系数
    double h_conv = calculateConvectionCoeff(windSpeed, surfaceTemp, ambientTemperature, pipeOd, dOuter);

    // 辐射换热系数
    double h_rad = calculateRadiationCoeff(surfaceTemp, ambientTemperature, emissivity);

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
    double sigma = 5.669e-8; // Stefan-Boltzmann常数
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
    qDebug()<<"pressureDrop"<<dpResults["pressureDrop"];
    // 2. 热损失计算
    std::map<std::string, double> heatLossResults = heatLossCalculation(
        inletTemperature, ambientTemperature, pipeOd,
        insulationThickness, insulationMaterial, protectionMaterial, length, windSpeed
        );

    // 3. 能量平衡
    double delta_h = (massFlow > 0) ? heatLossResults["totalHeatLoss"] / massFlow : 0;
    double outletEnthalpy = inletEnthalpy - delta_h;

    // 4. 出口状态
    double outletQuality = PropsSI("Q", "P", outletPressure, "H", outletEnthalpy, fluid);
    double outletTemperature = PropsSI("T", "P", outletPressure, "H", outletEnthalpy, fluid);
qDebug()<<"delta_h"<<delta_h<<"outletTemperature"<<outletTemperature;
    // 5. 计算气相和液相流量
    double vaporFlow = massFlow * outletQuality;
    double liquidFlow = massFlow * (1 - outletQuality);

    std::map<std::string, double> results;
    // 组装结果
    results["velocity"] = dpResults["velocity"];//inlet
    results["reynoldsr"] = dpResults["reynoldsr"];
    results["frictionFactor"] = dpResults["frictionFactor"];
    results["pressureDrop"] = dpResults["pressureDrop"];
    results["frictionPressureDrop"] = dpResults["frictionPressureDrop"];
    results["fittingsPressureDrop"] = dpResults["fittingsPressureDrop"];
    results["totalHeatLoss"] = heatLossResults["totalHeatLoss"];
    results["heatLossPerM"] = heatLossResults["heat_loss_per"];
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
{qDebug()<<"massFlow"<<10000/3600;
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
            insulationMaterial.density = 100;
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
        if (inletQuality != -1) {
            inletTemperature = PropsSI("T", "P", inletPressure, "Q", inletQuality, fluid);
            inletEnthalpy = PropsSI("H", "P", inletPressure, "Q", inletQuality, fluid);
            inletDensity = PropsSI("D", "P", inletPressure, "Q", inletQuality, fluid);
            inletViscosity = PropsSI("V", "P", inletPressure, "Q", inletQuality, fluid);
        } else {
            inletQuality = PropsSI("Q", "P", inletPressure, "T", inletTemperature, fluid);
            inletEnthalpy = PropsSI("H", "P", inletPressure, "T", inletTemperature, fluid);
            inletDensity = PropsSI("D", "P", inletPressure, "T", inletTemperature, fluid);
            inletViscosity = PropsSI("V", "P", inletPressure, "T", inletTemperature, fluid);
        }

        // 计算入口流速
        double area = M_PI * pow((pipeOd - 2 * pipeWallThickness) / 2, 2);
        double inletVelocity = massFlow / (inletDensity * area);

        //QVariantMap current_props = inlet_props;
        //QList<QVariantMap> segment_results;
        std::vector<std::map<std::string, double>> segmentResults;
        int num_segments = std::max(1, (int)ceil(length / segmentLength));
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

        if(!segmentResults.empty()){

        }

        result.massFlow = massFlow;
        result.inletPressure = inletPressure;
        result.inletTemperature = inletTemperature;
        result.inletEnthalpy = inletEnthalpy;
        result.inletQuality = inletQuality;
        result.inletVelocity = inletVelocity;
        result.inletDensity = inletDensity;
        result.inletVaporFlow = massFlow * inletQuality;
        result.inletLiquidFlow = massFlow * (1 - inletQuality);
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
            result.totalPressureDrop = result.outletPressure - result.inletPressure;
            result.maxVelocity = 0;
            result.maxSurfaceTemperature = 0;
            double velocitySum = 0;
            double surfaceTemperatureSum = 0;
            for (auto& segmentResult: segmentResults) {
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
        qDebug() << "管道分析错误:" << e.what();
        QMessageBox::critical(nullptr, "错误", QString("管道分析失败: %1").arg(e.what()));
    }

    return result;
}
/*
QVariantMap FluidAnalyzer::comprehensiveResultsAnalysis(const QList<QVariantMap>& results,
                                                        const QVariantMap& inletPressurerops,
                                                        double totalLength, double massFlow,
                                                        const QString& pipeName, const QString& fluid,
                                                        double fittingsResistance, double inletVelocity,
                                                        double roughness)
{
    QVariantMap analysis;

    if (results.isEmpty()) {
        return analysis;
    }

    const QVariantMap& final_result = results.last();

    double final_pressure_pa = final_result["pressure_pa"].toDouble();
    double final_temperature_k = final_result["temperature_k"].toDouble();
    double final_quality = final_result["quality"].toDouble();
    double final_velocity = final_result["velocity_m_s"].toDouble();
    double final_vaporFlow = final_result["vaporFlow_kg_s"].toDouble();
    double final_liquidFlow = final_result["liquidFlow_kg_s"].toDouble();

    double total_pressureDropa = inletPressurerops["pressure_pa"].toDouble() - final_pressure_pa;
    double total_temperature_drop = inletPressurerops["temperature_k"].toDouble() - final_temperature_k;

    double totalHeatLoss_w = 0;
    double total_frictionPressureDropa = 0;
    double total_fittingsPressureDropa = 0;

    double max_surfaceTemperature = 0;
    double min_velocity = final_velocity;
    double max_velocity = final_velocity;
    double total_reynolds = 0;
    double total_friction = 0;

    for (const QVariantMap& segment : results) {
        totalHeatLoss_w += segment["totalHeatLoss_w"].toDouble();
        total_frictionPressureDropa += segment["frictionPressureDropa"].toDouble();
        total_fittingsPressureDropa += segment["fittingsPressureDropa"].toDouble();

        max_surfaceTemperature = std::max(max_surfaceTemperature, segment["surfaceTemperature_k"].toDouble());
        min_velocity = std::min(min_velocity, segment["velocity_m_s"].toDouble());
        max_velocity = std::max(max_velocity, segment["velocity_m_s"].toDouble());

        total_reynolds += segment["reynolds_number"].toDouble();
        total_friction += segment["frictionFactor"].toDouble();
    }

    double pressureDropa_m = total_pressureDropa / totalLength;
    double pressure_ratio = total_pressureDropa / inletPressurerops["pressure_pa"].toDouble();

    // 热损失参数
    double avg_heatLossPerM = totalHeatLoss_w / totalLength;
    double avg_heatLossPerArea = 0;
    for (const QVariantMap& segment : results) {
        avg_heatLossPerArea += segment["heatLossPerArea_w"].toDouble();
    }
    avg_heatLossPerArea /= results.size();

    double avg_surfaceTemperature = 0;
    for (const QVariantMap& segment : results) {
        avg_surfaceTemperature += segment["surfaceTemperature_k"].toDouble();
    }
    avg_surfaceTemperature /= results.size();

    // 流动参数
    double avg_velocity = 0;
    for (const QVariantMap& segment : results) {
        avg_velocity += segment["velocity_m_s"].toDouble();
    }
    avg_velocity /= results.size();

    double avg_reynolds = total_reynolds / results.size();
    double avg_friction = total_friction / results.size();

    analysis["pipe_name"] = pipeName;
    analysis["fluid"] = fluid;
    analysis["inletPressure_pa"] = inletPressurerops["pressure_pa"];
    analysis["inletTemperature_k"] = inletPressurerops["temperature_k"];
    analysis["inlet_quality"] = inletPressurerops["quality"];
    analysis["inlet_velocity_m_s"] = inletVelocity;
    analysis["outletPressure_pa"] = final_pressure_pa;
    analysis["outletTemperature_k"] = final_temperature_k;
    analysis["outletQuality"] = final_quality;
    analysis["outlet_velocity_m_s"] = final_velocity;
    analysis["outlet_vaporFlow_kg_s"] = final_vaporFlow;
    analysis["outlet_liquidFlow_kg_s"] = final_liquidFlow;
    analysis["total_pressureDropa"] = total_pressureDropa;
    analysis["pressureDropa_m"] = pressureDropa_m;
    analysis["pressure_ratio"] = pressure_ratio;
    analysis["total_temperature_drop_k"] = total_temperature_drop;
    analysis["totalHeatLoss_w"] = totalHeatLoss_w;
    analysis["total_frictionPressureDropa"] = total_frictionPressureDropa;
    analysis["total_fittingsPressureDropa"] = total_fittingsPressureDropa;
    analysis["fittings_resistance"] = fittingsResistance;
    analysis["pipe_roughness_m"] = roughness;
    analysis["avg_heatLossPerM_w"] = avg_heatLossPerM;
    analysis["avg_heatLossPerArea_w"] = avg_heatLossPerArea;
    analysis["max_surfaceTemperature_k"] = max_surfaceTemperature;
    analysis["avg_surfaceTemperature_k"] = avg_surfaceTemperature;
    analysis["max_velocity_m_s"] = max_velocity;
    analysis["min_velocity_m_s"] = min_velocity;
    analysis["avg_velocity_m_s"] = avg_velocity;
    analysis["avg_reynolds"] = avg_reynolds;
    analysis["avg_friction"] = avg_friction;

    return analysis;
}
*/
