#include "pipeline.h"
#include "MaterialManager.h"
extern "C" {
    #include "CoolPropLib.h"
}
#include "string.h"
#include <string>
#include <cmath>
#include <QTextStream>
#include <QString>
#include <QDebug>

Pipeline::Pipeline()
    : phead(nullptr)
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
    , phead(new SegmentParameters)
{
    pipeId = pipeOd - 2 * pipeWallThickness;
}
Pipeline::~Pipeline()
{
    SegmentParameters* current = phead;
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
    if (phead == nullptr) {
        return nullptr;
    }

    phead->pressure = inletPressure;

    double quality = -1;
    if (inletQuality == -1) {
        phead->temperature = inletTemperature;
        phead->enthalpy = PropsSI("H", "P", inletPressure, "T", inletTemperature, fluid);
        quality = PropsSI("Q", "P", inletPressure, "T", inletTemperature, fluid);
    } else {
        quality = inletQuality;
        phead->temperature = PropsSI("T", "P", inletPressure, "Q", quality, fluid);;
        phead->enthalpy = PropsSI("H", "P", inletPressure, "Q", quality, fluid);
    }

    char phase_str[256];
    PhaseSI("P", inletPressure, "H", phead->enthalpy, fluid, phase_str, sizeof(phase_str));

    if (fabs(quality + 1) <1e-10) {// 判断quality是否==-1

        if(strcmp(phase_str, "gas") == 0 || strcmp(phase_str, "supercritical_gas") == 0) {
            phead->vaporFlow = massFlow;
        } else {
            phead->liquidFlow = massFlow;
        }
    } else {
        phead->vaporFlow = massFlow * quality;
        phead->liquidFlow = massFlow * (1 - quality);
    }

    return phead;
}

Pipeline::SegmentParameters* Pipeline::getLast()
{
    if (phead == nullptr) {
        return nullptr;
    }

    SegmentParameters* current = phead;
    while (current->next != nullptr) {
        current = current->next;
    }
    return current;
}

// 定义科尔布鲁克-怀特方程的内部函数
// 该方程描述了过渡区和湍流状态下摩擦因子与雷诺数、相对粗糙度的关系
double Pipeline::colebrook_equation(double friction_factor, double reynolds, double relative_roughness) {
    // 方程形式：1/sqrt(f) = -2 * log10((ε/D)/3.7 + 2.51/(Re * sqrt(f)))
    // 我们将其重写为：1/sqrt(f) + 2 * log10((ε/D)/3.7 + 2.51/(Re * sqrt(f))) = 0
    return 1.0 / sqrt(friction_factor) +
           2.0 * log10((relative_roughness / 3.7) +
                       (2.51 / (reynolds * sqrt(friction_factor))));
}

