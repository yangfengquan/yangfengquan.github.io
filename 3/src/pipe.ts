/* 严格采用国际单位（SI），无例外 */

/**
 * 圆 周长
 * @param d 直径 m
 * @returns m
 */
function circleCircumference(d:number/* 直径 */):number {
    return Math.PI * d;
}

/**
 * 圆面积
 * @param d 直径 m
 * @returns m2
 */
function circleArea(d:number/* 直径 */):number {
    return Math.PI * Math.pow(d, 2) * 0.25;
}

/**
 * 圆环面积
 * @param di 
 * @param do_ 
 * @returns m2
 */
function cylinderArea(di:number /* 内径 m */, do_:number /* 外径 m */):number {
    return circleArea(do_) - circleArea(di);
}

/**
 * 雷诺数
 */
function reynolds(di:number /* 内径 m */, velocity:number, density:number, viscosity:number/* 动力粘度 Pa.s */) {
    return di * velocity * density / viscosity;
}

class Pipe {
    do_:number;
    thk:number;
    density:number;
    fluidDensity:number;
    insulThk:number;
    insulDensity:number;
    cladThk:number;
    cladDensity:number;

    flowRate:number = 0; //质量流量

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
    weight():number {
        return cylinderArea(this.do_ - 2 * this.thk, this.do_) * this.density;
    }

    area():number {
        return circleCircumference(this.do_);
    }

    fluidWeight():number {
        return circleArea(this.do_ - 2 * this.thk)* this.fluidDensity;
    }

    waterWeight():number {
        return circleArea(this.do_ - 2 * this.thk) * 1000;
    }

    insulVolume():number {
        return cylinderArea(this.do_, this.do_ + 2 * this.insulThk);
    }

    insulWeight():number {
        return cylinderArea(this.do_, this.do_ + 2 * this.insulThk) * this.insulDensity;
    }

    cladArea():number {
        return circleCircumference(this.do_ + 2 * this.insulThk);
    }

    cladWeight():number {
        let dw = this.do_ + 2 * this.insulThk + 2 * this.cladThk;
        let di = this.do_ + 2 * this.insulThk;
        return cylinderArea(di, dw) * this.cladDensity;
    }
}