"use strict";
const Pi = 3.1415926;
const Sch = ["SCH5", "SCH10", "SCH20", "SCH30", "SCH40", "SCH60", "SCH80", "SCH100", "SCH120", "SCH140", "SCH160", "STD", "XS", "XXS", "SCH5S", "SCH10S", "SCH40S", "SCH80S"];
const Do = {
    DN6: 10.3,
    DN8: 13.7,
    DN10: 17.1,
    DN15: 21.3,
    DN20: 26.7,
    DN25: 33.4,
    DN32: 42.2,
    DN40: 48.3,
    DN50: 60.3,
    DN65: 73,
    DN80: 88.9,
    DN90: 101.6,
    DN100: 114.3,
    DN125: 141.3,
    DN150: 168.3,
    DN200: 219.1,
    DN250: 273.1,
    DN300: 323.9,
    DN350: 355.6,
    DN400: 406.4,
    DN450: 457,
    DN500: 508,
    DN550: 559,
    DN600: 610,
};
const SHT3405 = {
    //["SCH5","SCH10","SCH20","SCH30","SCH40","SCH60","SCH80","SCH100","SCH120","SCH140","SCH160","STD","XS","XXS","SCH5S","SCH10S","SCH40S","SCH80S"]
    DN6: [, 1.24, , 1.45, 1.73, , 2.41, , , , , 1.73, 2.41, , , 1.24, 1.73, 2.41],
    DN8: [, 1.65, , 1.85, 2.24, , 3.02, , , , , 2.24, 3.02, , , 1.65, 2.24, 3.02],
    DN10: [, 1.65, , 1.85, 2.31, , 3.2, , , , , 2.31, 3.2, , , 1.65, 2.31, 3.2],
    DN15: [1.65, 2.11, , 2.41, 2.77, , 3.73, , , , 4.78, 2.77, 3.73, 7.47, 1.65, 2.11, 2.77, 3.73],
    DN20: [1.65, 2.11, , 2.41, 2.87, , 3.91, , , , 5.56, , , 7.82, , , 2.87, 3.91],
    DN25: [1.65, 2.77, , 2.91, 3.38, , 4.55, , , , 6.35, 3.38, 4.55, 9.09, 1.65, 2.77, 3.38, 4.55],
    DN32: [1.65, 2.77, , 2.97, 3.56, , 4.85, , , , 6.35, 3.56, 4.85, 9.7, 1.65, 2.77, 3.56, 4.85],
    DN40: [1.65, 2.77, , 3.18, 3.68, , 5.08, , , , 7.14, 3.68, 5.08, 10.15, 1.65, 2.77, 3.68, 5.08],
    DN50: [1.65, 2.77, , 3.18, 3.91, , 5.54, , , , 8.74, 3.91, 5.54, 11.07, 1.65, 2.77, 3.91, 5.54],
    DN65: [2.11, 3.05, , 4.78, 5.16, , 7.01, , , , 9.53, 5.16, 7.01, 14.02, 2.11, 3.05, 5.16, 7.01],
    DN80: [2.11, 3.05, , 4.78, 5.49, , 7.62, , , , 11.13, 5.49, 7.62, 15.24, 2.11, 3.05, 5.49, 7.62],
    DN90: [2.11, 3.05, , 4.78, 5.74, , 8.08, , , , , , , , 2.11, 3.05, 5.74, 8.08],
    DN100: [2.11, 3.05, , 4.78, 6.02, , 8.56, , 11.13, , 13.49, 6.02, 8.56, 17.12, 2.11, 3.05, 6.02, 8.56],
    DN125: [2.77, 3.4, , , 6.55, , 9.53, , 12.7, , 15.88, 6.55, 9.53, 19.05, 2.77, 3.4, 6.55, 9.53],
    DN150: [2.77, 3.4, , , 7.11, , 10.97, , 12.7, , 18.26, 7.11, 10.97, 19.05, 2.77, 3.4, 7.11, 10.97],
    DN200: [2.77, 3.76, 6.35, 7.04, 8.18, 10.13, 12.7, 15.09, 18.26, 20.62, 23.01, 8.18, 12.7, 22.23, 2.77, 3.76, 8.18, 12.7],
    DN250: [3.4, 4.19, 6.35, 7.8, 9.27, 12.7, 15.09, 18.26, 21.44, 25.4, 28.58, 9.27, 12.7, 25.4, 3.4, 4.19, 9.27, 12.7],
    DN300: [3.96, 4.57, 6.35, 8.38, 10.31, 14.27, 17.48, 21.44, 25.4, 28.58, 33.32, 9.53, 12.7, 25.4, 3.96, 4.78, 9.53, 12.7],
    DN350: [3.96, 6.35, 7.92, 9.53, 11.13, 15.09, 19.05, 23.83, 27.79, 31.75, 35.71, 9.53, 12.7, , 3.96, 4.78, 9.53, 12.7],
    DN400: [4.19, 6.35, 7.92, 9.53, 12.7, 16.66, 21.44, 26.19, 30.96, 36.53, 40.49, 9.53, 12.7, , 4.19, 4.78, 9.53, 12.7],
    DN450: [4.19, 6.35, 7.92, 11.13, 14.27, 19.05, 23.83, 29.36, 34.93, 39.67, 45.24, 9.53, 12.7, , 4.19, 4.78, 9.53, 12.7],
    DN500: [4.78, 6.35, 9.53, 12.7, 15.09, 20.62, 26.19, 32.54, 38.1, 44.45, 50.01, 9.53, 12.7, , 4.78, 5.54, 9.53, 12.7],
    DN550: [4.78, 6.35, 9.53, 12.7, , 22.23, 28.58, 34.93, 41.28, 47.63, 53.98, 9.53, 12.7, , 4.78, 5.54, 9.53, 12.7],
    DN600: [5.54, 6.35, 9.53, 14.27, 17.48, 24.61, 30.96, 38.89, 46.02, 52.37, 59.54, 9.53, 12.7, , 5.54, 6.35, 9.53, 12.7],
};
const Roughness = [
    ["无缝黄铜、铜及铅管", (0.005 + 0.01) / 2],
    ["操作中基本无腐蚀的无缝钢管", (0.05 + 0.1) / 2],
    ["操作中有轻度腐蚀的无缝钢管", (0.1 + 0.2) / 2],
    ["操作中有显著腐蚀的无缝钢管", (0.2 + 0.5) / 2],
    ["钢板卷管", 0.33],
    ["铸铁管", (0.5 + 0.85) / 2],
    ["干净的玻璃管", (0.0015 + 0.01) / 2]
];
const LocalResistace = {
    elbow45: 0.35,
    elbow: 0.75,
    elbow2: 1.30,
    elbow180: 1.5,
    globeValve: 6.00,
    angleValve: 3.00,
    gateValve: 0.17,
    plugValve: 0.05,
    butterflyValve: 0.24,
    checkValve0: 2.00,
    checkValve1: 10.00,
    footValve: 15.00,
};
const PipeFitting = {
    elbow45: "45°弯头",
    elbow: "90°弯头",
    elbow2: "90°斜接弯头",
    elbow180: "180°弯头"
};
const Gate = {
    globeValve: "截止阀",
    angleValve: "角阀",
    gateValve: "闸阀",
    plugValve: "旋塞阀",
    butterflyValve: "蝶阀",
    checkValve0: "旋启式止回阀",
    checkValve1: "升降式止回阀",
    footValve: "底阀"
};
/**
 * 计算管道内径（流速）
 * @param f flow [m3/h]
 * @param v flow rate [m/s]
 * @returns Pipe Internal diameter [mm]
 */