double Pipeline::_frictionFactor(double reynolds, double roughness) {
    // 输入参数验证
    if (reynolds <= 0.0) {
        return -1.0; // 错误代码：雷诺数必须为正数
    }

    if (roughness < 0.0) {
        return -1.0; // 错误代码：绝对粗糙度不能为负
    }

    if (pipeId <= 0.0) {
        return -1.0; // 错误代码：管道内径必须为正数
    }

    // 计算相对粗糙度：绝对粗糙度与管道内径的比值
    double relative_roughness = roughness / pipeId;

    // 层流区域处理：雷诺数小于等于临界值
    if (reynolds <= 2300) {
        // 层流摩擦因子有精确解析解：f = 64 / Re
        return 64.0 / reynolds;
    }

    // 过渡区
    if (reynolds <= 4000) {
        // 层流摩擦因子有精确解析解：f = 64 / Re
        return 0.3164 * pow(reynolds, -0.25);
    }

    // 湍流区域处理：雷诺数大于等于过渡区结束值
    // 使用二分法直接求解科尔布鲁克-怀特方程
    double lower_bound = 0.0001;   // 摩擦因子下界
    double upper_bound = 0.1;      // 摩擦因子上界

    // 计算边界点处的方程残差
    double residual_lower = colebrook_equation(lower_bound, reynolds, relative_roughness);
    double residual_upper = colebrook_equation(upper_bound, reynolds, relative_roughness);

    // 检查根是否在初始区间内
    if (residual_lower * residual_upper > 0) {
        // 根不在初始区间内，扩展搜索范围
        lower_bound = 0.000001; // 扩展下界
        upper_bound = 1;     // 扩展上界

        // 重新计算扩展区间边界处的残差
        residual_lower = colebrook_equation(lower_bound, reynolds, relative_roughness);
        residual_upper = colebrook_equation(upper_bound, reynolds, relative_roughness);

        // 检查扩展区间是否包含根
        if (residual_lower * residual_upper > 0) {
            return -1.0; // 错误代码：无法在扩展区间内找到根
        }
    }

    // 二分法迭代求解科尔布鲁克方程，无次数限制
    while (1) {
        double midpoint = (lower_bound + upper_bound) / 2.0; // 计算区间中点
        double residual_mid = colebrook_equation(midpoint, reynolds, relative_roughness); // 计算中点处残差

        // 收敛检查：残差足够小或区间足够小
        if (fabs(residual_mid) < 1e-15 || (upper_bound - lower_bound) < 1e-15) {
            return midpoint; // 达到收敛条件，返回摩擦因子
        }

        // 根据残差符号更新搜索区间
        if (residual_lower * residual_mid < 0) {
            // 根在左半区间：更新上界
            upper_bound = midpoint;
            residual_upper = residual_mid;
        } else {
            // 根在右半区间：更新下界
            lower_bound = midpoint;
            residual_lower = residual_mid;
        }
    }
}

void Pipeline::pressureDrop(
    double length,
    double roughness,
    double fittingsResistance,
    double density,
    double viscosity,
    double *frictionFactor,
    double *frictionPressureDrop,
    double *fittingsPressureDrop
    )
{
    double area = M_PI * pow(pipeId/2, 2);

    double velocity = massFlow / (density * area);
    double reynolds = (density * velocity * pipeId) / viscosity;
    *frictionFactor = _frictionFactor(reynolds, roughness);

    // 沿程阻力损失
    *frictionPressureDrop = (*frictionFactor) * (length / pipeId) * (density * pow(velocity, 2)) / 2;

    // 局部阻力损失
    *fittingsPressureDrop = fittingsResistance * (density * pow(velocity, 2)) / 2;
}

// 表面对流传热系数
double Pipeline::convectiveHeatTransferCoeff(double surfaceTemperature)
{
    double d_outer = pipeOd + 2 * insulationThickness;
    double h_conv ;

    if (windSpeed == 0) {
        // 无风时自然对流
        double T_avg = (surfaceTemperature + ambientTemperature) / 2;
        h_conv = 26.4 / sqrt(297 - 273.15 + 0.5 * T_avg) * pow((surfaceTemperature - ambientTemperature) / pipeOd, 0.25);
    } else if (windSpeed * d_outer <= 0.8) {
        // 低风速情况
        h_conv = 0.08 / d_outer + 4.2 * pow(windSpeed, 0.618) / pow(d_outer, 0.382);
    } else {
        // 强制对流
        h_conv = 4.53 * pow(windSpeed, 0.805) / pow(pipeOd, 0.195);
    }

    return std::max(1.0, h_conv); // 确保最小值
}

// 表面辐射传热系数
double Pipeline::radiationHeatTransferCoeffi(double surfaceTemperature, double emissivity)
{
    //double sigma = 5.669; // Stefan-Boltzmann常数
    double T_diff = surfaceTemperature - ambientTemperature;

    if (T_diff <= 0) {
        return 0.0;
    }

    double h_rad = 5.669 * emissivity / T_diff *
                   (pow(surfaceTemperature/100, 4) - pow(ambientTemperature/100, 4));

    return h_rad;
}

