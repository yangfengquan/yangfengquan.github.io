/**
 * 圆 周长
 * @param d 直径 m
 * @returns m
 */
function circleCircumference(d:number):number {
    return Math.PI * d;
}

/**
 * 圆面积
 * @param d 直径 m
 * @returns m2
 */
function circleArea(d:number):number {
    return Math.PI * Math.pow(d, 2) * 0.25;
}

/**
 * 圆环面积
 * @param di 
 * @param do_ 
 * @returns m2
 */
function cylinderArea(di: number /* 内径 m */, do_: number /* 外径 m */):number {
    return circleArea(do_) - circleArea(di);
}

class Pipe {
    /* 单位 SI kg m */
    do_:number;
    thk:number;
    density:number;
    fluidDensity:number;
    insulThk:number;
    insulDensity:number;
    kapThk:number;
    kapDensity:number;


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
        this.kapThk = 0;
        this.kapDensity = 0;
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

    cladWeight(insulThk:number /* 保温厚 m */, insulRho:number /* 保温材料密度 kg/m3 */):number {
        return cylinderArea(this.do_, this.do_ + 2 * insulThk) * insulRho;
    }
}