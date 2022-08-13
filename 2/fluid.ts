class Fluid {
    name:string = "";
    T:number = NaN; // K
    P:number = NaN; // Pa
    M:number = NaN; // kg/mol
    D:number = NaN; // density kg/m^3
    H:number = NaN; // Mass specific enthalpy J/kg
    Q:number = NaN; // Mass vapor quality mol/mol
    S:number = NaN; // Mass specific entropy J/kg/K
    U:number = NaN; // Mass specific internal energy J/kg
    viscosity:number = NaN; // Pa.s
    Z:number = NaN; // Compressibility factor
    flowRate_mass:number = 0; //质量流量
    flowRate_volume:number = 0; //体积流量
    constructor();
    constructor(key1?:string, val1?:number, key2?:string, val2?:number, name?:string) {
        if (key1 != undefined && key2 != undefined && name != undefined ) {
            this.name = name;
            this[key1] = val1;
            this[key2] = val2;
            ["T","P","M","D","H", "Q", "S", "U", "viscosity", "Z"].forEach(key=>{
                if ([key1, key2].indexOf(key) == -1) {
                    this[key] = Module.PropsSI(key, key1, val1, key2, val2, name);
                    //console.log(Module.PropsSI("T","P",100000, "Q", 1, "WATER"));
                }
            })
        }
    }
    
    setName(name:string):void {
        this.name = name;
    }

    setT(T:number):void {
        this.T = T;
    }

    setP(P:number):void {
        this.P = P;
    }

    setDensity(D:number):void {
        this.D = D;
    }

    setEnthalpy(H:number):void {
        this.H = H;
    }

    setViscosity(viscosity:number):void {
        this.viscosity = viscosity;
    }

    getT():number {
        return this.T;
    }

    getP():number {
        return this.P;
    }

    getM():number {
        return this.M;
    }

    getDensity():number {
        return this.D;
    }

    getEnthalpy():number {
        return this.H;
    }

    getEntropy():number {
        return this.S;
    }

    getInternalEnergy():number {
        return this.U;
    }

    getViscosity():number {//m2/s
        return this.viscosity / this.D; //运动粘度= 动力粘度 / 密度
    }
    getQ():number {
        return this.Q;
    }
    getZ():number {
        return this.Z;
    }

    getFlowRate_mass() {
        return isNaN(this.flowRate_mass) ? this.flowRate_volume * this.D : this.flowRate_mass;
    }

    getFlowRate_volume() {
        return isNaN(this.flowRate_volume) ? this.flowRate_mass / this.D : this.flowRate_volume;
    }
}

export { Fluid }