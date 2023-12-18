"use strict"

function unitConverter(type, value) {
    let units = type.split("to");
    if (units.length == 2) {
        if (units[0] == units[1]) {
            return value
        }
    } else {
        return;
    }

    switch (type) {
        case "mmtom":
            return mmtom(value);
        case "mtomm":
            return mtomm(value);
        case "CtoK":
            return CtoK(value);
        case "KtoC":
            return KtoC(value);
        case "m3/htom3/s":
            return stoh(value);
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