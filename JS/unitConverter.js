"use strict"

/**
 * 单位转换器，返回转换后的数值
 * @param {string} type 例如："mmtom"
 * @param {number} value 
 * @returns {number} 
 */
export default function unitConverter(type, value) {
    let units = type.split("to");
    if (units.length != 2 || !units[0] || !units[1] || units[0] == units[1]) {
        return value
    }

    switch (type) {
        case "mmtom":
            return mmtom(value);
        case "mtomm":
            return mtomm(value);
        case "mtokm":
            return mtokm(value);
        case "kmtom":
            return kmtom(value);
        case "CtoK":
            return CtoK(value);
        case "KtoC":
            return KtoC(value);
        case "m3/htom3/s":
        case "kg/htokg/s":
            return stoh(value);
        case "m3/stom3/h":
        case "kg/stokg/h":
            return htos(value);
        case "MPatoKPa":
            return MPatoKPa(value);
        case "KPatoMPa":
            return KPatoMPa(value);
        case "MPatoPa":
            return MPatoPa(value);
        case "PatoMPa":
            return PatoMPa(value);
        case "KPatoPa":
            return KPatoPa(value);
        case "PatoKPa":
            return PatoKPa(value);
        case "MPatoMPaA":
            return MPatoMPaA(value);
        case "KPatoKPaA":
            return KPatoKPaA(value);
        case "PatoPaA":
            return PatoPaA(value);
        default:
            break;
    }
}
function mmtom(value) {
    return value / 1000;
}
function mtomm(value) {
    return value * 1000;
}
function mtokm(value) {
    return value / 1000;
}
function kmtom(value) {
    return value * 1000;
}
function CtoK(value) {
    return value + 273.15;
}
function KtoC(value) {
    return value - 273.15;
}
function stoh(value) {
    return value / 3600;
}
function htos(value) {
    return value * 3600;
}
function MPatoKPa(value) {
    return value * 1000;
}
function KPatoMPa(value) {
    return value / 1000;
}
function MPatoPa(value) {
    return value * 1E6;
}
function PatoMPa(value) {
    return value / 1E6;
}
function KPatoPa(value) {
    return value * 1000;
}
function PatoKPa(value) {
    return value / 1000;
}
function MPatoMPaA(value) {
    return value + 101.325 / 1000;
}
function MPaAtoMPa(value) {
    return value - 101.325 / 1000;
}
function KPatoKPaA(value) {
    return value + 101.325;
}
function KPaAtoKPa(value) {
    return value - 101.325;
}
function PatoPaA(value) {
    return value + 101.325 * 1000;
}
function PaAtoPa(value) {
    return value - 101.325 * 1000;
}

