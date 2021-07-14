"use strict";
const Pi = 3.1415926;
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
 * 计算管道内径（允许压降）
 * @param f flow [m3/h]
 * @param dP allow pressure drop [kPa/100m]
 * @param rho density [kg/m3]
 * @param nu Kinematic viscosity[m2/s]
 * @returns Pipe Internal diameter [mm]
 */
function dim_dP(f, dP, rho, nu) {
    nu = nu * 10000;
    return 11.4 * Math.pow(rho, 0.207) * Math.pow(nu, 0.033) * Math.pow(f, 0.38) * Math.pow(dP, -0.207);
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
