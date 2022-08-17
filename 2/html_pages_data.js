"use strict"
import { Pipe } from "./pipe.js";
import { showResult } from "./html_page.js";

function verifyInputs(inputs) {
    for (const key in inputs) {
        if (Object.hasOwnProperty.call(inputs, key)) {
            if (inputs[key].value == "") return false;
        }
    }
    return true;
}

const html_pages_data = {
    pipe_diameter_velocity: {
        title: "流速>管径",
        inputs: {
            flowrate_volume: {title: "流量", unit: "m3/h", tagname: "input", type: "number", default_value: NaN, value: ""},
            velocity: {title: "流速", unit: "m/s", tagname: "input", type: "number", default_value: NaN, value: ""}
        },
        results: {
            di: {title: "内径", unit: "mm", value: ""}
        },
        options: {
            
        },
        multiple_rows: true,
        has_total_row: false,
        method: function () {
            if (!verifyInputs(html_pages_data.pipe_diameter_velocity.inputs)) {
                alert("输入数据错误!");
                return;
            }
             
            let pipe = new Pipe();
            pipe.fluid.flowRate_volume = parseFloat(html_pages_data.pipe_diameter_velocity.inputs.flowrate_volume.value) / 3600;
            html_pages_data.pipe_diameter_velocity.results.di.value = (pipe.diameter_velocity(parseFloat(html_pages_data.pipe_diameter_velocity.inputs.velocity.value)) * 1000).toFixed(2);
            showResult(html_pages_data.pipe_diameter_velocity.results);  
        }
    },
    pipe_diameter_drop_pressure: {
        title: "压降>管径",
        inputs: {
            flowrate_volume: {title: "流量", unit: "m3/h", tagname: "input", type: "number", default_value: NaN, value: ""},
            density: {title: "密度", unit: "kg/m3", tagname: "input", type: "number", default_value: NaN, value: ""},
            viscosity: {title: "运动粘度", unit: "m2/s", tagname: "input", type: "number", default_value: NaN, value: ""},
            length: {title: "管长", unit: "m", tagname: "input", type: "number", default_value: NaN, value: ""},
            drop_pressure: {title: "压降", unit: "MPa", tagname: "input", type: "number", default_value: NaN, value: ""}
        },
        results: {
            di: {title: "内径", unit: "mm", value: ""}
        },
        options: {
            
        },
        multiple_rows: true,
        has_total_row: false,
        method: function () {
            if (!verifyInputs(html_pages_data.pipe_diameter_drop_pressure.inputs)) {
                alert("输入数据错误!");
                return;
            }
             
            let pipe = new Pipe();
            pipe.fluid.flowRate_volume = parseFloat(html_pages_data.pipe_diameter_drop_pressure.inputs.flowrate_volume.value) / 3600;
            pipe.fluid.setDensity(parseFloat(html_pages_data.pipe_diameter_drop_pressure.inputs.density.value));
            pipe.fluid.setViscosity(parseFloat(html_pages_data.pipe_diameter_drop_pressure.inputs.viscosity.value));
            html_pages_data.pipe_diameter_velocity.results.di.value = (
                pipe.diameter_pressureDrop(
                    parseFloat(html_pages_data.pipe_diameter_drop_pressure.inputs.length.value),
                    parseFloat(html_pages_data.pipe_diameter_drop_pressure.inputs.drop_pressure.value) * 1e6
                ) * 1000
            ).toFixed(2);
            showResult(html_pages_data.pipe_diameter_velocity.results);  
        }
    },
    pipe_drop_pressure: {
        title: "阻力",
        inputs: {
            pipe: {title: "管道类别", tagname: "select", value: ""},
            di: {title: "管道内径", unit: "mm", tagname: "input", type: "number", default_value: NaN, value: ""},
            length: {title: "长度", unit: "m", tagname: "input", type: "number", default_value: NaN, value: ""},                       
            flowrate_mass: {title: "流量", unit: "kg/h", tagname: "input", type: "number", default_value: NaN, value: ""},
            density: {title: "密度", unit: "kg/m3", tagname: "input", type: "number", default_value: NaN, value: ""},
            viscosity: {title: "运动粘度", unit: "m2/s", tagname: "input", type: "number", default_value: NaN, value: ""},
            elbow45: {title: "45°弯头", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
            elbow90: {title: "90°弯头", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
            elbow90_x: {title: "90°斜接弯头", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
            elbow180: {title: "180°弯头", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
            globeValve: {title: "截止阀（全开）", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
            angleValve: {title: "角阀（全开）", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
            gateValve: {title: "闸阀（全开）", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
            plugValve: {title: "旋塞阀（全开）", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
            butterflyValve: {title: "蝶阀（全开）", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
            checkValve0: {title: "旋启式止回阀", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
            checkValve1: {title: "升降式止回阀", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
            footValve: {title: "底阀", unit: "个", tagname: "input", type: "number", default_value: NaN, value: ""},
        },
        results: {
            velocity: {title: "流速", unit: "m/s", value: ""},
            line_drop_pressure: {title: "直管阻力", unit: "MPa", value: ""},
            local_drop_pressure: {title: "局部阻力", unit: "MPa", value: ""},
            total_drop_pressure: {title: "总阻力", unit: "MPa", value: ""}
        },
        options: {
            pipe:[
                    {value: "0.0000075", text: "无缝黄铜、铜及铅管"},
                    {value: "0.000075", text: "操作中基本无腐蚀的无缝钢管"},
                    {value: "0.00015", text: "操作中有轻度腐蚀的无缝钢管"},
                    {value: "0.00035", text: "操作中有显著腐蚀的无缝钢管"},
                    {value: "0.00033", text: "钢板卷管"},
                    {value: "0.000675", text: "铸铁管"},
                    {value: "0.00000575", text: "干净的玻璃管"}
                ]
        },
        multiple_rows: true,
        has_total_row: true,
        method: function () {
            for (const key of ["pipe", "di", "length", "flowrate_mass", "density", "viscosity"]) {
                if (html_pages_data.pipe_drop_pressure.inputs[key].value == "") {
                    alert("输入数据错误!");
                    return;
                }
            }
            
            let pipe = new Pipe();
            pipe.fluid.flowRate_mass = parseFloat(html_pages_data.pipe_diameter_drop_pressure.inputs.flowrate_volume.value) / 3600;
            pipe.fluid.setDensity(parseFloat(html_pages_data.pipe_diameter_drop_pressure.inputs.density.value));
            pipe.fluid.setViscosity(parseFloat(html_pages_data.pipe_diameter_drop_pressure.inputs.viscosity.value));
            html_pages_data.pipe_diameter_velocity.results.di.value = (
                pipe.diameter_pressureDrop(
                    parseFloat(html_pages_data.pipe_diameter_drop_pressure.inputs.length.value),
                    parseFloat(html_pages_data.pipe_diameter_drop_pressure.inputs.drop_pressure.value) * 1e6
                ) * 1000
            ).toFixed(2);
            showResult(html_pages_data.pipe_diameter_velocity.results);  
        }
    },
    pipe_weight: {
        title: "钢管重量",
        inputs: {
            do_: {title: "外径", unit: "mm", tagname: "input", type: "number", default_value: NaN, value: ""},
            thk: {title: "壁厚", unit: "mm", tagname: "input", type: "number", default_value: NaN, value: ""},
            length: {title: "长度", unit: "m", tagname: "input", type: "number", default_value: NaN, value: ""}
        },
        results: {
            per_tube_weight: {title: "单位长度钢管自重", unit: "kg/m", value: ""},
            total_tube_weight: {title: "钢管总自重", unit: "kg", value: ""},
            per_water_weight: {title: "单位长度充水水重", unit: "kg/m", value: ""},
            total_water_weight: {title: "总水重", unit: "kg", value: ""},
            total_weight: {title: "总重", unit: "kg", value: ""}
        },
        options: {
            
        },
        multiple_rows: true,
        has_total_row: true
    },
    insul_pipe_weight: {
        title: "保温管道重量",
        inputs: {
            do_: {title: "外径", unit: "mm", tagname: "input", type: "number", default_value: NaN, value: ""},
            thk: {title: "壁厚", unit: "mm", tagname: "input", type: "number", default_value: NaN, value: ""},
            insul_thk: {title: "保温厚", unit: "mm", tagname: "input", type: "number", default_value: NaN, value: ""},
            insul_density: {title: "保温密度", unit: "mm", tagname: "input", type: "number", default_value: 200},
            clad_thk: {title: "保护层厚", unit: "mm", tagname: "input", type: "number", default_value: 0.6},
            clad_density: {title: "保护层密度", unit: "mm", tagname: "input", type: "number", default_value: 2720},
            length: {title: "长度", unit: "m", tagname: "input", type: "number", default_value: NaN, value: ""}
        },
        results: {
            per_weight: {title: "单重", unit: "kg", value: ""},
            total_weight: {title: "总重", unit: "kg", value: ""}
        },
        options: {
            
        },
        multiple_rows: true,
        has_total_row: true
    },
    anticorrosion_material: {
        title: "防腐材料量",
        inputs: {
            do_: {title: "管道外径", unit: "mm", tagname: "input", type: "number", default_value: NaN, value: ""},
            length: {title: "长度", unit: "m", tagname: "input", type: "number", default_value: NaN, value: ""}
        },
        results: {
            anticorrosion_area: {title: "刷漆量", unit: "m2", value: ""},
        },
        options: {
            
        },
        multiple_rows: true,
        has_total_row: true
    },
    insulation_material: {
        title: "保温材料量",
        inputs: {
            do_: {title: "外径", unit: "mm", tagname: "input", type: "number", default_value: NaN, value: ""},
            insul_thk: {title: "保温厚", unit: "mm", tagname: "input", type: "number", default_value: NaN, value: ""},
            length: {title: "长度", unit: "m", tagname: "input", type: "number", default_value: NaN, value: ""}
        },
        results: {
            insul_volume: {title: "保温材料量", unit: "m3", value: ""},
            clad_area: {title: "保护层材料量", unit: "m2", value: ""},
        },
        options: {
            
        },
        multiple_rows: true,
        has_total_row: true
    },
    property: {
        title: "物性",
        inputs: {
            fluid_name: {title: "名称", tagname: "select", value: ''},
            temperture: {title: "温度", unit: "℃", tagname: "input", type: "number", default_value: NaN, value: ""},
            pressure: {title: "压力", unit: "MPa(a)", tagname: "input", type: "number", default_value: NaN, value: ""},
            density: {title: "密度", unit: "kg/m3", tagname: "input", type: "number", default_value: NaN, value: ""},
            enthalpy: {title: "焓值", unit: "kj/kg", tagname: "input", type: "number", default_value: NaN, value: ""},
            entropy: {title: "比熵", unit: "kj/kg/K", tagname: "input", type: "number", default_value: NaN, value: ""},
            internal_energy: {title: "kj/kg", unit: "m", tagname: "input", type: "number", default_value: NaN, value: ""},
            vapor_quality: {title: "干度", unit: "m", tagname: "input", type: "number", default_value: NaN, value: ""}
        },
        results: {
            viscosity: {title: "粘度", unit: "m2/s", value: ""},
            z: {title: "压缩系数", unit: "", value: ""},
        },
        options: {
            fluid_name: [
                {value: "1-Butene", text: "1-Butene"},
                {value: "Acetone", text: "Acetone"},
                {value: "Air", text: "Air"},
                {value: "Ammonia", text: "Ammonia"},
                {value: "Argon", text: "Argon"},
                {value: "Benzene", text: "Benzene"},
                {value: "CarbonDioxide", text: "CarbonDioxide"},
                {value: "CarbonMonoxide", text: "CarbonMonoxide"},
                {value: "CarbonylSulfide", text: "CarbonylSulfide"},
                {value: "CycloHexane", text: "CycloHexane"},
                {value: "CycloPropane", text: "CycloPropane"},
                {value: "Cyclopentane", text: "Cyclopentane"},
                {value: "D4", text: "D4"},
                {value: "D5", text: "D5"},
                {value: "D6", text: "D6"},
                {value: "Deuterium", text: "Deuterium"},
                {value: "Dichloroethane", text: "Dichloroethane"},
                {value: "DiethylEther", text: "DiethylEther"},
                {value: "DimethylCarbonate", text: "DimethylCarbonate"},
                {value: "DimethylEther", text: "DimethylEther"},
                {value: "Ethane", text: "Ethane"},
                {value: "Ethanol", text: "Ethanol"},
                {value: "EthylBenzene", text: "EthylBenzene"},
                {value: "Ethylene", text: "Ethylene"},
                {value: "EthyleneOxide", text: "EthyleneOxide"},
                {value: "Fluorine", text: "Fluorine"},
                {value: "HFE143m", text: "HFE143m"},
                {value: "HeavyWater", text: "HeavyWater"},
                {value: "Helium", text: "Helium"},
                {value: "Hydrogen", text: "Hydrogen"},
                {value: "HydrogenChloride", text: "HydrogenChloride"},
                {value: "HydrogenSulfide", text: "HydrogenSulfide"},
                {value: "IsoButane", text: "IsoButane"},
                {value: "IsoButene", text: "IsoButene"},
                {value: "Isohexane", text: "Isohexane"},
                {value: "Isopentane", text: "Isopentane"},
                {value: "Krypton", text: "Krypton"},
                {value: "MD2M", text: "MD2M"},
                {value: "MD3M", text: "MD3M"},
                {value: "MD4M", text: "MD4M"},
                {value: "MDM", text: "MDM"},
                {value: "MM", text: "MM"},
                {value: "Methane", text: "Methane"},
                {value: "Methanol", text: "Methanol"},
                {value: "MethylLinoleate", text: "MethylLinoleate"},
                {value: "MethylLinolenate", text: "MethylLinolenate"},
                {value: "MethylOleate", text: "MethylOleate"},
                {value: "MethylPalmitate", text: "MethylPalmitate"},
                {value: "MethylStearate", text: "MethylStearate"},
                {value: "Neon", text: "Neon"},
                {value: "Neopentane", text: "Neopentane"},
                {value: "Nitrogen", text: "Nitrogen"},
                {value: "NitrousOxide", text: "NitrousOxide"},
                {value: "Novec649", text: "Novec649"},
                {value: "OrthoDeuterium", text: "OrthoDeuterium"},
                {value: "OrthoHydrogen", text: "OrthoHydrogen"},
                {value: "Oxygen", text: "Oxygen"},
                {value: "ParaDeuterium", text: "ParaDeuterium"},
                {value: "ParaHydrogen", text: "ParaHydrogen"},
                {value: "Propylene", text: "Propylene"},
                {value: "Propyne", text: "Propyne"},
                {value: "R11", text: "R11"},
                {value: "R113", text: "R113"},
                {value: "R114", text: "R114"},
                {value: "R115", text: "R115"},
                {value: "R116", text: "R116"},
                {value: "R12", text: "R12"},
                {value: "R123", text: "R123"},
                {value: "R1233zd(E)", text: "R1233zd(E)"},
                {value: "R1234yf", text: "R1234yf"},
                {value: "R1234ze(E)", text: "R1234ze(E)"},
                {value: "R1234ze(Z)", text: "R1234ze(Z)"},
                {value: "R124", text: "R124"},
                {value: "R125", text: "R125"},
                {value: "R13", text: "R13"},
                {value: "R134a", text: "R134a"},
                {value: "R13I1", text: "R13I1"},
                {value: "R14", text: "R14"},
                {value: "R141b", text: "R141b"},
                {value: "R142b", text: "R142b"},
                {value: "R143a", text: "R143a"},
                {value: "R152A", text: "R152A"},
                {value: "R161", text: "R161"},
                {value: "R21", text: "R21"},
                {value: "R218", text: "R218"},
                {value: "R22", text: "R22"},
                {value: "R227EA", text: "R227EA"},
                {value: "R23", text: "R23"},
                {value: "R236EA", text: "R236EA"},
                {value: "R236FA", text: "R236FA"},
                {value: "R245ca", text: "R245ca"},
                {value: "R245fa", text: "R245fa"},
                {value: "R32", text: "R32"},
                {value: "R365MFC", text: "R365MFC"},
                {value: "R40", text: "R40"},
                {value: "R404A", text: "R404A"},
                {value: "R407C", text: "R407C"},
                {value: "R41", text: "R41"},
                {value: "R410A", text: "R410A"},
                {value: "R507A", text: "R507A"},
                {value: "RC318", text: "RC318"},
                {value: "SES36", text: "SES36"},
                {value: "SulfurDioxide", text: "SulfurDioxide"},
                {value: "SulfurHexafluoride", text: "SulfurHexafluoride"},
                {value: "Toluene", text: "Toluene"},
                {value: "Water", text: "Water"},
                {value: "Xenon", text: "Xenon"},
                {value: "cis-2-Butene", text: "cis-2-Butene"},
                {value: "m-Xylene", text: "m-Xylene"},
                {value: "n-Butane", text: "n-Butane"},
                {value: "n-Decane", text: "n-Decane"},
                {value: "n-Dodecane", text: "n-Dodecane"},
                {value: "n-Heptane", text: "n-Heptane"},
                {value: "n-Hexane", text: "n-Hexane"},
                {value: "n-Nonane", text: "n-Nonane"},
                {value: "n-Octane", text: "n-Octane"},
                {value: "n-Pentane", text: "n-Pentane"},
                {value: "n-Propane", text: "n-Propane"},
                {value: "n-Undecane", text: "n-Undecane"},
                {value: "o-Xylene", text: "o-Xylene"},
                {value: "p-Xylene", text: "p-Xylene"},
                {value: "trans-2-Butene", text: "trans-2-Butene"}
            ]
        },
        multiple_rows: true,
        has_total_row: false
    }
}

export default html_pages_data;