#ifndef REPORT_H
#define REPORT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include "FluidAnalyzer.h"

class ResultFormatter {
private:
    static const int COLUMN_WIDTH = 15;

    std::string formatValue(double value, int precision = 5) {
        std::ostringstream oss;
        if (value == 0.0) {
            oss << std::setw(COLUMN_WIDTH) << "0.00000";
        } else if (std::abs(value) < 0.001) {
            oss << std::scientific << std::setprecision(precision) << value;
            std::string str = oss.str();
            // 格式化科学计数法显示
            size_t e_pos = str.find('e');
            if (e_pos != std::string::npos) {
                std::string base = str.substr(0, e_pos);
                std::string exp = str.substr(e_pos);
                oss.str("");
                oss << std::setw(COLUMN_WIDTH) << base << exp;
            }
        } else {
            oss << std::fixed << std::setprecision(precision) << std::setw(COLUMN_WIDTH) << value;
        }
        return oss.str();
    }

    std::string formatHeader(const std::string& text) {
        std::ostringstream oss;
        oss << "\n " << text << "\n";
        return oss.str();
    }

public:
    bool formatToFile(const FluidAnalyzer::AnalysisResult& result, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            return false;
        }

        // 写入UTF-8 BOM以确保中文正确显示
        const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
        file.write(reinterpret_cast<const char*>(bom), sizeof(bom));

        file << "UNIT 1, 'PI1'\n";
        file << "  \n";
        file << " Feeds                                S1\n";
        file << "  \n";
        file << " Products Vapor                       S2\n";
        file << "  \n";

        // 压力降计算
        file << formatHeader("PRESSURE DROP CALCULATION FOR LINE");
        file << " PRESSURE DROP CORRELATION USED: BEGGS-BRILL-MOODY\n";
        file << "  \n";

        // 操作条件
        file << formatHeader("OPERATING CONDITIONS");
        file << "  \n";
        file << " DUTY, M*KJ/HR" << std::setw(30) << formatValue(result.totalHeatLoss) << "\n";

        file << formatHeader("PRESSURE DROP SUMMARY");
        file << " LINE FRICTION, KPA" << std::setw(27) << formatValue(result.frictionPressureDrop) << "\n";
        file << " ELEVATION, KPA" << std::setw(31) << "0.00000" << "\n";
        file << " ACCELERATION, KPA" << std::setw(27) << formatValue(result.totalPressureDrop - result.frictionPressureDrop) << "\n";
        file << " TOTAL, KPA" << std::setw(34) << formatValue(result.totalPressureDrop) << "\n";
        file << "  \n";
        file << " CALC TOTAL PRESSURE DROP, KPA" << std::setw(16) << formatValue(result.totalPressureDrop) << "\n";
        file << " CALC MAX LINE FLUID VELOCITY, M/SEC" << std::setw(8) << formatValue(result.maxVelocity) << "\n";
        file << "  \n";

        // 混合物流动流体属性
        file << formatHeader("MIXTURE FLOWING FLUID PROPERTIES");
        file << std::setw(35) << "INLET" << std::setw(20) << "OUTLET\n";
        file << std::setw(35) << "-----------" << std::setw(20) << "------------\n";

        file << " TEMPERATURE, K" << std::setw(31) << formatValue(result.inletTemperature)
             << std::setw(20) << formatValue(result.outletTemperature) << "\n";
        file << " PRESSURE, KPA" << std::setw(32) << formatValue(result.inletPressure)
             << std::setw(20) << formatValue(result.outletPressure) << "\n";
        file << " MOLE FRACTION LIQUID" << std::setw(25) << formatValue(result.inletQuality)
             << std::setw(20) << formatValue(result.outletQuality) << "\n";
        file << " VELOCITY, M/SEC" << std::setw(30) << formatValue(result.inletVelocity)
             << std::setw(20) << formatValue(result.outletVelocity) << "\n";
        file << " SLIP DENSITY, KG/M3" << std::setw(26) << formatValue(result.inletDensity)
             << std::setw(20) << formatValue(result.outletDensity) << "\n";
        file << " FRICTION FACTOR" << std::setw(30) << formatValue(result.inletFrictionFactor)
             << std::setw(20) << formatValue(result.outletFrictionFactor) << "\n";
        file << " SLIP LIQUID HOLDUP FRACTION, (VOL/VOL)" << std::setw(11) << "0.00000"
             << std::setw(20) << "0.00000" << "\n";
        file << " TAITEL-DUKLER-BARNEA FLOW REGIME" << std::setw(14) << "SINGLE PHASE"
             << std::setw(20) << "SINGLE PHASE" << "\n";
        file << "  \n";

        // 一般数据
        file << formatHeader("GENERAL DATA");
        file << std::setw(35) << "LINE\n";
        file << std::setw(35) << "------------\n";
        file << " INSIDE DIAMETER, MM" << std::setw(26) << formatValue(result.pipeId) << "\n";
        file << " LINE LENGTH, M" << std::setw(30) << formatValue(result.length) << "\n";
        file << " AVERAGE MOODY FRICTION FACTOR" << std::setw(16) << formatValue((result.inletFrictionFactor + result.outletFrictionFactor) / 2.0) << "\n";
        file << " AVERAGE REYNOLDS NUMBER," << std::setw(20) << "4.63128E+05" << "\n"; // 示例值
        file << " ROUGHNESS, MM" << std::setw(32) << formatValue(result.roughness) << "\n";
        file << " ROUGHNESS, RELATIVE" << std::setw(27) << "MISSING" << "\n";
        file << " ELEVATION CHANGE, M" << std::setw(26) << "0.00000" << "\n";
        file << " FLOW EFFICIENCY, PCT" << std::setw(25) << "100.00000" << "\n";

        // 分段结果
        if (!result.segmentResults.empty()) {
            file << formatHeader("SEGMENT RESULTS");
            file << "  \n";

            // 表头
            std::vector<std::string> headers = {
                "velocity", "reynoldsr", "frictionFactor", "pressureDrop",
                "frictionPressureDrop", "fittingsPressureDrop", "totalHeatLoss",
                "heatLossPerM", "heatLossPerArea", "overallHeatTransferCoeff",
                "convectionCoeff", "surfaceTemperature", "insulationConductivity",
                "vaporFlow", "liquidFlow", "outletPressure", "outletTemperature",
                "outletEnthalpy", "outletQuality"
            };

            // 写入表头
            for (const auto& header : headers) {
                file << std::setw(COLUMN_WIDTH) << header;
            }
            file << "\n";

            // 写入数据
            for (const auto& segment : result.segmentResults) {
                for (const auto& header : headers) {
                    auto it = segment.find(header);
                    if (it != segment.end()) {
                        file << formatValue(it->second);
                    } else {
                        file << std::setw(COLUMN_WIDTH) << "N/A";
                    }
                }
                file << "\n";
            }
        }

        file.close();
        return true;
    }

    void openWithNotepad(const std::string& filename) {
        // 使用Windows自带的记事本打开文件
        std::string command = "notepad \"" + filename + "\"";
        system(command.c_str());
    }
};
#endif // REPORT_H
