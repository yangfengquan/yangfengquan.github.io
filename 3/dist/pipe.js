"use strict";
/**
 * 圆 周长
 * @param d 直径 m
 * @returns m
 */
function circleCircumference(d) {
    return Math.PI * d;
}
/**
 * 圆面积
 * @param d 直径 m
 * @returns m2
 */
function circleArea(d) {
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
class Pipe {
    // constructor(do_:number /* 外径 m */, thk:number /* 壁厚 m */, density:number = 7850 /* 密度 kg/m3 */, fluidDensity:number = 0,
    //     insulThk:number = 0, insulDensity:number = 0, kapThk:number = 0, kapDensity:number = 0) {
    //     this.do_ = do_;
    //     this.thk = thk;
    //     this.density = density;
    //     this.fluidDensity = fluidDensity;
    //     this.insulThk = insulThk;
    //     this.insulDensity = insulDensity;
    //     this.kapThk = kapThk;
    //     this.kapDensity = kapDensity;
    // }
    constructor() {
        this.do_ = 0;
        this.thk = 0;
        this.density = 7850;
        this.fluidDensity = 0;
        this.insulThk = 0;
        this.insulDensity = 0;
        this.cladThk = 0;
        this.cladDensity = 0;
    }
    /**
     * 单位长度质量 kg/m
     * @returns
     */
    weight() {
        return cylinderArea(this.do_ - 2 * this.thk, this.do_) * this.density;
    }
    area() {
        return circleCircumference(this.do_);
    }
    fluidWeight() {
        return circleArea(this.do_ - 2 * this.thk) * this.fluidDensity;
    }
    waterWeight() {
        return circleArea(this.do_ - 2 * this.thk) * 1000;
    }
    insulVolume() {
        return cylinderArea(this.do_, this.do_ + 2 * this.insulThk);
    }
    insulWeight() {
        return cylinderArea(this.do_, this.do_ + 2 * this.insulThk) * this.insulDensity;
    }
    cladArea() {
        return circleCircumference(this.do_ + 2 * this.insulThk);
    }
    cladWeight() {
        let dw = this.do_ + 2 * this.insulThk + 2 * this.cladThk;
        let di = this.do_ + 2 * this.insulThk;
        return cylinderArea(di, dw) * this.cladDensity;
    }
}
