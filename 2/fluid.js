class Fluid {
    constructor(key1, val1, key2, val2, name) {
        this.name = "";
        this.T = NaN; // K
        this.P = NaN; // Pa
        this.M = NaN; // kg/mol
        this.D = NaN; // density kg/m^3
        this.H = NaN; // Mass specific enthalpy J/kg
        this.Q = NaN; // Mass vapor quality mol/mol
        this.S = NaN; // Mass specific entropy J/kg/K
        this.U = NaN; // Mass specific internal energy J/kg
        this.viscosity = NaN; // Pa.s
        this.Z = NaN; // Compressibility factor
        this.flowRate_mass = 0; //质量流量
        this.flowRate_volume = 0; //体积流量
        if (key1 != undefined && key2 != undefined && name != undefined) {
            this.name = name;
            this[key1] = val1;
            this[key2] = val2;
            ["T", "P", "M", "D", "H", "Q", "S", "U", "viscosity", "Z"].forEach(key => {
                if ([key1, key2].indexOf(key) == -1) {
                    this[key] = Module.PropsSI(key, key1, val1, key2, val2, name);
                    //console.log(Module.PropsSI("T","P",100000, "Q", 1, "WATER"));
                }
            });
        }
    }
    setName(name) {
        this.name = name;
    }
    setT(T) {
        this.T = T;
    }
    setP(P) {
        this.P = P;
    }
    setDensity(D) {
        this.D = D;
    }
    setEnthalpy(H) {
        this.H = H;
    }
    setViscosity(viscosity) {
        this.viscosity = viscosity;
    }
    getT() {
        return this.T;
    }
    getP() {
        return this.P;
    }
    getM() {
        return this.M;
    }
    getDensity() {
        return this.D;
    }
    getEnthalpy() {
        return this.H;
    }
    getEntropy() {
        return this.S;
    }
    getInternalEnergy() {
        return this.U;
    }
    getViscosity() {
        return this.viscosity / this.D; //运动粘度= 动力粘度 / 密度
    }
    getQ() {
        return this.Q;
    }
    getZ() {
        return this.Z;
    }
    getFlowRate_mass() {
        return isNaN(this.flowRate_mass) ? this.flowRate_volume * this.D : this.flowRate_mass;
    }
    getFlowRate_volume() {
        return isNaN(this.flowRate_volume) ? this.flowRate_mass / this.D : this.flowRate_volume;
    }
}
export { Fluid };
