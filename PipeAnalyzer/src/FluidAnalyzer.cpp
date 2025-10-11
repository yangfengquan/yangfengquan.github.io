#include "FluidAnalyzer.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <cmath>
#include <algorithm>
extern "C" {
#include "CoolPropLib.h"
}
// 使用标准库函数
using std::exp;
using std::log;
using std::pow;
using std::sqrt;

FluidAnalyzer::FluidAnalyzer(QObject *parent)
    : QObject(parent)
    , materialManager(new MaterialManager(this))
{
    // 初始化 CoolProp（如果需要）
}

FluidAnalyzer::~FluidAnalyzer()
{
}

QVariantMap FluidAnalyzer::getFluidProperties(double pressurePa, double temperatureK,
                                              double enthalpy, double quality,
                                              const QString& fluid)
{
/*    QVariantMap props;

    try {
        // 将 QString 转换为 char*
        QByteArray fluidBa = fluid.toUtf8();
        const char* fluidStr = fluidBa.constData();

        double T_k, h, rho, mu, k_val, cp, quality_out;
        char errorMsg[256];
        int err = 0;

        if (quality >= 0 && quality <= 1) {
            // 使用干度计算
            T_k = PropsSI("T", "P", pressurePa, "Q", quality, fluidStr);
            h = PropsSI("H", "P", pressurePa, "Q", quality, fluidStr);
            quality_out = quality;
        } else if (enthalpy >= 0) {
            // 使用焓值计算
            T_k = PropsSI("T", "P", pressurePa, "H", enthalpy, fluidStr);
            h = enthalpy;
            quality_out = PropsSI("Q", "P", pressurePa, "H", enthalpy, fluidStr);
        } else {
            // 使用温度计算
            h = PropsSI("H", "P", pressurePa, "T", temperatureK, fluidStr);
            quality_out = PropsSI("Q", "P", pressurePa, "T", temperatureK, fluidStr);
            T_k = temperatureK;
        }

        // 获取其他物性
        rho = PropsSI("D", "P", pressurePa, "H", h, fluidStr);
        mu = PropsSI("V", "P", pressurePa, "H", h, fluidStr);
        k_val = PropsSI("L", "P", pressurePa, "H", h, fluidStr);
        cp = PropsSI("C", "P", pressurePa, "H", h, fluidStr);

        // 获取饱和温度
        double T_sat;
        try {
            T_sat = PropsSI("T", "P", pressurePa, "Q", 0, fluidStr);
        } catch (...) {
            T_sat = 373.15; // 默认100°C
        }

        QString phase;
        if (quality_out <= 0) {
            phase = (T_k < T_sat) ? "subcooled_liquid" : "saturated_liquid";
        } else if (quality_out >= 1) {
            phase = (T_k > T_sat) ? "superheated_vapor" : "saturated_vapor";
        } else {
            phase = "two_phase";
        }

        props["pressure_pa"] = pressurePa;
        props["temperature_k"] = T_k;
        props["density_kg_per_m3"] = rho;
        props["viscosity_Pa_s"] = mu;
        props["enthalpy_J_per_kg"] = h;
        props["conductivity_W_per_mK"] = k_val;
        props["specific_heat_J_per_kgK"] = cp;
        props["quality"] = std::max(0.0, std::min(1.0, quality_out));
        props["saturation_temperature_k"] = T_sat;
        props["phase"] = phase;
        props["superheat_k"] = std::max(0.0, T_k - T_sat);
        props["subcooling_k"] = std::max(0.0, T_sat - T_k);

    } catch (const std::exception& e) {
        qDebug() << "流体物性计算失败 (" << fluid << "):" << e.what();
        // 设置默认值
        props["pressure_pa"] = pressurePa;
        props["temperature_k"] = temperatureK;
        props["density_kg_per_m3"] = 1000;
        props["viscosity_Pa_s"] = 0.001;
        props["enthalpy_J_per_kg"] = 419000;
        props["conductivity_W_per_mK"] = 0.6;
        props["specific_heat_J_per_kgK"] = 4186;
        props["quality"] = 0.0;
        props["saturation_temperature_k"] = 373.15;
        props["phase"] = "subcooled_liquid";
        props["superheat_k"] = 0.0;
        props["subcooling_k"] = 0.0;
    }

    return props;
*/
    QVariantMap properties;

    try {
        // 确保使用 C 调用约定
        double density = ::PropsSI("D", "P", pressurePa, "T", temperatureK, "water");
        double enthalpy = ::PropsSI("H", "P", pressurePa, "T", temperatureK, "water");
        double viscosity = ::PropsSI("V", "P", pressurePa, "T", temperatureK, "water");
        double conductivity = ::PropsSI("L", "P", pressurePa, "T", temperatureK, "water");

        properties["density"] = density;
        properties["enthalpy"] = enthalpy;
        properties["viscosity"] = viscosity;
        properties["conductivity"] = conductivity;

    } catch (const std::exception& e) {
        throw std::runtime_error(QString("流体属性计算失败: %1").arg(e.what()).toStdString());
    }

    return properties;
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

QVariantMap FluidAnalyzer::pressureDropCalculation(const QVariantMap& props, double massFlow,
                                                   double diameter, double length,
                                                   double roughness, double fittingsResistance)
{
    QVariantMap result;

    try {
        double area = M_PI * pow(diameter/2, 2);
        double rho = props["density_kg_per_m3"].toDouble();
        double mu = props["viscosity_Pa_s"].toDouble();

        double velocity = massFlow / (rho * area);
        double Re = (rho * velocity * diameter) / mu;
        double f = frictionFactor(Re, roughness, diameter);

        // 沿程阻力损失
        double dp_friction = f * (length / diameter) * (rho * pow(velocity, 2)) / 2;

        // 局部阻力损失
        double dp_fittings = fittingsResistance * (rho * pow(velocity, 2)) / 2;

        double dp_total = dp_friction + dp_fittings;

        result["velocity_m_s"] = velocity;
        result["reynolds_number"] = Re;
        result["friction_factor"] = f;
        result["pressure_drop_pa"] = dp_total;
        result["friction_drop_pa"] = dp_friction;
        result["fittings_drop_pa"] = dp_fittings;

    } catch (const std::exception& e) {
        qDebug() << "压力降计算错误:" << e.what();
        result["velocity_m_s"] = 0.0;
        result["reynolds_number"] = 0.0;
        result["friction_factor"] = 0.0;
        result["pressure_drop_pa"] = 0.0;
        result["friction_drop_pa"] = 0.0;
        result["fittings_drop_pa"] = 0.0;
    }

    return result;
}

QVariantMap FluidAnalyzer::heatLossCalculation(double fluidTempK, double ambientTempK,
                                               double pipeOd, double insulationThickness,
                                               const InsulationMaterial& insulationMaterial,
                                               const OuterProtection& protectionMaterial,
                                               double length, double windSpeed)
{
    QVariantMap result;

    try {
        if (insulationThickness <= 0) {
            // 无保温情况
            double d_outer = pipeOd;
            double h_combined = calculateExternalHeatTransfer(
                fluidTempK, ambientTempK, windSpeed, protectionMaterial.emissivity, pipeOd, d_outer);

            double surface_area = M_PI * pipeOd * length;
            double Q_total = h_combined * surface_area * (fluidTempK - ambientTempK);
            double Q_per_m = Q_total / length;
            double surface_temp_k = ambientTempK + 0.7 * (fluidTempK - ambientTempK);

            // 计算单位外表面积热损失
            double surface_area_per_m = M_PI * d_outer;
            double Q_per_area = (surface_area_per_m > 0) ? Q_per_m / surface_area_per_m : 0.0;

            result["total_heat_loss_w"] = Q_total;
            result["heat_loss_per_m_w"] = Q_per_m;
            result["heat_loss_per_area_w"] = Q_per_area;
            result["overall_heat_transfer_coeff"] = h_combined;
            result["convection_coeff"] = h_combined * 0.8;
            result["surface_temp_k"] = surface_temp_k;
            result["insulation_conductivity"] = 0.0;

            return result;
        }

        // 有保温情况 - 迭代计算
        double d_outer = pipeOd + 2 * insulationThickness;

        // 初始假设表面温度
        double surface_temp_k = ambientTempK + 0.3 * (fluidTempK - ambientTempK);
        double insulation_k = 0.04; // 默认值
        double h_external = 0.0;
        double Q_per_m = 0.0;

        for (int iteration = 0; iteration < 20; ++iteration) {
            // 计算保温层平均温度
            double tm_k = (fluidTempK + surface_temp_k) / 2;

            // 计算保温材料导热系数
            insulation_k = insulationMaterial.calculateConductivity(tm_k);

            // 计算保温层热阻
            double R_insulation = log(d_outer / pipeOd) / (2 * M_PI * insulation_k);

            // 计算外部换热系数
            h_external = calculateExternalHeatTransfer(
                surface_temp_k, ambientTempK, windSpeed, protectionMaterial.emissivity, pipeOd, d_outer);

            // 计算外部热阻
            double R_external = 1 / (h_external * M_PI * d_outer);

            // 总热阻
            double R_total = R_insulation + R_external;

            // 单位长度热损失
            Q_per_m = (fluidTempK - ambientTempK) / R_total;

            // 更新表面温度
            double surface_temp_new_k = ambientTempK + Q_per_m * R_external;

            // 收敛检查
            if (fabs(surface_temp_new_k - surface_temp_k) < 0.1) {
                surface_temp_k = surface_temp_new_k;
                break;
            }

            surface_temp_k = surface_temp_new_k;
        }

        // 总热损失
        double Q_total = Q_per_m * length;

        // 计算对流换热系数
        double h_convection = calculateConvectionCoeff(windSpeed, surface_temp_k, ambientTempK, pipeOd, d_outer);

        // 计算单位外表面积热损失
        double surface_area_per_m = M_PI * d_outer;
        double Q_per_area = (surface_area_per_m > 0) ? Q_per_m / surface_area_per_m : 0.0;

        result["total_heat_loss_w"] = Q_total;
        result["heat_loss_per_m_w"] = Q_per_m;
        result["heat_loss_per_area_w"] = Q_per_area;
        result["overall_heat_transfer_coeff"] = h_external;
        result["convection_coeff"] = h_convection;
        result["surface_temp_k"] = surface_temp_k;
        result["insulation_conductivity"] = insulation_k;

    } catch (const std::exception& e) {
        qDebug() << "热损失计算错误:" << e.what();
        result["total_heat_loss_w"] = 0.0;
        result["heat_loss_per_m_w"] = 0.0;
        result["heat_loss_per_area_w"] = 0.0;
        result["overall_heat_transfer_coeff"] = 0.0;
        result["convection_coeff"] = 0.0;
        result["surface_temp_k"] = ambientTempK;
        result["insulation_conductivity"] = 0.0;
    }

    return result;
}

double FluidAnalyzer::calculateExternalHeatTransfer(double surfaceTempK, double ambientTempK,
                                                    double windSpeed, double emissivity,
                                                    double pipeOd, double dOuter)
{
    // 对流换热系数
    double h_conv = calculateConvectionCoeff(windSpeed, surfaceTempK, ambientTempK, pipeOd, dOuter);

    // 辐射换热系数
    double h_rad = calculateRadiationCoeff(surfaceTempK, ambientTempK, emissivity);

    return h_conv + h_rad;
}

double FluidAnalyzer::calculateConvectionCoeff(double windSpeed, double surfaceTempK,
                                               double ambientTempK, double pipeOd, double dOuter)
{
    double h_conv = 0.0;

    if (windSpeed == 0) {
        // 无风时自然对流
        double T_avg = (surfaceTempK + ambientTempK) / 2;
        h_conv = 26.4 / sqrt(297 - 273.15 + 0.5 * T_avg) * pow((surfaceTempK - ambientTempK) / pipeOd, 0.25);
    } else if (windSpeed * dOuter <= 0.8) {
        // 低风速情况
        h_conv = 0.08 / dOuter + 4.2 * pow(windSpeed, 0.618) / pow(dOuter, 0.382);
    } else {
        // 强制对流
        h_conv = 4.53 * pow(windSpeed, 0.805) / pow(pipeOd, 0.195);
    }

    return std::max(1.0, h_conv); // 确保最小值
}

double FluidAnalyzer::calculateRadiationCoeff(double surfaceTempK, double ambientTempK,
                                              double emissivity)
{
    double sigma = 5.669e-8; // Stefan-Boltzmann常数
    double T_diff = surfaceTempK - ambientTempK;

    if (T_diff <= 0) {
        return 0.0;
    }

    double h_rad = sigma * emissivity / T_diff *
                   (pow(surfaceTempK/100, 4) - pow(ambientTempK/100, 4));

    return std::max(0.0, h_rad);
}

QVariantMap FluidAnalyzer::analyzePipeSegment(const QVariantMap& inletProps, double massFlow,
                                              double pipeOd, double wallThickness, double length,
                                              const PipeType& pipeType, double insulationThickness,
                                              const InsulationMaterial& insulationMaterial,
                                              const OuterProtection& protectionMaterial,
                                              double ambientTempK, double fittingsResistance,
                                              double windSpeed, const QString& fluid)
{
    QVariantMap result;

    try {
        // 1. 压力降计算
        QVariantMap dp_result = pressureDropCalculation(
            inletProps, massFlow, pipeOd - 2*wallThickness, length,
            pipeType.roughness, fittingsResistance
            );

        double outlet_pressure = std::max(10.0, inletProps["pressure_pa"].toDouble() - dp_result["pressure_drop_pa"].toDouble());

        // 2. 热损失计算
        QVariantMap heat_loss_result = heatLossCalculation(
            inletProps["temperature_k"].toDouble(), ambientTempK, pipeOd,
            insulationThickness, insulationMaterial, protectionMaterial, length, windSpeed
            );

        // 3. 能量平衡
        double delta_h = (massFlow > 0) ? heat_loss_result["total_heat_loss_w"].toDouble() / massFlow : 0;
        double outlet_enthalpy = inletProps["enthalpy_J_per_kg"].toDouble() - delta_h;

        // 4. 出口状态
        QVariantMap outlet_props = getFluidProperties(outlet_pressure, -1, outlet_enthalpy, -1, fluid);

        // 5. 计算气相和液相流量
        double vapor_flow = massFlow * outlet_props["quality"].toDouble();
        double liquid_flow = massFlow * (1 - outlet_props["quality"].toDouble());

        // 组装结果
        result = outlet_props;
        result["velocity_m_s"] = dp_result["velocity_m_s"];
        result["reynolds_number"] = dp_result["reynolds_number"];
        result["friction_factor"] = dp_result["friction_factor"];
        result["pressure_drop_pa"] = dp_result["pressure_drop_pa"];
        result["friction_drop_pa"] = dp_result["friction_drop_pa"];
        result["fittings_drop_pa"] = dp_result["fittings_drop_pa"];
        result["total_heat_loss_w"] = heat_loss_result["total_heat_loss_w"];
        result["heat_loss_per_m_w"] = heat_loss_result["heat_loss_per_m_w"];
        result["heat_loss_per_area_w"] = heat_loss_result["heat_loss_per_area_w"];
        result["overall_heat_transfer_coeff"] = heat_loss_result["overall_heat_transfer_coeff"];
        result["convection_coeff"] = heat_loss_result["convection_coeff"];
        result["surface_temp_k"] = heat_loss_result["surface_temp_k"];
        result["insulation_conductivity"] = heat_loss_result["insulation_conductivity"];
        result["vapor_flow_kg_s"] = vapor_flow;
        result["liquid_flow_kg_s"] = liquid_flow;

    } catch (const std::exception& e) {
        qDebug() << "管段分析错误:" << e.what();
        result = inletProps; // 返回入口参数作为默认值
    }

    return result;
}

FluidAnalyzer::AnalysisResult FluidAnalyzer::analyzePipe(const QVariantMap& params)
{
    AnalysisResult result;

    try {
        // 解析输入参数
        QString pipeName = params["pipe_name"].toString();
        QString fluid = params["fluid"].toString();
        double inletPressurePa = params["inlet_pressure_pa"].toDouble();
        double inletTemperatureK = params["inlet_temperature_k"].toDouble();
        double massFlowKgS = params["mass_flow_kg_s"].toDouble();
        double pipeLengthM = params["pipe_length_m"].toDouble();
        double pipeOdM = params["pipe_od_m"].toDouble();
        double pipeWallThicknessM = params["pipe_wall_thickness_m"].toDouble();
        QString pipeTypeName = params["pipe_type"].toString();
        double insulationThicknessM = params["insulation_thickness_m"].toDouble();
        QString insulationMaterialName = params["insulation_material"].toString();
        QString protectionMaterialName = params["protection_material"].toString();
        double ambientTemperatureK = params["ambient_temperature_k"].toDouble();
        double windSpeedMS = params["wind_speed_m_s"].toDouble();
        double segmentLengthM = params["segment_length_m"].toDouble();

        // 获取材料
        PipeType pipeType;
        if (materialManager->getPipeTypes().contains(pipeTypeName)) {
            pipeType = materialManager->getPipeTypes()[pipeTypeName];
        } else {
            pipeType.name = "默认管道";
            pipeType.roughness = 2e-5;
            pipeType.description = "默认管道类型";
        }

        InsulationMaterial insulationMaterial;
        if (materialManager->getInsulationMaterials().contains(insulationMaterialName)) {
            insulationMaterial = materialManager->getInsulationMaterials()[insulationMaterialName];
        } else {
            insulationMaterial.name = "默认保温";
            insulationMaterial.density = 100;
            insulationMaterial.description = "默认保温材料";
        }

        OuterProtection protectionMaterial;
        if (materialManager->getProtectionMaterials().contains(protectionMaterialName)) {
            protectionMaterial = materialManager->getProtectionMaterials()[protectionMaterialName];
        } else {
            protectionMaterial.name = "默认保护层";
            protectionMaterial.emissivity = 0.3;
            protectionMaterial.description = "默认保护层材料";
        }

        // 计算总局部阻力系数
        double total_fittings_resistance = 0;
        QList<QVariant> fittings_data = params["fittings_data"].toList();
        for (const QVariant& fitting_var : fittings_data) {
            QVariantMap fitting = fitting_var.toMap();
            QString fitting_name = fitting["name"].toString();
            int count = fitting["count"].toInt();

            if (materialManager->getPipeFittings().contains(fitting_name)) {
                PipeFitting pipe_fitting = materialManager->getPipeFittings()[fitting_name];
                total_fittings_resistance += pipe_fitting.resistanceCoef * count;
            }
        }

        // 获取入口条件
        QVariantMap inlet_props;
        if (params.contains("inlet_quality")) {
            double inlet_quality = params["inlet_quality"].toDouble();
            inlet_props = getFluidProperties(inletPressurePa, -1, -1, inlet_quality, fluid);
        } else {
            inlet_props = getFluidProperties(inletPressurePa, inletTemperatureK, -1, -1, fluid);
        }

        // 计算入口流速
        double area = M_PI * pow((pipeOdM - 2 * pipeWallThicknessM) / 2, 2);
        double inlet_velocity = massFlowKgS / (inlet_props["density_kg_per_m3"].toDouble() * area);

        QVariantMap current_props = inlet_props;
        QList<QVariantMap> segment_results;

        int num_segments = std::max(1, (int)ceil(pipeLengthM / segmentLengthM));
        double segment_fittings_resistance = total_fittings_resistance / num_segments;

        for (int i = 0; i < num_segments; ++i) {
            double seg_length = std::min(segmentLengthM, pipeLengthM - i * segmentLengthM);
            double distance = (i + 1) * seg_length;

            QVariantMap segment_result = analyzePipeSegment(
                current_props, massFlowKgS, pipeOdM, pipeWallThicknessM, seg_length,
                pipeType, insulationThicknessM, insulationMaterial,
                protectionMaterial, ambientTemperatureK, segment_fittings_resistance,
                windSpeedMS, fluid
                );

            segment_result["segment"] = i + 1;
            segment_result["distance_m"] = distance;

            segment_results.append(segment_result);
            current_props = segment_result;
        }

        // 完整结果分析
        QVariantMap analysis_results = comprehensiveResultsAnalysis(
            segment_results, inlet_props, pipeLengthM, massFlowKgS, pipeName, fluid,
            total_fittings_resistance, inlet_velocity, pipeType.roughness
            );

        // 绘制完整图表
        QChart* chart = plotComprehensiveResults(segment_results, pipeName);

        result.segmentResults = segment_results;
        result.summary = analysis_results;
        result.chart = chart;

    } catch (const std::exception& e) {
        qDebug() << "管道分析错误:" << e.what();
        QMessageBox::critical(nullptr, "错误", QString("管道分析失败: %1").arg(e.what()));
    }

    return result;
}

QVariantMap FluidAnalyzer::comprehensiveResultsAnalysis(const QList<QVariantMap>& results,
                                                        const QVariantMap& inletProps,
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
    double final_vapor_flow = final_result["vapor_flow_kg_s"].toDouble();
    double final_liquid_flow = final_result["liquid_flow_kg_s"].toDouble();

    double total_pressure_drop_pa = inletProps["pressure_pa"].toDouble() - final_pressure_pa;
    double total_temperature_drop = inletProps["temperature_k"].toDouble() - final_temperature_k;

    double total_heat_loss_w = 0;
    double total_friction_drop_pa = 0;
    double total_fittings_drop_pa = 0;

    double max_surface_temp = 0;
    double min_velocity = final_velocity;
    double max_velocity = final_velocity;
    double total_reynolds = 0;
    double total_friction = 0;

    for (const QVariantMap& segment : results) {
        total_heat_loss_w += segment["total_heat_loss_w"].toDouble();
        total_friction_drop_pa += segment["friction_drop_pa"].toDouble();
        total_fittings_drop_pa += segment["fittings_drop_pa"].toDouble();

        max_surface_temp = std::max(max_surface_temp, segment["surface_temp_k"].toDouble());
        min_velocity = std::min(min_velocity, segment["velocity_m_s"].toDouble());
        max_velocity = std::max(max_velocity, segment["velocity_m_s"].toDouble());

        total_reynolds += segment["reynolds_number"].toDouble();
        total_friction += segment["friction_factor"].toDouble();
    }

    double pressure_drop_pa_m = total_pressure_drop_pa / totalLength;
    double pressure_ratio = total_pressure_drop_pa / inletProps["pressure_pa"].toDouble();

    // 热损失参数
    double avg_heat_loss_per_m = total_heat_loss_w / totalLength;
    double avg_heat_loss_per_area = 0;
    for (const QVariantMap& segment : results) {
        avg_heat_loss_per_area += segment["heat_loss_per_area_w"].toDouble();
    }
    avg_heat_loss_per_area /= results.size();

    double avg_surface_temp = 0;
    for (const QVariantMap& segment : results) {
        avg_surface_temp += segment["surface_temp_k"].toDouble();
    }
    avg_surface_temp /= results.size();

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
    analysis["inlet_pressure_pa"] = inletProps["pressure_pa"];
    analysis["inlet_temperature_k"] = inletProps["temperature_k"];
    analysis["inlet_quality"] = inletProps["quality"];
    analysis["inlet_velocity_m_s"] = inletVelocity;
    analysis["outlet_pressure_pa"] = final_pressure_pa;
    analysis["outlet_temperature_k"] = final_temperature_k;
    analysis["outlet_quality"] = final_quality;
    analysis["outlet_velocity_m_s"] = final_velocity;
    analysis["outlet_vapor_flow_kg_s"] = final_vapor_flow;
    analysis["outlet_liquid_flow_kg_s"] = final_liquid_flow;
    analysis["total_pressure_drop_pa"] = total_pressure_drop_pa;
    analysis["pressure_drop_pa_m"] = pressure_drop_pa_m;
    analysis["pressure_ratio"] = pressure_ratio;
    analysis["total_temperature_drop_k"] = total_temperature_drop;
    analysis["total_heat_loss_w"] = total_heat_loss_w;
    analysis["total_friction_drop_pa"] = total_friction_drop_pa;
    analysis["total_fittings_drop_pa"] = total_fittings_drop_pa;
    analysis["fittings_resistance"] = fittingsResistance;
    analysis["pipe_roughness_m"] = roughness;
    analysis["avg_heat_loss_per_m_w"] = avg_heat_loss_per_m;
    analysis["avg_heat_loss_per_area_w"] = avg_heat_loss_per_area;
    analysis["max_surface_temp_k"] = max_surface_temp;
    analysis["avg_surface_temp_k"] = avg_surface_temp;
    analysis["max_velocity_m_s"] = max_velocity;
    analysis["min_velocity_m_s"] = min_velocity;
    analysis["avg_velocity_m_s"] = avg_velocity;
    analysis["avg_reynolds"] = avg_reynolds;
    analysis["avg_friction"] = avg_friction;

    return analysis;
}

bool FluidAnalyzer::generateReport(const QVariantMap& params, const AnalysisResult& analysisResult,
                                   const QString& filename)
{
    // 这里应该实现Word报告生成功能
    // 由于Word报告生成比较复杂，这里只返回true表示成功
    // 实际实现需要使用Qt的文本处理或第三方库

    qDebug() << "生成报告:" << filename;
    qDebug() << "管道名称:" << params["pipe_name"].toString();
    qDebug() << "流体:" << params["fluid"].toString();

    // 创建简单的文本报告
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << "管道分析报告\n";
        stream << "============\n\n";
        stream << "管道名称: " << params["pipe_name"].toString() << "\n";
        stream << "流体介质: " << params["fluid"].toString() << "\n";
        stream << "管道长度: " << params["pipe_length_m"].toString() << " m\n";
        stream << "入口压力: " << params["inlet_pressure_pa"].toDouble() / 1e6 << " MPa\n";
        stream << "出口压力: " << analysisResult.summary["outlet_pressure_pa"].toDouble() / 1e6 << " MPa\n";
        stream << "总压力降: " << analysisResult.summary["total_pressure_drop_pa"].toDouble() / 1e6 << " MPa\n";
        stream << "总热损失: " << analysisResult.summary["total_heat_loss_w"].toDouble() / 1000 << " kW\n";
        file.close();
        return true;
    }

    return false;
}

QChart* FluidAnalyzer::plotComprehensiveResults(const QList<QVariantMap>& results,
                                                const QString& pipeName)
{
    // 这里应该实现图表绘制功能
    // 由于Qt Charts的使用比较复杂，这里返回nullptr
    // 实际实现需要使用Qt Charts库创建各种图表

    qDebug() << "绘制图表 for:" << pipeName;
    qDebug() << "数据点数:" << results.size();

    return nullptr; // 暂时返回空指针
}
