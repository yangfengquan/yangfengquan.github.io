"use strict"
/**
 * 计算泵轴功率
 * @param {number} flowRate 流量 kg/s
 * @param {number} h 扬程 m
 * @param {number} eta 效率 ＜1
 * @returns {number} 轴功率 W
 */
export function pump_power(flowRate, h, eta) {
    return flowRate * h * 9.81  / eta;
}