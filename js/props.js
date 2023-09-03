/**
 * 
 * @param {*} rows 
 */
function waterprops(rows) {
    let name1 = rows[0].key;
    let name2 = rows[1].key;
    let value1 = parseFloat(rows[0].value)
    let value2 = parseFloat(rows[1].value)
    if (name1 == 'T') {
        value1 += 273.15
    }
    if (name2 == 'T') {
        value2 += 273.15
    }
    let res = eval(name1 + name2)(value1, value2);
    return [
        res.p,
        res.T - 273.15,
        res.h,
        res.v,
        res.rho,
        res.s,
        res.u,
        res.Cp,
        res.Cv,
        res.w,
        res.my,
        res.tc,
        res.st,
        res.x,
        res.vx,
        res.Cp / res.Cv
    ];
}

/**
 * 
 * @param {*} rows 
 */
function props(rows) {
    const Props_En = {
        DELTA:  {unit: "",  description: "Reduced density (rho/rhoc)"},
        DMOLAR: {unit: "mol/m^3",  description: "Molar density"},
        D: {unit: "kg/m^3",  description: "Mass density"},
        HMOLAR: {unit: "J/mol",  description: "Molar specific enthalpy"},
        H: {unit: "J/kg",  description: "Mass specific enthalpy"},
        P: {unit: "MPaA",  description: "Pressure"},// coolProp Pa
        Q: {unit: "mol/mol",  description: "Mass vapor quality"},
        SMOLAR: {unit: "J/mol/K",  description: "Molar specific entropy"},
        S: {unit: "J/kg/K",  description: "Mass specific entropy"},
        TAU: {unit: "",  description: "Reciprocal reduced temperature (Tc/T)"},
        T: {unit: "℃",  description: "Temperature"},// coolProp K
        UMOLAR: {unit: "J/mol",  description: "Molar specific internal energy"},
        U: {unit: "J/kg",  description: "Mass specific internal energy"},
        ACENTRIC: {unit: "",  description: "Acentric factor"},
        ALPHA0: {unit: "",  description: "Ideal Helmholtz energy"},
        ALPHAR: {unit: "",  description: "Residual Helmholtz energy"},
        A: {unit: "m/s",  description: "Speed of sound"},
        BVIRIAL: {unit: "",  description: "Second virial coefficient"},
        CONDUCTIVITY: {unit: "W/m/K",  description: "Thermal conductivity"},
        CP0MASS: {unit: "J/kg/K",  description: "Ideal gas mass specific constant pressure specific heat"},
        CP0MOLAR: {unit: "J/mol/K",  description: "Ideal gas molar specific constant pressure specific heat"},
        CPMOLAR: {unit: "J/mol/K",  description: "Molar specific constant pressure specific heat"},
        CVIRIA: {unit: "",  description: "Third virial coefficient"},
        CVMASS: {unit: "J/kg/K",  description: "Mass specific constant volume specific heat"},
        CVMOLAR: {unit: "J/mol/K",  description: "Molar specific constant volume specific heat"},
        C: {unit: "J/kg/K",  description: "Mass specific constant pressure specific heat"},
        DALPHA0_DDELTA_CONSTTAU: {unit: "",  description: "Derivative of ideal Helmholtz energy with delta"},
        DALPHA0_DTAU_CONSTDELTA: {unit: "",  description: "Derivative of ideal Helmholtz energy with tau"},
        DALPHAR_DDELTA_CONSTTAU: {unit: "",  description: "Derivative of residual Helmholtz energy with delta"},
        DALPHAR_DTAU_CONSTDELTA: {unit: "",  description: "Derivative of residual Helmholtz energy with tau"},
        DBVIRIAL_DT: {unit: "",  description: "Derivative of second virial coefficient with respect to T"},
        DCVIRIAL_DT: {unit: "",  description: "Derivative of third virial coefficient with respect to T"},
        DIPOLE_MOMENT: {unit: "C m",  description: "Dipole moment"},
        FH: {unit: "",  description: "Flammability hazard"},
        FRACTION_MAX: {unit: "",  description: "Fraction (mole,mass,volume) maximum value for incompressible solutions"},
        FRACTION_MIN: {unit: "",  description: "Fraction (mole,mass,volume) minimum value for incompressible solutions"},
        FUNDAMENTAL_DERIVATIVE_OF_GAS_DYNAMICS: {unit: "",  description: "Fundamental derivative of gas dynamics"},
        GAS_CONSTANT: {unit: "J/mol/K",  description: "Molar gas constant"},
        GMOLAR_RESIDUAL: {unit: "J/mol/K",  description: "Residual molar Gibbs energy"},
        GMOLAR: {unit: "J/mol",  description: "Molar specific Gibbs energy"},
        GWP100: {unit: "",  description: "100-year global warming potential"},
        GWP20: {unit: "",  description: "20-year global warming potential"},
        GWP500: {unit: "",  description: "500-year global warming potential"},
        G: {unit: "J/kg",  description: "Mass specific Gibbs energy"},
        HELMHOLTZMASS: {unit: "J/kg",  description: "Mass specific Helmholtz energy"},
        HELMHOLTZMOLAR: {unit: "J/mol",  description: "Molar specific Helmholtz energy"},
        HH: {unit: "",  description: "Health hazard"},
        HMOLAR_RESIDUAL: {unit: "J/mol/K",  description: "Residual molar enthalpy"},
        ISENTROPIC_EXPANSION_COEFFICIENT: {unit: "",  description: "Isentropic expansion coefficient"},
        ISOBARIC_EXPANSION_COEFFICIENT: {unit: "1/K",  description: "Isobaric expansion coefficient"},
        ISOTHERMAL_COMPRESSIBILITY: {unit: "1/Pa",  description: "Isothermal compressibility"},
        SURFACE_TENSION: {unit: "N/m",  description: "Surface tension"},
        M: {unit: "kg/mol",  description: "Molar mass"},
        ODP: {unit: "",  description: "Ozone depletion potential"},
        PCRIT: {unit: "Pa",  description: "Pressure at the critical point"},
        PHASE: {unit: "",  description: "Phase index as a float"},
        PH: {unit: "",  description: "Physical hazard"},
        PIP: {unit: "",  description: "Phase identification parameter"},
        PMAX: {unit: "Pa",  description: "Maximum pressure limit"},
        PMIN: {unit: "Pa",  description: "Minimum pressure limit"},
        PRANDTL: {unit: "",  description: "Prandtl number"},
        PTRIPLE: {unit: "Pa",  description: "Pressure at the triple point (pure only)"},
        P_REDUCING: {unit: "Pa",  description: "Pressure at the reducing point"},
        RHOCRIT: {unit: "kg/m^3",  description: "Mass density at critical point"},
        RHOMASS_REDUCING: {unit: "kg/m^3",  description: "Mass density at reducing point"},
        RHOMOLAR_CRITICAL: {unit: "mol/m^3",  description: "Molar density at critical point"},
        RHOMOLAR_REDUCING: {unit: "mol/m^3",  description: "Molar density at reducing point"},
        SMOLAR_RESIDUAL: {unit: "J/mol/K",  description: "Residual molar entropy (sr/R = s(T,rho) - s^0(T,rho))"},
        TCRIT: {unit: "K",  description: "Temperature at the critical point"},
        TMAX: {unit: "K",  description: "Maximum temperature limit"},
        TMIN: {unit: "K",  description: "Minimum temperature limit"},
        TTRIPLE: {unit: "K",  description: "Temperature at the triple point"},
        T_FREEZE: {unit: "K",  description: "Freezing temperature for incompressible solutions"},
        T_REDUCING: {unit: "K",  description: "Temperature at the reducing point"},
        V: {unit: "Pa s",  description: "Viscosity"},
        Z: {unit: "",  description: "Compressibility factor"}
    };

    const Props_zh = {
        DELTA:  {unit: "",  description: "还原密度 (rho/rhoc)"},
        DMOLAR: {unit: "mol/m^3",  description: "摩尔密度"},
        D: {unit: "kg/m^3",  description: "质量密度"},
        HMOLAR: {unit: "J/mol",  description: "摩尔比焓"},
        H: {unit: "J/kg",  description: "质量比焓"},
        P: {unit: "MPaA",  description: "压力"},// coolProp Pa
        Q: {unit: "mol/mol",  description: "干度"},
        SMOLAR: {unit: "J/mol/K",  description: "摩尔比熵"},
        S: {unit: "J/kg/K",  description: "质量比熵"},
        TAU: {unit: "",  description: "倒数还原温度 (Tc/T)"},
        T: {unit: "℃",  description: "温度"},// coolProp K
        UMOLAR: {unit: "J/mol",  description: "摩尔比内能"},
        U: {unit: "J/kg",  description: "质量比内能"},
        ACENTRIC: {unit: "",  description: "偏心因子"},
        ALPHA0: {unit: "",  description: "理想亥姆霍兹能量"},
        ALPHAR: {unit: "",  description: "剩余亥姆霍兹能量"},
        A: {unit: "m/s",  description: "声速"},
        BVIRIAL: {unit: "",  description: "第二维里系数"},
        CONDUCTIVITY: {unit: "W/m/K",  description: "导热系数"},
        CP0MASS: {unit: "J/kg/K",  description: "理想气体质量比恒压比热"},
        CP0MOLAR: {unit: "J/mol/K",  description: "理想气体摩尔比恒压比热"},
        CPMOLAR: {unit: "J/mol/K",  description: "摩尔比定压比热"},
        CVIRIA: {unit: "",  description: "第三维里系数"},
        CVMASS: {unit: "J/kg/K",  description: "质量比定容比热"},
        CVMOLAR: {unit: "J/mol/K",  description: "摩尔比定容比热"},
        C: {unit: "J/kg/K",  description: "质量比定压比热"},
        DALPHA0_DDELTA_CONSTTAU: {unit: "",  description: "理想亥姆霍兹能与δ的导数"},
        DALPHA0_DTAU_CONSTDELTA: {unit: "",  description: "理想亥姆霍兹能与τ的导数"},
        DALPHAR_DDELTA_CONSTTAU: {unit: "",  description: "剩余亥姆霍兹能量与δ的导数"},
        DALPHAR_DTAU_CONSTDELTA: {unit: "",  description: "剩余亥姆霍兹能量与τ的导数"},
        DBVIRIAL_DT: {unit: "",  description: "第二维里系数对T的导数"},
        DCVIRIAL_DT: {unit: "",  description: "第三维里系数对T的导数"},
        DIPOLE_MOMENT: {unit: "C m",  description: "偶极矩"},
        FH: {unit: "",  description: "可燃性危险"},
        FRACTION_MAX: {unit: "",  description: "不可压缩溶液分数（摩尔，质量，体积）的最大值"},
        FRACTION_MIN: {unit: "",  description: "不可压缩溶液分数（摩尔、质量、体积）的最小值"},
        FUNDAMENTAL_DERIVATIVE_OF_GAS_DYNAMICS: {unit: "",  description: "气体动力学的基本导数"},
        GAS_CONSTANT: {unit: "J/mol/K",  description: "摩尔气体常数"},
        GMOLAR_RESIDUAL: {unit: "J/mol/K",  description: "残余摩尔吉布斯能"},
        GMOLAR: {unit: "J/mol",  description: "摩尔比吉布斯能"},
        GWP100: {unit: "",  description: "百年全球变暖潜能值"},
        GWP20: {unit: "",  description: "20年全球变暖潜能值"},
        GWP500: {unit: "",  description: "500年全球变暖潜能值"},
        G: {unit: "J/kg",  description: "质量比吉布斯能"},
        HELMHOLTZMASS: {unit: "J/kg",  description: "质量比亥姆霍兹能量"},
        HELMHOLTZMOLAR: {unit: "J/mol",  description: "摩尔比亥姆霍兹能"},
        HH: {unit: "",  description: "健康危害"},
        HMOLAR_RESIDUAL: {unit: "J/mol/K",  description: "残余摩尔焓"},
        ISENTROPIC_EXPANSION_COEFFICIENT: {unit: "",  description: "等熵膨胀系数"},
        ISOBARIC_EXPANSION_COEFFICIENT: {unit: "1/K",  description: "等压膨胀系数"},
        ISOTHERMAL_COMPRESSIBILITY: {unit: "1/Pa",  description: "等温压缩性"},
        SURFACE_TENSION: {unit: "N/m",  description: "表面张力"},
        M: {unit: "kg/mol",  description: "摩尔质量"},
        ODP: {unit: "",  description: "臭氧消耗潜能值"},
        PCRIT: {unit: "Pa",  description: "临界点压力"},
        PHASE: {unit: "",  description: "Phase index as a float"},
        PH: {unit: "",  description: "物理危害"},
        PIP: {unit: "",  description: "Phase identification parameter"},
        PMAX: {unit: "Pa",  description: "最大压力限制"},
        PMIN: {unit: "Pa",  description: "最小压力限制"},
        PRANDTL: {unit: "",  description: "普朗特数"},
        PTRIPLE: {unit: "Pa",  description: "三相点的压力"},
        P_REDUCING: {unit: "Pa",  description: "还原点压力"},
        RHOCRIT: {unit: "kg/m^3",  description: "临界点质量密度"},
        RHOMASS_REDUCING: {unit: "kg/m^3",  description: "还原点质量密度"},
        RHOMOLAR_CRITICAL: {unit: "mol/m^3",  description: "临界点摩尔密度"},
        RHOMOLAR_REDUCING: {unit: "mol/m^3",  description: "还原点摩尔密度"},
        SMOLAR_RESIDUAL: {unit: "J/mol/K",  description: "残差摩尔熵"},
        TCRIT: {unit: "K",  description: "临界点温度"},
        TMAX: {unit: "K",  description: "最高温度限制"},
        TMIN: {unit: "K",  description: "最低温度限制"},
        TTRIPLE: {unit: "K",  description: "三相点的温度"},
        T_FREEZE: {unit: "K",  description: "不可压缩溶液的冻结温度"},
        T_REDUCING: {unit: "K",  description: "还原点温度"},
        V: {unit: "Pa s",  description: "粘度"},
        Z: {unit: "",  description: "压缩因子"}
    }

    var normalArgs = ["P","T","DMOLAR","D","HMOLAR","H","SMOLAR","S","UMOLAR","U","C","CPMOLAR","CVMASS","CVMOLAR","A","CONDUCTIVITY","V","M","Z"];
    var otherArgs = ["ACENTRIC","ALPHA0","ALPHAR","BVIRIAL","CP0MASS","CP0MOLAR","CVIRIA","DALPHA0_DDELTA_CONSTTAU",
                        "DALPHA0_DTAU_CONSTDELTA","DALPHAR_DDELTA_CONSTTAU","DALPHAR_DTAU_CONSTDELTA",
                        "DBVIRIAL_DT","DCVIRIAL_DT","DIPOLE_MOMENT","FH","FRACTION_MAX","FRACTION_MIN",
                        "FUNDAMENTAL_DERIVATIVE_OF_GAS_DYNAMICS","GAS_CONSTANT","GMOLAR_RESIDUAL","GMOLAR",
                        "GWP20","GWP100","GWP500","G","HELMHOLTZMASS","HELMHOLTZMOLAR","HH","HMOLAR_RESIDUAL",
                        "ISENTROPIC_EXPANSION_COEFFICIENT","ISOBARIC_EXPANSION_COEFFICIENT","ISOTHERMAL_COMPRESSIBILITY",
                        "SURFACE_TENSION","ODP","PCRIT","PH","PRANDTL","PTRIPLE",
                        "P_REDUCING","RHOCRIT","RHOMASS_REDUCING","RHOMOLAR_CRITICAL","RHOMOLAR_REDUCING",
                        "SMOLAR_RESIDUAL","TCRIT","TTRIPLE","T_FREEZE","T_REDUCING"];
    
    
    let fluid = rows[0].value;
    let name1 = rows[1].key;
    let name2 = rows[2].key;
    let value1 = parseFloat(rows[1].value);
    let value2 = parseFloat(rows[2].value);
    if (fluid == '' || name1 == '0' || isNaN(value1) || isNaN(value2)) {
        alert("输入错误！");
        return;
    }

    if(name1 == "T") value1 += 273.15;
    if(name2 == "T") value2 += 273.15;

    if(name1 == "P") value1 *= 1E6;
    if(name2 == "P") value2 *= 1E6;
 
    let res = [];
    try {
        normalArgs.forEach(key => {
            let v = Module.PropsSI(key, name1, value1, name2, value2, fluid);
            if (key == "P") {
                v /= 1e6;
            }
            if (key == "T") {
                v -= 273.15;
            }
            res.push(v);
        })

        otherArgs.forEach(prop => {
            let v = Module.PropsSI(prop,rows[1],rows[2],rows[3],rows[4],rows[0]);
            if (prop == "P") {
                v /= 1e6;
            }
            if (prop == "T") {
                v -= 273.15;
            }
            res.push(v)
        })
    } catch (error) {
        alert(error, "\n请稍候再试。");
    }             
    return res;
}