// surface thermal resistance
// 返回单位长度表面热阻
double Pipeline::surfaceThermalResistance(double surfaceTemperature,double emissivity)
{

    // 对流换热系数
    double h_conv = convectiveHeatTransferCoeff(surfaceTemperature);

    // 辐射换热系数
    double h_rad = radiationHeatTransferCoeffi(surfaceTemperature, emissivity);

    double d_outer = pipeOd + 2 * insulationThickness;

    return 1 / ((h_conv + h_rad) * M_PI * d_outer);
}
void Pipeline::heatLoss(
    double fluidTemperature,
    double length,
    double emissivity,
    const InsulationMaterial& insulationMaterial,
    double* Q,
    double* surfaceTemperature
    )
{

    double d_outer = pipeOd + 2 * insulationThickness;

    // 初始假设表面温度
    *surfaceTemperature = ambientTemperature + 0.3 * (fluidTemperature - ambientTemperature);

    double Q_per_m = 0.0;

    // 迭代计算
    for (int i = 0; i < 100; ++i) {
        // 计算保温层平均温度
        double tm = (fluidTemperature + *surfaceTemperature) / 2;

        // 计算保温材料导热系数
        double conductivity = insulationMaterial.getConductivity(tm);

        // 计算保温层热阻
        double R_insulation = log(d_outer / pipeOd) / (2 * M_PI * conductivity);

        // 计算表面热阻
        double R_w = surfaceThermalResistance(*surfaceTemperature, emissivity);

        // 总热阻
        double R_total = R_insulation + R_w;

        // 单位长度热损失
        Q_per_m = (fluidTemperature - ambientTemperature) / R_total;

        // 更新表面温度
        double surfaceTemperature_new = ambientTemperature + Q_per_m * R_w;

        // 收敛检查
        if (fabs(surfaceTemperature_new - *surfaceTemperature) < 0.01) {
            *surfaceTemperature = surfaceTemperature_new;
            break;
        }

        *surfaceTemperature = surfaceTemperature_new;
    }

    // 总热损失
    *Q = Q_per_m * length;
}

Pipeline::SegmentParameters* Pipeline::segment(
    SegmentParameters* pprev,
    double length,
    double roughness,
    double emissivity,
    double fittingsResistance,
    const InsulationMaterial& insulationMaterial
    )
{
    double outletPressure = 0, outletTemperature = 0, outletEnthalpy = 0;
    double frictionFactor, frictionPressureDrop = 0, fittingsPressureDrop = 0, Q = 0, surfaceTemperature = 0;

    double pressure, fluidTemperature, enthalpy = pprev->enthalpy;
    double pressure_temp = pprev->pressure; double fluidTemperature_temp = pprev->temperature;

    for (int i = 0; i < 100; i++) {
        pressure = pressure_temp;
        fluidTemperature = fluidTemperature_temp;
        double density = PropsSI("D", "P", pressure, "H", enthalpy, fluid);
        double viscosity = PropsSI("V", "P", pressure, "H", enthalpy, fluid);

        pressureDrop(length, roughness, fittingsResistance, density,
                     viscosity, &frictionFactor, &frictionPressureDrop, &fittingsPressureDrop);

        outletPressure = pprev->pressure - frictionPressureDrop - fittingsPressureDrop;
        pressure_temp = pprev->pressure - 0.5 * (frictionPressureDrop + fittingsPressureDrop);

        heatLoss(fluidTemperature, length, emissivity, insulationMaterial,
                 &Q, &surfaceTemperature);

        double delta_h = Q / massFlow;
        enthalpy = pprev->enthalpy - 0.5 * delta_h;
        outletEnthalpy = pprev->enthalpy - delta_h;

        outletTemperature = PropsSI("T", "P", outletPressure, "H", outletEnthalpy, fluid);
        fluidTemperature_temp = 0.5 * (pprev->temperature + outletTemperature);

        if (fabs(pressure - pressure_temp) < 0.01 && fabs(fluidTemperature - fluidTemperature_temp) < 0.01) {
            break;
        }
    }

    // 计算气相和液相流量
    double outletQuality = PropsSI("Q", "P", outletPressure, "H", outletEnthalpy, fluid);

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

    SegmentParameters *current = new SegmentParameters;
    current->pressure = outletPressure;
    current->temperature = outletTemperature;
    current->enthalpy = outletEnthalpy;
    current->frictionFactor = frictionFactor;
    current->vaporFlow = vaporFlow;
    current->liquidFlow = liquidFlow;
    current->surfaceTemperature = surfaceTemperature;
    current->fittingsPressureDrop = fittingsPressureDrop;
    current->frictionPressureDrop = frictionPressureDrop;
    current->prev = pprev;
    current->next = nullptr;

    pprev->next = current;

    return current;
}

