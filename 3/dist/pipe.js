"use strict";
/* 严格采用国际单位（SI），无例外 */
/**
 * 圆 周长
 * @param d 直径 m
 * @returns m
 */
function circleCircumference(d /* 直径 */) {
    return Math.PI * d;
}
/**
 * 圆面积
 * @param d 直径 m
 * @returns m2
 */
function circleArea(d /* 直径 */) {
    return Math.PI * Math.pow(d, 2) * 0.25;
}
/**
 * 圆环面积
 * @param di
 * @param do_
 * @returns m2
 */
function cylinderArea(di /* 内径 m */, do_ /* 外径 m */) {
    return circleArea(do_) - circleArea(di);
}
/**
 * 雷诺数
 */
function reynolds(di /* 内径 m */, velocity, density, viscosity /* 动力粘度 Pa.s */) {
    return di * velocity * density / viscosity;
}
class Fluid {
    constructor() {
        this.density = 0;
        this.viscosity = 0;
        this.flowRate_mass = 0; //质量流量
        this.flowRate_volume = 0; //体积流量
    }
    getViscosity() {
        return this.viscosity;
    }
    getDensity() {
        return this.density;
    }
    getFlowRate_mass() {
        return this.flowRate_mass;
    }
    getFlowRate_volume() {
        return this.flowRate_volume;
    }
}
class PipeMaterial {
    constructor() {
        this.density = 7850;
    }
    getDensity() {
        return this.density;
    }
}
class Insul {
    constructor() {
        this.density = 0;
    }
    getDensity() {
        return this.density;
    }
}
class Clad {
    constructor() {
        this.density = 0;
    }
    getDensity() {
        return this.density;
    }
}
class Pipe {
    constructor(do_, di) {
        this.do_ = do_;
        this.di = di;
        this.material = new PipeMaterial();
        this.roughness = 0;
        this.k = 0;
        this.fluid = new Fluid();
        this.insul = new Insul();
        this.clad = new Clad();
        this.insulThk = 0;
        this.cladThk = 0;
    }
    /**
     * 单位长度质量 kg/m
     */
    weight() {
        return cylinderArea(this.di, this.do_) * this.material.getDensity();
    }
    area() {
        return circleCircumference(this.do_);
    }
    fluidWeight() {
        return circleArea(this.di) * this.fluid.getDensity();
    }
    waterWeight() {
        return circleArea(this.di) * 1000;
    }
    insulVolume() {
        return cylinderArea(this.do_, this.do_ + 2 * this.insulThk);
    }
    insulWeight() {
        return cylinderArea(this.do_, this.do_ + 2 * this.insulThk) * this.insul.getDensity();
    }
    cladArea() {
        return circleCircumference(this.do_ + 2 * this.insulThk);
    }
    cladWeight() {
        let do_clad = this.do_ + 2 * this.insulThk + 2 * this.cladThk;
        let di_clad = this.do_ + 2 * this.insulThk;
        return cylinderArea(di_clad, do_clad) * this.clad.getDensity();
    }
    velocity() {
        return this.fluid.getFlowRate_volume() / circleArea(this.di);
    }
    /**
     * 阻力系数
     */
    resistace() {
        let re = reynolds(this.di, this.velocity(), this.fluid.getDensity(), this.fluid.getViscosity());
        let lambda;
        if (re <= 2000) {
            lambda = 64 / re;
        }
        else if (re > 2000 && re <= 4000) {
            lambda = 0.3164 * Math.pow(re, -0.25);
        }
        else if (re > 4000 && re < 396 * this.di / this.roughness * Math.log10(3.7 * this.di / this.roughness)) {
            var x0 = 0, x1 = 0.1;
            do {
                var mid = (x0 + x1) / 2;
                if (1 / Math.pow(mid, 0.5) + 2 * Math.log10(this.roughness / (3.7 * this.di) + 2.51 / (re * Math.pow(mid, 0.5))) > 0) {
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
                if (1 / Math.pow(mid, 0.5) + 2 * Math.log10(this.roughness / (3.7 * this.di)) > 0) {
                    x0 = mid;
                }
                else {
                    x1 = mid;
                }
            } while (Math.abs(x0 - x1) > 1e-6);
            lambda = (x0 + x1) / 2;
        }
        return lambda;
    }
    /**
     * 单位长度直管阻力 Pa/m
     */
    dropPressure_line(resistace, velocity, density) {
        return resistace * density * Math.pow(velocity, 2) / (2 * this.di);
    }
    /**
     * 局部阻力 Pa
     */
    dropPressure_local(k /* 总局部阻力系数 */, velocity, density) {
        return k * density * Math.pow(velocity, 2) / 2;
    }
}
Pipe.Roughness = {
    copper_pipe: (0.005 + 0.01) / 2,
    non_corrosive_pipe: (0.05 + 0.1) / 2,
    mildly_corrosive_pipe: (0.1 + 0.2) / 2,
    deep_corrosive_pipe: (0.2 + 0.5) / 2,
    reel_pipe: 0.33,
    cast_iron_pipe: (0.5 + 0.85) / 2,
    glass_pipe: (0.0015 + 0.01) / 2
};
Pipe.LocalResistace = {
    elbow45: 0.35,
    elbow90: 0.75,
    elbow90_x: 1.30,
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