function dim_v(f, v) {
    return 18.8 * Math.sqrt(f / v);
}
/**
 * 计算管道内径（允许压降） 依据SH/T3405-2018
 * @param f flow [m3/h]
 * @param dP allow pressure drop [kPa/100m]
 * @param rho density [kg/m3]
 * @param nu Kinematic viscosity[m2/s]
 * @returns Pipe Internal diameter [mm]
 */
function dim_dP(f, l, dP, rho, nu) {
    //nu = nu * 10000;
    //return 11.4 * rho ** 0.207 * nu ** 0.033 * f ** 0.38 * dP **-0.207;
    var di = 0.007 * Math.pow(rho, 0.207) * Math.pow(nu, 0.033) * Math.pow(l, 0.207) * Math.pow(f, 0.38) * Math.pow(dP, -0.207);
    return di * 1000;
}
/**
 * 计算管子重量
 * @param don Pipe outer diameter [mm]
 * @param thk Pipe wall thickness [mm]
 * @param rho density [kg/m3]
 * @returns Weight [mm]
 */
function pipeWgt_thk(don, thk, rho = 7.85) {
    return Pi * rho * (don - thk) * thk / 1000;
}
/**
 * @param don Pipe outer diameter [mm]
 * @param thk Insultion thickness [mm]
 * @param l length [mm]
 * @return 保温体积 m3
 */