void Pipeline::calculate()
{
    MaterialManager* materialManager = new MaterialManager();

    // 获取材料
    PipeType pipeType;
    if (materialManager->getPipeTypes().contains(pipeTypeName.c_str())) {
        pipeType = materialManager->getPipeTypes()[pipeTypeName.c_str()];
    }

    InsulationMaterial insulationMaterial;
    if (materialManager->getInsulationMaterials().contains(insulationMaterialName.c_str())) {
        insulationMaterial = materialManager->getInsulationMaterials()[insulationMaterialName.c_str()];
    }

    CladMaterial cladMaterial;
    if (materialManager->getCladMaterials().contains(cladMaterialName.c_str())) {
        cladMaterial = materialManager->getCladMaterials()[cladMaterialName.c_str()];
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

    segmentLength = segmentLength ? segmentLength : length / 10;
    int num_segments = std::max(1, (int)ceil(length / segmentLength));

    double segmentFittingsResistance = totalFittingsResistance / num_segments;
    SegmentParameters *current = getHead();
    for (int i = 0; i < num_segments; ++i) {
        double seg_length = std::min(segmentLength, length - i * segmentLength);

        current = segment(current, seg_length, pipeType.roughness, cladMaterial.emissivity, segmentFittingsResistance,
                          insulationMaterial);
    }
}

QString Pipeline::getReport()
{
    SegmentParameters* ptail = getLast();
    double frictionPressureDrop = 0, fittingsPressureDrop = 0;

    SegmentParameters* seg = phead;
    seg = seg->next;
    while (seg != nullptr) {
        frictionPressureDrop += seg->frictionPressureDrop;
        fittingsPressureDrop += seg->fittingsPressureDrop;
        seg = seg->next;
    }

    double Q = (phead->enthalpy - ptail->enthalpy) * massFlow;
    double Q_per_m = Q / length;
    double Q_per_m2 = Q / (M_PI * (pipeOd + 2 * insulationThickness) * length);

    double area = M_PI * pow(pipeId/2, 2);

    double inletDensity = PropsSI("D", "P", phead->pressure, "H", phead->enthalpy, fluid);
    double inletVelocity = massFlow / (inletDensity * area);

    double outletDensity = PropsSI("D", "P", ptail->pressure, "H", ptail->enthalpy, fluid);
    double outletVelocity = massFlow / (outletDensity * area);

    // 获取材料
    MaterialManager* materialManager = new MaterialManager();
    PipeType pipeType;
    if (materialManager->getPipeTypes().contains(pipeTypeName.c_str())) {
        pipeType = materialManager->getPipeTypes()[pipeTypeName.c_str()];
    }

    // 获取管件阻力系数
    double fittingsResistance = 0;
    for (std::map<std::string, int>::iterator it = fittingsData.begin(); it != fittingsData.end(); ++it) {
        //std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
        if (materialManager->getPipeFittings().contains(it->first.c_str())) {
            PipeFitting pipe_fitting = materialManager->getPipeFittings()[it->first.c_str()];
            fittingsResistance += pipe_fitting.resistanceCoef * it->second;
        }
    }

    CladMaterial cladMaterial;
    if (materialManager->getCladMaterials().contains(cladMaterialName.c_str())) {
        cladMaterial = materialManager->getCladMaterials()[cladMaterialName.c_str()];
    }

    QString report;
    // Qt 6中设置UTF-8编码
    QTextStream out(&report);
    out.setEncoding(QStringConverter::Utf8);

    // 报告头部
    out << "===================================================================================\n";
    out << "                          管道压力损失与热损失分析报告\n";
    out << "===================================================================================\n";

    // 1. 操作参数（标签宽度28，数值宽度20）
    out << "操作参数\n";
    out << "-----------------------------------------------------------------------------------\n";
    out << QString("%1: %2\n").arg("质量流量（kg/hr）", -28).arg(massFlow * 3600, 20, 'f', 4);
    out << QString("%1: %2\n").arg("总压力损失（kPa）", -27).arg((phead->pressure - ptail->pressure)/ 1000, 20, 'f', 4);
    out << QString("%1: %2\n").arg("摩擦压力损失（kPa）", -26).arg(frictionPressureDrop / 1000, 20, 'f', 4);
    out << QString("%1: %2\n").arg("管件压力损失（kPa）", -26).arg(fittingsPressureDrop / 1000, 20, 'f', 4);
    //out << QString("%1: %2\n").arg("最大流速（m/sec）", -28).arg(result.maxVelocity, 20, 'f', 4);
    //out << QString("%1: %2\n").arg("平均流速（m/sec）", -28).arg(result.avgVelocity, 20, 'f', 4);
    out << QString("%1: %2\n").arg("温度降（C）", -29).arg(phead->temperature - ptail->temperature, 20, 'f', 4);
    out << QString("%1: %2\n").arg("焓降（kJ/kg）", -30).arg((phead->enthalpy - ptail->enthalpy) / 1000, 20, 'f', 4);
    out << QString("%1: %2\n").arg("总热损失（kW）", -28).arg(Q / 1000, 20, 'f', 4);
    out << QString("%1: %2\n").arg("每米平均热损失（W/m）", -25).arg(Q_per_m, 20, 'f', 4);
    out << QString("%1: %2\n\n").arg("每平方米平均热损失（W/m2）", -23).arg(Q_per_m2, 20, 'f', 4);

    // 2. 进口和出口流体特性
    out << "流体特性参数\n";
    out << "-----------------------------------------------------------------------------------\n";
    out << QString("%1%2%3\n").arg("", -32).arg("进口", 20).arg("出口", 18);
    out << QString("%1%2%3\n").arg("", -34).arg("---------", 20).arg("---------", 20);
    out << QString("%1%2%3\n").arg("压力（kPa）", -30).arg(phead->pressure / 1000, 20, 'f', 2).arg(ptail->pressure / 1000, 20, 'f', 2);
    out << QString("%1%2%3\n").arg("温度（C）", -30).arg(phead->temperature - 273.15, 20, 'f', 2).arg(ptail->temperature - 273.15, 20, 'f', 2);
    out << QString("%1%2%3\n").arg("焓值（kJ/kg）", -30).arg(phead->enthalpy / 1000, 20, 'f', 2).arg(ptail->enthalpy / 1000, 20, 'f', 2);
    //out << QString("%1%2%3\n").arg("干度（-）", -30).arg(result.inletQuality, 20, 'f', 4).arg(result.outletQuality, 20, 'f', 4);
    out << QString("%1%2%3\n").arg("流速（m/sec）", -30).arg(inletVelocity, 20, 'f', 4).arg(outletVelocity, 20, 'f', 4);
    out << QString("%1%2%3\n").arg("密度（kg/m3）", -30).arg(inletDensity, 20, 'f', 4).arg(outletDensity, 20, 'f', 4);
    out << QString("%1%2%3\n").arg("气相流量（kg/hr）", -28).arg(phead->vaporFlow * 3600, 20, 'f', 2).arg(ptail->vaporFlow * 3600, 20, 'f', 2);
    out << QString("%1%2%3\n").arg("液相流量（kg/hr）", -28).arg(phead->liquidFlow * 3600, 20, 'f', 2).arg(ptail->liquidFlow * 3600, 20, 'f', 2);
    out << QString("%1%2%3\n\n").arg("摩擦系数（-）", -28).arg(phead->next->frictionFactor, 20, 'f', 4).arg(ptail->frictionFactor, 20, 'f', 4);

    // 3. 其它参数
    out << "其它参数\n";
    out << "-----------------------------------------------------------------------------------\n";
    out << QString("%1: %2\n").arg("管道外径（mm）", -28).arg(pipeOd * 1000, 20, 'f', 2);
    out << QString("%1: %2\n").arg("管道内径（mm）", -28).arg(pipeId * 1000, 20, 'f', 2);
    out << QString("%1: %2\n").arg("管道长度（m）", -28).arg(length, 20, 'f', 2);
    out << QString("%1: %2\n").arg("管道粗糙度（mm）", -27).arg(pipeType.roughness * 1000, 20, 'f', 4);
    out << QString("%1: %2\n").arg("管件阻力系数", -28).arg(fittingsResistance, 20, 'f', 4);
    out << QString("%1: %2\n").arg("保温厚度（mm）", -28).arg(insulationThickness * 1000, 20, 'f', 2);
    out << QString("%1: %2\n").arg("保护层黑度", -29).arg(cladMaterial.emissivity, 20, 'f', 4);
    out << QString("%1: %2\n").arg("环境温度（C）", -28).arg(ambientTemperature - 273.15, 20, 'f', 2);
    out << QString("%1: %2\n").arg("风速（m/sec）", -30).arg(windSpeed, 20, 'f', 2);
    out << QString("%1: %2\n").arg("管道类型", -30).arg(QString::fromStdString(pipeTypeName), 20);
    out << QString("%1: %2\n").arg("保温材料", -30).arg(QString::fromStdString(insulationMaterialName), 19);
    out << QString("%1: %2\n\n").arg("保护层材料", -29).arg(QString::fromStdString(cladMaterialName), 12);

    // 4. 分段计算结果
    out << "分段计算结果\n";
    out << "-----------------------------------------------------------------------------------\n";
    out << QString("%1  %2  %3  %4  %5  %6  %7  %8\n")
               .arg("分段序号", -2)
               .arg("压力", -6)
               .arg("温度", -6)
               .arg("流速", -5)
               .arg("焓值", -6)
               .arg("表面温度", -4)
               .arg("气相流量", -6)
               .arg("液相流量", -6);
    out << QString("%1  %2  %3  %4  %5  %6  %7  %8\n")
               .arg("--------", -8) 
               .arg("(kPa)", -8)
               .arg("(C)", -8)
               .arg("(m/s)", -7)
               .arg("(kJ/kg)", -8)
               .arg("(C)", -8)
               .arg("(kg/hr)", -10)
               .arg("(kg/hr)", -10);
    out << "-----------------------------------------------------------------------------------\n";

    // 写入分段数据
    seg = phead;
    int i = 1;
    seg = seg->next;
    while (seg != nullptr) {
        double pressure = 0.5 * (seg->prev->pressure + seg->pressure);
        double temperature = 0.5 * (seg->prev->temperature + seg->temperature);
        double enthalpy = 0.5 * (seg->prev->enthalpy + seg->enthalpy);
        double density = PropsSI("D", "P", phead->pressure, "H", enthalpy, fluid);
        double velocity = massFlow / (density * area);

        out << QString("%1  %2  %3  %4  %5  %6  %7  %8\n")
                   .arg(i, -8)
                   .arg(pressure / 1000, -8, 'f', 2)
                   .arg(temperature - 273.15, -8, 'f', 2)
                   .arg(velocity, -7, 'f', 2)
                   .arg(enthalpy / 1000, -9, 'f', 2)
                   .arg(seg->surfaceTemperature - 273.15, -8, 'f', 2)
                   .arg(seg ->vaporFlow* 3600, -10, 'f', 2)
                   .arg(seg->liquidFlow * 3600, -8, 'f', 2);
        seg = seg->next;
        ++i;
    }

    // 报告尾部
    out << "===================================================================================\n";
    out << "报告生成工具：汤圆工具集 管道阻力和绝热分析\n";
    out << "作者：杨奉全，qq交流群：816103114\n";
    out << "===================================================================================\n";

    return report;
}
