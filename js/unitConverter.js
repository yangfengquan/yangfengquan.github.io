"use strict"
const length_unit = {
    "mm": {
        to_anchor: 1 / 1000
    },
    "cm": {
        to_anchor: 1 / 100
    },
    "dm": {
        to_anchor: 1 / 10
    },
    "m": {
        to_anchor: 1
    },
    "km": {
        to_anchor: 1000
    }
}
const mass_unit = {
    "g": {
        to_anchor: 1 / 1000,
    },
    "kg": {
        to_anchor: 1,
    },
    "t": {
        to_anchor: 1000,
    }
}
const time_unit = {
    "s": {
        to_anchor: 1
    },
    "min": {
        to_anchor: 60
    },
    "h": {
        to_anchor: 3600
    }
}
const temperature_unit = {
    "K": {
        to_anchor: 1,
        anchor_shift: 0
    },
    "C": {
        to_anchor: 1,
        anchor_shift: 273.15 //均相对于基本单位SI
    }
}
const pressure_unit = {
    "Pa": {
        to_anchor: 1,
        anchor_shift: 0 //均相对于基本单位SI
    },
    "kPa": {
        to_anchor: 1000,
        anchor_shift: 0
    },
    "MPa": {
        to_anchor: 1e6,
        anchor_shift: 0
    },
    "PaA": {
        to_anchor: 1,
        anchor_shift: -101325 //均相对于基本单位SI
    },
    "kPaA": {
        to_anchor: 1000,
        anchor_shift: -101325 //均相对于基本单位SI
    },
    "MPaA": {
        to_anchor: 1e6,
        anchor_shift: -101325 //均相对于基本单位SI
    }
}
const energy_unit = {
    "J": {
        to_anchor: 1,
    },
    "kJ": {
        to_anchor: 1000,
    },
    "MJ": {
        to_anchor: 1e6,
    },
    "GJ": {
        to_anchor: 1e9,
    }
}
const power_unit = {
    "W": {
        to_anchor: 1,
    },
    "kW": {
        to_anchor: 1000,
    },
    "MW": {
        to_anchor: 1e6,
    },
    "GW": {
        to_anchor: 1e9,
    }
}
const force_unit = {
    "N": {
        to_anchor: 1,
    },
    "kN": {
        to_anchor: 1000,
    },
}
const speed_unit = {
    "m/s": {
        to_anchor: 1,
    },
    "km/h": {
        to_anchor: 1 / 3.6,
    }
}
const density_unit = {
    "g/m3": {
        to_anchor: 1 / 1000,
    },
    "kg/m3": {
        to_anchor: 1,
    }
}
const specificVolume_unit = {
    "m3/g": {
        to_anchor: 1000,
    },
    "m3/kg": {
        to_anchor: 1,
    }
}
const massFlowRate_unit = {
    "kg/s": {
        to_anchor: 1
    },
    "kg/h": {
        to_anchor: 1 / 3600
    },
    "t/h": {
        to_anchor: 1 / 3.6
    }
}
const volumeFlowRate_unit = {
    "m3/s": {
        to_anchor: 1
    },
    "m3/h": {
        to_anchor: 1 / 3600
    }
}

//基础单位
const base_unit = {
    "baseUnit": {
        to_anchor: 1,
        anchor_shift: 0
    }
}
/**
 * 单位换算，并返回换算后数值
 * @param {number} value 
 * @param {string} originalUnit 
 * @param {string} destUnit 
 * @returns {number}
 */
export default function unitConverter(value, originalUnit = "baseUnit", destUnit = "baseUnit") { 
    if (originalUnit === destUnit) {
        return value;
    }
    if (originalUnit === '' || destUnit === '') {
        return value;
    }
    
    let unit = Object.assign({}, length_unit, mass_unit, time_unit, temperature_unit, 
        pressure_unit, energy_unit, power_unit, force_unit, speed_unit, density_unit,
        specificVolume_unit, massFlowRate_unit, volumeFlowRate_unit, base_unit);
    
    //缺少的单位，直接返回原数值。
    if (!unit.hasOwnProperty(originalUnit) || !unit.hasOwnProperty(destUnit)) {
        return value;
    }
    //下列语句顺序不可修改
    value *= unit[originalUnit].to_anchor;
    if (unit[originalUnit].hasOwnProperty("anchor_shift")) {
        value += unit[originalUnit].anchor_shift;
    }

    if (unit[destUnit].hasOwnProperty("anchor_shift")) {
        value -= unit[destUnit].anchor_shift;
    }
    value /= unit[destUnit].to_anchor;
    
    return value;
}

export {
    length_unit,
    mass_unit,
    time_unit,
    temperature_unit,
    pressure_unit,
    energy_unit,
    power_unit,
    force_unit, 
    speed_unit,
    density_unit,
    specificVolume_unit,
    massFlowRate_unit,
    volumeFlowRate_unit
};