function VInsultion(don, ithk, l) {
    return Pi * (don + 1.033 * ithk) * 1.033 * ithk / 1e6 * l;
}
/**
 * @param don Pipe outer diameter [mm]
 * @param thk Insultion thickness [mm]
 * @param l length [mm]
 * @return 保温表面积 m2
 */
function SInsultion(don, ithk, l) {
    return Pi * (don + 2.1 * ithk + 8.2) / 1e3 * l;
}
/**
 * 雷诺数
 * @param dim Pipe Internal diameter [m]
 * @param ve flow rate [m/s]
 * @param rho density [kg/m3]
 * @param mu Dynamic viscosity [Ps.s]
 * @returns reynolds number
 */
function reynolds(dim, ve, rho, mu) {
    return dim * ve * rho / mu;
}
/**
 * 阻力系数
 * @param re reynolds number
 * @param dim  Pipe Internal diameter [m]
 * @param e Roughness [m]
 * @returns resistance coefficient
 */
function resistace(re, dim = 0, e = 0) {
    var lambda;
    if (re <= 2000) {
        lambda = 64 / re;
    }
    else if (re > 2000 && re <= 4000) {
        lambda = 0.3164 * Math.pow(re, -0.25);
    }
    else if (re > 4000 && re < 396 * dim / e * Math.log10(3.7 * dim / e)) {
        var x0 = 0, x1 = 0.1;
        do {
            var mid = (x0 + x1) / 2;
            if (1 / Math.pow(mid, 0.5) + 2 * Math.log10(e / (3.7 * dim) + 2.51 / (re * Math.pow(mid, 0.5))) > 0) {
                x0 = mid;
            }
            else {
                x1 = mid;
            }
        } while (Math.abs(x0 - x1) > 1e-6);
        lambda = (x0 + x1) / 2;
    }
    else {
        var x0 = 0, x1 = 0.1;
        do {
            var mid = (x0 + x1) / 2;
            if (1 / Math.pow(mid, 0.5) + 2 * Math.log10(e / (3.7 * dim)) > 0) {
                x0 = mid;
            }
            else {
                x1 = mid;
            }
            console.log(mid);
        } while (Math.abs(x0 - x1) > 1e-6);
        lambda = (x0 + x1) / 2;
    }
    return lambda;
}
/**
 * 直管阻力
 * @param f f flow [m3/h]
 * @param e Roughness [m]
 * @param dim Pipe Internal diameter [m]
 * @param l length [m]
 * @param rho density [kg/m3]
 * @param mu Dynamic viscosity [Ps.s]
 * @returns dp [kPa]
 */
function pipeDp0(f, e, dim, l, rho, mu) {
    var ve = f / 3600 / (Pi * Math.pow((dim / 2), 2));
    var re = reynolds(dim, ve, rho, mu);
    var lambda = resistace(re, dim, e);
    var dp0 = lambda * (l / dim) * (rho * Math.pow(ve, 2) / 2) * 0.001;
    console.log("ve:", ve, "\nre", re, "\nlambda", lambda);
    return dp0;
}
function pipeDp1(ksum, ve, rho) {
    var dp1 = ksum * rho * Math.pow(ve, 2) / 2 * 0.001;
    return dp1;
}
//console.log(resistace(4100, 0.5, 0.000075));
//console.log(396 * 0.2 / 0.000075 * Math.log10(3.7 * 0.2 / 0.000075));
console.log(pipeDp0(100, 0.075 / 1000, 0.2, 200, 1000, 0.001));
